#include "pch.h"
#include "EngineResource.h"
#include "EngineDefine/EngineDefine.h"
#include "EngineString.h"

EngineResource* EngineResource::ResourceManager = nullptr;

void EngineResource::FindImageRecursive(fs::path _my) {
	fs::path pp("");
	for (const fs::directory_entry& entry : fs::directory_iterator(_my)) {
		if (entry.is_directory()) {
			FindImageRecursive(entry.path());
		}
		else if (entry.is_regular_file()) {
			if (entry.path().extension() == ".png") {
				std::string fileName = entry.path().filename().stem().string();
				EngineString::Upper(fileName);
				// PNG 파일을 Bitmap으로 로드
				Gdiplus::Image* NImage = Gdiplus::Image::FromFile(entry.path().wstring().c_str());
				if (NImage == nullptr) {
					ErrorCheck("이미지생성이 잘못됨");
					return;
				}
				Gdiplus::Bitmap* pBitmap = reinterpret_cast<Gdiplus::Bitmap*>(NImage->Clone());
				if (pBitmap->GetLastStatus() != Gdiplus::Ok) {
					delete pBitmap;
					continue; // 파일 로드 실패 시 건너뜀
				}

				HBITMAP hBitmap = NULL;
				COLORREF backgroundColor = RGB(255, 255, 255); 
				//이 부분은 투명한 부분이 있을 때 그 부분을 어떻게 그릴건지에 대한 내용임
				if (pBitmap) {
					// GDI+ Bitmap 객체에서 HBITMAP으로 변환
					pBitmap->GetHBITMAP(backgroundColor, &hBitmap);
				}
				if (hBitmap != NULL) {
					EngineImage* NewImage = new EngineImage(hBitmap);
					Resources[fileName] = NewImage;
				}
				//Bitmap을 HBITMAP으로 변환 후 EngineImage에서 쓸 수 있게 만들것
				// Bitmap 객체를 Resources에 추가
			}
		}
	}
}

EngineImage* EngineResource::GetImage(std::string_view _str) {
	std::string key(_str);
	EngineString::Upper(key);
	if (Resources.contains(key) == true) {
		return Resources[key];
	}
	else {
		ErrorCheck("Resource is None" + std::string(_str));
		return nullptr;
	}
}
