#pragma once
#include "EngineCore/Actor.h"

class Wall : public Actor {
public:
	Wall() {

	}

	~Wall();

	void BeginPlay();

	void Tick(float _deltatime);
};