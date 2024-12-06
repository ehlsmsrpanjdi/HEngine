#include "pch.h"

#include "EngineImage.h"
#include "EnginePath.h"
#include "EngineCore/EngineCore.h"

#pragma comment (lib, "gdiplus.lib")


ULONG_PTR ImageInit::gdiplusToken = 0;
HDC EngineImage::hdc = nullptr;

EngineImage::~EngineImage() {
	DeleteObject(hBitmap);
}

void EngineImage::DrawImage(int x, int y, int width, int height) {
	if (hBitmap == nullptr) {
		// path가 설정되지 않은 경우 예외 처리
		return;
	}

	HDC backBuffer = CreateCompatibleDC(hdc);
	bool a = SelectObject(backBuffer, hBitmap);
	/*if ( == NULL) {
		Rectangle(hdc, 10, 10, 200, 200);
		return;
	}*/
	BitBlt(hdc, 0, 0, 1000, 1000, backBuffer, 0, 0, SRCCOPY); // 백 버퍼 -> 메인 HDC
	DeleteDC(backBuffer);
}