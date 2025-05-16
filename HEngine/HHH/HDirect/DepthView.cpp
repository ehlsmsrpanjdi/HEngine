#include "DepthView.h"
#include "Windows.h"
#include "GraphicsEngine.h"
#include "GraphicDevice.h"
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
	else {
		m_desc.SampleDesc.Count = 1;
		m_desc.SampleDesc.Quality = 0;
	}

	m_desc.Usage = D3D11_USAGE_DEFAULT;
	m_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_desc.CPUAccessFlags = 0;
	m_desc.MiscFlags = 0;

	ID3D11Texture2D* m_dsb = nullptr;
	HRESULT hr = GraphicsEngine::get()->m_Device->Get()->CreateTexture2D(&m_desc, 0, &m_dsb);
	if (FAILED(hr) || m_dsb == nullptr) {
		assert(false && "Failed to create depth stencil buffer");
		return false;
	}

	hr = GraphicsEngine::get()->m_Device->Get()->CreateDepthStencilView(m_dsb, 0, &m_dsv);
	if (FAILED(hr) || m_dsv == nullptr) {
		m_dsb->Release();
		assert(false && "Failed to create depth stencil view");
		return false;
	}

	m_dsb->Release();
	m_dsb = nullptr;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ID3D11DepthStencilState* depthStencilState;
	GraphicsEngine::get()->m_Device->Get()->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);


	return true;
}

void DepthView::release()
{
	if (m_dsv != nullptr) {
		m_dsv->Release();
		m_dsv = nullptr;
	}
}