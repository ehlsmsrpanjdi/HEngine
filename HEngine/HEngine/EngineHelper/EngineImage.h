#pragma once
#include <windows.h>
#include <ole2.h>
#include <gdiplus.h>
#include <string>
#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

class EngineImage {
public:
    EngineImage() {}

    void DrawImage(int x, int y, int width, int height);

    void SetPath(const std::wstring& _path) {
        path = _path;
    }

    static void SetDC(HDC _hdc) {
        if (hdc == nullptr) {
            hdc = _hdc;
        }
    }

private:
    static HDC hdc;
    std::wstring path;
};

class ImageInit {
public:
    friend class SMEngine;

    ImageInit() {}

    ~ImageInit() {}

private:
    static void ImageIniteralize() {
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    }

    static void ImageExit() {
        GdiplusShutdown(gdiplusToken);
    }

    static ULONG_PTR gdiplusToken;
};