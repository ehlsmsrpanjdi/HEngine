#include "pch.h"
#include "Wall.h"
void Wall::BeginPlay() {
	Actor::BeginPlay();
	EngineCollision* Collision =  CreateCollision(static_cast<int>(CollisionEnum::Wall));
	Collision->SetScale(100.f, 100.f);
	Collision->PushFunction([Collision]() {
		if (nullptr != Collision->CollisionCheck(static_cast<int>(CollisionEnum::Player))) {
			int a = 0;
		}
		});
}

void Wall::Tick(float _deltatime) {
	Actor::Tick(_deltatime);
}