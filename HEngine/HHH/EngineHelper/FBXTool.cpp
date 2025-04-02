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
		// ���� ó��
		return;
	}

	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	lImporter->Import(lScene);
	lImporter->Destroy();

	
	if (true == FBXConverter->Triangulate(lScene, true)) {
	AllScene.push_back(lScene);
	ProcessNode(lScene->GetRootNode(), _Name);
	}
	else {
		assert(false);
	}
}

void LoadNode(FbxNode* node)
{
	// ������ �۾��� �����Ѵ�.

	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute) {

		// �� ����� �Ӽ��� �޽�.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FbxMesh�� ĳ���õ� ��� �Ӽ��� �����͸� �����´�.
			FbxMesh* mesh = node->GetMesh();
		}
	}
	const int childCount = node->GetChildCount();

	for (unsigned int i = 0; i < childCount; ++i) {
		LoadNode(node->GetChild(i));
	}
};

void ProcessControlPoints(FbxMesh* mesh)
{

	// �������� ������ �����´�.
	unsigned int count = mesh->GetControlPointsCount();

	std::vector<DirectX::XMFLOAT3> vertices;
	for (unsigned int i = 0; i < count; ++i)
	{
		DirectX::XMFLOAT3 positions;
		// �������� ���������� GetControlPointAt(int index) ��� �Լ��� �̿��Ѵ�.
		positions.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]); // x��ǥ
		positions.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]); // y��ǥ
		positions.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]); // z��ǥ

		vertices.push_back(positions);
	}
}



void FBXTool::ProcessNode(FbxNode* _pNode, std::string _Name)
{
	FbxMesh* pMesh = _pNode->GetMesh();
	if (pMesh) {
		//std::string str = pMesh->GetName();
		//std::string nodeName = _pNode->GetName();
		//FbxNodeAttribute* attr = _pNode->GetNodeAttribute();
		//FbxNodeAttribute::EType type = attr->GetAttributeType();

		//if (type == FbxNodeAttribute::eMesh) {
		//	std::cout << nodeName << " is a Mesh (Surface)\n";
		//}
		//else if (type == FbxNodeAttribute::eSkeleton) {
		//	std::cout << nodeName << " is a Joint (Skeleton)\n";
		//}
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
	// ���ؽ� ������ ����
	for (int i = 0; i < pMesh->GetControlPointsCount(); i++) {
		FbxVector4 vertex = pMesh->GetControlPointAt(i);
		mesh->vertices.push_back(DirectX::XMFLOAT3(static_cast<float>(vertex[0]), static_cast<float>(vertex[1]), static_cast<float>(vertex[2])));
	}
	// �ε��� ������ ����
	for (int i = 0; i < pMesh->GetPolygonCount(); i++) {
		int PolygonSize = pMesh->GetPolygonSize(i);
		if (PolygonSize != 3) {
			int a = 0;
		}
		for (int j = 0; j < PolygonSize; j++) {
			mesh->indices.push_back(pMesh->GetPolygonVertex(i, j));
		}
	}

	AllMesh.push_back(mesh);
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
