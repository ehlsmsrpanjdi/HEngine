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

	BoneSort(_Name);
}


void FBXTool::ProcessNode(FbxNode* _pNode, std::string_view _Name)
{
	FindBones(_pNode);

	FbxMesh* pMesh = _pNode->GetMesh();
	if (pMesh) {

		tempmatrix = _pNode->EvaluateGlobalTransform();
		std::shared_ptr<FMesh> mMesh = ProcessMesh(pMesh);
		std::string TexName;
		TexName = ProcessMaterial(_pNode);
		mMesh->TextureName = TexName;

		AllMeshMap[_Name.data()][pMesh->GetName()] = mMesh;

		BoneWeight(pMesh);
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
			std::string str = GetFileNameWithoutExtension(texture->GetFileName());
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
				buffer.controlpointindex = index;
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

void FBXTool::FindBones(FbxNode* _Node)
{
	if (boneNodeToIndex.find(_Node) != boneNodeToIndex.end())
		return;

	if (_Node->GetNodeAttribute() &&
		_Node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		// boneIndex 증가시키면서 map에 저장
		boneNodeToIndex[_Node] = boneIndexCounter++;  // <-- 여기가 핵심!!
		printf("Bone Name: %s (%p) -> Index: %d\n", _Node->GetName(), _Node, boneNodeToIndex[_Node]);
	}

	for (int i = 0; i < _Node->GetChildCount(); ++i)
	{
		FindBones(_Node->GetChild(i));
	}
}

void FBXTool::BoneWeight(FbxMesh* pMesh)
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
			int boneIndex = boneNodeToIndex[boneNode]; // 우리가 위에서 만든 map 사용

			int* indices = pCluster->GetControlPointIndices();
			double* weights = pCluster->GetControlPointWeights();
			int count = pCluster->GetControlPointIndicesCount();

			for (int i = 0; i < count; ++i)
			{
				int controlPointIndex = indices[i];
				double weight = weights[i];

				controlPointSkinData[controlPointIndex].weights.push_back({ boneIndex, weight });
				// ctrlPointIndex 번째 버텍스가 pCluster(Bone)에게 weight 만큼 영향받음
			}
		}
	}
}

void FBXTool::BoneSort(std::string_view _str)
{
	for (auto& it : controlPointSkinData)
	{
		int controlPointIndex = it.first;
		auto& weightList = it.second.weights;

		// 내림차순 정렬
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

		// 정규화
		if (totalWeight > 0.0f)
		{
			for (int i = 0; i < 4; ++i)
			{
				boneWeights[i] /= totalWeight;
			}
		}
		 AllMeshMap[_str.data()];
		int a = 0;

		// 이제 이 정보를 해당 controlPointIndex의 버텍스에 저장해주면 돼
		// 예: vertices[controlPointIndex].BoneIndex = boneIndices;
		//     vertices[controlPointIndex].BoneWeight = boneWeights;
	}


}


