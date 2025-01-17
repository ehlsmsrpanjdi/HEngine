#pragma once
#include <d3d11.h>
class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* _context)
		:m_d3d_context(_context)
	{

	}
	bool clearRenderTargetColor(class SwapChain* _swapchain, float _red, float _green, float _blue, float _alpha);

	void release();

private:
	ID3D11DeviceContext* m_d3d_context = nullptr;
	friend class GraphicDevice;
};

