#include "FBXTool.h"
#include "EngineFile.h"
#include "AllStruct.h"
#include "EngineFScene.h"
#include "EngineAnimation.h"
#include <set>

std::vector<AnimMetaData> FBXTool::LoadAnim(FbxImporter* Importer, FbxScene* _Scene)
{
	std::vector<AnimMetaData> anims;


	FbxAnimStack* currAnimStack = _Scene->GetSrcObject<FbxAnimStack>(1);
	if (currAnimStack == nullptr) {
		return anims;
	}
	if (currAnimStack != nullptr) {
		FbxString animStackName = currAnimStack->GetName();
	}

	FbxNode* rootNode = _Scene->GetRootNode();
	if (rootNode == nullptr) {
		return anims;
	}
	float frameRate = static_cast<float>(FbxTime::GetFrameRate(_Scene->GetGlobalSettings().GetTimeMode()));

	FbxArray<FbxString*> animationArray;

	FbxDocument* document = dynamic_cast<FbxDocument*>(_Scene);

	if (document != nullptr) {
		document->FillAnimStackNameArray(animationArray);
	}
	int animCount = Importer->GetAnimStackCount();
	for (int i = 0; i < animCount; ++i) {
		FbxTakeInfo* animationInfo = Importer->GetTakeInfo(1);
		std::string animationName = animationInfo->mName.Buffer();

		FbxTimeSpan span = animationInfo->mLocalTimeSpan;

		double startTime = span.GetStart().GetSecondDouble();
		double endTime = span.GetSignedDuration().GetSecondDouble();

		if (startTime < endTime) {
			int keyFrames = static_cast<int>((endTime - startTime) * static_cast<double>(frameRate));
		}

		anims.emplace_back(animationName, startTime, endTime);

	}
	for (int i = 0; i < animationArray.Size(); ++i) {
		delete animationArray[i];
	}
	animationArray.Clear();

	return anims;
}

std::vector<struct AnimMetaData> FBXTool::LoadMixamo(FbxImporter* Importer, FbxScene* _Scene)
{
	std::vector<AnimMetaData> anims;

	FbxAnimStack* currAnimStack = _Scene->GetSrcObject<FbxAnimStack>(1);
	if (currAnimStack == nullptr) {
		return anims;
	}

	FbxNode* rootNode = _Scene->GetRootNode();
	if (rootNode == nullptr) {
		return anims;
	}

	float frameRate = static_cast<float>(FbxTime::GetFrameRate(_Scene->GetGlobalSettings().GetTimeMode()));

	FbxTakeInfo* animationInfo = Importer->GetTakeInfo(1);
	std::string Name = animationInfo->mName.Buffer();
	FbxTimeSpan span = animationInfo->mLocalTimeSpan;

	double startTime = span.GetStart().GetSecondDouble();
	double endTime = span.GetSignedDuration().GetSecondDouble();

	if (startTime < endTime) {
		int keyFrames = static_cast<int>((endTime - startTime) * static_cast<double>(frameRate));
	}

	anims.emplace_back(Name, startTime, endTime);

	return anims;
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

	SceneMap.clear();
	EngineScenes.clear();

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

	for (auto& [name, element] : SceneMap) {
		EngineScenes.push_back(element);
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

	std::shared_ptr<EngineFScene> EScene = std::make_shared<EngineFScene>();

	if (_Name.find('_') != std::string::npos) {
		EScene->SceneType = type::Anim;
		ProcessAnimScene(EScene, _Name, lImporter, lScene	);
	}
	else {
		EScene->SceneType = type::Mesh;
		ProcessMeshScene(EScene, _Name, lScene);
	}

	lImporter->Destroy();

}

std::vector<std::shared_ptr<class EngineFScene>>& FBXTool::GetScene()
{
	return EngineScenes;
}

void FBXTool::ProcessMeshScene(std::shared_ptr<EngineFScene> EScene, std::string_view _Name, FbxScene* lScene)
{
	if (SceneMap.contains(_Name.data()) == true) {
		return;
	}
	EScene->MeshInit(lScene, _Name);
	SceneMap[_Name.data()] = EScene;
}

void FBXTool::ProcessAnimScene(std::shared_ptr<EngineFScene> EScene, std::string_view _Name, FbxImporter* Importer, FbxScene* lScene)
{
	int index = _Name.find('_');
	std::string SceneName(_Name.substr(0, index));
	std::string AnimName(_Name.substr(index + 1));

	if (SceneMap.contains(SceneName) != true) {
		ProcessMeshScene(EScene, SceneName, lScene);
	}

	EScene = SceneMap[SceneName];

	EScene->AnimData = LoadMixamo(Importer, lScene);
	if (EScene->AnimData.size() == 0) assert(false); // 애니메이션이 있는 scene만 여기 함수에 와야하는데 이게 0이면 안됨
	EScene->AnimInit(lScene, AnimName);

}


