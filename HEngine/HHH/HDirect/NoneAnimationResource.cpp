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

	Layout.push_back({ "BONEINDICES",  0, DXGI_FORMAT_R32_UINT,           0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "BONEINDICES",  1, DXGI_FORMAT_R32_UINT,           0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "BONEINDICES",  2, DXGI_FORMAT_R32_UINT,           0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "BONEINDICES",  3, DXGI_FORMAT_R32_UINT,           0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	Layout.push_back({ "BONEWEIGHTS",  0, DXGI_FORMAT_R32_FLOAT,          0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "BONEWEIGHTS",  1, DXGI_FORMAT_R32_FLOAT,          0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "BONEWEIGHTS",  2, DXGI_FORMAT_R32_FLOAT,          0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	Layout.push_back({ "BONEWEIGHTS",  3, DXGI_FORMAT_R32_FLOAT,          0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	Layout.push_back({ "NORMAL" ,        0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 });
}