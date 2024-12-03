#pragma once
#include <functional>
#include "Windows.h"
#include "EngineWindow/EngineWindow.h"
#include <list>
#include <iostream>

class EngineTime;
class Actor;
class EngineCore {
public:

	EngineCore(HINSTANCE _inst, int _x, int _y);

	virtual void EngineStart();

	virtual void BeginPlay();

protected:
	virtual void EngineTick();
	virtual void EngineEnd();

	std::list<Actor*> Actors;

	template <typename Act>
	Act* SpawnActor(std::string_view _str) {
		Act* Actor = new Act();
		Actor->SetName(_str);
		Actor->BeginPlay();
		Actors.push_back(Actor);
		return Actor;
	}

	EngineWindow MainWindow;
	EngineTime* TimeManager = nullptr;
};