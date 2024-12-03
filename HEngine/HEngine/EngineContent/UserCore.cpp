#include "UserCore.h"
#include "Player.h"

void SMCore::EngineStart() {
	EngineCore::EngineStart();
}

void SMCore::BeginPlay() {
	EngineCore::BeginPlay();

	SpawnActor<Player>("MainPlayer");
}

void SMCore::EngineTick() {
	EngineCore::EngineTick();
}
void SMCore::EngineEnd() {
	EngineCore::EngineEnd();
}