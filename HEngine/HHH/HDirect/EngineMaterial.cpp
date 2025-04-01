#include "EngineMaterial.h"
#include "EngineHelper/EngineFile.h"
#include <d3dcompiler.h>
#include <cassert>
#include "GraphicDevice.h"
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/HString.h"


EngineMaterial::EngineMaterial() 
{
}

EngineMaterial::~EngineMaterial() 
{
}

MT* EngineMaterial::GetMaterial(std::string_view _str)
{
	std::string str = HString::Upper(_str.data());
	if (MaterialMap.contains(str) == false) {
		assert(false);
	}
	return MaterialMap[str].get();
	return nullptr;
}

void EngineMaterial::CreateMaterial(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager)
{
	std::shared_ptr<MT> material = std::make_shared<MT>();
	CreateHlsl(_Device, _fileManager, material);
	CreateLayout(_Device, material);
	MaterialMap.insert(std::make_pair(HString::Upper("Basic"), material));
}

void EngineMaterial::CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<EngineFile> _fileManager, std::shared_ptr<MT> _Material)
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
		hr = _Device->Get()->CreateVertexShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &_Material->VS);
		if (hr != S_OK)
		{
			assert(false);
			return;
		}
	}

	for (std::pair<const std::string, ID3DBlob*>& pa : PSBlobMap) {
		hr = _Device->Get()->CreatePixelShader(pa.second->GetBufferPointer(), pa.second->GetBufferSize(), nullptr, &_Material->PS);
		if (hr != S_OK)
		{
			assert(false);
			return;
		}
	}
}

void EngineMaterial::CreateLayout(std::shared_ptr<GraphicDevice> _Device, std::shared_ptr<MT> _Material)
{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
			{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0}
		};

		UINT size_layout = ARRAYSIZE(layout);
		HRESULT hr;
		hr = _Device->Get()->CreateInputLayout(layout, size_layout, VSBlobMap["vsmain"]->GetBufferPointer(), (UINT)VSBlobMap["vsmain"]->GetBufferSize(), &_Material->Layout);
		if (hr != S_OK)
		{
			assert(false);
		}
}

