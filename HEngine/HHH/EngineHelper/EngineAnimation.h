#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
// Ό³Έν :
class EngineAnimation
{
public:
	// constrcuter destructer
	EngineAnimation();
	~EngineAnimation();

	// delete Function
	EngineAnimation(const EngineAnimation& _Other) = delete;
	EngineAnimation(EngineAnimation&& _Other) noexcept = delete;
	EngineAnimation& operator=(const EngineAnimation& _Other) = delete;
	EngineAnimation& operator=(EngineAnimation&& _Other) noexcept = delete;


	void SetDuration(float startTime, float endTime);

	void ExtractAnimationKeys(FbxScene* scene);
	void TraverseAndExtract(FbxNode* node, FbxAnimEvaluator* evaluator, FbxTime time);

	std::string AnimationName;


private:
	friend class EngineFScene;
	friend class EngineScene;
	std::unordered_map<int, std::vector<struct KeyFrame>> keyframesPerBoneIndex;


	std::shared_ptr<class EngineFSkeleton> Skeleton = nullptr;

	float StartTime = 0.f;
	float EndTime = 0.f;
};

