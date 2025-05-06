#pragma once
#include "d3d11.h"
#include "iostream"
#include "d3dcompiler.h"
#include <vector>

// Ό³Έν :
class EngineHlslResource 
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
protected:
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
};

