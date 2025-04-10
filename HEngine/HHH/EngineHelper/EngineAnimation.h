#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
// 설명 :
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
	void Update(float deltaTime); // 나중에 애니메이션 재생할 때 사용

protected:

private:
	std::string Name;
	float StartTime = 0.f;
	float EndTime = 0.f;
	float CurrentTime = 0.f;

	// bone name -> frame index -> transform
	std::unordered_map<std::string, std::unordered_map<int, class EngineTransform>> BoneAnimationData;

};

