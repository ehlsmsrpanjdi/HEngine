#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

// 설명 :
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

	
	std::vector<std::shared_ptr<class EngineFScene>>& GetScene();

	void ProcessMeshScene(std::shared_ptr<class EngineFScene> EScene, std::string_view _Name, FbxScene* lScene);
	void ProcessAnimScene(std::shared_ptr<class EngineFScene> EScene, std::string_view _Name, FbxImporter* Importer, FbxScene* lScene);


#pragma region "애니메이션"
	std::vector<struct AnimMetaData> LoadAnim(FbxImporter* Importer, FbxScene* _Scene);

#pragma endregion

private:
	FBXTool();
	FbxManager* lSdkManager = nullptr;
	FbxIOSettings* ios = nullptr;
	FbxGeometryConverter* FBXConverter = nullptr;
	std::vector<FbxScene*> AllScene;

	std::vector<std::shared_ptr<class EngineFScene>> EngineScenes;
	std::unordered_map<std::string, std::shared_ptr<class EngineFScene>> SceneMap;

public:
};

