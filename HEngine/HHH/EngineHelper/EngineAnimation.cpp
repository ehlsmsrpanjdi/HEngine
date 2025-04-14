#include "EngineAnimation.h"
#include "EngineTransform.h"
#include "AllStruct.h"
#include "EngineFSkeleton.h"
#include "EngineFbxMath.h"

EngineAnimation::EngineAnimation()
{
}

EngineAnimation::~EngineAnimation()
{
	Skeleton = nullptr;
    keyframesPerBoneIndex.clear();
}

void EngineAnimation::AddBoneKeyFrame(const std::string& boneName, int frame, const EngineTransform& transform)
{
}

void EngineAnimation::SetDuration(float startTime, float endTime)
{
	StartTime = startTime;
	EndTime = endTime;
}

void EngineAnimation::ExtractAnimationKeys(FbxScene* scene)
{
	FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator();
	FbxNode* rootNode = scene->GetRootNode();
	if (!rootNode) return;

	FbxAnimStack* animStack = scene->GetCurrentAnimationStack();
	FbxTimeSpan timeSpan = animStack->GetLocalTimeSpan();


	FbxTime startTime = timeSpan.GetStart();
	FbxTime endTime = timeSpan.GetStop();

    SetDuration((float)startTime.GetSecondDouble(), (float)endTime.GetSecondDouble());


	FbxTime frameTime;
	frameTime.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

	for (FbxTime currentTime = startTime; currentTime <= endTime; currentTime += frameTime) {
		TraverseAndExtract(rootNode, evaluator, currentTime);
	}
}

void EngineAnimation::TraverseAndExtract(FbxNode* node, FbxAnimEvaluator* evaluator, FbxTime time)
{
	if (!node) return;

	std::string name = node->GetName();

	if (Skeleton->boneNameToIndex.contains(name) != false) {
		int boneindex = Skeleton->boneNameToIndex[name];
		FbxAMatrix localTransform = evaluator->GetNodeLocalTransform(node, time);
		FbxVector4 translation = localTransform.GetT();
		FbxQuaternion rotation = localTransform.GetQ();

		KeyFrame kf;
		kf.time = time.GetSecondDouble();
		kf.position = { (float)translation[0], (float)translation[1], (float)translation[2] };
		kf.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], (float)rotation[3] };

		keyframesPerBoneIndex[boneindex].emplace_back(kf); // 본 이름별로 저장 (Map<string, vector<KeyFrame>>)

	}

	// 자식 노드 순회
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i) {
		TraverseAndExtract(node->GetChild(i), evaluator, time);
	}
}

std::vector<DirectX::XMMATRIX> EngineAnimation::EvaluateAnimation(float time)
{
    size_t boneCount = Skeleton->Bones.size();
    std::vector<XMMATRIX> boneMatrices(boneCount);

    for (size_t i = 0; i < boneCount; ++i)
    {
        const auto& keyframes =  keyframesPerBoneIndex[i];
        if (keyframes.empty())
        {
            // 키프레임이 없으면 바인드 포즈 그대로 사용
            boneMatrices[i] = EngineFbxMath::ConvertFbxMatrixToXM(Skeleton->Bones[i].globalBindPose);
            continue;
        }

        // 1. 현재 시간에 맞는 두 키프레임 찾기
        const KeyFrame* prev = nullptr;
        const KeyFrame* next = nullptr;

        for (size_t k = 0; k < keyframes.size(); ++k)
        {
            if (keyframes[k].time > time)
            {
                next = &keyframes[k];
                if (k > 0) prev = &keyframes[k - 1];
                break;
            }
        }

        // 2. 못 찾았으면 마지막 키프레임 사용
        if (!prev) prev = &keyframes.back();
        if (!next) next = prev;

        // 3. 보간 비율 계산
        float duration = static_cast<float>(next->time - prev->time);
        float lerpFactor = static_cast<float>((duration > 0.0f) ? (time - prev->time) / duration : 0.0f);

        // 4. position, rotation 보간
        XMVECTOR pos1 = XMLoadFloat3(&prev->position);
        XMVECTOR pos2 = XMLoadFloat3(&next->position);
        XMVECTOR lerpedPos = XMVectorLerp(pos1, pos2, lerpFactor);

        XMVECTOR rot1 = XMLoadFloat4(&prev->rotation);
        XMVECTOR rot2 = XMLoadFloat4(&next->rotation);
        XMVECTOR lerpedRot = XMQuaternionSlerp(rot1, rot2, lerpFactor);

        // 5. 로컬 행렬 생성
        XMMATRIX localTransform = XMMatrixRotationQuaternion(lerpedRot) * XMMatrixTranslationFromVector(lerpedPos);

        // 6. 부모의 최종 행렬과 곱해 월드로 변환
        int parentIndex = Skeleton->Bones[i].parentIndex;
        if (parentIndex >= 0)
        {
            boneMatrices[i] = localTransform * boneMatrices[parentIndex];
        }
        else
        {
            boneMatrices[i] = localTransform;
        }
    }

    return boneMatrices;
}
