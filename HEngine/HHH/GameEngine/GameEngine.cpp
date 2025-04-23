#include "GameEngine.h"
#include "Actor.h"
#include <cassert>
#include "HDirect/GraphicsEngine.h"
#include "EngineHelper/EngineDebug.h"	
#include "EngineHelper/EngineKey.h"
#include "EngineHelper/EngineNamespace.h"
#include "Level.h"

#include "TestLevel.h"


GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
	Levels.clear();
	SelectedLevel = nullptr;
	KeyManager = nullptr;
}

void GameEngine::Init(RECT _rc)
{
	float width = (float)(_rc.right - _rc.left);
	float height = (float)(_rc.bottom - _rc.top);

	AspectRatio = width / height;

	if (isPersepectiveChange == true) {
		isPersepectiveChange = false;
		PerseMatrix = XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
	}

	KeyManager = std::make_shared<EngineKey>();

	KeyManager->Init();


	CreateLevel<TestLevel>("TestLevel");
	ChangeLevel("TestLevel");

}

void GameEngine::release()
{
	Levels.clear();
}



void GameEngine::Update(float _DeltaTime)
{
	if (isPersepectiveChange == true) {
		isPersepectiveChange = false;
		PerseMatrix = XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ);
	}


	SelectedLevel->Tick(_DeltaTime);

	GraphicsEngine::get()->Clear(1.0f, 0.5, 0.5, 1.0f);

	SelectedLevel->Render(_DeltaTime);

	SelectedLevel->CollisionRender(_DeltaTime);

	GraphicsEngine::get()->Present(true);
}

std::shared_ptr<Level> GameEngine::ChangeLevel(std::string_view _Name)
{
	std::string str = HString::Upper(_Name.data());
	if (Levels.contains(str) == false) {
		assert(false);
	}
	if (SelectedLevel == nullptr) {
		SelectedLevel = Levels[str];
		SelectedLevel->StartLevel();
		return SelectedLevel;
	}
	else {
		std::shared_ptr<Level> PrevLv = SelectedLevel;
		std::shared_ptr<Level> CurrentLv = Levels[str];

		PrevLv->EndLevel();
		CurrentLv->StartLevel();

		SelectedLevel = CurrentLv;
	}
}


