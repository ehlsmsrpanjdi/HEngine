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

	std::map<std::string_view, BITMAP*> Resources;


	static EngineResource* ResourceManager;
};