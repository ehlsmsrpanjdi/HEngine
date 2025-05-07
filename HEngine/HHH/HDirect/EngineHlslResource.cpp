#include "EngineHlslResource.h"

namespace HlslNamespace {
	std::string Default = "DEFAULT";
	std::string Animation = "ANIMATION";
	std::string NoneAnimation = "NONEANIMATION";
}

EngineHlslResource::EngineHlslResource() 
{
}

EngineHlslResource::~EngineHlslResource() 
{
	HlslData = nullptr;
}

