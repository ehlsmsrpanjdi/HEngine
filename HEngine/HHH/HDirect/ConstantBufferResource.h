#pragma once
#include "d3d11.h"
#include "iostream"
#include "EngineResource.h"
// Ό³Έν :

namespace Cbuffer {
	extern std::string Animation;
	extern std::string NoneAnimation;
	extern std::string SkyBox;
	extern std::string Light;
}


class ConstantBufferResource : public EngineResource<ConstantBufferResource>, public std::enable_shared_from_this<ConstantBufferResource>
{
public:
	// constrcuter destructer
	ConstantBufferResource();
	virtual ~ConstantBufferResource();

	// delete Function
	ConstantBufferResource(const ConstantBufferResource& _Other) = delete;
	ConstantBufferResource(ConstantBufferResource&& _Other) noexcept = delete;
	ConstantBufferResource& operator=(const ConstantBufferResource& _Other) = delete;
	ConstantBufferResource& operator=(ConstantBufferResource&& _Other) noexcept = delete;

	std::string_view GetName() {
		return BufferName;
	}
	virtual void Init() {

	}

	void CreateBuffer(std::shared_ptr<class GraphicDevice> m_Device);

	static void UpdateConstantBuffer(const void* Data, std::string_view _str);

	int GetBufferCount() {
		return BufferCount;
	}

	ID3D11Buffer* GetBuffer() {
		return Buffer;
	}

	UINT GetBufferSize() {
		return BufferSize;
	}

	static void SetVSConstantBuffer(int _index, std::string_view _str);
	static void SetPSConstantBuffer(int _index, std::string_view _str);

protected:
	friend class GraphicsEngine;

	class ID3D11Buffer* Buffer = nullptr;
	std::string BufferName;
	UINT BufferSize = 0;
	int BufferCount = 0;
private:

};

