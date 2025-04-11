#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
// ���� :
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


	void AddBoneKeyFrame(const std::string& boneName, int frame, const class EngineTransform& transform);
	void SetDuration(float startTime, float endTime);
	void Update(float deltaTime); // ���߿� �ִϸ��̼� ����� �� ���

	void init(FbxNode* _Node, std::string_view AnimName, float frameRate, float start, float stop);

	void ExtractAnimationKeys(FbxScene* scene);
	void TraverseAndExtract(FbxNode* node, FbxAnimEvaluator* evaluator, FbxTime time);

	std::string AnimationName;

private:
	std::unordered_map<std::string, std::vector<struct KeyFrame>> keyframesPerBone;


	float StartTime = 0.f;
	float EndTime = 0.f;
	float CurrentTime = 0.f;
	

	// bone name -> frame index -> transform
	std::vector<struct KeyFrame> keyframes;
};

