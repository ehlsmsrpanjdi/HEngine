#include "UserCore.h"
#include "Player.h"
#include "EngineHelper/EngineResource.h"
void SMCore::EngineStart() {
	EngineCore::EngineStart();
}

void SMCore::BeginPlay() {
	EngineCore::BeginPlay();

	EngineResource::GetInst()->Move_parent();
	EngineResource::GetInst()->FindFile("Resources");

	SpawnActor<Player>("MainPlayer");
}

void SMCore::EngineTick() {
	EngineCore::EngineTick();
}
void SMCore::EngineEnd() {
	EngineCore::EngineEnd();
}