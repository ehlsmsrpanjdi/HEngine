#pragma once
#include <d3d11.h>

//typedef struct D3D11_TEXTURE2D_DESC {
//	UINT Width;
//	UINT Height;
//	UINT MipLevels;
//	UINT ArraySize;
//	DXGI_FORMAT Format;
//	DXGI_SAMPLE_DESC SampleDesc;
//	D3D11_USAGE Usage;
//	UINT BindFlags;
//	UINT CPUACCESSFlags;
//	UINT MiscFlags;
//} ;

class DepthView
{
public:


	bool init(HWND hwnd, UINT width, UINT height);

	bool release();

private:
	ID3D11Texture2D* m_DepthStencil_Buffer = nullptr;
	ID3D11DepthStencilView* mDepthStencil_View = nullptr;
	friend class DeviceContext;

private:
	D3D11_TEXTURE2D_DESC m_desc = D3D11_TEXTURE2D_DESC();
};

