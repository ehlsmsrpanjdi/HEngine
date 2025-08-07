#include "EngineHlsl.h"
#include "EngineHelper/EngineFile.h"
#include <d3dcompiler.h>
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "EngineHlslResource.h"
#include "iostream"
#include "DefaultResource.h"
#include "SkyResource.h"
#include "vector"
#include "EngineSamplerResource.h"
#include "DefaultSampler.h"
#include "NoneAnimationResource.h"


EngineHlsl::EngineHlsl()
{
	HlslResourceMap.insert(std::make_pair(HlslNamespace::Default, std::make_shared<DefaultResource>()));
	HlslResourceMap.insert(std::make_pair(HlslNamespace::NoneAnimation, std::make_shared<NoneAnimationResource>()));
	HlslResourceMap.insert(std::make_pair(HlslNamespace::Background, std::make_shared<SkyResource>()));

	for (auto& [key, value] : HlslResourceMap) {
		value->Init();
	}


	SamplerVector.push_back(std::make_shared<DefaultSampler>());

	for (auto sampler : SamplerVector) {
		sampler->Init();
	}
}



void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::string_view _filename, std::string_view _filepath, std::shared_ptr<HS> _Hlsl)
{
	std::string str = HString::Upper(_filename.data());

	std::vector<D3D11_INPUT_ELEMENT_DESC> layoutvec = HlslResourceMap[str]->GetLayoutInfo();

	HRESULT hr;

	ID3DBlob* VSBlob = nullptr;
	ID3DBlob* PSBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::wstring ws = HString::StoWC(_filepath.data());
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

	UINT size_layout = static_cast<UINT>(layoutvec.size());
	//HRESULT hr;
	hr = _Device->Get()->CreateInputLayout(layoutvec.data(), size_layout, VSBlob->GetBufferPointer(), (UINT)VSBlob->GetBufferSize(), &_Hlsl->Layout);
	if (hr != S_OK)
	{
		assert(false);
	}
	HlslResourceMap[str]->SetHlsl(_Hlsl);
	EngineHlslResource::SetResource(HlslResourceMap[str], str);

	PSBlob->Release();
	VSBlob->Release();
}



EngineHlsl::~EngineHlsl()
{
	HlslResourceMap.clear();
	SamplerVector.clear();
}

void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager)
{
	for (auto sampler : SamplerVector) {
		sampler->CreateSampler();
	}


	for (const std::pair<const std::string, std::string>& pa : _fileManager->GetAllFile("hlsl")) {
		std::shared_ptr<HS> Hlsl = std::make_shared<HS>();
		CreateHlsl(_Device, pa.first, pa.second, Hlsl);
	}
}
