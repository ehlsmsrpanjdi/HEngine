#pragma once
#include "BaseObject.h"
#include <list>
#include <iostream>
#include "EngineHelper/EngineKey.h"

class Renderer;

class Actor : public BaseObject {
public:
	friend class EngineCore;

	Actor() {

	}

	~Actor();

	virtual void BeginPlay();

	virtual void Tick(float _deltatime) override;

	Renderer* CreateRenderer(std::string_view _str);

	std::list<Renderer*> Renderers;

};