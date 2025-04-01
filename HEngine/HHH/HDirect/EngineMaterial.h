#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>
// Ό³Έν :

struct MT {
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11InputLayout* Layout;
};

class EngineMaterial
{
public:
	// constrcuter destructer
	EngineMaterial();
	~EngineMaterial();

	// delete Function
	EngineMaterial(const EngineMaterial& _Other) = delete;
	EngineMaterial(EngineMaterial&& _Other) noexcept = delete;
	EngineMaterial& operator=(const EngineMaterial& _Other) = delete;
	EngineMaterial& operator=(EngineMaterial&& _Other) noexcept = delete;

	void CreateMaterial(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager);

	void CreateHlsl(std::shared_ptr<class GraphicDevice> _Device, std::shared_ptr<class EngineFile> _fileManager);
	void CreateLayout(std::shared_ptr<class GraphicDevice> _Device);
protected:

private:
	std::map<std::string, ID3D11VertexShader*> VSShader;
	std::map<std::string, ID3D11PixelShader*> PSShader;

	std::map<std::string, ID3DBlob*> VSBlobMap;
	std::map<std::string, ID3DBlob*> PSBlobMap;


	std::map<std::string, ID3D11VertexShader*> VSShader;
	std::map<std::string, ID3D11PixelShader*> PSShader;
	std::map<std::string, ID3D11InputLayout*> LayoutMap;

	std::map<std::string, ID3DBlob*> ErrorBlobMap;

};

