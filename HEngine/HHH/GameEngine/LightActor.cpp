#include "LightActor.h"

LightActor::LightActor() 
{
}

LightActor::~LightActor() 
{
}

void LightActor::BeginPlay()
{
	Actor::BeginPlay();
	if (Type == LightType::None) {	//타입지정이 잘못되면 안댐  무조건 spawnlight로 스폰되게할거임
		assert(false);
	}
}

void LightActor::Tick(float _DeltaTime)
{
}

