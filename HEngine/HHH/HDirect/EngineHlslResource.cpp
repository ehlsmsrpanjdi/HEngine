#include "EngineHlslResource.h"

namespace HlslNamespace {
	const std::string Default = "DEFAULT";
	const std::string Animation = "ANIMATION";
	const std::string NoneAnimation = "NONEANIMATION";
	const std::string Background = "BACKGROUND";
}

EngineHlslResource::EngineHlslResource() 
{
}

EngineHlslResource::~EngineHlslResource() 
{
	HlslData = nullptr;
}

