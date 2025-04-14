#pragma once
#include <iostream>
#include <DirectXMath.h>
#include <unordered_map>
#include <vector>
#include "fbxsdk.h"

// Ό³Έν :
class EngineAnimatinSkeleton
{
public:
	// constrcuter destructer
	EngineAnimatinSkeleton();
	~EngineAnimatinSkeleton();

	// delete Function
	EngineAnimatinSkeleton(const EngineAnimatinSkeleton& _Other) = delete;
	EngineAnimatinSkeleton(EngineAnimatinSkeleton&& _Other) noexcept = delete;
	EngineAnimatinSkeleton& operator=(const EngineAnimatinSkeleton& _Other) = delete;
	EngineAnimatinSkeleton& operator=(EngineAnimatinSkeleton&& _Other) noexcept = delete;

	//std::vector<DirectX::XMMATRIX> EvaluateAnimation(float time);



private:
	friend class EngineScene;
	std::vector<struct Bone> Bones;
	std::unordered_map<std::string, std::vector<std::vector<struct KeyFrame>>> keyframesPerBoneMap;
	std::unordered_map<std::string, std::pair<float, float>> AnimationTime;

	std::vector<std::vector<struct KeyFrame>>* SeletectFrame = nullptr;
	float SelectStartTime = 0.0f;
	float SelectEndTime = 0.0f;


public:
	//void Evaluate(float time, std::vector<DirectX::XMMATRIX>& outBoneMatrices) const;


protected:

private:

};

