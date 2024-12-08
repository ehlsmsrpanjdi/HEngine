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
		// path�� �������� ���� ��� ���� ó��
		return;
	}

	HDC backBuffer = CreateCompatibleDC(hdc);
	bool a = SelectObject(backBuffer, hBitmap);

	Int2D size = GetImageSize();


	StretchBlt(hdc,        // ��� DC			/bitblt �Ⱦ� ������ bitblt�� ���� �̹��� ũ�� ������ �ȴ�
		x, y,   // ��� ��ġ
		width, height,   // ����� �̹��� ũ��
		backBuffer,      // ���� DC
		0, 0,       // ���� ��ġ
		size.X, size.Y,   // ���� �̹��� ũ��
		SRCCOPY);   // ���� ���

	DeleteDC(backBuffer);
}