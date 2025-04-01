#include "FBXTool.h"
#include "EngineFile.h"
#include "AllStruct.h"
FBXTool::FBXTool()
{
}

FBXTool::~FBXTool()
{
	if (lSdkManager) {
		lSdkManager->Destroy();
	}
	lSdkManager = nullptr;

	for (FBXMesh* mesh : AllMesh) {
		if (mesh != nullptr) {
			delete mesh;
			mesh = nullptr;
		}
	}
	AllMesh.clear();
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
}



void FBXTool::LoadALLFBX(EngineFile* _fileManager)
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

void FBXTool::LoadFBX(const char* _filename, std::string _Name)
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
	// 모델 데이터 파싱
	ProcessNode(lScene->GetRootNode(), _Name);
}

void FBXTool::ProcessNode(FbxNode* _pNode, std::string _Name)
{
	FbxMesh* pMesh = _pNode->GetMesh();
	if (pMesh) {
		ProcessMesh(pMesh, _Name);
	}

	for (int i = 0; i < _pNode->GetChildCount(); i++) {
		ProcessNode(_pNode->GetChild(i), _Name);
	}
}

void FBXTool::ProcessMesh(FbxMesh* pMesh, std::string _Name)
{
	FBXMesh* mesh = new FBXMesh();
	mesh->MeshName = _Name;
	// 버텍스 데이터 추출
	for (int i = 0; i < pMesh->GetControlPointsCount(); i++) {
		FbxVector4 vertex = pMesh->GetControlPointAt(i);
		mesh->vertices.push_back(DirectX::XMFLOAT3(static_cast<float>(vertex[0]), static_cast<float>(vertex[1]), static_cast<float>(vertex[2])));
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

	// 인덱스 데이터 추출
	for (int i = 0; i < pMesh->GetPolygonCount(); i++) {
		for (int j = 0; j < pMesh->GetPolygonSize(i); j++) {
			mesh->indices.push_back(pMesh->GetPolygonVertex(i, j));
		}
	}

	AllMesh.push_back(mesh);
}
