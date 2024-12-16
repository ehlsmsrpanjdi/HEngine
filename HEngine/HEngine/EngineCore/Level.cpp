#include "pch.h"
#include "Level.h"
#include "Actor.h"
#include "Renderer.h"
#include "EngineCollision.h"

Level::Level() {

}

Level::~Level() {
	for (Actor* Act : Actors) {
		if (Act != nullptr) {
			delete Act;
		}
	}


	for (std::pair<const int, Renderer*>& pa : Renderers) {
		if (pa.second != nullptr) {
			delete pa.second;
		}
	}

	for (std::pair<const int, EngineCollision*>& pa : Collisions) {
		if (pa.second != nullptr) {
			delete pa.second;
		}
	}

}

void Level::BeginPlay() {

}

void Level::Tick(float _deltatime) {

	for (Actor* Act : Actors) {
		Act->Tick(_deltatime);
	}

}

void Level::RenderTick(float _deltatime) {
	for (std::pair<const int, Renderer*>& pa : Renderers) {
		pa.second->Rendering();
	}
}
