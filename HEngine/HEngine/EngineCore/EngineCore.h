#pragma once

#include "Windows.h"
#include "EngineWindow/EngineWindow.h"
#include <list>
#include <iostream>

class EngineTime;
class Actor;
class EngineCore {
public:

	EngineCore(HINSTANCE _inst, int _x, int _y);

	static EngineCore* GetInst() {
		if (MainCore != nullptr) {
			return MainCore;
		}
		else {
			return nullptr;
		}
	}


	virtual void EngineStart();

	virtual void BeginPlay();

	EngineWindow& GetWindow() { // <<- 이 병신새끼가 &를 빼고 함수를 만들었다고 한다면 믿으시겠습니까?
		return MainWindow;
	}

	void SetCollisionRendering(bool _is) {
		CollisionRendering = _is;
	}

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

	EngineWindow MainWindow = EngineWindow();
	EngineTime* TimeManager = nullptr;

	static EngineCore* MainCore;

	bool CollisionRendering = true;
};