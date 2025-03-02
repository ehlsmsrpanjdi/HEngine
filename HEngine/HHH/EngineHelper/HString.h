#pragma once
#include <iostream>

class HString {
public:
	static std::wstring StoWC(std::string _str) {
        std::wstring wstr(_str.begin(), _str.end());
        return wstr;
	}
};