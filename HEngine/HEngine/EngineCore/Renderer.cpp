#include "Renderer.h"
#include "EngineHelper/EngineImage.h"
#include "Actor.h"
#include "EngineCore.h"
void Renderer::SetImage(EngineImage* _Image) {
	RImage = _Image;
}


void Renderer::SetOwner(Actor* _Actor) {
	Owner = _Actor;
}

void Renderer::Rendering() {
	if (RImage != nullptr) {
		RImage->DrawImage(0,0,100,100);
		//HDC mhdc = EngineCore::GetInst()->GetWindow().GetHDC();
		//HDC nhdc = CreateCompatibleDC(mhdc);
		//HBITMAP OldBitmap = (HBITMAP)SelectObject(nhdc, RImage->GetBitmap());
		//BITMAP bitmap;
		//GetObject(RImage->GetBitmap(), sizeof(BITMAP), &bitmap);
		//BitBlt(mhdc, 0, 0, 100, 100, nhdc, 0, 0, SRCCOPY);

		//SelectObject(mhdc, OldBitmap);
		//DeleteDC(nhdc);
	}
}

