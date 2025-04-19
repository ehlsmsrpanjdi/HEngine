//#include "EngineAnimator.h"
//#include "EngineHelper/EngineAnimation.h"
//#include "EngineHelper/EngineFSkeleton.h"
//#include "vector"
//
//#include <DirectXMath.h>
//
//
//
//EngineAnimator::EngineAnimator() 
//{
//}
//
//EngineAnimator::~EngineAnimator() 
//{
//	currentAnimation = nullptr;
//}
//
//void EngineAnimator::Update(float deltaTime)
//{
//	if (!currentAnimation)
//		return;
//
//	currentTime += deltaTime * animationSpeed;
//
//	// 루프 체크
//	if (loop && currentTime > currentAnimation->GetDuration())
//	{
//		currentTime = fmod(currentTime, currentAnimation->GetDuration());
//	}
//	else if (!loop && currentTime > currentAnimation->GetDuration())
//	{
//		currentTime = currentAnimation->GetDuration();
//	}
//
//	std::vector<DirectX::XMMATRIX> boneMatrices = currentAnimation->EvaluateAnimation(currentTime);
//
//	// -> 이 boneMatrices를 스키닝 메시로 넘기기 (예: skinnedMesh->ApplyBoneMatrices(...))
//}
//
