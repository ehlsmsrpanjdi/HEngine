#pragma once
#include <iostream>
#include <algorithm>
class EngineString {
public:

	static void Upper(std::string& _str) {
		std::transform(_str.begin(), _str.end(), _str.begin(), [](unsigned char _c) {
			return std::toupper(_c);
			});
	}

};