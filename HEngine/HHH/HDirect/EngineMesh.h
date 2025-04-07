#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include "iostream"
#include <memory>
#include <unordered_map>


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

	void CreateMesh(std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<struct FMesh>>>& _AllMesh, std::shared_ptr<class GraphicDevice> _Device);
	std::unordered_map<std::string, std::shared_ptr<struct MH>>& GetMesh(std::string_view _str);



	void Test(std::shared_ptr<GraphicDevice> _Device);


protected:
	ID3D11Buffer* CreateBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::shared_ptr<class GraphicDevice> _Device);
	ID3D11Buffer* CreateIndexBuffer(UINT _ArraySize, UINT _Size, UINT* _List, std::shared_ptr<class GraphicDevice> _Device);
private:
	EngineMesh();


	std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<struct MH>>> AllMeshMap;
};

