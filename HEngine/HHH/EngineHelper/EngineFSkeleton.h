#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>

// Ό³Έν :
class EngineFSkeleton
{
private:
	struct Bone {
		std::string name;
		int parentIndex;
		FbxAMatrix localBindPose;
		FbxAMatrix globalBindPose;
		FbxAMatrix inverseGlobalBindPose; 
	};

	struct SkinWeight
	{
		std::vector<std::pair<int, double>> weights; // (BoneIndex, Weight)
	};

public:
	// constrcuter destructer
	EngineFSkeleton();
	~EngineFSkeleton();

	// delete Function
	EngineFSkeleton(const EngineFSkeleton& _Other) = delete;
	EngineFSkeleton(EngineFSkeleton&& _Other) noexcept = delete;
	EngineFSkeleton& operator=(const EngineFSkeleton& _Other) = delete;
	EngineFSkeleton& operator=(EngineFSkeleton&& _Other) noexcept = delete;


	void init(FbxNode* _Node);
	void FindBones(FbxNode* _Node);
	void MakeBones();
	void BoneWeight(FbxMesh* pMesh);
	void BoneSort(std::vector<struct FBuffer>& vertices);

	bool NoneSkel = false;

protected:

private:
	friend class EngineAnimation;
	std::vector<FbxNode*> rootBones;
	std::vector<Bone> Bones;
	std::unordered_map<std::string, int> boneNameToIndex;
	std::unordered_map<FbxNode*, int> boneNodeToIndex;
	std::unordered_map<int, struct SkinWeight> controlPointSkinData;
	int boneIndexCounter = 0;
	int rootIndex = 0;

};

