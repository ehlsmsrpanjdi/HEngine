#pragma once
#include "EngineCore/Level.h"
class MainWorld : public Level{
public:
	virtual void BeginPlay() override;

	virtual void Tick(float _deltatime) override;
};