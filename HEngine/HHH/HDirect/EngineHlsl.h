#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>


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

	struct HS* GetHlsl(std::string_view _str);

	void CreateHlsl(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager);

	void createSampler(ID3D11Device* _Device);

	void CreateHlsl(std::shared_ptr<GraphicDevice> _Device, std::string_view _str, std::shared_ptr<HS> _Hlsl);

protected:

private:
	EngineHlsl();

	std::map<std::string, std::shared_ptr<struct HS>> HlslMap;
	std::map<std::string, ID3D11SamplerState*> SamplerMap;
};

