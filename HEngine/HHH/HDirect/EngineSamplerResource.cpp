#include "EngineSamplerResource.h"
#include "GraphicDevice.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "cassert"


namespace SamplerNameSpace {
	std::string DEFAULT = "DEFAULT";;
}


EngineSamplerResource::EngineSamplerResource()
{
}

EngineSamplerResource::~EngineSamplerResource()
{
	if (SamplerData != nullptr) {
		SamplerData->Release();
	}
}

void EngineSamplerResource::CreateSampler()
{
	ID3D11SamplerState* samplerState = nullptr;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GraphicsEngine::get()->GetDevice()->Get()->CreateSamplerState(&sampDesc, &samplerState);

	SamplerData = samplerState;

	if (SamplerName == "NONE") {
		assert(false);
	}
	EngineSamplerResource::SetResource(shared_from_this(), SamplerName);
}

void EngineSamplerResource::Init()
{
}


