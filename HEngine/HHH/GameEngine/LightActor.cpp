#include "LightActor.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "HDirect/ConstantBufferStruct.h"

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
	//GraphicsEngine::get()->CreateDirectionalLightBuffer();

	//LightBuffer lightBuffer;

	//lightBuffer.Color = XMFLOAT3(1.f, 1.f,1.f);
	//lightBuffer.Padding1 = 0.f; // 16����Ʈ ������ ���� �е�
	//lightBuffer.Intensity = 1.f;
	//lightBuffer.DirectionOrPosition = XMFLOAT3(0.f, 0.f, 1.f); // ���� ������ �����մϴ�.

	//GraphicsEngine::get()->UpdateConstantBuffer(lightBuffer, Cbuffer::LIGHT);
	//GraphicsEngine::get()->SetConstantBuffer(Cbuffer::LIGHT);
}

void LightActor::Tick(float _DeltaTime)
{
}

