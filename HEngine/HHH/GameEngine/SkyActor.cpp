#include "SkyActor.h"
#include "HDirect/GraphicsEngine.h"
#include "HDirect/EngineScene.h"
#include "HDirect/EngineAnimatinSkeleton.h"
#include "Collision.h"
#include "HDirect/EngineHlslResource.h"
#include "EngineHelper/EngineKey.h"

SkyActor::SkyActor()
{
}

SkyActor::~SkyActor()
{
}


void SkyActor::BeginPlay()
{
	Actor::BeginPlay();


	SetHlsl(HlslNamespace::Background);
	SetScene("skybox");
	SetTexture("sky");
	Name = "SkyBackGround";

}

void SkyActor::Tick(float _deltatime)
{
	Actor::Tick(_deltatime);

}