#include "pch.h"
#include "Player.h"

void Player::BeginPlay() {
	Actor::BeginPlay();


	CreateRenderer("LeftPipe");
}


void Player::Tick(float _DeltaTime) {
	Actor::Tick(_DeltaTime);
}