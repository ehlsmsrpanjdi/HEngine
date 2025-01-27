#include "DepthView.h"
#include "Windows.h"
#include "GraphicsEngine.h"
#include <cassert>
bool DepthView::init(HWND hwnd, UINT width, UINT height)
{
	ZeroMemory(&m_desc, sizeof(m_desc));
	m_desc.Width = width;
	m_desc.Height = height;
	m_desc.MipLevels = 1;
	m_desc.ArraySize = 1;
	m_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (GraphicsEngine::get()->m4xMsaaQuality > 0) {
		m_desc.SampleDesc.Count = 4;
		m_desc.SampleDesc.Quality = GraphicsEngine::get()->m4xMsaaQuality - 1;
	}

	m_desc.Usage = D3D11_USAGE_DEFAULT;
	m_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_desc.CPUAccessFlags = 0;
	m_desc.MiscFlags = 0;

	HRESULT hr = GraphicsEngine::get()->m_d3d_device->CreateTexture2D(&m_desc, 0, &m_DepthStencil_Buffer);
	if (hr != S_OK) {
		assert("Warrning");
	}
	hr = GraphicsEngine::get()->m_d3d_device->CreateDepthStencilView(m_DepthStencil_Buffer, 0, &mDepthStencil_View);
	if (hr != S_OK) {
		assert("Warrning");
	}
	return true;
}

bool DepthView::release()
{
	m_DepthStencil_Buffer->Release();
	mDepthStencil_View->Release();
	return false;
}
