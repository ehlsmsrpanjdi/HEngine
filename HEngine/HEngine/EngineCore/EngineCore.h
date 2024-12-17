#pragma once

#include "Windows.h"
#include "EngineWindow/EngineWindow.h"
#include <list>
#include <iostream>

class EngineTime;
class Actor;
class Level;
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

	template <typename _Level>
	_Level* CreateLevel(std::string_view _str) {
		_Level* Lv = new _Level();
		Lv->SetName(_str);
		Lv->BeginPlay();
		Levels.push_back(Lv);
		return Lv;
	}


	std::list<Level*> Levels;

	virtual void EngineStart();

	virtual void BeginPlay();

	EngineWindow& GetWindow() { // <<- 이 병신새끼가 &를 빼고 함수를 만들었다고 한다면 믿으시겠습니까?
		return MainWindow;
	}

	void SetCollisionRendering(bool _is) {
		CollisionRendering = _is;
	}

	void DoubleBuffering();

protected:
	virtual void EngineTick();
	virtual void EngineEnd();


	EngineWindow MainWindow = EngineWindow();
	EngineTime* TimeManager = nullptr;

	static EngineCore* MainCore;

	bool CollisionRendering = true;
};