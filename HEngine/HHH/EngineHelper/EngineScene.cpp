#include "EngineScene.h"
#include "EngineFMesh.h"
#include "AllStruct.h"
#include "EngineSkeleton.h"
#include "HString.h"
#include "filesystem"

std::string GetFileNameWithoutExtension(const std::string& fullPath)
{
	std::filesystem::path path(fullPath);
	std::string str = path.stem().string();
	return HString::Upper(str);
}


EngineScene::EngineScene()
{
}

EngineScene::~EngineScene()
{
}

void EngineScene::init(FbxScene* _Scene, std::string_view _Name)
{
	Skeleton = std::make_shared<EngineSkeleton>();
	Skeleton->init(_Scene->GetRootNode());
	if (Skeleton->NoneSkel == true) {
		Skeleton = nullptr;
	}


	ProcessNode(_Scene->GetRootNode());
	SceneName = _Name;
}

void EngineScene::ProcessNode(FbxNode* _pNode)
{

	FbxMesh* pMesh = _pNode->GetMesh();
	if (pMesh) {
		std::shared_ptr<EngineFMesh> Mesh = std::make_shared<EngineFMesh>();
		Mesh->init(_pNode);
		if (Skeleton != nullptr) {
		Skeleton->BoneWeight(pMesh);
		Skeleton->BoneSort(Mesh->vertices);
		Mesh->Skeleton = Skeleton;
		}
		Mesh->TextureName = ProcessMaterial(_pNode);
		MeshMap[pMesh->GetName()] = Mesh;
	}

	for (int i = 0; i < _pNode->GetChildCount(); i++) {
		ProcessNode(_pNode->GetChild(i));
	}
}

//
//std::shared_ptr<EngineFMesh> EngineScene::ProcessMesh(FbxMesh* pMesh)
//{
//	std::shared_ptr<EngineFMesh> mesh = std::make_shared<EngineFMesh>();
//	FBuffer buffer;
//	int PolygonCount = pMesh->GetPolygonCount();
//
//	FbxLayerElementUV* uvs = pMesh->GetLayer(0)->GetUVs();
//
//	int totalVertexCount = 0;
//	if (uvs->GetMappingMode() == FbxLayerElement::eByPolygonVertex) {
//		for (int i = 0; i < PolygonCount; ++i) {
//			int polygonSize = pMesh->GetPolygonSize(i); // 이 폴리곤이 몇 개의 정점을 가지는지
//			for (int j = 0; j < polygonSize - 2; ++j) {
//				mesh->indices.push_back(totalVertexCount + 0);
//				mesh->indices.push_back(totalVertexCount + j + 1);
//				mesh->indices.push_back(totalVertexCount + j + 2);
//			}
//			for (int j = 0; j < polygonSize; ++j) {
//				int index = pMesh->GetPolygonVertex(i, j);
//				FbxVector4 pos = pMesh->GetControlPointAt(index);
//				pos = tempmatrix.MultT(pos);
//				buffer.position = DirectX::XMFLOAT3(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));
//				buffer.controlpointindex = index;
//				//uvIndex는 PolygonVertex 기준으로 계산
//				int polygonVertexIndex = totalVertexCount + j;
//
//				int uvIndex = uvs->GetIndexArray().GetAt(polygonVertexIndex);
//
//				if (uvs->GetReferenceMode() == FbxLayerElement::eDirect) {
//					uvIndex = polygonVertexIndex;
//				}
//				else if (uvs->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
//					uvIndex = uvs->GetIndexArray().GetAt(polygonVertexIndex);
//				}
//				else {
//					assert(false && "Unsupported ReferenceMode for UVs");
//				}
//
//				FbxVector2 uv = uvs->GetDirectArray().GetAt(uvIndex);
//				buffer.uv = DirectX::XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1]));
//				buffer.uv.y = 1.0f - buffer.uv.y;
//				mesh->vertices.push_back(buffer);
//			}
//			totalVertexCount += polygonSize;
//		}
//	}
//	else {
//		assert(false);
//	}
//	return mesh;
//
//}



std::string EngineScene::ProcessMaterial(FbxNode* _pNode)
{
	int materialCount = _pNode->GetMaterialCount();
	if (materialCount == 0) return "DEFAULT";

	fbxsdk::FbxSurfaceMaterial* material = _pNode->GetMaterial(0); // 하나만 있다고 가정
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

