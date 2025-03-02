#include "GraphicDevice.h"

GraphicDevice::GraphicDevice()
{
}

GraphicDevice::~GraphicDevice()
{
}

void GraphicDevice::init(ID3D11Device* _Device)
{

}

void GraphicDevice::release()
{
	if (m_d3d_device != nullptr) {
		m_d3d_device->Release();
		m_d3d_device = nullptr;
	}
}

ID3D11Device* GraphicDevice::Get()
{
	if (m_d3d_device != nullptr) {
		return m_d3d_device;
	}
	else {
		return nullptr;
	}
}

