#pragma once
#include <Windows.h>

class EngineTime {
public:
    EngineTime() {
        // 타이머 주파수 초기화
        QueryPerformanceFrequency(&frequency);

        // 첫 번째 타이머 값을 얻어 lastTime에 저장
        QueryPerformanceCounter(&lastTime);
    }

    double UpdateDeltaTime()
    {
        QueryPerformanceCounter(&currentTime);

        // deltaTime 계산: (현재 타이머 - 마지막 타이머) / 주파수
        deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

        // 마지막 타이머 값을 현재 시간으로 업데이트
        lastTime = currentTime;
        return deltaTime;
    }

private:
    LARGE_INTEGER frequency = { 0 };
    LARGE_INTEGER lastTime = { 0 };
    LARGE_INTEGER currentTime = { 0 };
    double deltaTime = 0;        // deltaTime 계산값
};