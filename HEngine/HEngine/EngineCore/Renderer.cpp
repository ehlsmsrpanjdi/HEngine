#include "Renderer.h"
#include "EngineHelper/EngineImage.h"
#include "Actor.h"
#include "EngineCore.h"
void Renderer::SetImage(Bitmap* _bitmap) {
	RBitmap = _bitmap;
}


void Renderer::SetOwner(Actor* _Actor) {
	Owner = _Actor;
}

void Renderer::Rendering() {
	if (RBitmap != nullptr) {
		HDC mhdc = EngineCore::GetInst()->GetWindow().GetHDC();
		HDC nhdc = CreateCompatibleDC(mhdc);
		HBITMAP OldBitmap = (HBITMAP)SelectObject(nhdc, RBitmap);
		BITMAP bitmap;
		GetObject(RBitmap, sizeof(BITMAP), &bitmap);
		BitBlt(mhdc, 0, 0, 100, 100, nhdc, 0, 0, SRCCOPY);

		SelectObject(mhdc, OldBitmap);
		DeleteDC(nhdc);
	}
}

