#pragma once
#include <windows.h>
#include <ole2.h>
#include <gdiplus.h>
#include <string>
#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

class EngineImage {
public:
    EngineImage(Bitmap* _Bitmap) 
        :hBitmap(_Bitmap)
    {}

    void DrawImage(int x, int y, int width, int height);

    void SetPath(const std::wstring& _path) {
        path = _path;
    }

    static void SetDC(HDC _hdc) {
        if (hdc == nullptr) {
            hdc = _hdc;
        }
    }

    void SetBitmap(Bitmap* _Bitmap) {
        hBitmap = _Bitmap;
    }

    Bitmap* GetBitmap() {
        return hBitmap;
    }

private:
    static HDC hdc;
    Bitmap* hBitmap = nullptr;
    std::wstring path;
};

class ImageInit {
public:
    friend class EngineCore;

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