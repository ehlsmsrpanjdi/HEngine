#include "GameEngine.h"
#include "Actor.h"
#include "assert.h"
#include "Windows.h"
#include "EngineHelper/HString.h"

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

void GameEngine::Update(float _DeltaTime)
{
	if (MainCamera == nullptr) {
		MessageBoxA(NULL, "메인카메라없음","경고", MB_OK | MB_ICONERROR);
		exit(EXIT_FAILURE);
		return;
	}
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

std::shared_ptr<Actor> GameEngine::SpawnActor()
{
	std::shared_ptr<Actor> AActor = std::make_shared<Actor>();
	AllActor.push_back(AActor);
	return AActor;
}



