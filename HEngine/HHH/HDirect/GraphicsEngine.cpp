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
#include "BufferInfo.h"
#include "EngineHelper/EngineTransform.h"
#include "EngineHelper/EngineDebug.h"

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

	m_Context->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
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
	SetBuffer();
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

#pragma region "게터"
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

#pragma region "쉐이더"
void GraphicsEngine::CreateHlsl(EngineFile* _fileManager)
{
	CompileShader(_fileManager);
	CreateBuffer();
	CreateLayout();
	CreateIndexBuffer();
	CreateAllCBuffer();
}

void GraphicsEngine::CompileShader(EngineFile* _fileManager)
{
	HRESULT hr;

	std::wstring ws = HString::StoWC(_fileManager->GetFile("Shaderfx.hlsl"));
	const WCHAR* wcc = ws.c_str();
	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &VSBlobMap["vsmain"], &ErrorBlobMap["vsmain"]);

	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &PSBlobMap["psmain"], &ErrorBlobMap["psmain"]);

	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	for (std::pair<const std::string, ID3DBlob*>& pa : VSBlobMap) {
		hr = m_Device->Get()->CreateVertexShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &VSShader[pa.first]);
		if (hr != S_OK)
		{
			assert(false);
			return;
		}
	}

	for (std::pair<const std::string, ID3DBlob*>& pa : PSBlobMap) {
		hr = m_Device->Get()->CreatePixelShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &PSShader[pa.first]);
		if (hr != S_OK)
		{
			assert(false);
			return;
		}
	}
}

void GraphicsEngine::CompileShader(EngineFile* _fileManager, BufferInfo* _Info)
{
	HRESULT hr;
	std::wstring ws = HString::StoWC(_fileManager->GetFile("Shaderfx.hlsl"));
	const WCHAR* wcc = ws.c_str();

	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &_Info->VSBlob, &_Info->ErrorBlob);
	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &_Info->PSBlob, &_Info->ErrorBlob);
	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = m_Device->Get()->CreateVertexShader(_Info->VSBlob->GetBufferPointer(), _Info->VSBlob->GetBufferSize(), nullptr, &_Info->VS);
	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = m_Device->Get()->CreatePixelShader(_Info->PSBlob->GetBufferPointer(), _Info->PSBlob->GetBufferSize(), nullptr, &_Info->PS);
	if (hr != S_OK)
	{
		assert(false);
		return;
	}
}

void GraphicsEngine::CreateLayout(BufferInfo* _Info)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0}
	};

	UINT size_layout = ARRAYSIZE(layout);
	HRESULT hr;

	hr = m_Device->Get()->CreateInputLayout(layout, size_layout, _Info->VSBlob->GetBufferPointer(), (UINT)_Info->VSBlob->GetBufferSize(), &_Info->Layout);
	if (hr != S_OK)
	{
		assert(false);
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
	if (hr != S_OK)
	{
		assert(false);
	}

}

void GraphicsEngine::CreateIndexBuffer()
{
	UINT list[] = {
	0, 1, 2, // 첫 번째 삼각형
	2, 1, 3  // 두 번째 삼각형
	};

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buff_desc.ByteWidth = sizeof(UINT) * ARRAYSIZE(list);
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	buff_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list;

	HRESULT hr = m_Device->Get()->CreateBuffer(&buff_desc, &init_data, &IndexBufferMap["vsmain"]);
	if (hr != S_OK)
	{
		assert(false);
	}
}

void GraphicsEngine::CreateAllCBuffer()
{
	CreateConstantBuffer("World");
	CreateConstantBuffer("Camera");
}

void GraphicsEngine::CreateConstantBuffer(std::string _str)
{
	std::string str = HString::Upper(_str);
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DYNAMIC;
	buff_desc.ByteWidth = sizeof(DirectX::XMMATRIX);
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buff_desc.MiscFlags = 0;
	buff_desc.StructureByteStride = 0;
	HRESULT hr = m_Device->Get()->CreateBuffer(&buff_desc, nullptr, &ConstantBufferMap[str]);
	if (hr != S_OK)
	{
		assert(false);
	}
}

