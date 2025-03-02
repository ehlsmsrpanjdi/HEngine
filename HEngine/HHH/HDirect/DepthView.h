#pragma once
#include <d3d11.h>

class DepthView
{
public:


	bool init(HWND hwnd, UINT width, UINT height);

	void release();

private:
	ID3D11DepthStencilView* m_dsv = nullptr;
	friend class DeviceContext;

private:
	D3D11_TEXTURE2D_DESC m_desc = D3D11_TEXTURE2D_DESC();
	friend class GraphicsEngine;
};

