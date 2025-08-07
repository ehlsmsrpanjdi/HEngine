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
	SetScene("WoodBox");
	SetTexture("Wood");
	Name = "Box";
}

void BoxActor::Tick(float _deltatime)
{
}
