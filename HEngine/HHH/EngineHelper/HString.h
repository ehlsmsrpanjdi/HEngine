#pragma once
#include <iostream>

class HString {
public:
	static std::wstring StoWC(std::string _str) {
        std::wstring wstr(_str.begin(), _str.end());
        return wstr;
	}

	static std::string Upper(std::string _str) {
		std::string str = _str;
		for (int i = 0; i < str.size(); i++) {
			str[i] = toupper(str[i]);
		}
		return str;
	}
};