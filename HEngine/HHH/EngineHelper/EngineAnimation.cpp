#include "EngineAnimation.h"
#include "EngineTransform.h"
#include "AllStruct.h"

EngineAnimation::EngineAnimation()
{
}

EngineAnimation::~EngineAnimation()
{
}

void EngineAnimation::AddBoneKeyFrame(const std::string& boneName, int frame, const EngineTransform& transform)
{
}

void EngineAnimation::SetDuration(float startTime, float endTime)
{
	StartTime = startTime;
	EndTime = endTime;
}

void EngineAnimation::Update(float deltaTime)
{
}

void EngineAnimation::init(FbxNode* _Node, std::string_view AnimName, float frameRate, float start, float stop)
{
	FbxNodeAttribute* nodeAttribute = _Node->GetNodeAttribute();
	if (nodeAttribute == nullptr) {
		return;
	}
	if (nodeAttribute->GetAttributeType() != FbxNodeAttribute::eSkeleton) {
		return;
	}
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
	FbxTime frameTime;
	frameTime.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

	for (FbxTime currentTime = startTime; currentTime <= endTime; currentTime += frameTime) {
		TraverseAndExtract(rootNode, evaluator, currentTime);
	}
}

void EngineAnimation::TraverseAndExtract(FbxNode* node, FbxAnimEvaluator* evaluator, FbxTime time)
{
	if (!node) return;

	FbxAMatrix localTransform = evaluator->GetNodeLocalTransform(node, time);
	FbxVector4 translation = localTransform.GetT();
	FbxQuaternion rotation = localTransform.GetQ();

	KeyFrame kf;
	kf.time = time.GetSecondDouble();
	kf.position = { (float)translation[0], (float)translation[1], (float)translation[2] };
	kf.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], (float)rotation[3] };

	std::string name = node->GetName();
	keyframesPerBone[name].emplace_back(kf); // 본 이름별로 저장 (Map<string, vector<KeyFrame>>)

	// 자식 노드 순회
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; ++i) {
		TraverseAndExtract(node->GetChild(i), evaluator, time);
	}
}