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
				// PNG ������ Bitmap���� �ε�
				Gdiplus::Image* NImage = Gdiplus::Image::FromFile(entry.path().wstring().c_str());
				if (NImage == nullptr) {
					ErrorCheck("�̹��������� �߸���");
					return;
				}
				Gdiplus::Bitmap* pBitmap = reinterpret_cast<Gdiplus::Bitmap*>(NImage->Clone());
				if (pBitmap->GetLastStatus() != Gdiplus::Ok) {
					delete pBitmap;
					continue; // ���� �ε� ���� �� �ǳʶ�
				}

				HBITMAP hBitmap = NULL;
				COLORREF backgroundColor = RGB(255, 255, 255); 
				//�� �κ��� ������ �κ��� ���� �� �� �κ��� ��� �׸������� ���� ������
				if (pBitmap) {
					// GDI+ Bitmap ��ü���� HBITMAP���� ��ȯ
					pBitmap->GetHBITMAP(backgroundColor, &hBitmap);
				}
				if (hBitmap != NULL) {
					EngineImage* NewImage = new EngineImage(hBitmap);
					Resources[fileName] = NewImage;
				}
				//Bitmap�� HBITMAP���� ��ȯ �� EngineImage���� �� �� �ְ� �����
				// Bitmap ��ü�� Resources�� �߰�
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
