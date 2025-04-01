#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include "iostream"
#include <memory>

// Ό³Έν :
class EngineMesh
{
public:
	// constrcuter destructer
	~EngineMesh();

	static EngineMesh& Get() {
		static EngineMesh Mesh;
		return Mesh;
	}

	// delete Function
	EngineMesh(const EngineMesh& _Other) = delete;
	EngineMesh(EngineMesh&& _Other) noexcept = delete;
	EngineMesh& operator=(const EngineMesh& _Other) = delete;
	EngineMesh& operator=(EngineMesh&& _Other) noexcept = delete;
	void CreateMesh(std::vector<struct FBXMesh*>& _AllMesh, class GraphicDevice* _Device);

protected:
	ID3D11Buffer* CreateBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::string _str, class GraphicDevice* _Device);
	ID3D11Buffer* CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::string _str, class GraphicDevice* _Device);
private:
	EngineMesh();

	std::map<std::string, std::shared_ptr<struct MH>> MeshMap;
};

