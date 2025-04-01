#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>


#pragma comment(lib, "d3d11.lib")
// Ό³Έν :

class EngineMaterial
{
public:
	// constrcuter destructer
	~EngineMaterial();

	// delete Function
	EngineMaterial(const EngineMaterial& _Other) = delete;
	EngineMaterial(EngineMaterial&& _Other) noexcept = delete;
	EngineMaterial& operator=(const EngineMaterial& _Other) = delete;
	EngineMaterial& operator=(EngineMaterial&& _Other) noexcept = delete;

	static EngineMaterial& Get() {
		static EngineMaterial Material;
		return Material;
	}

	struct MT* GetMaterial(std::string_view _str);

	void CreateMaterial(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager);

	void CreateHlsl(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager, std::shared_ptr<struct MT> _Material);
	void CreateLayout(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<struct MT> _Material);
protected:

private:
	EngineMaterial();
	std::map<std::string, ID3D11VertexShader*> VSShader;
	std::map<std::string, ID3D11PixelShader*> PSShader;

	std::map<std::string, ID3DBlob*> VSBlobMap;
	std::map<std::string, ID3DBlob*> PSBlobMap;

	std::map<std::string, ID3D11InputLayout*> LayoutMap;

	std::map<std::string, ID3DBlob*> ErrorBlobMap;

	std::map<std::string, std::shared_ptr<struct MT>> MaterialMap;
};

