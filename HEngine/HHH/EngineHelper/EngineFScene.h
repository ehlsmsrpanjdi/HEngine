#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>

// Ό³Έν :
class EngineFScene
{
public:
	// constrcuter destructer
	EngineFScene();
	~EngineFScene();

	// delete Function
	EngineFScene(const EngineFScene& _Other) = delete;
	EngineFScene(EngineFScene&& _Other) noexcept = delete;
	EngineFScene& operator=(const EngineFScene& _Other) = delete;
	EngineFScene& operator=(EngineFScene&& _Other) noexcept = delete;

	void init(FbxScene* _Scene, std::string_view _Name);
	void ProcessNode(FbxNode* _pNode);
	//std::shared_ptr<class EngineFMesh> ProcessMesh(class FbxMesh* pMesh);
	std::string ProcessMaterial(FbxNode* _pNode);
	void ProcessAnim(FbxScene* _Scene);
protected:


private:
	friend class EngineScene;
	friend class FBXTool;
	std::vector<struct AnimMetaData> Anims;
	std::unordered_map<std::string, std::shared_ptr<class EngineFMesh>> MeshMap;
	std::unordered_map<std::string, std::shared_ptr<class EngineAnimation>> AnimMap;
	std::string SceneName;
	std::shared_ptr<class EngineFSkeleton> Skeleton = nullptr;

};