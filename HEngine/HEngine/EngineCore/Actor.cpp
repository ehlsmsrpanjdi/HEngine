#include "Actor.h"
#define RM EngineResource::GetInst()
#include "EngineHelper/EngineResource.h"
#include "Renderer.h"

void Actor::BeginPlay() {
	int a = 0;
}

void Actor::Tick(float _deltatime) {
	for (Renderer* Render : Renderers) {
			
	}
}

Renderer* Actor::CreateRenderer(std::string_view _str) {
	Renderer* Render = new Renderer();
	Render->SetImage(RM->GetImage(_str));
	Renderers.push_back(Render);
	return Render;
}