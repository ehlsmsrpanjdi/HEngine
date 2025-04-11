#include "EngineFSkeleton.h"
#include "algorithm"
#include "AllStruct.h"
#include "EngineFMesh.h"

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
    MakeBones();
}

void EngineFSkeleton::FindBones(FbxNode* _Node)
{
	if (boneNodeToIndex.find(_Node) != boneNodeToIndex.end())
		return;

	if (_Node->GetNodeAttribute() &&
		_Node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		//HAVETODELETE
		std::cout << "Bone Name :  " << _Node->GetName() << std::endl;
		std::string str = _Node->GetName();
		// ��Ʈ ������ üũ
		FbxNode* parent = _Node->GetParent();
		if (parent == nullptr ||
			parent->GetNodeAttribute() == nullptr ||
			parent->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eSkeleton)
		{
			rootBones.push_back(_Node);
		}

		// boneIndex ������Ű�鼭 map�� ����
		boneNodeToIndex[_Node] = boneIndexCounter++;
		//printf("Bone Name: %s (%p) -> Index: %d\n", _Node->GetName(), _Node, boneNodeToIndex[_Node]);
	}

	for (int i = 0; i < _Node->GetChildCount(); ++i)
	{
		FindBones(_Node->GetChild(i));
	}
}
void EngineFSkeleton::MakeBones()
{
    Bones.resize(boneNodeToIndex.size());

    for (const auto& [node, index] : boneNodeToIndex)
    {
        Bone& bone = Bones[index];
        bone.name = node->GetName();

        // �θ� ������ Ȯ���ؼ� parentIndex ����
        FbxNode* parent = node->GetParent();
        if (parent && boneNodeToIndex.find(parent) != boneNodeToIndex.end())
        {
            bone.parentIndex = boneNodeToIndex[parent];
        }
        else
        {
            bone.parentIndex = -1; // ��Ʈ
            rootIndex = index;     // ��Ʈ�� ����
        }
        // ���� ���ε� ���� ����
        bone.localBindPose = node->EvaluateLocalTransform();

		FbxAMatrix global = node->EvaluateGlobalTransform();
		bone.globalBindPose = global;
		bone.inverseGlobalBindPose = global.Inverse();

		boneNameToIndex[bone.name] = index;
    }
}


void EngineFSkeleton::BoneWeight(FbxMesh* pMesh)
{
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)
	{
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(skinIndex, FbxDeformer::eSkin);

		int clusterCount = pSkin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);

			FbxNode* boneNode = pCluster->GetLink();
			int boneIndex = boneNodeToIndex[boneNode]; // �츮�� ������ ���� map ���

			int* indices = pCluster->GetControlPointIndices();
			double* weights = pCluster->GetControlPointWeights();
			int count = pCluster->GetControlPointIndicesCount();

			for (int i = 0; i < count; ++i)
			{
				int controlPointIndex = indices[i];
				double weight = weights[i];

				controlPointSkinData[controlPointIndex].weights.push_back({ boneIndex, weight });
				// ctrlPointIndex ��° ���ؽ��� pCluster(Bone)���� weight ��ŭ �������
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
			for (int i = 0; i < 4; ++i)
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


		//for(controlPointIndex)
	}

	//for (auto& it : controlPointSkinData)
	//{
	//	int controlPointIndex = it.first;
	//	auto& weightList = it.second.weights;

	//	// �������� ����
	//	std::sort(weightList.begin(), weightList.end(),
	//		[](const std::pair<int, double>& a, const std::pair<int, double>& b)
	//		{
	//			return a.second > b.second;
	//		});

	//	int boneIndices[4] = { 0 };
	//	float boneWeights[4] = { 0.0f };

	//	float totalWeight = 0.0f;
	//	for (int i = 0; i < 4 && i < weightList.size(); ++i)
	//	{
	//		boneIndices[i] = weightList[i].first;
	//		boneWeights[i] = static_cast<float>(weightList[i].second);
	//		totalWeight += boneWeights[i];
	//	}

	//	// ����ȭ
	//	if (totalWeight > 0.0f)
	//	{
	//		for (int i = 0; i < 4; ++i)
	//		{
	//			boneWeights[i] /= totalWeight;
	//		}
	//	}
	//	// ���� �� ������ �ش� controlPointIndex�� ���ؽ��� �������ָ� ��
	//	// ��: vertices[controlPointIndex].BoneIndex = boneIndices;
	//	//     vertices[controlPointIndex].BoneWeight = boneWeights;
	//}
}


