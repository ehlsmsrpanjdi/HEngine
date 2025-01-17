#pragma once
#include "d3d11.h"
class SwapChain
{
public:
	void init(HWND _hwnd, RECT _rc);
	void release();
	SwapChain();
private:


private:
	bool isinit = false;
	IDXGISwapChain* m_dxgi_swapchain = nullptr;
};

