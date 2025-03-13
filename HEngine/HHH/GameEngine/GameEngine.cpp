#include "GameEngine.h"
#include "Actor.h"
#include "assert.h"
#include "Windows.h"
#include "EngineHelper/HString.h"
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/EngineDebug.h"	


GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init()
{
	CreateCamera("temp");
	SetMainCamera("temp");
}

void GameEngine::release()
{
	AllCamera.clear();
	AllActor.clear();
}

void GameEngine::Update(float _DeltaTime)
{


	CameraUpdate();

}

void GameEngine::CreateCamera(std::string _Name)
{
	std::string str = HString::Upper(_Name);
	if (AllCamera.contains(str) == true) {
		return;
	}
	std::shared_ptr<Actor> Camera = SpawnActor();
	AllCamera[str] = Camera.get();
}

void GameEngine::SetMainCamera(std::string _Name)
{
	std::string str = HString::Upper(_Name);
	if (AllCamera.contains(str) == true) {
		MainCamera = AllCamera[str];
	}
	return;
}

void GameEngine::CameraUpdate()
{
	if (MainCamera == nullptr) {
		EngineDebug::Error("카메라 없는데 업데이트중");
		exit(EXIT_FAILURE);
		return;
	}
	else {
		GraphicsEngine::get()->UpdateConstantBuffer(MainCamera->GetTransform(), "camera");
	}

}

std::shared_ptr<Actor> GameEngine::SpawnActor()
{
	std::shared_ptr<Actor> AActor = std::make_shared<Actor>();
	AllActor.push_back(AActor);
	return AActor;
}



