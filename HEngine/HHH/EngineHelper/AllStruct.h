#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "iostream"
#include <Windows.h>
#include <d3d11.h>
#include <map>
// 설명 :
struct FMesh;
struct MH;
struct HS;



struct FBuffer {
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3();
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2();

	int BoneIndices[4] = { 0,0,0,0 };
	float BoneWeights[4] = { 0,0,0,0 };

	int controlpointindex = -1;
};

//struct alignas(16) FBuffer
//{
//	DirectX::XMFLOAT3 position;     // 12 bytes
//	float pad0 = 0.0f;              // 4 bytes - 정렬용
//
//	DirectX::XMFLOAT2 uv;          // 8 bytes
//	float pad1[2] = { 0.0f, 0.0f }; // 8 bytes - 정렬용
//
//	int BoneIndices[4] = { 0, 0, 0, 0 };    // 16 bytes
//	float BoneWeights[4] = { 0.f, 0.f, 0.f, 0.f }; // 16 bytes
//
//	int controlpointindex = -1;     // 4 bytes
//	float pad2[3] = { 0.0f, 0.0f, 0.0f };  // 12 bytes - 정렬용
//};

struct FMesh {
	std::vector<FBuffer> vertices; // vertex buffer
	std::vector<UINT> indices;

	std::string TextureName;

	int GetSize() {
		sizeof(vertices[0]) + sizeof(indices[0]);
	}
};

struct MH {
	ID3D11Buffer* Index;
	UINT IndexBufferSize;
	ID3D11Buffer* Vertex;
	UINT BufferSize;

	std::string TextureName;

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

// 정점이 어떤 본에게 영향을 받는지
struct VertexSkinWeight {
	int boneIndices[4] = { 0 };
	float boneWeights[4] = { 0.0f };
};

// 최종 정점 구조
struct SkinnedVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	VertexSkinWeight skinWeight;
};
