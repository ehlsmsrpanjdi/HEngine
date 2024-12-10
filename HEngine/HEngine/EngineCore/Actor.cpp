#include "pch.h"
#include "Actor.h"
#define RM EngineResource::GetInst()
#include "EngineHelper/EngineResource.h"
#include "Renderer.h"
#include "EngineCollision.h"


Actor::~Actor() {
	for (Renderer* Render : Renderers) {
		delete Render;
	}

	for (EngineCollision* Collision: Collisions) {
		delete Collision;
	}
}

void Actor::BeginPlay() {
	int a = 0;
}

void Actor::Tick(float _deltatime) {
	for (Renderer* Render : Renderers) {
		if (Render != nullptr) {
			Render->Rendering();
		}
	}
	for (EngineCollision* Collision : Collisions) {
		if (Collision != nullptr) {
			Collision->Collisioning();
		}
	}
}

Renderer* Actor::CreateRenderer(std::string_view _str) {
	Renderer* Render = new Renderer();
	Render->SetImage(RM->GetImage(_str));
	Render->SetOwner(this);
	Renderers.push_back(Render);
	return Render;
}

EngineCollision* Actor::CreateCollision(int _Type) {
	EngineCollision* Collision = EngineCollision::CreateCollision(_Type);
	Collision->SetOwner(this);
	Collisions.push_back(Collision);
	return Collision;
}