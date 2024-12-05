#include "pch.h"
#include "EngineString.h"

void EngineString::Upper(std::string& _str) {
	std::transform(_str.begin(), _str.end(), _str.begin(), [](unsigned char _c) {
		return std::toupper(_c);
		});
}