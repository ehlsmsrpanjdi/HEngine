#pragma once
#include "EngineHlslResource.h"
// Ό³Έν :
class NoneAnimationResource : public EngineHlslResource
{
public:
	// constrcuter destructer
	NoneAnimationResource();
	~NoneAnimationResource();

	// delete Function
	NoneAnimationResource(const NoneAnimationResource& _Other) = delete;
	NoneAnimationResource(NoneAnimationResource&& _Other) noexcept = delete;
	NoneAnimationResource& operator=(const NoneAnimationResource& _Other) = delete;
	NoneAnimationResource& operator=(NoneAnimationResource&& _Other) noexcept = delete;

	void Init() override;

protected:

private:

};

