#include "pch.h"

#include "EngineImage.h"
#include "EnginePath.h"

#pragma comment (lib, "gdiplus.lib")


ULONG_PTR ImageInit::gdiplusToken = 0;
HDC EngineImage::hdc = nullptr;

void EngineImage::DrawImage(int x, int y, int width, int height) {
    if (path.empty()) {
        // path�� �������� ���� ��� ���� ó��
        return;
    }

    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Image image(path.c_str());  // std::wstring�� c_str()�� ��ȯ
    graphics.DrawImage(&image, x, y, width, height);
}