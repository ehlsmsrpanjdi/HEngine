//#pragma once
//#include <memory>
//#include <unordered_map>
//#include <iostream>
//
//#include "fbxsdk.h"
//#pragma comment(lib, "libfbxsdk-md.lib")
//#pragma comment(lib, "libxml2-md.lib")
//#pragma comment(lib, "zlib-md.lib")
//
//// Ό³Έν :
//class EngineAnimator
//{
//public:
//	// constrcuter destructer
//	EngineAnimator();
//	~EngineAnimator();
//
//	// delete Function
//	EngineAnimator(const EngineAnimator& _Other) = delete;
//	EngineAnimator(EngineAnimator&& _Other) noexcept = delete;
//	EngineAnimator& operator=(const EngineAnimator& _Other) = delete;
//	EngineAnimator& operator=(EngineAnimator&& _Other) noexcept = delete;
//
//	void Update(float deltaTime);
//
//	
//protected:
//
//private:
//	 std::shared_ptr<class EngineAnimation> currentAnimation = nullptr;
//	 float currentTime = 0.0f;
//	 float animationSpeed = 1.0f;
//	 bool loop = true;
//
//};
//
