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
#include "HDirect/ConstantBufferStruct.h"
#include "AllConstantBufferRes.h"
#include "ConstantBufferResource.h"

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
	TestVertex->Release();
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

struct SimpleVertex
{
	DirectX::XMFLOAT4 Position;
};

void GraphicsEngine::Test()
{
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT4(0.0f, 0.5f, 0.0f,1.0f)}, // top
		{ DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f,1.0f)}, // right
		{ DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f,1.0f)}  // left
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	HRESULT hr = m_Device->Get()->CreateBuffer(&bd, &initData, &TestVertex);
	if (FAILED(hr))
	{
		assert(false);
	}
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
	AllConstantBufferRes AllRes;
	AllRes.Init();
	AllRes.CreateAllBuffer(m_Device);
	Test();
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



	std::string str = _Mesh->TextureName;
	if (TextureMap->contains(str) == false) {
		_Mesh->TextureName = "DEFAULT";
		str = "DEFAULT";
	}
	ID3D11ShaderResourceView* tex = (*TextureMap)[str]->textureSRV;
	//TextureMap->find(str)->second->textureSRV;
	m_Context->Get()->PSSetShaderResources(0, 1, &tex);
	m_Context->Get()->PSSetSamplers(0, 1, &_Hlsl->samplerState);
	m_Context->Get()->DrawIndexed(_Mesh->IndexBufferSize, 0, 0);


	UINT stride = sizeof(SimpleVertex);
	offset = 0;
	m_Context->Get()->IASetVertexBuffers(0, 1, &TestVertex, &stride, &offset);
	m_Context->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Context->Get()->Draw(3, 0);
}

void GraphicsEngine::CollisionRender(HS* _Hlsl, MH* _Mesh)
{
	UINT offset = 0;
	m_Context->Get()->OMSetRenderTargets(1, &m_SwapChain->m_rtv, m_DepthView->m_dsv);
	m_Context->Get()->IASetVertexBuffers(0, 1, &_Mesh->Vertex, &_Mesh->BufferSize, &offset);
	m_Context->Get()->IASetIndexBuffer(_Mesh->Index, DXGI_FORMAT_R32_UINT, 0);
	m_Context->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	m_Context->Get()->IASetInputLayout(_Hlsl->Layout);
	m_Context->Get()->VSSetShader(_Hlsl->VS, nullptr, 0);
	m_Context->Get()->PSSetShader(_Hlsl->PS, nullptr, 0);
	ID3D11ShaderResourceView* tex = (*TextureMap)["DEFAULT"]->textureSRV;
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

