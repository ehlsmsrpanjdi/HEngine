#pragma once
#include "EngineImage.h"
#include "EngineFile.h"
#include <iostream>
#include <map>
#include <filesystem>
class EngineResource : public EngineFile{
public:
	EngineResource()
		:EngineFile()
	{

	}

	static EngineResource* GetInst() {
		if (ResourceManager == nullptr) {
			ResourceManager = new EngineResource();
		}
		return ResourceManager;
	}

	std::map<std::string, EngineImage*> Resources;

	EngineImage* GetImage(std::string_view _str);

	void FindImage(std::string_view _str);

	void FindImageRecursive(fs::path _my);

	void MakeImage();
	static EngineResource* ResourceManager;
};