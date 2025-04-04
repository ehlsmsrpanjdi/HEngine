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

	void CreateHlsl(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager, std::shared_ptr<struct HS> _Material);
	void CreateLayout(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<struct HS> _Hlsl);
protected:

private:
	EngineHlsl();
	std::map<std::string, ID3D11VertexShader*> VSShader;
	std::map<std::string, ID3D11PixelShader*> PSShader;

	std::map<std::string, ID3DBlob*> VSBlobMap;
	std::map<std::string, ID3DBlob*> PSBlobMap;

	std::map<std::string, ID3D11InputLayout*> LayoutMap;

	std::map<std::string, ID3DBlob*> ErrorBlobMap;

	std::map<std::string, std::shared_ptr<struct HS>> HlslMap;
};

