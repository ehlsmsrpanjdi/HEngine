#pragma once
#include "d3d11.h"
#include "d3dcompiler.h"
#include "EngineResource.h"
#include <memory>
#include "iostream"
// Ό³Έν :

namespace SamplerNameSpace {
	extern std::string DEFAULT;
}




class EngineSamplerResource : public EngineResource	<EngineSamplerResource>, public std::enable_shared_from_this<EngineSamplerResource>
{
public:
	// constrcuter destructer
	EngineSamplerResource();
	virtual ~EngineSamplerResource();

	// delete Function
	EngineSamplerResource(const EngineSamplerResource& _Other) = delete;
	EngineSamplerResource(EngineSamplerResource&& _Other) noexcept = delete;
	EngineSamplerResource& operator=(const EngineSamplerResource& _Other) = delete;
	EngineSamplerResource& operator=(EngineSamplerResource&& _Other) noexcept = delete;

	void CreateSampler();
	ID3D11SamplerState* GetSampler() {
		if (SamplerData != nullptr) {
			return SamplerData;
		}
		else {
			assert(false);
			return nullptr;
		}
	}

	virtual void Init();

protected:
	ID3D11SamplerState* SamplerData = nullptr;

	std::string SamplerName = "NONE";
private:

};

