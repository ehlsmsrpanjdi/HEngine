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

#define APPSTART(hInstance, width, height) \
    do { \
        UNREFERENCED_PARAMETER(hPrevInstance); \
        UNREFERENCED_PARAMETER(lpCmdLine); \
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
        SMCore* Core = new SMCore(hInstance, width, height); \
        Core->EngineStart(); \
        delete Core; \
    } while(0)