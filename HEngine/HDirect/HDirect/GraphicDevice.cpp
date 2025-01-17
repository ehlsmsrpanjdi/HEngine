#include "GraphicDevice.h"
#include "SwapChain.h"

void GraphicDevice::init()
{
	if (isinit) {
		return;
	}

	D3D_DRIVER_TYPE types[] = {
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE
	};
	UINT8 num_drivertype = ARRAYSIZE(types);

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_0
	};


	for (UINT8 index = 0; index < num_drivertype;) {
		HRESULT hr = D3D11CreateDevice(NULL, types[index], NULL, NULL, levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &m_d3d_device, &m_d3d_featurelevel, &m_d3d_devicecontext);
		if (hr == S_OK) {
			isinit = true;
			break;
		}
	}

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

}

void GraphicDevice::release()
{
	m_d3d_device->Release();
	m_d3d_devicecontext->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

}

GraphicDevice::GraphicDevice()
{

}

GraphicDevice* GraphicDevice::GetEngine()
{
	static GraphicDevice Device = GraphicDevice();
	return &Device;
}

