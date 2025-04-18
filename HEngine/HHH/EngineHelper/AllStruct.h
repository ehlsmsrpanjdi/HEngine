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


#pragma region "Buffer"
struct FBuffer {
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3();
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2();

	int BoneIndices[4] = { 0,0,0,0 };
	float BoneWeights[4] = { 0,0,0,0 };

	int controlpointindex = -1;
};

struct MH {
	ID3D11Buffer* Vertex = nullptr;
	UINT BufferSize;

	ID3D11Buffer* Index = nullptr;
	UINT IndexBufferSize;

	std::string TextureName;

	DirectX::XMMATRIX tempmatrix;

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
	ID3D11VertexShader* VS = nullptr;
	ID3D11PixelShader* PS = nullptr;
	ID3D11InputLayout* Layout = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

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
#pragma endregion

#pragma region "anim"
struct AnimMetaData {
	std::string name;
	double startTime;
	double endTime;
};

struct KeyFrame {
	double time; // 초 단위
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 rotation;
};
#pragma endregion

#pragma region "bone"
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
#pragma endregion