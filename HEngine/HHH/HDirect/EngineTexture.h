#pragma once
#include <d3d11.h>
#include <memory>
#include <unordered_map>
#include <map>
#include "iostream"

// Ό³Έν :
class EngineTexture
{
public:
	// constrcuter destructer
	EngineTexture();
	~EngineTexture();

	static EngineTexture& GetInst() {
		static EngineTexture texture;
		return texture;
	}

	// delete Function
	EngineTexture(const EngineTexture& _Other) = delete;
	EngineTexture(EngineTexture&& _Other) noexcept = delete;
	EngineTexture& operator=(const EngineTexture& _Other) = delete;
	EngineTexture& operator=(EngineTexture&& _Other) noexcept = delete;

	void CreateAllTexture(ID3D11Device* device, ID3D11DeviceContext* context, std::shared_ptr<class EngineFile> _filemanager);

	void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filename, std::string_view _Origin);

	std::unordered_map<std::string, std::shared_ptr<struct Tex>> TextureMap;
protected:

private:
	friend class GraphicsEngine;
	std::unordered_map<std::string, std::shared_ptr<struct Tex>>& GetTexture();
};

