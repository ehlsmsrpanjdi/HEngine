#include "LightActor.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/AllStruct.h"
#include "HDirect/ConstantBufferStruct.h"
#include "HDirect/ConstantBufferResource.h"

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
	//GraphicsEngine::get()->CreateDirectionalLightBuffer();

	LightBuffer lightBuffer;

	lightBuffer.Color = XMFLOAT3(1.f, 1.f,1.f);
	lightBuffer.Padding1 = 0.f; // 16바이트 정렬을 위한 패딩
	lightBuffer.Intensity = 1.f;
	lightBuffer.DirectionOrPosition = XMFLOAT3(0.f, 0.f, 1.f); // 빛의 방향을 설정합니다.

	ConstantBufferResource::UpdateConstantBuffer(static_cast<void*>(&lightBuffer), Cbuffer::LIGHT);
}

void LightActor::Tick(float _DeltaTime)
{
}

