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
	if (Type == LightType::None) {	//Ÿ�������� �߸��Ǹ� �ȴ�  ������ spawnlight�� �����ǰ��Ұ���
		assert(false);
	}
}

void LightActor::Tick(float _DeltaTime)
{
}

