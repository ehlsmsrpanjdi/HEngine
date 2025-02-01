#pragma once
#pragma once
#include <Windows.h>
#include <map>

class Key;

class EngineKey {
public:
	EngineKey() {
	}
	void Init();
	void Update();

	static bool IsDown(int _Key);
	static bool IsPressed(int _Key);


private:
	static std::map<int, Key> Keys;
};

class Key {
public:
	bool Down = false;
	bool Up = false;
	bool Pressed = false;
	bool UnPressed = false;


	int KeyType = -1;
};