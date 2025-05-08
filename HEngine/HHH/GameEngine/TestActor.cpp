#include "TestActor.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"
#include "HDirect/EngineHlslResource.h"

TestActor::TestActor() 
{
}

TestActor::~TestActor() 
{
}


void TestActor::BeginPlay()
{
	Actor::BeginPlay();


	SetHlsl(HlslNamespace::Default);
	SetScene("character");
	SetAnimation("dance");
	Name = "Test";

	OffAnimation();
	SetActorScale(0.1f, 0.1f, 0.1f);


	//CreateCollision(CollisionType::Normal);
}

void TestActor::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);
	//AddActorRotation(0.f, 5.0f * _deltatime, 0.f);
	//AddActorRotation(0.f,0.f,10.f * _deltatime);
	//ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(AnimationArr), Cbuffer::ANI);
}