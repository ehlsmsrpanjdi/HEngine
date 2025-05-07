#pragma once
#include "ConstantBufferResource.h"
#include "ConstantBufferStruct.h"
#include "EngineResource.h"
// Ό³Έν :
class DirectionLightBuffer : public ConstantBufferResource
{
public:
	// constrcuter destructer
	DirectionLightBuffer();
	~DirectionLightBuffer();

	// delete Function
	DirectionLightBuffer(const DirectionLightBuffer& _Other) = delete;
	DirectionLightBuffer(DirectionLightBuffer&& _Other) noexcept = delete;
	DirectionLightBuffer& operator=(const DirectionLightBuffer& _Other) = delete;
	DirectionLightBuffer& operator=(DirectionLightBuffer&& _Other) noexcept = delete;

	void Init() override;
protected:

private:

};

