#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include <d3dcompiler.h>
#include <cassert>
#include "DepthView.h"
#include "unordered_map"
#include "DirectXMath.h"
#include "EngineHelper/EngineFile.h"
#include "EngineHelper/HString.h"
#include "GraphicDevice.h"


GraphicsEngine::GraphicsEngine()
{
	m_Context = std::make_shared<DeviceContext>();
	m_SwapChain = std::make_shared<SwapChain>();
	m_DepthView = std::make_shared<DepthView>();
	m_Device = std::make_shared<GraphicDevice>();
}

bool GraphicsEngine::init(HWND _hwnd, RECT rc)
{
	m_hwnd = _hwnd;
	WindowSize = rc;
	createD3DDevice();
	m_Device->Get()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	m_SwapChain->init(_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m4xMsaaQuality;
	assert(m4xMsaaQuality > 0);

	m_DepthView->init(_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	return true;
}


bool GraphicsEngine::release()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();
	m_Device = nullptr;
	m_Context = nullptr;
	m_SwapChain = nullptr;
	m_DepthView = nullptr;

	return true;
}

void GraphicsEngine::Presnet(float red, float green, float blue, float alpha, bool _bool)
{
	m_Context->clearRenderTargetColor(m_SwapChain->m_rtv, m_DepthView->m_dsv, red, green, blue, alpha);
	m_SwapChain->present(_bool);
}

GraphicsEngine::~GraphicsEngine()
{
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
			num_feature_levels, D3D11_SDK_VERSION, &m_Device->m_d3d_device, &m_feature_level, &m_Context->m_device_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}


	m_Device->m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);
	return true;
}

#pragma region "°ÔÅÍ"
DeviceContext* GraphicsEngine::GetContext()
{
	return this->m_Context.get();
}
GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
SwapChain* GraphicsEngine::getSwapChain() {
	return m_SwapChain.get();
}
GraphicDevice* GraphicsEngine::GetDevice()
{
	if (m_Device != nullptr)
	{
		return m_Device.get();
	}
	else
	{
		return nullptr;
	}
}
#pragma endregion


void GraphicsEngine::ResizeBuffers()
{
	/*m_SwapChain->m_swap_chain->ResizeBuffers();*/
}

void GraphicsEngine::ClearRenderTargetView(float red, float green, float blue, float alpha)
{
	//m_Context->clearRenderTargetColor(m_SwapChain->m_rtv, m_DepthView->mDepthStencil_View, red, green, blue, alpha);
	m_Context->clearRenderTargetColor(m_SwapChain->m_rtv, nullptr, red, green, blue, alpha);
}


#pragma region "½¦ÀÌ´õ"
void GraphicsEngine::CreateHlsl(EngineFile* _fileManager)
{
	CompileShader(_fileManager);
	CreateBuffer();
	CreateLayout();
}

void GraphicsEngine::CompileShader(EngineFile* _fileManager)
{
	//vsblob
	HRESULT hr;
	std::wstring ws = HString::StoWC(_fileManager->Files["Shaderfx.hlsl"]);
	const WCHAR* wcc = ws.c_str();
	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &VSBlobMap["vsmain"], &ErrorBlobMap["vsmain"]);

	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &PSBlobMap["psmain"], &ErrorBlobMap["psmain"]);


	for (std::pair<const std::string, ID3DBlob*>& pa : VSBlobMap) {
		hr = m_Device->Get()->CreateVertexShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &VSShader[pa.first]);
	}

	for (std::pair<const std::string, ID3DBlob*>& pa : PSBlobMap) {
		hr = m_Device->Get()->CreatePixelShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &PSShader[pa.first]);
	}

}

void GraphicsEngine::CreateBuffer()
{
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

	if (FAILED(m_Device->Get()->CreateBuffer(&buff_desc, &init_data, &BufferMap["vsmain"])))
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
	HRESULT hr;
	hr = m_Device->Get()->CreateInputLayout(layout, size_layout, VSBlobMap["vsmain"]->GetBufferPointer(), (UINT)VSBlobMap["vsmain"]->GetBufferSize(), &LayoutMap["vsmain"]);
	if (FAILED(m_Device->Get()->CreateInputLayout(layout, size_layout, VSBlobMap["vsmain"]->GetBufferPointer(), (UINT)VSBlobMap["vsmain"]->GetBufferSize(), &LayoutMap["vsmain"])))
	{
		return;
	}

}

void GraphicsEngine::SetBuffer()
{
	DirectX::XMFLOAT3 list[] =
	{
		//X - Y - Z
		{-0.5f,-0.5f,0.0f}, // POS1
		{0.0f,0.5f,0.0f}, // POS2
		{ 0.5f,-0.5f,0.0f}
	};
	UINT size_list = ARRAYSIZE(list);
	UINT vertex_size = sizeof(DirectX::XMFLOAT3);

	if (BufferMap.find("vsmain") != BufferMap.end() && BufferMap["vsmain"] != nullptr)
	{
		UINT stride = vertex_size;
		UINT offset = 0;
		m_Context->Get()->IASetVertexBuffers(0, 1, &BufferMap["vsmain"], &stride, &offset);
	}
	else
	{
		// Handle error: buffer not found or not initialized
	}
	m_Context->drawIndexedTriangleList(0, 3, 0);
}

#pragma endregion

