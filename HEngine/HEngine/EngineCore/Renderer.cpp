#include "pch.h"
#include "Renderer.h"
#include "EngineHelper/EngineImage.h"
#include "Actor.h"
#include "EngineCore.h"

Renderer::~Renderer() {
	RImage = nullptr;
}

void Renderer::SetImage(EngineImage* _Image) {
	RImage = _Image;
}


void Renderer::SetOwner(Actor* _Actor) {
	Owner = _Actor;
}



void Renderer::Rendering() {
	if (RImage != nullptr) {

		float2D Location = Owner->GetLocation() + GetTransform().Position;
		float2D Scale = Owner->GetScale() * GetTransform().Scale;

		

		RImage->DrawImage(Location.FloatToInt() ,Scale.FloatToInt());
	}
}

