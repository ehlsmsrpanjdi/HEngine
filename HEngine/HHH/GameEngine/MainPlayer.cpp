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
	SetScene("StandingGreeting");
	//SetScene("WoodenBox");
	SetActorScale(0.1f, 0.1f, 0.1f);

	ActorScene->AnimSkeleton->SetAnimationTemp();
	IsAnimation = true;
	ActorScene->AnimSkeleton->EvaluateAnimation(0.f, outBoneMatrices);
	GraphicsEngine::get()->CreateConstantBuffer1("MainPlayer", outBoneMatrices.size());
}

void MainPlayer::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

	GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}

