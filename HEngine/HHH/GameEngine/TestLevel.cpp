#include "TestLevel.h"
#include <EngineHelper/EngineKey.h>
#include "MainPlayer.h"
#include "SkyActor.h"
#include "LightActor.h"
#include "BoxActor.h"

TestLevel::TestLevel()
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::BeginPlay()
{
	Level::BeginPlay();
	mainPlayer = SpawnActor<MainPlayer>();
	Actor* act = CreateBackGround<SkyActor>("Sky").get();
	act->SetActorScale(0.0001f, 0.0001f, 0.0001f);

	std::shared_ptr<Actor> camera = CreateCamera<Actor>("Main");
	SetMainCamera("main");
	camera->SetRoot(mainPlayer.get());
	camera->SetActorScale(10.f, 10.f, 10.f);
	camera->AddActorLocation(0.f, 200.f, -200.f);

	std::shared_ptr<BoxActor> box_1 = SpawnActor<BoxActor>();
	box_1->SetActorScale(0.1f, 0.1f, 0.1f);

	std::shared_ptr<BoxActor> box_2 = SpawnActor<BoxActor>();
	box_2->SetActorScale(0.1f, 0.1f, 0.1f);
	box_2->AddActorLocation(0, 0, 100);

	CreateLight(LightType::Directional);

}

void TestLevel::Tick(float _DeltaTime)
{
	Level::Tick(_DeltaTime);

	//if (EngineKey::IsPressed('W')) {
	//	MainCamera->Move(0.0f, 0.0f, 10.f * _DeltaTime);
	//}
	//if (EngineKey::IsPressed('S')) {
	//	MainCamera->Move(0.0f, 0.0f, -10.f * _DeltaTime);
	//}
	//if (EngineKey::IsPressed('A')) {
	//	MainCamera->Move(-10.f * _DeltaTime, 0.0f, 0.0f);
	//}
	//if (EngineKey::IsPressed('D')) {
	//	MainCamera->Move(10.f * _DeltaTime, 0.0f, 0.0f);
	//}

	//x축 마우스가 움직여야 y축으로 움직여야하니까 x축으로 넣어야함 ㅋㅋㅋ;; 
	mainPlayer->Rotate(0.f, EngineKey::MouseX * _DeltaTime, 0.f);

}

