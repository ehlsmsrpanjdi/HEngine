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



void EngineAnimation::SetDuration(float startTime, float endTime)
{
	StartTime = startTime;
	EndTime = endTime;
}

void EngineAnimation::ExtractAnimationKeys(FbxScene* scene)
{
	FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator();
    //Ư�� �ð��� ���� � ��ġ, ȸ���� �������� ����� ���ִ� ��ü
	FbxNode* rootNode = scene->GetRootNode();
	if (!rootNode) return;

	FbxAnimStack* animStack = scene->GetCurrentAnimationStack();
    fbxsdk::FbxTimeSpan timeSpan = animStack->GetLocalTimeSpan();


    fbxsdk::FbxTime startTime = timeSpan.GetStart();
    fbxsdk::FbxTime endTime = timeSpan.GetStop();

    SetDuration((float)startTime.GetSecondDouble(), (float)endTime.GetSecondDouble());


    fbxsdk::FbxTime frameTime;
	frameTime.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());
    for (fbxsdk::FbxTime currentTime = startTime; currentTime <= endTime; currentTime += frameTime) {
		TraverseAndExtract(rootNode, evaluator, currentTime);
	}
}

void EngineAnimation::TraverseAndExtract(FbxNode* node, FbxAnimEvaluator* evaluator, fbxsdk::FbxTime time)
{
	if (!node) return;

	std::string name = node->GetName();
	if (Skeleton->boneNameToIndex.contains(name) != false) {
		int boneindex = Skeleton->boneNameToIndex[name];
		FbxAMatrix localTransform = evaluator->GetNodeGlobalTransform(node, time);
		FbxVector4 translation = localTransform.GetT();
		FbxQuaternion rotation = localTransform.GetQ();

		KeyFrame kf;
		kf.time = time.GetSecondDouble();
		kf.position = { (float)translation[0], (float)translation[1], (float)translation[2] };
		kf.rotation = { (float)rotation.GetAt(0), (float)rotation.GetAt(1), (float)rotation.GetAt(2), (float)rotation.GetAt(3)};

		keyframesPerBoneIndex[boneindex].emplace_back(kf); // �� �̸����� ���� (Map<string, vector<KeyFrame>>)

	}

	// �ڽ� ��� ��ȸ
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i) {
		TraverseAndExtract(node->GetChild(i), evaluator, time);
	}
}
