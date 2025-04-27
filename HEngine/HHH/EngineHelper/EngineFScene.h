#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>


enum class type {
	None = -1,
	Mesh = 0,
	Anim,
};

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

	void ProcessNode(FbxNode* _pNode);
	std::string ProcessMaterial(FbxNode* _pNode);

	type SceneType = type::None;


	void AnimInit(FbxScene* _Scene, std::string_view _Name);
	void MeshInit(FbxScene* _Scene, std::string_view _Name);


protected:


private:
	friend class EngineScene;
	friend class FBXTool;
	std::vector<struct AnimMetaData> AnimData;
	std::unordered_map<std::string, std::shared_ptr<class EngineFMesh>> MeshMap;
	std::unordered_map<std::string, std::shared_ptr<class EngineAnimation>> AnimMap;
	std::string SceneName;
	std::shared_ptr<class EngineFSkeleton> Skeleton = nullptr;

};