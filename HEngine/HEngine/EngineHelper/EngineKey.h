#pragma once
#pragma once
#include <Windows.h>
#include <map>

class Key;

class EngineKey {
public:
	EngineKey() {
	}
	static void InitKey();
	static void KeyCheck();
	static bool IsInput(int _Key);


	static std::map<int, Key> Keys;
};

class Key {
public:
	bool Down = false;
	bool Up = false;
	bool On = false;
	bool Off = true;

	void KeyPress() {
		Down = true;
		Up = false;
		On = true;
		Off = false;
	}

	void KeyUp() {
		Down = false;
		Up = false;
		On = false;
		Off = true;
	}

	int KeyType = -1;
};