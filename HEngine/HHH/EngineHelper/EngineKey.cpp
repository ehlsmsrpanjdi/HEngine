#include "EngineKey.h"

std::map<int, Key> EngineKey::Keys;
int EngineKey::MouseX = 0;
int EngineKey::MouseY = 0;

void EngineKey::Update() {
	for (std::pair<const int, Key>& pa : Keys) {
		if (GetAsyncKeyState(pa.first) & 0x8000) {
			if (pa.second.Down == true) {
				pa.second.Pressed = true;
				pa.second.UnPressed = false;
				pa.second.Up = false;
				pa.second.Down = false;
			}
			else if (pa.second.Down == false) {
				pa.second.Down = true;
				pa.second.Pressed = true;
				pa.second.Up = false;
				pa.second.UnPressed = false;
			}
		}
		else {
			if (pa.second.Pressed == true) {
				pa.second.Pressed = false;
				pa.second.UnPressed = true;
				pa.second.Up = true;
				pa.second.Down = false;
			}
			else if (pa.second.Pressed == false) {
				pa.second.Down = false;
				pa.second.Pressed = false;
				pa.second.Up = false;
				pa.second.UnPressed = true;
			}
		}
	}
}

bool EngineKey::IsDown(int _Key)
{
	if (Keys[_Key].Down == true) {
		return true;
	}
	return false;
}

bool EngineKey::IsPressed(int _Key)
{
	if (Keys[_Key].Pressed == true) {
		return true;
	}
	return false;
}

void EngineKey::Init() {
	Keys[VK_LEFT];
	Keys[VK_RIGHT];
	Keys[VK_UP];
	Keys[VK_DOWN];
	Keys[VK_RBUTTON];
	Keys['Z'];
}


