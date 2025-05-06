#pragma once
#include "EngineHlslResource.h"
// ���� :
class DefaultResource : public EngineHlslResource
{
public:
	// constrcuter destructer
	DefaultResource();
	~DefaultResource();

	// delete Function
	DefaultResource(const DefaultResource& _Other) = delete;
	DefaultResource(DefaultResource&& _Other) noexcept = delete;
	DefaultResource& operator=(const DefaultResource& _Other) = delete;
	DefaultResource& operator=(DefaultResource&& _Other) noexcept = delete;

	void Init() override;
protected:

private:

};

