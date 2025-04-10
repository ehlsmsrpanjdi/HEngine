#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>

// Ό³Έν :
class EngineScene
{
public:
	// constrcuter destructer
	EngineScene();
	~EngineScene();

	// delete Function
	EngineScene(const EngineScene& _Other) = delete;
	EngineScene(EngineScene&& _Other) noexcept = delete;
	EngineScene& operator=(const EngineScene& _Other) = delete;
	EngineScene& operator=(EngineScene&& _Other) noexcept = delete;

	void init(FbxScene* _Scene, std::string_view _Name);
	void ProcessNode(FbxNode* _pNode);
	//std::shared_ptr<class EngineFMesh> ProcessMesh(class FbxMesh* pMesh);
	std::string ProcessMaterial(FbxNode* _pNode);
protected:


private:
	std::unordered_map<std::string, std::shared_ptr<class EngineFMesh>> MeshMap;
	std::string SceneName;
	std::shared_ptr<class EngineSkeleton> Skeleton = nullptr;
};