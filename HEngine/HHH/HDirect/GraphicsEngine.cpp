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
#include "EngineMesh.h"
#include "EngineHlsl.h"

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
void GraphicsEngine::CreateHlsl(std::shared_ptr<EngineFile> _fileManager)
{
	CreateAllCBuffer();
	EngineHlsl::Get().CreateHlsl(m_Device, _fileManager);
	

	//CreateTexture(m_Device->Get(), m_Context->Get(), HString::StoWC(_fileManager->GetFile("png", "heart")).c_str());
}

void GraphicsEngine::CreateMesh(std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<FMesh>>>& _AllMesh)
{
	EngineMesh::Get().CreateMesh(_AllMesh, m_Device);
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

//void GraphicsEngine::CreateTexture(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filename)
//{
//	Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;
//	HRESULT hr;
//	hr = CoInitialize(nullptr);
//	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
//
//	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
//	wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
//
//	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
//	decoder->GetFrame(0, &frame);
//
//	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
//	wicFactory->CreateFormatConverter(&converter);
//	converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);
//
//	UINT width, height;
//	frame->GetSize(&width, &height);
//
//	std::vector<UINT8> imageData(width * height * 4);
//	converter->CopyPixels(nullptr, width * 4, imageData.size(), imageData.data());
//
//	D3D11_TEXTURE2D_DESC textureDesc = {};
//	textureDesc.Width = width;
//	textureDesc.Height = height;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1;
//	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//
//	D3D11_SUBRESOURCE_DATA initData = {};
//	initData.pSysMem = imageData.data();
//	initData.SysMemPitch = width * 4;
//
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
//	device->CreateTexture2D(&textureDesc, &initData, &texture);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = textureDesc.Format;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = 1;
//
//	device->CreateShaderResourceView(texture.Get(), &srvDesc, &textureSRV);
//
//}

void GraphicsEngine::Render(HS* _Hlsl, MH* _Mesh)
{
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	UINT offset = 0;
	m_Context->Get()->OMSetRenderTargets(1, &m_SwapChain->m_rtv, m_DepthView->m_dsv);
	m_Context->Get()->IASetVertexBuffers(0, 1, &_Mesh->Vertex, &_Mesh->BufferSize, &offset);
	m_Context->Get()->IASetIndexBuffer(_Mesh->Index, DXGI_FORMAT_R32_UINT, 0);
	m_Context->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Context->Get()->IASetInputLayout(_Hlsl->Layout);
	m_Context->Get()->VSSetShader(_Hlsl->VS, nullptr, 0);
	m_Context->Get()->PSSetShader(_Hlsl->PS, nullptr, 0);
	m_Context->Get()->VSSetConstantBuffers(0, 1, &ConstantBufferMap[HString::Upper(Cbuffer::WVP)]);
	//m_Context->Get()->PSSetShaderResources(0, 1, &textureSRV);
	m_Context->Get()->PSSetSamplers(0, 1, &_Hlsl->samplerState);
	m_Context->Get()->DrawIndexed(_Mesh->IndexBufferSize, 0, 0);
}

HS* GraphicsEngine::GetHlsl(std::string_view _str)
{
	return EngineHlsl::Get().GetHlsl(_str);
}

std::unordered_map<std::string, std::shared_ptr<MH>>& GraphicsEngine::GetMesh(std::string_view _str)
{
	return EngineMesh::Get().GetMesh(_str);
}

#pragma endregion

