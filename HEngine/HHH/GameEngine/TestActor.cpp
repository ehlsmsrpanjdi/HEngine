#include "TestActor.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"

TestActor::TestActor() 
{
}

TestActor::~TestActor() 
{
}


void TestActor::BeginPlay()
{
	Actor::BeginPlay();
	SetHlsl("default");
	SetScene("boxcollision");
	//SetActorScale(0.1f, 0.1f, 0.1f);
	AddActorLocation(0.f, 300.f);

	//ActorScene->AnimSkeleton->SetAnimation("dance");
	//IsAnimation = true;
	Name = "Test";

	//CreateCollision(CollisionType::Normal);
}

void TestActor::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);
	AddActorRotation(0.f,0.f,10.f * _deltatime);
	//GraphicsEngine::get()->UpdateConstantBuffer(outBoneMatrices, "MainPlayer");

}