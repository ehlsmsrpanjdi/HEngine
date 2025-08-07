#include "BoxActor.h"
#include "HDirect/EngineHlslResource.h"

BoxActor::BoxActor()
{
}

BoxActor::~BoxActor()
{
}

void BoxActor::BeginPlay()
{
	Actor::BeginPlay();

	SetHlsl(HlslNamespace::NoneAnimation);
	SetScene("box");
	//SetTexture("GlassCube");
	Name = "Box";
}

void BoxActor::Tick(float _deltatime)
{
}
