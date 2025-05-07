#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "iostream"
#include <Windows.h>
#include <d3d11.h>
#include <map>
// ���� :
struct FMesh;
struct MH;
struct HS;


#pragma region "Buffer"
struct FBuffer {
	DirectX::XMFLOAT4 position = DirectX::XMFLOAT4();
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2();
	DirectX::XMFLOAT2 padding = DirectX::XMFLOAT2();

	int BoneIndices[4] = { 0,0,0,0 };
	float BoneWeights[4] = { 0,0,0,0 };

	DirectX::XMFLOAT3 Normal = DirectX::XMFLOAT3(0.f,0.f,1.f);
	int controlpointindex = -1;
};

struct MH {
	ID3D11Buffer* Vertex = nullptr;
	UINT BufferSize;

	ID3D11Buffer* Index = nullptr;
	UINT IndexBufferSize;

	std::string TextureName;

	DirectX::XMMATRIX MeshMatrix;

	~MH() {
		if (Index != nullptr) {
		Index->Release();
		}
		if (Vertex != nullptr) {
			Vertex->Release();
		}
	}
};

#pragma endregion


struct HS {
	ID3D11VertexShader* VS = nullptr;  //�����
	ID3D11PixelShader* PS = nullptr;	  //�����
	ID3D11InputLayout* Layout = nullptr;     //�����

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
	double time; // �� ����
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 rotation;
};
#pragma endregion

#pragma region "bone"
// ������ � ������ ������ �޴���
struct VertexSkinWeight {
	int boneIndices[4] = { 0 };
	float boneWeights[4] = { 0.0f };
};

// ���� ���� ����
struct SkinnedVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	VertexSkinWeight skinWeight;
};
#pragma endregion