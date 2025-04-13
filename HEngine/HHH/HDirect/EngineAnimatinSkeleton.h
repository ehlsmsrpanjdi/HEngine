#pragma once
#include <iostream>
#include <DirectXMath.h>
#include <unordered_map>
#include <vector>

// Ό³Έν :
class EngineAnimatinSkeleton
{
public:
	struct Bone {
		std::string name;
		int parentIndex;
		DirectX::XMMATRIX localBindPose;
		DirectX::XMMATRIX globalBindPose;
		DirectX::XMMATRIX inverseGlobalBindPose;
	};
	// constrcuter destructer
	EngineAnimatinSkeleton();
	~EngineAnimatinSkeleton();

	// delete Function
	EngineAnimatinSkeleton(const EngineAnimatinSkeleton& _Other) = delete;
	EngineAnimatinSkeleton(EngineAnimatinSkeleton&& _Other) noexcept = delete;
	EngineAnimatinSkeleton& operator=(const EngineAnimatinSkeleton& _Other) = delete;
	EngineAnimatinSkeleton& operator=(EngineAnimatinSkeleton&& _Other) noexcept = delete;


	struct KeyFrame {
		float time;
		DirectX::XMMATRIX transform;
	};

private:
	std::vector<Bone> Bones;
	std::unordered_map<int, std::vector<KeyFrame>> keyframesPerBoneIndex;

	float startTime = 0.f;
	float endTime = 0.f;

public:
	//void Evaluate(float time, std::vector<DirectX::XMMATRIX>& outBoneMatrices) const;


protected:

private:

};

