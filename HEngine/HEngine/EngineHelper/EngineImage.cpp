#include "pch.h"

#include "EngineImage.h"
#include "EnginePath.h"
#include "EngineCore/EngineCore.h"

#pragma comment (lib, "gdiplus.lib")


ULONG_PTR ImageInit::gdiplusToken = 0;
HDC EngineImage::hdc = nullptr;

EngineImage::~EngineImage() {
	hBitmap = nullptr;
}

void EngineImage::DrawImage(int x, int y, int width, int height) {
	if (hBitmap == nullptr) {
		// path가 설정되지 않은 경우 예외 처리
		return;
	}

	HDC backBuffer = CreateCompatibleDC(hdc);
	bool a = SelectObject(backBuffer, hBitmap);

	Int2D size = GetImageSize();

	StretchBlt(hdc,        // 대상 DC			/bitblt 안쓴 이유는 bitblt은 사진 이미지 크기 조절이 안댐
		100, 100,   // 대상 위치
		300, 200,   // 출력할 이미지 크기
		backBuffer,      // 원본 DC
		0, 0,       // 원본 위치
		100, 100,   // 원본 이미지 크기
		SRCCOPY);   // 복사 방식

	DeleteDC(backBuffer);
}