#include "MainPlayer.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"

MainPlayer::MainPlayer() 
{
}

MainPlayer::~MainPlayer() 
{
}

void MainPlayer::BeginPlay()
{
	Actor::BeginPlay();
	SetHlsl("default");
	SetScene("Ball");
	SetActorScale(0.1f, 0.1f, 0.1f);

	/*ActorScene->AnimSkeleton->SetAnimationTemp();
	IsAnimation = true;
	ActorScene->AnimSkeleton->EvaluateAnimation(0.f, outBoneMatrices);*/
	GraphicsEngine::get()->CreateConstantBuffer1("MainPlayer", outBoneMatrices.size());
}

void MainPlayer::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);
	//for (auto& mat : outBoneMatrices) {
	//	mat = DirectX::XMMatrixIdentity();
	//	// 또는 mat.r[0].m128_f32[0] = 1.0f; 로 직접 0번 요소에 값 넣어도 됨
	//}
	GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}

