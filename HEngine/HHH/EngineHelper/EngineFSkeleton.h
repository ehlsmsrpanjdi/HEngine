#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>

	struct SkinWeight
	{
		std::vector<std::pair<int, double>> weights; // (BoneIndex, Weight)
	};

	struct Bone {
		std::string name;
		int parentIndex;
		FbxAMatrix localBindPose;
		FbxAMatrix globalBindPose;
		FbxAMatrix inverseGlobalBindPose;
	};

// ���� :
class EngineFSkeleton
{
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
	void BoneWeight(FbxMesh* pMesh);
	void BoneSort(std::vector<struct FBuffer>& vertices);

	std::vector<Bone> GetBone();

	bool NoneSkel = false;
protected:

private:
	friend class EngineAnimation;
	std::vector<FbxNode*> rootBones;
	std::vector<Bone> Bones;
	std::unordered_map<std::string, int> boneNameToIndex;
	std::unordered_map<int, Bone> IndexToBone;
	std::unordered_map<FbxNode*, int> boneNodeToIndex;
	std::unordered_map<std::string, std::unordered_map<int, struct SkinWeight>> AllcontrolPointSkinData;


	int boneIndexCounter = 0;
	int rootIndex = 0;

};

