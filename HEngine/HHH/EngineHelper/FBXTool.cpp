#include "FBXTool.h"
#include "EngineFile.h"
#include "AllStruct.h"
#include <set>

std::string GetFileNameWithoutExtension(const std::string& fullPath)
{
	std::filesystem::path path(fullPath);
	std::string str = path.stem().string();
	return HString::Upper(str);
}

FBXTool::FBXTool()
{
}

FBXTool::~FBXTool()
{
	if (FBXConverter != nullptr) {
		delete FBXConverter;
		FBXConverter = nullptr;
	}

	AllMeshMap.clear();

	if (lSdkManager) {
		lSdkManager->Destroy();
	}
	lSdkManager = nullptr;

}

FBXTool& FBXTool::GetInst()
{
	static FBXTool FBXManager;
	return FBXManager;
}

void FBXTool::Init()
{
	lSdkManager = FbxManager::Create();
	ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FBXConverter = new FbxGeometryConverter(lSdkManager);
}



void FBXTool::LoadALLFBX(std::shared_ptr<EngineFile> _fileManager)
{
	const std::map<std::string, std::string>& map = _fileManager->GetAllFile("fbx");
	for (const std::pair<const std::string, std::string>& pa : map) {
		LoadFBX(pa.second.c_str(), pa.first);
	}
	ios->Destroy();
	ios = nullptr;
	if (!AllScene.empty()) {
		for (FbxScene* Scene : AllScene) {
			if (Scene != nullptr) {
				if (Scene->GetRootNode() != nullptr) {
					Scene->Destroy();
				}
				Scene = nullptr;
			}
		}
		AllScene.clear();
	}
}

void FBXTool::LoadFBX(const char* _filename, std::string_view _Name)
{
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(_filename, -1, lSdkManager->GetIOSettings())) {
		// 오류 처리
		return;
	}

	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	lImporter->Import(lScene);
	lImporter->Destroy();


	AllScene.push_back(lScene);
	ProcessNode(lScene->GetRootNode(), _Name);

}


void FBXTool::ProcessNode(FbxNode* _pNode, std::string_view _Name)
{
	FbxMesh* pMesh = _pNode->GetMesh();
	if (pMesh) {
		
		tempmatrix = _pNode->EvaluateGlobalTransform();
		std::shared_ptr<FMesh> mMesh = ProcessMesh(pMesh);
		std::string TexName;
		TexName = ProcessMaterial(_pNode);
		mMesh->TextureName =  TexName;

		AllMeshMap[_Name.data()][pMesh->GetName()] = mMesh;
	}

	for (int i = 0; i < _pNode->GetChildCount(); i++) {
		ProcessNode(_pNode->GetChild(i), _Name);
	}
}

std::string FBXTool::ProcessMaterial(FbxNode* _pNode)
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
				std::string str =GetFileNameWithoutExtension(texture->GetFileName());
				return str;
			}
	}
	return "DEFAULT";
}


std::shared_ptr<FMesh> FBXTool::ProcessMesh(FbxMesh* pMesh)
{
	std::shared_ptr<FMesh> mesh = std::make_shared<FMesh>();
	const char* _Name = pMesh->GetName();
	FBuffer buffer;
	int PolygonCount = pMesh->GetPolygonCount();

	FbxLayerElementUV* uvs = pMesh->GetLayer(0)->GetUVs();

	int totalVertexCount = 0;
	if (uvs->GetMappingMode() == FbxLayerElement::eByPolygonVertex) {
		for (int i = 0; i < PolygonCount; ++i) {
			int polygonSize = pMesh->GetPolygonSize(i); // 이 폴리곤이 몇 개의 정점을 가지는지
			for (int j = 0; j < polygonSize - 2; ++j) {
				mesh->indices.push_back(totalVertexCount + 0);
				mesh->indices.push_back(totalVertexCount + j + 1);
				mesh->indices.push_back(totalVertexCount + j + 2);
			}
			for (int j = 0; j < polygonSize; ++j) {
				int index = pMesh->GetPolygonVertex(i, j);
				FbxVector4 pos = pMesh->GetControlPointAt(index);
				pos = tempmatrix.MultT(pos);
				buffer.position = DirectX::XMFLOAT3(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));

				//uvIndex는 PolygonVertex 기준으로 계산
				int polygonVertexIndex = totalVertexCount + j;

				int uvIndex = uvs->GetIndexArray().GetAt(polygonVertexIndex);

				if (uvs->GetReferenceMode() == FbxLayerElement::eDirect) {
					uvIndex = polygonVertexIndex;
				}
				else if (uvs->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
					uvIndex = uvs->GetIndexArray().GetAt(polygonVertexIndex);
				}
				else {
					assert(false && "Unsupported ReferenceMode for UVs");
				}

				FbxVector2 uv = uvs->GetDirectArray().GetAt(uvIndex);
				buffer.uv = DirectX::XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1]));
				buffer.uv.y = 1.0f - buffer.uv.y;
				mesh->vertices.push_back(buffer);
			}
			totalVertexCount += polygonSize;
		}
	}
	else {
		assert(false);
	}
	return mesh;

}

std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<struct FMesh>>>& FBXTool::GetMesh()
{
	return AllMeshMap;
}




// 노멀 데이터 추출
//if (pMesh->GetElementNormalCount() > 0) {
//	FbxGeometryElementNormal* normalElement = pMesh->GetElementNormal(0);
//	if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
//		for (int i = 0; i < pMesh->GetControlPointsCount(); i++) {
//			int normalIndex = (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) ? i : normalElement->GetIndexArray().GetAt(i);
//			FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
//			mesh->normals.push_back(DirectX::XMFLOAT3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));
//		}
//	}
//}

// 텍스처 좌표 추출
//if (pMesh->GetElementUVCount() > 0) {
//	FbxGeometryElementUV* uvElement = pMesh->GetElementUV(0);
//	if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
//		for (int i = 0; i < pMesh->GetPolygonCount(); i++) {
//			for (int j = 0; j < pMesh->GetPolygonSize(i); j++) {
//				int uvIndex = pMesh->GetTextureUVIndex(i, j);
//				FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
//				mesh->uvs.push_back(DirectX::XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));
//			}
//		}
//	}
//}
