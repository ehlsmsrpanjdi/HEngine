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
	//	std::cout << "���̸�  :  " << node->GetName() << "  ==   " << Bones[index].name  << std::endl;
	//	std::cout << "��index  :  " <<  index << "  ==  " << boneNameToIndex[node->GetName()] <<   std::endl;
	//	std::cout << "��parent  :  " << node->GetParent()->GetName() << "  ==  " << Bones[boneNameToIndex[node->GetName()]].parentIndex << std::endl;
	//	std::cout << "\n\n";
	//}

	//for (int i = 0; i < Bones.size(); ++i) {
	//	std::cout << i <<"�� ��  :  " << Bones[i].name << " " << Bones[i].parentIndex << "\n";
	//}
	//std::cout << "\n\n";

}

void EngineFSkeleton::FindBones(FbxNode* _Node)
{
	FbxSkeleton* fbxSkeleton = _Node->GetSkeleton();

	if (fbxSkeleton != nullptr) {
		FbxNode* parent = _Node->GetParent();

		// �θ� ���ų�, �θ� ���̷����� �ƴ϶�� => ��Ʈ ���̷��� ���̴�
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
	for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)		//�Ǻ� ������ �������� (���� 1��)
	{
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(skinIndex, FbxDeformer::eSkin);

		int clusterCount = pSkin->GetClusterCount();		// ������ bone�� ���� �������� bone�� 65���� cluster�� 65��
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);  //n��° cluster��������
			FbxNode* boneNode = pCluster->GetLink();
			int boneIndex = boneNodeToIndex[boneNode]; // �츮�� ������ ���� map ���

			int* indices = pCluster->GetControlPointIndices();		//�ش� bone�� ������ �ִ� control point �ε������� ������  0,5,8�̸� 0,5,8���� controlpoint�� ������ ��
			double* weights = pCluster->GetControlPointWeights();	//�ش� control point�� �󸶳� ������ �ִ����� ���� �迭 0.5,0.4,0.3 �̸� 0�� controlpoint�� 50% 1���� 40% 
			int count = pCluster->GetControlPointIndicesCount();		//�� �迭�� ����

			for (int i = 0; i < count; ++i)
			{
				int controlPointIndex = indices[i];
				double weight = weights[i];

				controlPointSkinData[controlPointIndex].weights.push_back({ boneIndex, weight });
				//controlpointindex ��ȣ�� �����ִ� ���ؽ� ���۰� n��° ������ m��ŭ�� ������ ����

				//���ؽ����� 3���� 0�� 5�� 9�� ���� ������ ������ �޴´ٰ� �����Ѱ�
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

		// ����ȭ
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


