#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "iostream"
#include <Windows.h>
// ���� :
struct FBXMesh {
	std::vector<DirectX::XMFLOAT3> vertices;
	//std::vector<DirectX::XMFLOAT3> normals;
	//std::vector<DirectX::XMFLOAT2> uvs;
	std::vector<UINT> indices;
	std::string MeshName;
};