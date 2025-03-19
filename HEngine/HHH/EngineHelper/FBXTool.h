#pragma once
#include <fbxsdk.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <Windows.h>
// Ό³Έν :
class FBXTool
{
public:
	// constrcuter destructer
	~FBXTool();

	// delete Function
	FBXTool(const FBXTool& _Other) = delete;
	FBXTool(FBXTool&& _Other) noexcept = delete;
	FBXTool& operator=(const FBXTool& _Other) = delete;
	FBXTool& operator=(FBXTool&& _Other) noexcept = delete;

	static FBXTool& GetInst();

	void Init();

	void LoadFBX(const char* _filename);

	void ProcessNode(FbxNode* _pNode);
	void ProcessMesh(FbxMesh* pMesh);

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> uvs;
	std::vector<UINT> indices;

protected:

private:
	FBXTool();
	FbxManager* lSdkManager = nullptr;
	FbxIOSettings* ios = nullptr;
	std::vector<FbxScene*> AllScene;
};

