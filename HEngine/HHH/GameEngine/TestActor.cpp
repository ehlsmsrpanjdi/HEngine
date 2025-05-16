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


	SetHlsl(HlslNamespace::NoneAnimation);
	SetScene("character");
	//SetAnimation("dance");
	Name = "Test";

	//SetActorScale(0.1f, 0.1f, 0.1f);


	//CreateCollision(CollisionType::Normal);
}

void TestActor::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

	if (EngineKey::IsDown('O') == true) {
		OnAnimation();
	}

	if (EngineKey::IsDown('I') == true) {
		OffAnimation();
	}

	//if(EngineKey::)

}