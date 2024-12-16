#include "pch.h"
#include "MainWorld.h"
#include "Wall.h"
#include "Player.h"
void MainWorld::BeginPlay() {
	Level::BeginPlay();
	SpawnActor<Player>("Player");
	SpawnActor<Wall>("Wall");
}

void MainWorld::Tick(float _deltatime) {
	Level::Tick(_deltatime);

}