#pragma once
#include "d3d11.h"
#include "iostream"
#include "d3dcompiler.h"
#include <vector>
#include "EngineResource.h"
#include "EngineHelper/AllStruct.h"
#include "memory"

namespace HlslNamespace {
	extern std::string Default;
	extern std::string Animation;
	extern std::string NoneAnimation;
}

// Ό³Έν :
class EngineHlslResource : public EngineResource<EngineHlslResource>
{
public:
	// constrcuter destructer
	EngineHlslResource();
	~EngineHlslResource();

	// delete Function
	EngineHlslResource(const EngineHlslResource& _Other) = delete;
	EngineHlslResource(EngineHlslResource&& _Other) noexcept = delete;
	EngineHlslResource& operator=(const EngineHlslResource& _Other) = delete;
	EngineHlslResource& operator=(EngineHlslResource&& _Other) noexcept = delete;

	virtual void Init() = 0;

	virtual std::vector<D3D11_INPUT_ELEMENT_DESC>& GetLayoutInfo() {
		return Layout;
	}

	std::string_view GetName() {
		return HlslName;
	}

	std::shared_ptr<HS> GetHlsl() {
		return HlslData;
	}

	void SetHlsl(std::shared_ptr<HS> _Hlsl) {
		if (HlslData != nullptr) {
			return;
		}
		HlslData = _Hlsl;
	}

protected:
	std::shared_ptr<HS> HlslData = nullptr;
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
	std::string HlslName;
};