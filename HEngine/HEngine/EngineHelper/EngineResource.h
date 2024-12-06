#pragma once
#include "EngineImage.h"
#include "EngineFile.h"
#include <iostream>
#include <map>
#include <filesystem>
class EngineResource : public EngineFile {
public:
	friend class EngineCore;

	EngineResource()
		:EngineFile()
	{

	}
private:
	static void ReleaseResources() {
		for (std::pair<const std::string&, EngineImage*> pair : ResourceManager->Resources) {
			if (pair.second != nullptr) {
				delete pair.second;
			}
		}
	}
public:
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