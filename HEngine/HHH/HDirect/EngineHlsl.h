#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>
#include <unordered_map>
#include "EngineHelper/AllStruct.h"

#pragma comment(lib, "d3d11.lib")
// Ό³Έν :

class EngineHlsl
{
public:
	// constrcuter destructer
	~EngineHlsl();

	// delete Function
	EngineHlsl(const EngineHlsl& _Other) = delete;
	EngineHlsl(EngineHlsl&& _Other) noexcept = delete;
	EngineHlsl& operator=(const EngineHlsl& _Other) = delete;
	EngineHlsl& operator=(EngineHlsl&& _Other) noexcept = delete;

	static EngineHlsl& Get() {
		static EngineHlsl HS;
		return HS;
	}

	void CreateHlsl(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager);

	void CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::string_view _filename, std::string_view _filepath, std::shared_ptr<HS> _Hlsl);

protected:

private:
	EngineHlsl();

	std::unordered_map<std::string, std::shared_ptr<class EngineHlslResource>> HlslResourceMap;
	std::vector<std::shared_ptr<class EngineSamplerResource>> SamplerVector;
};