void GraphicsEngine::UpdateConstantBuffer(const EngineTransform& _transform, std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (ConstantBufferMap.contains(str) == false) {
		EngineDebug::Error("없는상수버퍼업데이트");
		return;
	}
	DirectX::XMMATRIX worldViewProj = _transform.GetWorldMatrix();

	// D3D11_MAPPED_SUBRESOURCE 구조체를 선언합니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Constant Buffer를 매핑하여 CPU 메모리 공간에 접근할 수 있도록 합니다.
	HRESULT hr = m_Context->Get()->Map(ConstantBufferMap[str], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (hr != S_OK)
	{
		assert(false);
	}

	// 매핑된 메모리 공간에 WorldViewProj 행렬 데이터를 씁니다.
	memcpy(mappedResource.pData, &worldViewProj, sizeof(DirectX::XMMATRIX));

	// 매핑된 메모리를 해제하여 GPU가 다시 리소스에 접근할 수 있도록 합니다.
	m_Context->Get()->Unmap(ConstantBufferMap[str], 0);
}

void GraphicsEngine::CreateBuffer()
{
	DirectX::XMFLOAT3 list[] = {
		{-0.5f, -0.5f, 0.f},	//좌측 하단
		{-0.5f, 0.5f, 0.f},		//좌측 상단
		{0.5f, -0.5f, 0.f},		//우측 하단
		{0.5f, 0.5f, 0.f}		//우측 상단
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

	HRESULT hr = m_Device->Get()->CreateBuffer(&buff_desc, &init_data, &BufferMap["vsmain"]);

	if (hr != S_OK)
	{
		assert(false);
	}
}

void GraphicsEngine::SetBuffer()
{

	DirectX::XMFLOAT3 list[] = {
		{-0.5f, -0.5f, 0.f},	//좌측 하단
		{-0.5f, 0.5f, 0.f},		//좌측 상단
		{0.5f, -0.5f, 0.f},		//우측 하단
		{0.5f, 0.5f, 0.f}		//우측 상단
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
		// Handle error: BufferInfo not found or not initialized
	}
	//UpdateConstantBuffer();

	m_Context->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Context->Get()->IASetInputLayout(LayoutMap["vsmain"]);
	m_Context->Get()->IASetIndexBuffer(IndexBufferMap["vsmain"], DXGI_FORMAT_R32_UINT, 0);
	m_Context->Get()->VSSetShader(VSShader["vsmain"], nullptr, 0);
	m_Context->Get()->PSSetShader(PSShader["psmain"], nullptr, 0);

	m_Context->Get()->VSSetConstantBuffers(0, 1, &ConstantBufferMap["CAMERA"]);

	m_Context->Get()->DrawIndexed(6, 0, 0); // DrawIndexed를 사용하여 인덱스 버퍼를 사용
	m_Context->Get()->OMSetRenderTargets(1, &m_SwapChain->m_rtv, m_DepthView->m_dsv);
}

void GraphicsEngine::CreateBuffer(enum class BufferType _Type, std::string _vs, std::string _ps)
{
	BufferInfo* buffer = new BufferInfo(_Type);
	switch (_Type)
	{
	case BufferType::VERTEX:
		buffer->Layout = LayoutMap[_vs];
		buffer->VS = VSShader[_vs];
		buffer->PS = PSShader[_ps];

		break;
	case BufferType::INDEX:
		break;
	case BufferType::CONSTANT:
		break;
	case BufferType::STRUCTURED:
		break;
	case BufferType::APPEND:
		break;
	case BufferType::COUNTER:
		break;
	case BufferType::INDIRECT:
		break;
	case BufferType::ARGUMENT:
		break;
	default:
		break;
	}
}

#pragma endregion

