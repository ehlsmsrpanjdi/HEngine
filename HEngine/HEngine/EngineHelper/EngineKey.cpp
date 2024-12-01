#include "EngineKey.h"

std::map<int, Key> EngineKey::Keys;

void EngineKey::KeyCheck() {
	for (std::pair<const int, Key>& pa : Keys) {
		if (GetAsyncKeyState(pa.first) & 0x8000) {
			pa.second.On = true;
			pa.second.Off = false;
		}
		else {
			pa.second.On = false;
			pa.second.Off = true;
		}
	}
}

void EngineKey::InitKey() {
	Keys[VK_LEFT];
	Keys[VK_RIGHT];
	Keys[VK_UP];
	Keys[VK_DOWN];
}

bool EngineKey::IsInput(int _Key) {
	if (Keys[_Key].On == true) {
		return true;
	}
	else {
		return false;
	}
}
