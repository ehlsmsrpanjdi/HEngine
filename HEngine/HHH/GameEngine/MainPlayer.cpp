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
	SetScene("character");
	SetActorScale(0.1f, 0.1f, 0.1f);

	ActorScene->AnimSkeleton->SetAnimation("dance");
	IsAnimation = true;
}

void MainPlayer::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

	//GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}

