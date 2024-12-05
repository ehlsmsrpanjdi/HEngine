#include "pch.h"

#include "EngineImage.h"
#include "EnginePath.h"
#include "EngineCore/EngineCore.h"

#pragma comment (lib, "gdiplus.lib")


ULONG_PTR ImageInit::gdiplusToken = 0;
HDC EngineImage::hdc = nullptr;

void EngineImage::DrawImage(int x, int y, int width, int height) {
	if (hBitmap == nullptr) {
		// path가 설정되지 않은 경우 예외 처리
		return;
	}
	Gdiplus::Graphics g(hdc);

	// (x, y)에 width X height 크기의 이미지를 그립니다.
	g.DrawImage(hBitmap, x, y, width, height);

	// 데이터 메모리 해제
	ReleaseDC(EngineCore::GetInst()->GetWindow().GetHWND(), hdc);

	//HDC backBuffer = CreateCompatibleDC(hdc);
	//bool a = SelectObject(backBuffer, hBitmap);
	///*if ( == NULL) {
	//	Rectangle(hdc, 10, 10, 200, 200);
	//	return;
	//}*/
	//BitBlt(hdc, 0, 0, 1000, 1000, backBuffer, 0, 0, SRCCOPY); // 백 버퍼 -> 메인 HDC
	//DeleteDC(backBuffer);
}