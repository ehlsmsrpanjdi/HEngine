#include "EngineHlsl.h"
#include "EngineHelper/EngineFile.h"
#include <d3dcompiler.h>
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"


EngineHlsl::EngineHlsl()
{
}

void EngineHlsl::createSampler(ID3D11Device* _Device)
{
	ID3D11SamplerState* samplerState = nullptr;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	_Device->CreateSamplerState(&sampDesc, &samplerState);
	GraphicsEngine::get()->GetContext()->Get()->PSSetSamplers(0, 1, &samplerState);

	SamplerMap["Default"] = samplerState;
}

void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::string_view _str, std::shared_ptr<HS> _Hlsl)
{
	HRESULT hr;

	ID3DBlob* VSBlob = nullptr;
	ID3DBlob* PSBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	std::wstring ws = HString::StoWC(_str.data());
	const WCHAR* wcc = ws.c_str();
	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &VSBlob, &ErrorBlob);

	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &PSBlob, &ErrorBlob);

	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = _Device->Get()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &_Hlsl->VS);
	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	hr = _Device->Get()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &_Hlsl->PS);
	if (hr != S_OK)
	{
		assert(false);
		return;
	}

	PSBlob->Release();
	VSBlob->Release();
	ErrorBlob->Release();
}


EngineHlsl::~EngineHlsl()
{
	for (std::pair<const std::string, ID3DBlob*>& pa : VSBlobMap) {
		if (pa.second != nullptr) {
			pa.second->Release();
		}
	}
	for (std::pair<const std::string, ID3DBlob*>& pa : PSBlobMap) {
		if (pa.second != nullptr) {
			pa.second->Release();
		}
	}
	for (std::pair<const std::string, ID3D11InputLayout*>& pa : LayoutMap) {
		if (pa.second != nullptr) {
			pa.second->Release();
		}
	}
	for (std::pair<const std::string, ID3DBlob*>& pa : ErrorBlobMap) {
		if (pa.second != nullptr) {
			pa.second->Release();
		}
	}
	for (std::pair<const std::string, std::shared_ptr<HS>>& pa : HlslMap) {
		if (pa.second != nullptr) {
			pa.second->PS->Release();
			pa.second->VS->Release();
			pa.second->Layout->Release();
		}
	}
}

HS* EngineHlsl::GetHlsl(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (HlslMap.contains(str) == false) {
		assert(false);
	}
	return HlslMap[str].get();
}

void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager)
{
	createSampler(_Device->Get());
	std::shared_ptr<HS> Hlsl = std::make_shared<HS>();
	Hlsl->samplerState = SamplerMap["Default"];
	CreateHlsl(_Device, _fileManager, Hlsl);
	CreateLayout(_Device, Hlsl);
	HlslMap.insert(std::make_pair(HString::Upper("Basic"), Hlsl));



	for (const std::pair<const std::string, std::string>& pa : _fileManager->GetAllFile("hlsl")) {
		std::shared_ptr<HS> Hlsl = std::make_shared<HS>();
		Hlsl->samplerState = SamplerMap["Default"];
		CreateHlsl(_Device, _fileManager, Hlsl);
		CreateLayout(_Device, Hlsl);
		HlslMap.insert(std::make_pair(HString::Upper(pa.first), Hlsl));
	}
}


void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager, std::shared_ptr<HS> _Hlsl)
{
	HRESULT hr;

	ID3DBlob* VSBlob = nullptr;
	ID3DBlob* PSBlob = nullptr;

	std::wstring ws = HString::StoWC(_fileManager->GetFile("hlsl", "Shaderfx"));
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
		hr = _Device->Get()->CreateVertexShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &_Hlsl->VS);
		if (hr != S_OK)
		{
			assert(false);
			return;
		}
	}

	for (std::pair<const std::string, ID3DBlob*>& pa : PSBlobMap) {
		hr = _Device->Get()->CreatePixelShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &_Hlsl->PS);
		if (hr != S_OK)
		{
			assert(false);
			return;
		}
	}
}

void EngineHlsl::CreateLayout(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<HS> _Hlsl)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0}
	};

	UINT size_layout = ARRAYSIZE(layout);
	HRESULT hr;
	hr = _Device->Get()->CreateInputLayout(layout, size_layout, VSBlobMap["vsmain"]->GetBufferPointer(), (UINT)VSBlobMap["vsmain"]->GetBufferSize(), &_Hlsl->Layout);
	if (hr != S_OK)
	{
		assert(false);
	}
}

