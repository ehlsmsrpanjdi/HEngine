#include "UserCore.h"
#include "Player.h"
#include "EngineHelper/EngineResource.h"

#define RM EngineResource::GetInst()


void SMCore::EngineStart() {
	EngineCore::EngineStart();
}

void SMCore::BeginPlay() {
	EngineCore::BeginPlay();

	RM->Move_parent();
	if (RM->FindFile("Resources") == true) {
		RM->FindImageRecursive(RM->GetPath());
	}

	SpawnActor<Player>("MainPlayer");
}

void SMCore::EngineTick() {
	EngineCore::EngineTick();
}
void SMCore::EngineEnd() {
	EngineCore::EngineEnd();
}