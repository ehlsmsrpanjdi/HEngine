#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include <d3dcompiler.h>
#include <cassert>
#include "DepthView.h"
#include "DirectXMath.h"
#include "EngineHelper/EngineFile.h"
#include "EngineHelper/HString.h"
#include "GraphicDevice.h"
#include "EngineHelper/EngineTransform.h"
#include "EngineHelper/EngineDebug.h"
#include "EngineHelper/AllStruct.h"
#include "EngineScene.h"
#include "EngineHlsl.h"
#include "EngineTexture.h"
#include "EngineHelper/EngineFScene.h"
#include "EngineHelper/EngineNamespace.h"



#pragma region "Init"

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

	//D3D11_RASTERIZER_DESC rasterDesc = {};
	//rasterDesc.CullMode = D3D11_CULL_NONE;  // 컬링 비활성화
	//rasterDesc.FillMode = D3D11_FILL_SOLID;

	//ID3D11RasterizerState* rasterState;
	//m_Device->Get()->CreateRasterizerState(&rasterDesc, &rasterState);
	//GetContext()->Get()->RSSetState(rasterState);

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
	TextureMap = nullptr;

	for (std::pair<const std::string, ID3D11Buffer*>& pair : ConstantBufferMap) {
		if (pair.second != nullptr) {
			pair.second->Release();
		}
	}
	ConstantBufferMap.clear();

	return true;
}



void GraphicsEngine::Clear(float red, float green, float blue, float alpha)
{
	m_Context->clearRenderTargetColor(m_SwapChain->m_rtv, m_DepthView->m_dsv, red, green, blue, alpha);
}

void GraphicsEngine::Present(bool _bool)
{
	m_SwapChain->present(_bool);
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

GraphicsEngine::~GraphicsEngine()
{
}

#pragma endregion

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
void GraphicsEngine::CreateHlsl(std::shared_ptr<EngineFile> _fileManager)
{
	CreateAllCBuffer();
	EngineHlsl::Get().CreateHlsl(m_Device, _fileManager);
}

void GraphicsEngine::CreateScene(std::vector<std::shared_ptr<EngineFScene>>& _Scenes)
{
	EngineScene::Get().CreateScene(_Scenes, m_Device);
}

void GraphicsEngine::CreateTexture(std::shared_ptr<class EngineFile> _fileManager)
{
	EngineTexture::GetInst().CreateAllTexture(m_Device->Get(), m_Context->Get(), _fileManager);
	TextureMap = &EngineTexture::GetInst().GetTexture();
}


void GraphicsEngine::CreateAllCBuffer()
{
	CreateWVPBuffer();
	CreateMeshBuffer();
	CreateAnimationBuffer();
}

void GraphicsEngine::CreateWVPBuffer()
{
	std::string str = Cbuffer::WVP;
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

void GraphicsEngine::CreateMeshBuffer()
{
	std::string upperName = Cbuffer::MESH;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(DirectX::XMMATRIX);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	HRESULT hr = m_Device->Get()->CreateBuffer(&desc, nullptr, &ConstantBufferMap[upperName]);
	if (FAILED(hr))
	{
		assert(false && "CreateBoneMatrixBuffer Failed");
	}
}

void GraphicsEngine::CreateAnimationBuffer()
{
	std::string upperName = Cbuffer::ANI;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMMATRIX) * 100);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	HRESULT hr = m_Device->Get()->CreateBuffer(&desc, nullptr, &ConstantBufferMap[upperName]);
	if (FAILED(hr))
	{
		assert(false && "CreateBoneMatrixBuffer Failed");
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

void GraphicsEngine::UpdateConstantBuffer(const std::vector<DirectX::XMMATRIX>& matrices, std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (!ConstantBufferMap.contains(str)) {
		EngineDebug::Error("없는 상수버퍼 업데이트");
		return;
	}

	size_t bufferSize = sizeof(DirectX::XMMATRIX) * matrices.size();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_Context->Get()->Map(ConstantBufferMap[str], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) {
		assert(false && "상수버퍼 매핑 실패");
		return;
	}

	memcpy(mappedResource.pData, matrices.data(), bufferSize);
	m_Context->Get()->Unmap(ConstantBufferMap[str], 0);
}

void GraphicsEngine::Render(HS* _Hlsl, MH* _Mesh)
{
	UINT offset = 0;
	m_Context->Get()->OMSetRenderTargets(1, &m_SwapChain->m_rtv, m_DepthView->m_dsv);
	m_Context->Get()->IASetVertexBuffers(0, 1, &_Mesh->Vertex, &_Mesh->BufferSize, &offset);
	m_Context->Get()->IASetIndexBuffer(_Mesh->Index, DXGI_FORMAT_R32_UINT, 0);
	m_Context->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Context->Get()->IASetInputLayout(_Hlsl->Layout);
	m_Context->Get()->VSSetShader(_Hlsl->VS, nullptr, 0);
	m_Context->Get()->PSSetShader(_Hlsl->PS, nullptr, 0);
	m_Context->Get()->VSSetConstantBuffers(0, 1, &ConstantBufferMap[Cbuffer::WVP]);
	m_Context->Get()->VSSetConstantBuffers(1, 1, &ConstantBufferMap[Cbuffer::ANI]);
	m_Context->Get()->VSSetConstantBuffers(2, 1, &ConstantBufferMap[Cbuffer::MESH]);

	std::string str = _Mesh->TextureName;
	if (TextureMap->contains(str) == false) {
		_Mesh->TextureName = "DEFAULT";
		str = "DEFAULT";
	}
	ID3D11ShaderResourceView* tex = (*TextureMap)[str]->textureSRV;
	TextureMap->find(str)->second->textureSRV;
	m_Context->Get()->PSSetShaderResources(0, 1, &tex);
	m_Context->Get()->PSSetSamplers(0, 1, &_Hlsl->samplerState);
	m_Context->Get()->DrawIndexed(_Mesh->IndexBufferSize, 0, 0);
}

HS* GraphicsEngine::GetHlsl(std::string_view _str)
{
	return EngineHlsl::Get().GetHlsl(_str);
}

std::shared_ptr<FScene> GraphicsEngine::GetScene(std::string_view _str)
{
	return EngineScene::Get().GetScene(_str);
}

#pragma endregion

