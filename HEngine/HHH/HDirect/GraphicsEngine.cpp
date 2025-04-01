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
#include "EngineHelper/AllStruct.h"

namespace Cbuffer {
	std::string WVP = "WVPMatrix";
}


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
	CreateAllCBuffer();
}

void GraphicsEngine::MeshCreateBuffer(std::vector<FBXMesh*>& _AllMesh)
{
	for (FBXMesh* mesh : _AllMesh) {
		mesh->MeshName = mesh->MeshName;
		mesh->vertices = mesh->vertices;

		UINT Size = sizeof(mesh->vertices[0]);
		UINT ArraySize = mesh->vertices.size();

		CreateBuffer(ArraySize, Size, (UINT*)mesh->vertices.data(), mesh->MeshName);

		mesh->indices = mesh->indices;

		Size = sizeof(mesh->indices[0]); 
		ArraySize = mesh->indices.size();

		CreateIndexBuffer(ArraySize, Size, mesh->indices.data(), mesh->MeshName);
	}
}

void GraphicsEngine::CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::string _str)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_IMMUTABLE;
	buff_desc.ByteWidth = _ArraySize * _Size;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	buff_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = _List;

	HRESULT hr = m_Device->Get()->CreateBuffer(&buff_desc, &init_data, &IndexBufferMap[_str]);
	if (hr != S_OK)
	{
		assert(false);
	}
}

void GraphicsEngine::CreateAllCBuffer()
{
	CreateConstantBuffer(Cbuffer::WVP);
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

void GraphicsEngine::UpdateConstantBuffer(const XMMATRIX& _Matrix, std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (ConstantBufferMap.contains(str) == false) {
		EngineDebug::Error("없는상수버퍼업데이트");
		return;
	}
	// D3D11_MAPPED_SUBRESOURCE 구조체를 선언합니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Constant Buffer를 매핑하여 CPU 메모리 공간에 접근할 수 있도록 합니다.
	HRESULT hr = m_Context->Get()->Map(ConstantBufferMap[str], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (hr != S_OK)
	{
		assert(false);
	}


	// 매핑된 메모리 공간에 WorldViewProj 행렬 데이터를 씁니다.
	memcpy(mappedResource.pData, &_Matrix, sizeof(DirectX::XMMATRIX));

	// 매핑된 메모리를 해제하여 GPU가 다시 리소스에 접근할 수 있도록 합니다.
	m_Context->Get()->Unmap(ConstantBufferMap[str], 0);
}

void GraphicsEngine::CreateBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::string _str)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = _ArraySize * _Size;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = _List;

	HRESULT hr = m_Device->Get()->CreateBuffer(&buff_desc, &init_data, &BufferMap[_str]);

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
	//m_Context->Get()->IASetInputLayout(LayoutMap["vsmain"]);
	m_Context->Get()->IASetIndexBuffer(IndexBufferMap["vsmain"], DXGI_FORMAT_R32_UINT, 0);
	//m_Context->Get()->VSSetShader(VSShader["vsmain"], nullptr, 0);
	//m_Context->Get()->PSSetShader(PSShader["psmain"], nullptr, 0);

	m_Context->Get()->VSSetConstantBuffers(0, 1, &ConstantBufferMap[HString::Upper(Cbuffer::WVP)]);

	m_Context->Get()->DrawIndexed(6, 0, 0); // DrawIndexed를 사용하여 인덱스 버퍼를 사용
	m_Context->Get()->OMSetRenderTargets(1, &m_SwapChain->m_rtv, m_DepthView->m_dsv);
}

#pragma endregion

