#include "EngineAnimatinSkeleton.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/EngineFbxMath.h"
#include "DirectXMath.h"
#include "EngineHelper/EngineFSkeleton.h"

EngineAnimatinSkeleton::EngineAnimatinSkeleton() 
{
}

EngineAnimatinSkeleton::~EngineAnimatinSkeleton() 
{
	keyframesPerBoneMap.clear();
	Bones.clear();
}


//std::vector<DirectX::XMMATRIX> EngineAnimatinSkeleton::EvaluateAnimation(float time)
//{
//    size_t boneCount = Bones.size();
//    std::vector<DirectX::XMMATRIX> boneMatrices(boneCount);
//
//    for (size_t i = 0; i < boneCount; ++i)
//    {
//        const auto& keyframes = keyframesPerBoneIndex[i];
//        if (keyframes.empty())
//        {
//            // Ű�������� ������ ���ε� ���� �״�� ���
//            boneMatrices[i] = EngineFbxMath::ConvertFbxMatrixToXM(Bones[i].globalBindPose);
//            continue;
//        }
//
//        // 1. ���� �ð��� �´� �� Ű������ ã��
//        const KeyFrame* prev = nullptr;
//        const KeyFrame* next = nullptr;
//
//        for (size_t k = 0; k < keyframes.size(); ++k)
//        {
//            if (keyframes[k].time > time)
//            {
//                next = &keyframes[k];
//                if (k > 0) prev = &keyframes[k - 1];
//                break;
//            }
//        }
//
//        // 2. �� ã������ ������ Ű������ ���
//        if (!prev) prev = &keyframes.back();
//        if (!next) next = prev;
//
//        // 3. ���� ���� ���
//        float duration = static_cast<float>(next->time - prev->time);
//        float lerpFactor = static_cast<float>((duration > 0.0f) ? (time - prev->time) / duration : 0.0f);
//
//        // 4. position, rotation ����
//        DirectX::XMVECTOR pos1 = XMLoadFloat3(&prev->position);
//        DirectX::XMVECTOR pos2 = XMLoadFloat3(&next->position);
//        DirectX::XMVECTOR lerpedPos = DirectX::XMVectorLerp(pos1, pos2, lerpFactor);
//
//        DirectX::XMVECTOR rot1 = XMLoadFloat4(&prev->rotation);
//        DirectX::XMVECTOR rot2 = XMLoadFloat4(&next->rotation);
//        DirectX::XMVECTOR lerpedRot = DirectX::XMQuaternionSlerp(rot1, rot2, lerpFactor);
//
//        // 5. ���� ��� ����
//        DirectX::XMMATRIX localTransform = DirectX::XMMatrixRotationQuaternion(lerpedRot) * DirectX::XMMatrixTranslationFromVector(lerpedPos);
//
//        // 6. �θ��� ���� ��İ� ���� ����� ��ȯ
//        int parentIndex = Bones[i].parentIndex;
//        if (parentIndex >= 0)
//        {
//            boneMatrices[i] = localTransform * boneMatrices[parentIndex];
//        }
//        else
//        {
//            boneMatrices[i] = localTransform;
//        }
//    }
//
//    return boneMatrices;
//}
