#include "LightActor.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/EngineNamespace.h"

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
	GraphicsEngine::get()->CreateDirectionalLightBuffer();

	DirectionalLightBuffer lightBuffer;

	lightBuffer.Color = XMFLOAT3(1.f, 1.f,1.f);
	lightBuffer.Padding1 = 0.f; // 16����Ʈ ������ ���� �е�
	lightBuffer.Intensity = 10.f;
	lightBuffer.Direction = XMFLOAT3(180.f, 180.f, 180.f); // ���� ������ �����մϴ�.

	GraphicsEngine::get()->UpdateConstantBuffer(lightBuffer, Cbuffer::DirectLight);
	GraphicsEngine::get()->SetConstantBuffer(Cbuffer::DirectLight);
}

void LightActor::Tick(float _DeltaTime)
{
}

