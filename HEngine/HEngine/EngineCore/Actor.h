#pragma once
#include "BaseObject.h"
#include <list>
#include <iostream>
#include "EngineHelper/EngineKey.h"

class Renderer;
class EngineCollision;
class Actor : public BaseObject {
public:
	friend class EngineCore;

	Actor() {

	}

	~Actor();

	virtual void BeginPlay();

	virtual void Tick(float _deltatime) override;

	Renderer* CreateRenderer(std::string_view _str);

	EngineCollision* CreateCollision(int _Type);
private:
	std::list<Renderer*> Renderers;
	std::list<EngineCollision*> Collisions;
};