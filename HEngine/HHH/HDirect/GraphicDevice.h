#pragma once
#include <d3d11.h>
class GraphicsEngine;
// Ό³Έν :
class GraphicDevice
{
public:
	// constrcuter destructer
	GraphicDevice();
	~GraphicDevice();

	// delete Function
	GraphicDevice(const GraphicDevice& _Other) = delete;
	GraphicDevice(GraphicDevice&& _Other) noexcept = delete;
	GraphicDevice& operator=(const GraphicDevice& _Other) = delete;
	GraphicDevice& operator=(GraphicDevice&& _Other) noexcept = delete;

	void init(ID3D11Device* _Device);
	void release();
	ID3D11Device* Get();
protected:

private:
	ID3D11Device* m_d3d_device = nullptr;
	friend GraphicsEngine;
};

