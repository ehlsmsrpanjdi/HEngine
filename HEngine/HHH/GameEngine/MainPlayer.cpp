#include "MainPlayer.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"

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
	Name = "Player";

	CreateCollision(CollisionType::Normal);
}

void MainPlayer::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

	//GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}

