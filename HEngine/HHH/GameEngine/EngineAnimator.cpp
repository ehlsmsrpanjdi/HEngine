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
//	// ���� üũ
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
//	// -> �� boneMatrices�� ��Ű�� �޽÷� �ѱ�� (��: skinnedMesh->ApplyBoneMatrices(...))
//}
//
