#pragma once
#include "ConstantBufferResource.h"
#include "ConstantBufferStruct.h"
#include "EngineResource.h"
// Ό³Έν :
class WVPConstantBuffer : public ConstantBufferResource
{
public:
	// constrcuter destructer
	WVPConstantBuffer();
	~WVPConstantBuffer();

	// delete Function
	WVPConstantBuffer(const WVPConstantBuffer& _Other) = delete;
	WVPConstantBuffer(WVPConstantBuffer&& _Other) noexcept = delete;
	WVPConstantBuffer& operator=(const WVPConstantBuffer& _Other) = delete;
	WVPConstantBuffer& operator=(WVPConstantBuffer&& _Other) noexcept = delete;

	void Init() override;
protected:

private:

};

