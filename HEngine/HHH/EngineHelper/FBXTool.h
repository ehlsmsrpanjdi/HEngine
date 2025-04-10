#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
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
	void LoadFBX(const char* _filename, std::string_view _Name);

	void ProcessNode(FbxNode* _pNode, std::string_view _Name);


	std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<struct FMesh>>> AllMeshMap;

	std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<struct FMesh>>>& GetMesh();


#pragma region "Bone"



	void BoneWeight(FbxMesh* pMesh);
	void BoneSort(std::string_view _str);
	//std::unordered_map<int, std::vector<struct FBuffer*>> cpToVertices;
#pragma endregion

	FbxAMatrix tempmatrix = FbxAMatrix();
protected:

private:
	FBXTool();
	FbxManager* lSdkManager = nullptr;
	FbxIOSettings* ios = nullptr;
	FbxGeometryConverter* FBXConverter = nullptr;
	std::vector<FbxScene*> AllScene;

	std::vector<std::shared_ptr<class EngineScene>> EngineScenes;

public:
};

