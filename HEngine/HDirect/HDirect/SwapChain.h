#pragma once
#include "d3d11.h"

class SwapChain
{
public:
	void init(HWND _hwnd, RECT _rc);
	void release();
	SwapChain();

	bool present(bool vsync);
private:


private:
	bool isinit = false;
	IDXGISwapChain* m_dxgi_swapchain = nullptr;
	ID3D11RenderTargetView* m_d3d_rtv = nullptr;
	friend class DeviceContext;
};

