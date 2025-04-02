#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <Windows.h>

#include "fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")

// Ό³Έν :
class FBXTool
{
public:
	// constrcuter destructer
	~FBXTool();

	FBXTool(const FBXTool& _Other) = delete;
	FBXTool(FBXTool&& _Other) noexcept = delete;
	FBXTool& operator=(const FBXTool& _Other) = delete;
	FBXTool& operator=(FBXTool&& _Other) noexcept = delete;

	static FBXTool& GetInst();

	void Init();


	void LoadALLFBX(std::shared_ptr<class EngineFile> _fileManager);
	void LoadFBX(const char* _filename, std::string _Name);

	void ProcessNode(FbxNode* _pNode, std::string _Name);
	void ProcessMesh(FbxMesh* pMesh, std::string _Name);

	std::vector<struct FMesh*> AllMesh;

	std::vector<struct FMesh*>& GetMesh() {
		return AllMesh;
	}


	//std::vector<DirectX::XMFLOAT3> vertices;
	//std::vector<DirectX::XMFLOAT3> normals;
	//std::vector<DirectX::XMFLOAT2> uvs;
	//std::vector<UINT> indices;

protected:

private:
	FBXTool();
	FbxManager* lSdkManager = nullptr;
	FbxIOSettings* ios = nullptr;
	FbxGeometryConverter* FBXConverter = nullptr;
	std::vector<FbxScene*> AllScene;
};

