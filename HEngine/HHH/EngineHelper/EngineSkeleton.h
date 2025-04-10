#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "unordered_map"
#include <Windows.h>

#include "fbxsdk.h"
#include <unordered_set>

// Ό³Έν :
class EngineSkeleton
{
private:
	struct Bone {
		std::string name;
		int parentIndex;
		FbxAMatrix localBindPose;
	};

	struct SkinWeight
	{
		std::vector<std::pair<int, double>> weights; // (BoneIndex, Weight)
	};

public:
	// constrcuter destructer
	EngineSkeleton();
	~EngineSkeleton();

	// delete Function
	EngineSkeleton(const EngineSkeleton& _Other) = delete;
	EngineSkeleton(EngineSkeleton&& _Other) noexcept = delete;
	EngineSkeleton& operator=(const EngineSkeleton& _Other) = delete;
	EngineSkeleton& operator=(EngineSkeleton&& _Other) noexcept = delete;


	void init(FbxNode* _Node);
	void FindBones(FbxNode* _Node);
	void MakeBones();
	void BoneWeight(FbxMesh* pMesh);
	void BoneSort(std::vector<struct FBuffer>& vertices);

	bool NoneSkel = false;
protected:

private:
	std::vector<FbxNode*> rootBones;
	std::vector<Bone> Bones;
	std::unordered_map<FbxNode*, int> boneNodeToIndex;
	std::unordered_map<int, struct SkinWeight> controlPointSkinData;
	int boneIndexCounter = 0;
	int rootIndex = 0;

};

