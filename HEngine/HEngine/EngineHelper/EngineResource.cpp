#include "EngineResource.h"
#include "EngineDefine/EngineDefine.h"


EngineResource* EngineResource::ResourceManager = nullptr;

void EngineResource::FindImageRecursive(fs::path _my) {
	fs::path pp("");
	for (const fs::directory_entry& entry : fs::directory_iterator(_my)) {
		if (entry.is_directory()) {
			FindImageRecursive(entry.path());
		}
		else if (entry.is_regular_file()) {
			if (entry.path().extension() == ".png") {
				std::string fileName = entry.path().filename().string();

				// PNG ������ Bitmap���� �ε�
				Bitmap* pBitmap = new Bitmap(entry.path().wstring().c_str());

				// Bitmap ��ü�� Resources�� �߰�
				Resources[fileName] = pBitmap;
			}
		}
	}
}

Bitmap* EngineResource::GetImage(std::string_view _str) {
	if (Resources.contains(_str) == true) {
		return Resources[_str];
	}
	else {
		ErrorCheck("Resource is None" + std::string(_str));
		return nullptr;
	}
}
