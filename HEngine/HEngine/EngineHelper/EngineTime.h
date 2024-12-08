#pragma once
#include <Windows.h>

class EngineTime {
public:
    EngineTime() {
        // Ÿ�̸� ���ļ� �ʱ�ȭ
        QueryPerformanceFrequency(&frequency);

        // ù ��° Ÿ�̸� ���� ��� lastTime�� ����
        QueryPerformanceCounter(&lastTime);
    }

    double UpdateDeltaTime()
    {
        QueryPerformanceCounter(&currentTime);

        // deltaTime ���: (���� Ÿ�̸� - ������ Ÿ�̸�) / ���ļ�
        deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

        // ������ Ÿ�̸� ���� ���� �ð����� ������Ʈ
        lastTime = currentTime;
        return deltaTime;
    }

private:
    LARGE_INTEGER frequency = { 0 };
    LARGE_INTEGER lastTime = { 0 };
    LARGE_INTEGER currentTime = { 0 };
    double deltaTime = 0;        // deltaTime ��갪
};