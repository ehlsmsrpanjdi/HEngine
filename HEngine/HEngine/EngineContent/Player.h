#pragma once
#include "EngineCore/Actor.h"

class Player : public Actor{
public:
	Player() {

	}

	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

};