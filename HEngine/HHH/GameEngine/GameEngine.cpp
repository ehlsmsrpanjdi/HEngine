#include "GameEngine.h"
#include "Actor.h"

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::CreateCamera(std::string _Name)
{
	if (AllCamera.contains(_Name) == true) {
		return;
	}
	std::shared_ptr<Actor> Camera = SpawnActor();
	AllCamera[_Name] = Camera.get();

}

void GameEngine::SetCamera(std::string _Name)
{
	if (AllCamera.contains(_Name) == true) {
		MainCamera = AllCamera[_Name];
	}
	return;
}

std::shared_ptr<Actor> GameEngine::SpawnActor()
{
	std::shared_ptr<Actor> AActor = std::make_shared<Actor>();
	AllActor.push_back(AActor);
	return AActor;
}



