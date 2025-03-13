#pragma once
#include <iostream>
#include <Windows.h>
// ���� :
class EngineDebug
{
public:
	// constrcuter destructer
	EngineDebug() = delete;
	~EngineDebug() = delete;

	// delete Function
	EngineDebug(const EngineDebug& _Other) = delete;
	EngineDebug(EngineDebug&& _Other) noexcept = delete;
	EngineDebug& operator=(const EngineDebug& _Other) = delete;
	EngineDebug& operator=(EngineDebug&& _Other) noexcept = delete;


	static void Error(std::string str) {
		MessageBoxA(NULL, str.c_str(), "���", MB_OK | MB_ICONERROR);
		exit(0);
	}
protected:

private:

};

