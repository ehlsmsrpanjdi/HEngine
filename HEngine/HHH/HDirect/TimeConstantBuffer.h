#pragma once
#include "ConstantBufferResource.h"
#include "ConstantBufferStruct.h"
#include "EngineResource.h"
// Ό³Έν :
class TimeConstantBuffer : public ConstantBufferResource
{
public:
	// constrcuter destructer
	TimeConstantBuffer();
	~TimeConstantBuffer();

	// delete Function
	TimeConstantBuffer(const TimeConstantBuffer& _Other) = delete;
	TimeConstantBuffer(TimeConstantBuffer&& _Other) noexcept = delete;
	TimeConstantBuffer& operator=(const TimeConstantBuffer& _Other) = delete;
	TimeConstantBuffer& operator=(TimeConstantBuffer&& _Other) noexcept = delete;

	void Init() override;
protected:

private:

};

