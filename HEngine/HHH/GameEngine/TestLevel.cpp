#include "TestLevel.h"
#include <EngineHelper/EngineKey.h>
#include "MainPlayer.h"
#include "TestActor.h"
#include "TestTestActor.h"
#include "LightActor.h"

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::BeginPlay()
{
	Level::BeginPlay();
	Player = SpawnActor<MainPlayer>().get();
	SpawnActor<TestActor>()->AddActorLocation(100.f);
	std::shared_ptr<Actor> camera = CreateCamera<Actor>("Main");
	SetMainCamera("main");
	camera->AddActorLocation(100.f, 0.f, -50.f);

	CreateLight(LightType::Directional);

}

void TestLevel::Tick(float _DeltaTime)
{
	Level::Tick(_DeltaTime);

	if (EngineKey::IsPressed('W')) {
		MainCamera->Move(0.0f, 0.0f, 10.f * _DeltaTime);
	}
	if (EngineKey::IsPressed('S')) {
		MainCamera->Move(0.0f, 0.0f, -10.f * _DeltaTime);
	}
	if (EngineKey::IsPressed('A')) {
		MainCamera->Move(-10.f * _DeltaTime, 0.0f, 0.0f);
	}
	if (EngineKey::IsPressed('D')) {
		MainCamera->Move(10.f * _DeltaTime, 0.0f, 0.0f);
	}

	if (EngineKey::IsPressed(VK_RBUTTON)) {
		MainCamera->Rotate(EngineKey::MouseX * _DeltaTime, EngineKey::MouseY * _DeltaTime, 0.f);
	}

}

