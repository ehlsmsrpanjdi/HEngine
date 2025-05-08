#include "NoneAnimationResource.h"

NoneAnimationResource::NoneAnimationResource() 
{
}

NoneAnimationResource::~NoneAnimationResource() 
{
}

void NoneAnimationResource::Init()
{
	Layout.push_back({ "POSITION",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 });

	Layout.push_back({ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,       0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	Layout.push_back({ "PADDING", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 });
}