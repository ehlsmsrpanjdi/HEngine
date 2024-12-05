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
				Bitmap* pBitmap = new Bitmap(entry.path().wstring().c_str());
				if (pBitmap->GetLastStatus() != Ok) {
					delete pBitmap;
					continue; // 파일 로드 실패 시 건너뜀
				}

				EngineImage* NewImage = new EngineImage(pBitmap);

				// Bitmap 객체를 Resources에 추가
				Resources[fileName] = NewImage;
			}
		}
	}
}

EngineImage* EngineResource::GetImage(std::string_view _str) {
	std::string key(_str);
	if (Resources.contains(key) == true) {
		return Resources[key];
	}
	else {
		ErrorCheck("Resource is None" + std::string(_str));
		return nullptr;
	}
}
