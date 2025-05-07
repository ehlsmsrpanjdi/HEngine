#pragma once
#include "ConstantBufferResource.h"
#include "ConstantBufferStruct.h"
#include "EngineResource.h"
// Ό³Έν :
class AnimationConstantBuffer : public ConstantBufferResource
{
public:
	// constrcuter destructer
	AnimationConstantBuffer();
	~AnimationConstantBuffer();

	// delete Function
	AnimationConstantBuffer(const AnimationConstantBuffer& _Other) = delete;
	AnimationConstantBuffer(AnimationConstantBuffer&& _Other) noexcept = delete;
	AnimationConstantBuffer& operator=(const AnimationConstantBuffer& _Other) = delete;
	AnimationConstantBuffer& operator=(AnimationConstantBuffer&& _Other) noexcept = delete;

	void Init() override;	

protected:

private:

};

