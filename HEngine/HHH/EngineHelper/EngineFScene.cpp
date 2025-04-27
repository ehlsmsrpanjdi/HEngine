#include "EngineFScene.h"
#include "EngineFMesh.h"
#include "AllStruct.h"
#include "EngineFSkeleton.h"
#include "HString.h"
#include "filesystem"
#include "EngineAnimation.h"

std::string GetFileNameWithoutExtension(const std::string& fullPath)
{
	std::filesystem::path path(fullPath);
	std::string str = path.stem().string();
	return HString::Upper(str);
}


EngineFScene::EngineFScene()
{
}

EngineFScene::~EngineFScene()
{
	Skeleton = nullptr;
	MeshMap.clear();
	AnimMap.clear();
	AnimData.clear();
}

void EngineFScene::ProcessNode(FbxNode* _pNode)
{

	FbxMesh* pMesh = _pNode->GetMesh();

	if (pMesh) {
		std::string meshname = pMesh->GetName();
		std::shared_ptr<EngineFMesh> Mesh = std::make_shared<EngineFMesh>();
		Mesh->init(_pNode);
		if (Skeleton != nullptr) {
			Skeleton->BoneWeight(pMesh);
			Skeleton->BoneSort(Mesh->vertices);
		}
		else {
			Mesh->BoneValueDelete();
		}
		Mesh->TextureName = ProcessMaterial(_pNode);
		MeshMap[pMesh->GetName()] = Mesh;
	}

	for (int i = 0; i < _pNode->GetChildCount(); i++) {
		ProcessNode(_pNode->GetChild(i));
	}
}


std::string EngineFScene::ProcessMaterial(FbxNode* _pNode)
{
	int materialCount = _pNode->GetMaterialCount();
	if (materialCount == 0) return "DEFAULT";

	fbxsdk::FbxSurfaceMaterial* material = _pNode->GetMaterial(0); // �ϳ��� �ִٰ� ����
	if (!material) return "DEFAULT";


	FbxProperty prop = material->FindProperty("DiffuseColor");
	int textureCount = prop.GetSrcObjectCount();
	for (int i = 0; i < textureCount; ++i) {
		FbxObject* obj = prop.GetSrcObject(i);
		FbxFileTexture* texture = static_cast<FbxFileTexture*>(obj);

		if (texture) {
			std::string str = GetFileNameWithoutExtension(texture->GetFileName());
			return str;
		}
	}
	return "DEFAULT";
}


void EngineFScene::AnimInit(FbxScene* _Scene, std::string_view _Name)
{
	for (const AnimMetaData& Data : AnimData) {
		FbxAnimStack* stack = _Scene->FindMember<FbxAnimStack>(Data.name.c_str());
		//�ִϸ��̼��� �������µ�, stack�̶�� �Ϳ� �ִϸ��̼��� �����ؼ� Scene�� �� �ִϸ��̼��� ���õ� ���·� �����
		if (stack != nullptr)
			_Scene->SetCurrentAnimationStack(stack);
		//������ �ִٸ� �ִϸ��̼��� �ִ°Ŵϱ� ���� �ִϸ��̼��� �ش� stack���� �ٲٱ�
		else {
			assert(false);
		}

		std::shared_ptr<EngineAnimation> Ani = std::make_shared<EngineAnimation>();
		Ani->AnimationName = _Name.data();
		Ani->Skeleton = Skeleton;
		Ani->ExtractAnimationKeys(_Scene);
		if (AnimMap.contains(_Name.data())) {
			assert(false);
		}
		AnimMap[_Name.data()] = Ani;
	}
}

void EngineFScene::MeshInit(FbxScene* _Scene, std::string_view _Name)
{
	Skeleton = std::make_shared<EngineFSkeleton>();
	Skeleton->init(_Scene->GetRootNode());
	if (Skeleton->GetBone().size() == 0) {
		Skeleton = nullptr;
	}
	ProcessNode(_Scene->GetRootNode());
	SceneName = _Name;
}

