#include "FBXTool.h"
#include "EngineFile.h"
#include "AllStruct.h"


FBXTool::FBXTool()
{
}

FBXTool::~FBXTool()
{
	if (FBXConverter != nullptr) {
		delete FBXConverter;
		FBXConverter = nullptr;
	}

	for (FMesh* mesh : AllMesh) {
		if (mesh != nullptr) {
			delete mesh;
			mesh = nullptr;
		}
	}
	AllMesh.clear();

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

	if (false == FBXConverter->Triangulate(lScene, true)) {
		return;
		if (lScene != nullptr) {
			lScene->Destroy();
			lScene = nullptr;
		}
	}

	AllScene.push_back(lScene);
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
	FMesh* mesh = new FMesh();
	mesh->MeshName = _Name;


	int Count = pMesh->GetPolygonCount();
	int VertexCount = pMesh->GetPolygonVertexCount();

	FbxVector4* ControlledVertex = pMesh->GetControlPoints();
	int PSize = pMesh->GetControlPointsCount();
	mesh->vertices.resize(PSize);
	mesh->indices.reserve(VertexCount);
	FbxVector4 Vertex;

	for (int i = 0; i < Count; ++i) {
		for (int j = 0; j < 3; ++j) {
		int index = pMesh->GetPolygonVertex(i, j);
		mesh->indices.push_back(index);
		Vertex = ControlledVertex[index];
		mesh->vertices[index] = DirectX::XMFLOAT3(static_cast<float>(Vertex[0]), static_cast<float>(Vertex[1]), static_cast<float>(Vertex[2]));
		}

	}

	//// 버텍스 데이터 추출
	//for (int i = 0; i < pMesh->GetControlPointsCount(); i++) {
	//	FbxVector4 vertex = pMesh->GetControlPointAt(i);
	//	mesh->vertices.push_back(DirectX::XMFLOAT3(static_cast<float>(vertex[0]), static_cast<float>(vertex[1]), static_cast<float>(vertex[2])));
	//}

	//// 인덱스 데이터 추출
	//for (int i = 0; i < pMesh->GetPolygonCount(); i++) {
	//	int PolygonSize = pMesh->GetPolygonSize(i);
	//	int v0 = pMesh->GetPolygonVertex(i, 0);

	//	for (int j = 1; j < PolygonSize - 1; j++) {
	//		int v1 = pMesh->GetPolygonVertex(i, j);
	//		int v2 = pMesh->GetPolygonVertex(i, j + 1);

	//		// 삼각형으로 변환
	//		mesh->indices.push_back(v0);
	//		mesh->indices.push_back(v2);
	//		mesh->indices.push_back(v1);
	//	}
	//}
	AllMesh.push_back(mesh);
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
