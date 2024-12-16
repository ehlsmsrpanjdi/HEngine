#include "pch.h"
#include "Actor.h"
#include "EngineHelper/EngineResource.h"
#include "Renderer.h"
#include "EngineCollision.h"
#include "Level.h"
#define RM EngineResource::GetInst()

Actor::~Actor() {

}

void Actor::BeginPlay() {
	int a = 0;
}

void Actor::Tick(float _deltatime) {
	//for (Renderer* Render : Renderers) {
	//	if (nullptr == Render->GetOwner()) {
	//		int a = 0;
	//	}
	//	if (Render != nullptr) {
	//		Render->Rendering();
	//	}
	//}
	//for (EngineCollision* Collision : Collisions) {
	//	if (nullptr == Collision->GetOwner()) {
	//		int a = 0;
	//	}
	//	if (Collision != nullptr) {
	//		Collision->Collisioning();
	//	}
	//}
}

Renderer* Actor::CreateRenderer(std::string_view _str, int _type) {
	Renderer* Render = new Renderer();
	Render->SetImage(RM->GetImage(_str));
	Render->SetOwner(this);
	Current_Level->Renderers[_type] = Render;
	return Render;
}

EngineCollision* Actor::CreateCollision(int _Type) {
	EngineCollision* Collision = EngineCollision::CreateCollision(_Type);
	Collision->SetOwner(this);
	Current_Level->Collisions[_Type] = Collision;
	return Collision;
}