#pragma once
#include <d3d11.h>

class SwapChain;

class DeviceContext
{
public:
	DeviceContext();

	ID3D11DeviceContext* Get() {
		if (m_device_context != nullptr) {
			return m_device_context;
		}
		else {
			return nullptr;
		}
	}
	void clearRenderTargetColor(ID3D11RenderTargetView* _rtv, ID3D11DepthStencilView* _dtv, float red, float green, float blue, float alpha);


	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void  release();
	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context = nullptr;
	friend class GraphicsEngine;
};