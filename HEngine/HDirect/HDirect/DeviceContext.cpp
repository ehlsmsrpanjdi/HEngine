#include "DeviceContext.h"
#include "SwapChain.h"
bool DeviceContext::clearRenderTargetColor(SwapChain* _swapchain, float _red, float _green, float _blue, float _alpha)
{
	FLOAT clear_color[] = { _red, _green, _blue, _alpha };
	m_d3d_context->ClearRenderTargetView(_swapchain->m_d3d_rtv, clear_color);
	return false;
}

void DeviceContext::release()
{
	m_d3d_context->Release();
	delete this;
}
