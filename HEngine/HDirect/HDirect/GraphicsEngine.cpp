#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "iostream"
#include <d3dcompiler.h>
#include <cassert>
#include "DepthView.h"
#include "unordered_map"
#include "DirectXMath.h"

GraphicsEngine::GraphicsEngine()
{
	m_SwapChain = createSwapChain();
	m_imm_device_context = createDeviceContext();
	m_DepthView = createDepthView();

}

bool GraphicsEngine::init(HWND _hwnd, RECT rc)
{
	m_hwnd = _hwnd;

	createD3DDevice();
	m_SwapChain->init(_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m4xMsaaQuality;
	m_d3d_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	m_DepthView->init(_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	return true;
}


bool GraphicsEngine::release()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();
	m_imm_device_context->release();
	m_d3d_device->Release();
	m_SwapChain->release();

	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::createDeviceContext()
{
	return new DeviceContext();
}

bool GraphicsEngine::createD3DDevice()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_device_context->m_device_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}


	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
	return true;
}

DepthView* GraphicsEngine::createDepthView()
{
	return new DepthView();
}


#pragma region "∞‘≈Õ"
DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}
GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
SwapChain* GraphicsEngine::getSwapChain() {
	return m_SwapChain;
}
#pragma endregion


void GraphicsEngine::ResizeBuffers()
{
	/*m_SwapChain->m_swap_chain->ResizeBuffers();*/
}

void GraphicsEngine::ClearRenderTargetView(float red, float green, float blue, float alpha)
{
	m_imm_device_context->clearRenderTargetColor(m_SwapChain->m_rtv, m_DepthView->mDepthStencil_View, red, green, blue, alpha);
}


#pragma region "Ω¶¿Ã¥ı"
void GraphicsEngine::CreateHlsl()
{
	CompileShader();
	CreateBuffer();
	CreateLayout();
}
void GraphicsEngine::CompileShader()
{
	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &VSBlobMap["vsmain"], &ErrorBlobMap["vsmain"]);
	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &PSBlobMap["psmain"], &ErrorBlobMap["psmain"]);


	for (std::pair<const std::string, ID3DBlob*>& pa : VSBlobMap) {
		m_d3d_device->CreateVertexShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &VSShader[pa.first]);
	}

	for (std::pair<const std::string, ID3DBlob*>& pa : PSBlobMap) {
		m_d3d_device->CreatePixelShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &PSShader[pa.first]);
	}

}

void GraphicsEngine::CreateBuffer()
{

	VSBlobMap["vsmain"];
	DirectX::XMFLOAT3 list[] =
	{
		//X - Y - Z
		{-0.5f,-0.5f,0.0f}, // POS1
		{0.0f,0.5f,0.0f}, // POS2
		{ 0.5f,-0.5f,0.0f}
	};
	UINT size_vertex = sizeof(DirectX::XMFLOAT3);
	UINT size_list = ARRAYSIZE(list);

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list;

	if (FAILED(m_d3d_device->CreateBuffer(&buff_desc, &init_data, &BufferMap["vsmain"])))
	{
		return;
	}
}


void GraphicsEngine::CreateLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0}
	};

	UINT size_layout = ARRAYSIZE(layout);


	if (FAILED(m_d3d_device->CreateInputLayout(layout, size_layout, VSBlobMap["vsmain"]->GetBufferPointer(), (UINT)VSBlobMap["vsmain"]->GetBufferSize(), &LayoutMap["vsmain"])))
	{
		return;
	}

}

#pragma endregion

