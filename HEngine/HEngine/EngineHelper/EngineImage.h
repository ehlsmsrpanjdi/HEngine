#pragma once
#include <windows.h>
#include <ole2.h>
#include <gdiplus.h>
#include <string>
#include "EngineHelper/EngineMath.h"

class EngineImage {
public:
	EngineImage(HBITMAP _Bitmap)
		:hBitmap(_Bitmap)
	{
	}

	~EngineImage();

	void DrawImage(int x, int y, int width, int height);

	/*void SetPath(const std::wstring& _path) {
		path = _path;
	}*/

	static void SetDC(HDC _hdc) {
		if (hdc == nullptr) {
			hdc = _hdc;
		}
	}

	void SetBitmap(HBITMAP _Bitmap) {
		hBitmap = _Bitmap;
	}

	HBITMAP GetBitmap() {
		return hBitmap;
	}

	void SetImageSize(int _x, int _y) {
		width = _x;
		height = _y;
	}

	Int2D GetImageSize() {
		return Int2D(width, height);
	}

private:
	static HDC hdc;
	HBITMAP hBitmap;
	int width = -1;
	int height = -1;

	//std::wstring path;
};

class ImageInit {
public:
	friend class EngineCore;

	ImageInit() {}

	~ImageInit() {}

private:
	static void ImageIniteralize(HDC _hdc) {
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
		EngineImage::SetDC(_hdc);
	}

	static void ImageExit() {
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	static ULONG_PTR gdiplusToken;
};