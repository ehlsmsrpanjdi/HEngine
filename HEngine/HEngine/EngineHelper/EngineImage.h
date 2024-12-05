#pragma once
#include <windows.h>
#include <ole2.h>
#include <gdiplus.h>
#include <string>


class EngineImage {
public:
	EngineImage(Gdiplus::Bitmap* _Bitmap)
		:hBitmap(_Bitmap)
	{
	}

	void DrawImage(int x, int y, int width, int height);

	/*void SetPath(const std::wstring& _path) {
		path = _path;
	}*/

	static void SetDC(HDC _hdc) {
		if (hdc == nullptr) {
			hdc = _hdc;
		}
	}

	void SetBitmap(Gdiplus::Bitmap* _Bitmap) {
		hBitmap = _Bitmap;
	}

	Gdiplus::Bitmap* GetBitmap() {
		return hBitmap;
	}

private:
	static HDC hdc;
	Gdiplus::Bitmap* hBitmap = nullptr;
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