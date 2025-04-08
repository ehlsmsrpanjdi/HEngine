#include "FBXTool.h"
#include "EngineFile.h"
#include "AllStruct.h"
#include <set>


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
		ProcessMaterial(_pNode, _Name);

		tempmatrix = _pNode->EvaluateGlobalTransform();
		AllMeshMap[_Name.data()][pMesh->GetName()] = ProcessMesh(pMesh);
	}

	for (int i = 0; i < _pNode->GetChildCount(); i++) {
		ProcessNode(_pNode->GetChild(i), _Name);
	}
}

void FBXTool::ProcessMaterial(FbxNode* _pNode, std::string_view _Name)
{
	int materialCount = _pNode->GetMaterialCount();
	if (materialCount == 0) return;

	fbxsdk::FbxSurfaceMaterial* material = _pNode->GetMaterial(0); // 하나만 있다고 가정
	if (!material) return;


	FbxProperty prop = material->FindProperty("DiffuseColor");
	int textureCount = prop.GetSrcObjectCount();
	for (int i = 0; i < textureCount; ++i) {
		FbxObject* obj = prop.GetSrcObject(i);
			FbxFileTexture* texture = static_cast<FbxFileTexture*>(obj);

			if (texture) {
				printf("텍스처 경로: %s\n", texture->GetFileName());
				int a = 0;
			}
	}
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



	//int PSize = pMesh->GetControlPointsCount();
	//mesh->vertices.resize(PSize);
	//mesh->indices.reserve(VertexCount);


	//FbxVector4 Vertex;

	//for (int i = 0; i < PSize; ++i) {
	//	Vertex = ControlledVertex[i];
	//	buffer.position = DirectX::XMFLOAT3(static_cast<float>(Vertex[0]), static_cast<float>(Vertex[1]), static_cast<float>(Vertex[2]));
	//	mesh->vertices[i].position = buffer.position;
	//}

	//FbxLayer* layer = pMesh->GetLayer(0);
	//FbxLayerElementUV* uvElement = layer->GetUVs();

	//FbxLayerElement::EMappingMode mappingMode = uvElement->GetMappingMode();
	//FbxLayerElement::EReferenceMode referenceMode = uvElement->GetReferenceMode();

	//const FbxLayerElementArrayTemplate<FbxVector2>& uvArray = uvElement->GetDirectArray();
	//const FbxLayerElementArrayTemplate<int>& indexArray = uvElement->GetIndexArray();

	//int mappingindex = 0;
	//if (mappingMode == FbxLayerElement::eByControlPoint) {

	//}



	//for (int i = 0; i < PolygonCount; ++i) {
	//	int polygonSize = pMesh->GetPolygonSize(i); // 이 폴리곤이 몇 개의 정점을 가지는지

	//	// polygonSize가 3 이상일 때만 삼각형으로 분해 가능
	//	if (polygonSize >= 3) {
	//		// 기준 정점 v0
	//		int v0 = pMesh->GetPolygonVertex(i, 0);

	//		for (int j = 1; j < polygonSize - 1; ++j) {
	//			int v1 = pMesh->GetPolygonVertex(i, j);
	//			int v2 = pMesh->GetPolygonVertex(i, j + 1);

	//			mesh->indices.push_back(v0);
	//			mesh->indices.push_back(v1);
	//			mesh->indices.push_back(v2);
	//		}
	//	}
	//}

	//mesh->vertices.resize(mesh->indices.size());

	//FbxLayer* layer = pMesh->GetLayer(0);
	//FbxLayerElementUV* uvElement = layer->GetUVs();

	//if (uvElement && uvElement->GetMappingMode() == FbxLayerElement::eByPolygonVertex)
	//{
	//	const FbxLayerElementArrayTemplate<FbxVector2>& uvArray = uvElement->GetDirectArray();
	//	const FbxLayerElementArrayTemplate<int>& uvIndexArray = uvElement->GetIndexArray();

	//	for (int i = 0; i < mesh->indices.size() / 3; i++) // 삼각형 개수만큼
	//	{
	//		for (int j = 0; j < 3; ++j) // 꼭지점 3개
	//		{
	//			int index = mesh->indices[i * 3 + j]; // control point 인덱스
	//			FbxVector4 pos = pMesh->GetControlPointAt(index); // 위치
	//			buffer.position = DirectX::XMFLOAT3(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));

	//			 //uvIndex는 PolygonVertex 기준으로 계산
	//			int polygonVertexIndex = i * 3 + j;
	//			int uvIndex = uvIndexArray.GetAt(polygonVertexIndex);
	//			FbxVector2 uv = uvArray.GetAt(uvIndex);
	//			buffer.uv = DirectX::XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1]));
	//			mesh->vertices[index] = buffer;
	//		}
	//	}
	//}



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
