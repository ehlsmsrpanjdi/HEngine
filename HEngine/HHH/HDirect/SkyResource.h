#pragma once
#include "EngineHlslResource.h"
// Ό³Έν :
class SkyResource : public EngineHlslResource
{
public:
	// constrcuter destructer
	SkyResource();
	~SkyResource();

	// delete Function
	SkyResource(const SkyResource& _Other) = delete;
	SkyResource(SkyResource&& _Other) noexcept = delete;
	SkyResource& operator=(const SkyResource& _Other) = delete;
	SkyResource& operator=(SkyResource&& _Other) noexcept = delete;

	void Init() override;	

protected:

private:

};

