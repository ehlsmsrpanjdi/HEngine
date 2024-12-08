#include "pch.h"
#include "Player.h"
#include "EngineCore/Renderer.h"
#include "EngineCore/EngineCore.h"

void Player::BeginPlay() {
	Actor::BeginPlay();


	 Renderer* Render = CreateRenderer("LeftPipe");
	 Render->SetRenderSize(128.0f, 134.0f);
}


void Player::Tick(float _DeltaTime) {
	Actor::Tick(_DeltaTime);

	if (EngineKey::IsInput(VK_LEFT) == true) {
		AddLocation({ -100.f * _DeltaTime, 0.f });
	}
	if (EngineKey::IsInput(VK_RIGHT) == true) {
		AddLocation({ 100.f * _DeltaTime, 0.f });
	}
	if (EngineKey::IsInput(VK_UP) == true) {
		AddLocation({ 0.f, - 100.f * _DeltaTime, });
	}
	if (EngineKey::IsInput(VK_DOWN) == true) {
		AddLocation({ 0.f, + 100.f * _DeltaTime});
	}
}