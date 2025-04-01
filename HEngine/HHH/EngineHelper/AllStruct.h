#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "iostream"
#include <Windows.h>
#include <d3d11.h>
// Ό³Έν :
struct FBXMesh {
	std::vector<DirectX::XMFLOAT3> vertices;
	//std::vector<DirectX::XMFLOAT3> normals;
	//std::vector<DirectX::XMFLOAT2> uvs;
	std::vector<UINT> indices;
	std::string MeshName;
};

struct MH {
	ID3D11Buffer* Index;
	UINT IndexBufferSize;
	ID3D11Buffer* Vertex;
	UINT BufferSize;

	~MH() {
		if (Index != nullptr) {
		Index->Release();
		}
		if (Vertex != nullptr) {
			Vertex->Release();
		}
	}
};


struct MT {
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11InputLayout* Layout;

	~MT() {
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
