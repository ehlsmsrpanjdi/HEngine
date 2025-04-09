#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "iostream"
#include <Windows.h>
#include <d3d11.h>
#include <map>
// Ό³Έν :
struct FMesh;
struct MH;
struct HS;


struct FScene {
	std::map<std::string, std::vector<std::shared_ptr<FMesh>>> MeshMap;
};

struct FBuffer {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
};

struct FMesh {
	std::vector<FBuffer> vertices; // vertex buffer
	std::vector<UINT> indices;

	int GetSize() {
		sizeof(vertices[0]) + sizeof(indices[0]);
	}
};

struct MH {
	ID3D11Buffer* Index;
	UINT IndexBufferSize;
	ID3D11Buffer* Vertex;
	UINT BufferSize;

	std::string Name;

	~MH() {
		if (Index != nullptr) {
		Index->Release();
		}
		if (Vertex != nullptr) {
			Vertex->Release();
		}
	}
};


struct HS {
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11InputLayout* Layout;
	ID3D11SamplerState* samplerState;

	~HS() {
		if (VS != nullptr) {
			VS->Release();
		}
		if (PS != nullptr) {
			PS->Release();
		}
		if (Layout != nullptr) {
			Layout->Release();
		}
	}
};


struct Tex {

	ID3D11ShaderResourceView* textureSRV = nullptr;

	~Tex() {
		if (textureSRV != nullptr) {
			textureSRV->Release();
		}

	}
};