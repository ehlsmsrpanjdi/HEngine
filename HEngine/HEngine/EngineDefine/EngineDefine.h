#pragma once
#include "assert.h"
#define EngineName L"HEngine"
#define EngineTitle L"HEngine"


#define ErrorCheck(msg) { \
    std::string errorMsg = "Assertion Failed!\nFile: "; \
    errorMsg += __FILE__; \
    errorMsg += "\nLine: " + std::to_string(__LINE__); \
    errorMsg += "\nMessage: " + std::string(msg); \
    MessageBoxA(nullptr, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR); \
    assert(false); \
}