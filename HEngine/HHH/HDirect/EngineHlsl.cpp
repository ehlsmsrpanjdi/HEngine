#include "EngineHlsl.h"
#include "EngineHelper/EngineFile.h"
#include <d3dcompiler.h>
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"


EngineHlsl::EngineHlsl() 
{
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
	return nullptr;
}

void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager)
{
	std::shared_ptr<HS> material = std::make_shared<HS>();
	CreateHlsl(_Device, _fileManager, material);
	CreateLayout(_Device, material);
	HlslMap.insert(std::make_pair(HString::Upper("Basic"), material));
}

void EngineHlsl::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager, std::shared_ptr<HS> _Hlsl)
{
	HRESULT hr;


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

