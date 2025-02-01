#pragma once
#include <Windows.h>

class EngineTime {
public:
	EngineTime() {

	}

	void Init();

	double Update();

private:
	LARGE_INTEGER frequency = { 0 };
	LARGE_INTEGER lastTime = { 0 };
	LARGE_INTEGER currentTime = { 0 };
	double deltaTime = 0;        // deltaTime °è»ê°ª
};