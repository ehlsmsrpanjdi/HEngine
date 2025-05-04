#include "DeviceContext.h"
#include "SwapChain.h"
#include "GraphicsEngine.h"

DeviceContext::DeviceContext()
{
}

void DeviceContext::clearRenderTargetColor(ID3D11RenderTargetView* _rtv, ID3D11DepthStencilView* _dtv, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(_rtv, clear_color);
	m_device_context->ClearDepthStencilView(_dtv, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_device_context->OMSetRenderTargets(1, &_rtv, _dtv);

}



void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}


void DeviceContext::release()
{
	if (m_device_context != nullptr) {
	m_device_context->Release();
	m_device_context = nullptr;
	}
}

DeviceContext::~DeviceContext()
{
}