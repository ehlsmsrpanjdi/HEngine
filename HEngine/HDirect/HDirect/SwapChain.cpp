#include "SwapChain.h"
#include "GraphicDevice.h"
void SwapChain::init(HWND _hwnd, RECT _rc)
{
	if (isinit) {
		return;
	}
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.BufferCount = 1;
	desc.BufferDesc.Width = _rc.right - _rc.left;
	desc.BufferDesc.Height = _rc.bottom - _rc.top;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = _hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	GraphicDevice::GetEngine()->m_dxgi_factory->CreateSwapChain(GraphicDevice::GetEngine()->m_d3d_device, &desc, &m_dxgi_swapchain);
	isinit = true;
}

void SwapChain::release()
{
	m_dxgi_swapchain->Release();
	delete this;
}

SwapChain::SwapChain()
{
}
