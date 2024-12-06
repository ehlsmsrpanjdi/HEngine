#include "pch.h"
#include "Actor.h"
#define RM EngineResource::GetInst()
#include "EngineHelper/EngineResource.h"
#include "Renderer.h"

Actor::~Actor() {
	for (Renderer* Render : Renderers) {
		delete Render;
	}
}

void Actor::BeginPlay() {
	int a = 0;
}

void Actor::Tick(float _deltatime) {
	for (Renderer* Render : Renderers) {
		Render->Rendering();
	}
}

Renderer* Actor::CreateRenderer(std::string_view _str) {
	Renderer* Render = new Renderer();
	Render->SetImage(RM->GetImage(_str));
	Render->SetOwner(this);
	Renderers.push_back(Render);
	return Render;
}