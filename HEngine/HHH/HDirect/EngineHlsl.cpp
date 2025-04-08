#include "EngineHlsl.h"
#include "EngineHelper/EngineFile.h"
#include <d3dcompiler.h>
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "InputElement.h"

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

	SamplerMap["DEFAULT"] = samplerState;
}

void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::string_view _str, std::shared_ptr<HS> _Hlsl)
{
	HRESULT hr;

	ID3DBlob* VSBlob = nullptr;
	ID3DBlob* PSBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::wstring ws = HString::StoWC(_str.data());
	const WCHAR* wcc = ws.c_str();
	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &VSBlob, &errorBlob);

	if (hr != S_OK)
	{
		printf("Shader Compilation Error: %s\n", (char*)errorBlob->GetBufferPointer());
		assert(false);
		return;
	}

	hr = D3DCompileFromFile(wcc, nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &PSBlob, &errorBlob);

	if (hr != S_OK)
	{
		printf("Shader Compilation Error: %s\n", (char*)errorBlob->GetBufferPointer());
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

	std::vector<InputElement> inputElements = ParseHLSLForInputLayout(_str.data());
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = CreateInputLayoutFromHLSL(inputElements);

	UINT size_layout = static_cast<UINT>(layout.size());
	//HRESULT hr;
	hr = _Device->Get()->CreateInputLayout(layout.data(), size_layout, VSBlob->GetBufferPointer(), (UINT)VSBlob->GetBufferSize(), &_Hlsl->Layout);
	if (hr != S_OK)
	{
		assert(false);
	}

	PSBlob->Release();
	VSBlob->Release();
}



EngineHlsl::~EngineHlsl()
{
	HlslMap.clear();
	for (std::pair<const std::string, ID3D11SamplerState*>& pa : SamplerMap) {
		if (pa.second != nullptr) {
			pa.second->Release();
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

	for (const std::pair<const std::string, std::string>& pa : _fileManager->GetAllFile("hlsl")) {
		std::shared_ptr<HS> Hlsl = std::make_shared<HS>();
		Hlsl->samplerState = SamplerMap["DEFAULT"];
		CreateHlsl(_Device, pa.second, Hlsl);
		HlslMap.insert(std::make_pair(HString::Upper(pa.first), Hlsl));
	}
}
