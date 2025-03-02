#pragma once
#include <d3d11.h>

class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	//Initialize SwapChain for a window
	bool init(HWND hwnd, UINT width, UINT height);


	bool present(bool vsync);

	//Release the swap chain
	void release();
	~SwapChain();

private:


private:
	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;


private:
	friend class DeviceContext;
	friend class GraphicsEngine;
};