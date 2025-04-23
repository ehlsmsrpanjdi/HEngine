#include "TestTestActor.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"

TestTestActor::TestTestActor() 
{
}

TestTestActor::~TestTestActor() 
{
}



void TestTestActor::BeginPlay()
{
	Actor::BeginPlay();
	SetHlsl("default");
	SetScene("boxcollision");
	//SetActorScale(0.1f, 0.1f, 0.1f);
	AddActorLocation(300.f);

	//ActorScene->AnimSkeleton->SetAnimation("dance");
	//IsAnimation = true;
	Name = "Test";

	//CreateCollision(CollisionType::Normal);
}

void TestTestActor::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);
	//AddActorRotation(10.f * _deltatime);
	//GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}