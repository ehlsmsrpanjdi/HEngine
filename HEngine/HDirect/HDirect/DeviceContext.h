#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

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
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void setIndexBuffer(IndexBuffer* index_buffer);


	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

	bool release();
	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context = nullptr;
	friend ConstantBuffer;
	friend class GraphicsEngine;
};