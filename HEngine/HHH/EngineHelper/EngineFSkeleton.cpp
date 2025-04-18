#include "EngineFSkeleton.h"
#include "algorithm"
#include "AllStruct.h"
#include "EngineFMesh.h"
#include "EngineFbxMath.h"
#include "iostream"


EngineFSkeleton::EngineFSkeleton()
{
}

EngineFSkeleton::~EngineFSkeleton()
{
	rootBones.clear();
	boneNodeToIndex.clear();
}



void EngineFSkeleton::init(FbxNode* _Node)
{
	FindBones(_Node);
	if (rootBones.size() == 0) {
		NoneSkel = true;
		assert(true);
	}
	if (rootBones.size() > 1) {
		assert(true);
	}


	//for (auto& [node , index] : boneNodeToIndex) {
	//	std::cout << "본이름  :  " << node->GetName() << "  ==   " << Bones[index].name  << std::endl;
	//	std::cout << "본index  :  " <<  index << "  ==  " << boneNameToIndex[node->GetName()] <<   std::endl;
	//	std::cout << "본parent  :  " << node->GetParent()->GetName() << "  ==  " << Bones[boneNameToIndex[node->GetName()]].parentIndex << std::endl;
	//	std::cout << "\n\n";
	//}

	//for (int i = 0; i < Bones.size(); ++i) {
	//	std::cout << i <<"번 본  :  " << Bones[i].name << " " << Bones[i].parentIndex << "\n";
	//}
	//std::cout << "\n\n";

}

void EngineFSkeleton::FindBones(FbxNode* _Node)
{
	FbxSkeleton* fbxSkeleton = _Node->GetSkeleton();

	if (fbxSkeleton != nullptr) {
		FbxNode* parent = _Node->GetParent();

		// 부모가 없거나, 부모가 스켈레톤이 아니라면 => 루트 스켈레톤 본이다
		if (parent == nullptr || parent->GetSkeleton() == nullptr) {
			int a = 0;
		}
	}
	if (fbxSkeleton != nullptr) {
		Bone bone;
		int parentBoneIndex = -1;

		FbxNode* parent = _Node->GetParent();

		if (parent != nullptr) {
			std::string parentName = parent->GetName();
			if (boneNameToIndex.contains(parentName) != false) {
				parentBoneIndex = boneNameToIndex[parentName];
			}
			else {
				assert(true);
			}
		}

		std::string boneName = _Node->GetName();

		bone.parentIndex = parentBoneIndex;
		boneNodeToIndex[_Node] = boneIndexCounter;
		boneNameToIndex[boneName] = boneIndexCounter;
		bone.name = boneName;

		bone.localBindPose = _Node->EvaluateLocalTransform();
		FbxAMatrix global = _Node->EvaluateGlobalTransform();
		bone.globalBindPose = global;

		bone.inverseGlobalBindPose = global.Inverse();

		if (parentBoneIndex == -1) {
			rootBones.push_back(_Node);
		}
		Bones.push_back(bone);
		IndexToBone[boneIndexCounter++] = bone;

	}

	for (int i = 0; i < _Node->GetChildCount(); ++i)
	{
		FindBones(_Node->GetChild(i));
	}
}

void EngineFSkeleton::BoneWeight(FbxMesh* pMesh)
{
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)		//피부 정보를 가져오고 (보통 1개)
	{
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(skinIndex, FbxDeformer::eSkin);

		int clusterCount = pSkin->GetClusterCount();		// 각각의 bone에 대한 정보들임 bone이 65개면 cluster도 65개
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);  //n번째 cluster가져오고
			FbxNode* boneNode = pCluster->GetLink();
			int boneIndex = boneNodeToIndex[boneNode]; // 우리가 위에서 만든 map 사용

			int* indices = pCluster->GetControlPointIndices();		//해당 bone이 영향을 주는 control point 인덱스들을 가져옴  0,5,8이면 0,5,8번의 controlpoint에 영향을 줌
			double* weights = pCluster->GetControlPointWeights();	//해당 control point에 얼마나 영향을 주는지에 대한 배열 0.5,0.4,0.3 이면 0번 controlpoint는 50% 1번은 40% 
			int count = pCluster->GetControlPointIndicesCount();		//위 배열의 길이

			for (int i = 0; i < count; ++i)
			{
				int controlPointIndex = indices[i];
				double weight = weights[i];

				controlPointSkinData[controlPointIndex].weights.push_back({ boneIndex, weight });
				//controlpointindex 번호를 갖고있는 버텍스 버퍼가 n번째 본에게 m만큼의 영향을 받음

				//버텍스버퍼 3번은 0번 5번 9번 본에 이정도 영향을 받는다고 저장한것
			}
		}
	}
}


void EngineFSkeleton::BoneSort(std::vector<FBuffer>& vertices)
{

	for (auto& it : controlPointSkinData)
	{
		int controlPointIndex = it.first;
		auto& weightList = it.second.weights;

		std::sort(weightList.begin(), weightList.end(),
			[](const std::pair<int, double>& a, const std::pair<int, double>& b)
			{
				return a.second > b.second;
			});

		int boneIndices[4] = { 0 };
		float boneWeights[4] = { 0.0f };
		float totalWeight = 0.0f;

		for (int i = 0; i < 4 && i < weightList.size(); ++i)
		{
			boneIndices[i] = weightList[i].first;
			boneWeights[i] = static_cast<float>(weightList[i].second);
			totalWeight += boneWeights[i];
		}

		// 정규화
		if (totalWeight > 0.0f)
		{
			for (int i = 0; i < 4 && i < weightList.size(); ++i)
			{
				boneWeights[i] /= totalWeight;
			}
		}

		for (FBuffer& vert : vertices) {
			if (vert.controlpointindex == controlPointIndex) {
				memcpy(vert.BoneIndices, boneIndices, sizeof(boneIndices));
				memcpy(vert.BoneWeights, boneWeights, sizeof(boneWeights));
			}
		}
	}
}

std::vector<Bone> EngineFSkeleton::GetBone()
{
	return Bones;
}


