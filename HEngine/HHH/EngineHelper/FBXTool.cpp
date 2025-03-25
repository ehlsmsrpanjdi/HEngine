#include "FBXTool.h"

FBXTool::FBXTool()
{
}

FBXTool::~FBXTool()
{
	if (lSdkManager) {
		lSdkManager->Destroy();
	}
	lSdkManager = nullptr;
	
	if (!AllScene.empty()) {
		for (FbxScene* Scene : AllScene) {
			Scene->Destroy();
			Scene = nullptr;
		}
		AllScene.clear();
	}

	if (ios) {
		ios->Destroy();
		ios = nullptr;
	}
}

FBXTool& FBXTool::GetInst()
{
	static FBXTool FBXManager;
	return FBXManager;
}
//
//void FBXTool::Init()
//{
//	lSdkManager = FbxManager::Create();
//	ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
//	lSdkManager->SetIOSettings(ios);
//}
//
//void FBXTool::LoadFBX(const char* _filename)
//{
//	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
//	if (!lImporter->Initialize(_filename, -1, lSdkManager->GetIOSettings())) {
//		// 오류 처리
//		return;
//	}
//
//	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
//	lImporter->Import(lScene);
//	lImporter->Destroy();
//
//	AllScene.push_back(lScene);
//	// 모델 데이터 파싱
//	ProcessNode(lScene->GetRootNode());
//}
//
//void FBXTool::ProcessNode(FbxNode* _pNode)
//{
//		FbxMesh* pMesh = _pNode->GetMesh();
//	if (pMesh) {
//		ProcessMesh(pMesh);
//	}
//
//	for (int i = 0; i < _pNode->GetChildCount(); i++) {
//		ProcessNode(_pNode->GetChild(i));
//	}
//}
//
//void FBXTool::ProcessMesh(FbxMesh* pMesh)
//{
//	// 버텍스 데이터 추출
//	for (int i = 0; i < pMesh->GetControlPointsCount(); i++) {
//		FbxVector4 vertex = pMesh->GetControlPointAt(i);
//		vertices.push_back(DirectX::XMFLOAT3(static_cast<float>(vertex[0]), static_cast<float>(vertex[1]), static_cast<float>(vertex[2])));
//	}
//
//	//// 노멀 데이터 추출
//	//if (pMesh->GetElementNormalCount() > 0) {
//	//	FbxGeometryElementNormal* normalElement = pMesh->GetElementNormal(0);
//	//	if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
//	//		for (int i = 0; i < pMesh->GetControlPointsCount(); i++) {
//	//			int normalIndex = (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) ? i : normalElement->GetIndexArray().GetAt(i);
//	//			FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
//	//			normals.push_back(DirectX::XMFLOAT3(static_cast<float>(normal[0]), static_cast<float>(normal[1]), static_cast<float>(normal[2])));
//	//		}
//	//	}
//	//}
//
//	//// 텍스처 좌표 추출
//	//if (pMesh->GetElementUVCount() > 0) {
//	//	FbxGeometryElementUV* uvElement = pMesh->GetElementUV(0);
//	//	if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
//	//		for (int i = 0; i < pMesh->GetPolygonCount(); i++) {
//	//			for (int j = 0; j < pMesh->GetPolygonSize(i); j++) {
//	//				int uvIndex = pMesh->GetTextureUVIndex(i, j);
//	//				FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
//	//				uvs.push_back(DirectX::XMFLOAT2(static_cast<float>(uv[0]), static_cast<float>(uv[1])));
//	//			}
//	//		}
//	//	}
//	//}
//
//	// 인덱스 데이터 추출
//	for (int i = 0; i < pMesh->GetPolygonCount(); i++) {
//		for (int j = 0; j < pMesh->GetPolygonSize(i); j++) {
//			indices.push_back(pMesh->GetPolygonVertex(i, j));
//		}
//	}
//}
