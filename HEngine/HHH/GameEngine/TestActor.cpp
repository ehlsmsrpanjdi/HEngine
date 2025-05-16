#include "TestActor.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"
#include "HDirect/EngineHlslResource.h"
#include "EngineHelper/EngineKey.h"

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
	//SetAnimation("dance");
	Name = "Test";

	OffAnimation();
	//SetActorScale(0.1f, 0.1f, 0.1f);


	//CreateCollision(CollisionType::Normal);
}

void TestActor::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

}