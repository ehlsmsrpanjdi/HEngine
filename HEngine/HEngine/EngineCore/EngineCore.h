#pragma once
#include "Windows.h"
#include "EngineWindow/EngineWindow.h"
#include <functional>

class EngineCore {
public:

	EngineCore(HINSTANCE _inst, int _x,int _y) {
		MainWindow.SetWindowSize(_x, _y);
		MainWindow.WindowRegister(_inst);
		MainWindow.WindowOpen();
		
	}

	void EngineStart(std::function<void(void)> _Start, std::function<void(void)> _End);

	void EngineTick();
	void EngineEnd();

	EngineWindow MainWindow;
};