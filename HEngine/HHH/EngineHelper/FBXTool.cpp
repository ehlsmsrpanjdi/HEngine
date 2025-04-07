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
		// ���� ó��
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


void FBXTool::ProcessNode(FbxNode* _pNode, std::string_view _Name)
{
	FbxMesh* pMesh = _pNode->GetMesh();
	if (pMesh) {
		AllMeshMap[_Name.data()][pMesh->GetName()] = ProcessMesh(pMesh);
	}

	for (int i = 0; i < _pNode->GetChildCount(); i++) {
		ProcessNode(_pNode->GetChild(i), _Name);
	}
}

std::shared_ptr<FMesh> FBXTool::ProcessMesh(FbxMesh* pMesh)
{
	std::shared_ptr<FMesh> mesh = std::make_shared<FMesh>();
	const char* _Name = pMesh->GetName();
	FBuffer buffer;


	int Count = pMesh->GetPolygonCount();
	int VertexCount = pMesh->GetPolygonVertexCount();

	FbxVector4* ControlledVertex = pMesh->GetControlPoints();
	int PSize = pMesh->GetControlPointsCount();
	mesh->vertices.resize(PSize);
	mesh->indices.reserve(VertexCount);
	//mesh->uvs.resize(PSize); // �̰� �߿���

	FbxVector4 Vertex;

	for (int i = 0; i < PSize; ++i) {
		Vertex = ControlledVertex[i];
		buffer.position = DirectX::XMFLOAT3(static_cast<float>(Vertex[0]), static_cast<float>(Vertex[1]), static_cast<float>(Vertex[2]));
		mesh->vertices[i].position = buffer.position;
	}

	int uvIndex = 0;
	const char* uvSetName = nullptr;

	//if (pMesh->GetElementUVCount() > 0) {
	//	uvSetName = pMesh->GetElementUV(uvIndex)->GetName();
	//}


	for (int i = 0; i < Count; ++i) {
		for (int j = 0; j < 3; ++j) {
			int index = pMesh->GetPolygonVertex(i, j);
			mesh->indices.push_back(index);

			//if (uvSetName) {
			//	FbxVector2 uv;
			//	bool unmapped;

			//	pMesh->GetPolygonVertexUV(i, j, uvSetName, uv, unmapped);
			//	mesh->uvs[PSize] = DirectX::XMFLOAT2(
			//		static_cast<float>(uv[0]), static_cast<float>(uv[1]));
			//}

		}

	}

	return mesh;

}

std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<struct FMesh>>>& FBXTool::GetMesh()
{
	return AllMeshMap;
}




// ��� ������ ����
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

// �ؽ�ó ��ǥ ����
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
