#include "pch.h"
#include "UserCore.h"
#include "Player.h"
#include "Wall.h"
#include "EngineHelper/EngineResource.h"
#include "MainWorld.h"
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

	CreateLevel<MainWorld>("MainLevel");
}

void SMCore::EngineTick() {
	EngineCore::EngineTick();
}
void SMCore::EngineEnd() {
	EngineCore::EngineEnd();
}