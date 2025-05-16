#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <wrl.h>
#include <wincodec.h>
#include "EngineHelper/EngineTransform.h"

class SwapChain;
class DeviceContext;
class GraphicDevice;
class DepthView;


class GraphicsEngine
{
private:
	GraphicsEngine();
public:
	GraphicsEngine(const GraphicsEngine&) = delete;
	GraphicsEngine& operator=(const GraphicsEngine&) = delete;

	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init(HWND _hwnd, RECT rc);
	//Release all the resources loaded
	bool release();

	void Clear(float red, float green, float blue, float alpha);
	void Present(bool _bool);

	~GraphicsEngine();
public:
	bool createD3DDevice();


#pragma region "����"
public:
	DeviceContext* GetContext();
	SwapChain* getSwapChain();
	GraphicDevice* GetDevice();
	static GraphicsEngine* get();

#pragma endregion

#pragma region "����"
	void Test();
	ID3D11Buffer* TestVertex = nullptr;
#pragma endregion



public:
	void ResizeBuffers();
public:
#pragma region "Create"
	void CreateHlsl(std::shared_ptr<class EngineFile> _fileManager);
	void CreateScene(std::vector<std::shared_ptr<class EngineFScene>>& _Scenes);
	void CreateTexture(std::shared_ptr<class EngineFile> _fileManager);

	void CreateAllCBuffer();

#pragma endregion


	void Render(struct HS* _Hlsl, struct MH* _Mesh, ID3D11SamplerState* _Sampler);
	void CollisionRender(struct HS* _Hlsl, struct MH* _Mesh, ID3D11SamplerState* _Sampler);
	void SkyBoxRender(struct HS* _Hlsl, struct MH* _Mesh, ID3D11SamplerState* _Sampler);

#pragma region "����"
	std::shared_ptr<class FScene> GetScene(std::string_view _str);
	//.
#pragma endregion

#pragma region " ������ "
private:
	std::unordered_map<std::string, std::shared_ptr<struct Tex>>* TextureMap = nullptr;
#pragma endregion

public:
	UINT m4xMsaaQuality = 0;

	RECT WindowSize = { 0,0 };

private:
	std::shared_ptr<DeviceContext> m_Context = nullptr;
	std::shared_ptr<SwapChain> m_SwapChain = nullptr;
	std::shared_ptr<DepthView> m_DepthView = nullptr;
	std::shared_ptr<GraphicDevice> m_Device = nullptr;

private:
	D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL();
private:
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;

	ID3D11RasterizerState* BackFaceCull = nullptr;
	ID3D11RasterizerState* FrontFaceCull = nullptr;

private:
	HWND m_hwnd = NULL;

private:
	friend class SwapChain;
	friend class DepthView;
};