#include "EngineImage.h"
#include "EnginePath.h"

ULONG_PTR ImageInit::gdiplusToken = 0;
HDC EngineImage::hdc = nullptr;

void EngineImage::DrawImage(int x, int y, int width, int height) {
    if (path.empty()) {
        // path가 설정되지 않은 경우 예외 처리
        return;
    }

    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Image image(path.c_str());  // std::wstring을 c_str()로 변환
    graphics.DrawImage(&image, x, y, width, height);
}