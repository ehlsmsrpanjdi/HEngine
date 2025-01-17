#pragma once
#include "d3d11.h"

class GraphicDevice
{
public:
	void init();
	void release();
	GraphicDevice();

	static GraphicDevice* GetEngine();


	class DeviceContext* GetContext();
private:
	D3D_FEATURE_LEVEL m_d3d_featurelevel = D3D_FEATURE_LEVEL();
	bool isinit = false;


	ID3D11Device* m_d3d_device = nullptr;                 // Device °´Ã¼
	class DeviceContext* m_d3d_context = nullptr;   // Device Context °´Ã¼

private:
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory= nullptr;

	friend class SwapChain;
};

