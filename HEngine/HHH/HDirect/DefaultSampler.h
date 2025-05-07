#pragma once
#include "EngineSamplerResource.h"
// ���� :
class DefaultSampler : public EngineSamplerResource
{
public:
	// constrcuter destructer
	DefaultSampler();
	~DefaultSampler();

	// delete Function
	DefaultSampler(const DefaultSampler& _Other) = delete;
	DefaultSampler(DefaultSampler&& _Other) noexcept = delete;
	DefaultSampler& operator=(const DefaultSampler& _Other) = delete;
	DefaultSampler& operator=(DefaultSampler&& _Other) noexcept = delete;

	void Init() override;
protected:

private:

};

