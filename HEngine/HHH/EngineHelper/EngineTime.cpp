#include "EngineTime.h"

void EngineTime::Init()
{
    // Ÿ�̸� ���ļ� �ʱ�ȭ
    QueryPerformanceFrequency(&frequency);

    // ù ��° Ÿ�̸� ���� ��� lastTime�� ����
    QueryPerformanceCounter(&lastTime);
}

double EngineTime::Update()
{
    QueryPerformanceCounter(&currentTime);

    // deltaTime ���: (���� Ÿ�̸� - ������ Ÿ�̸�) / ���ļ�
    deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

    // ������ Ÿ�̸� ���� ���� �ð����� ������Ʈ
    lastTime = currentTime;
    return deltaTime;
}
