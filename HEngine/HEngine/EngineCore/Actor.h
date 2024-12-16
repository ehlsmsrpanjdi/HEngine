#pragma once
#include "BaseObject.h"
#include <list>
#include <iostream>
#include "EngineHelper/EngineKey.h"

class Renderer;
class EngineCollision;
class Level;
class Actor : public BaseObject {
public:
	friend class EngineCore;
	friend class Level;
	Actor() {

	}

	~Actor();

	virtual void BeginPlay();

	virtual void Tick(float _deltatime) override;

	Renderer* CreateRenderer(std::string_view _str, int _type);

	EngineCollision* CreateCollision(int _Type);
private:
	Level* Current_Level = nullptr;
};