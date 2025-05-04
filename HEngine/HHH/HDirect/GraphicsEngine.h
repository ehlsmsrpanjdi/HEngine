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


#pragma region "게터"
public:
	DeviceContext* GetContext();
	SwapChain* getSwapChain();
	GraphicDevice* GetDevice();
	static GraphicsEngine* get();

#pragma endregion


public:
	void ResizeBuffers();
public:
#pragma region "멤버변수"
	std::unordered_map<std::string, ID3D11Buffer*> ConstantBufferMap;

	
#pragma endregion

#pragma region "Create"
	void CreateHlsl(std::shared_ptr<class EngineFile> _fileManager);
	void CreateScene(std::vector<std::shared_ptr<class EngineFScene>>& _Scenes);
	void CreateTexture(std::shared_ptr<class EngineFile> _fileManager);

	void CreateAllCBuffer();
	void CreateWVPBuffer();
	void CreateMeshBuffer();
	void CreateAnimationBuffer();

	void CreateLight();


#pragma endregion

	void UpdateConstantBuffer(const XMMATRIX& _Matrix, std::string_view _str);
	void UpdateConstantBuffer(const std::vector<DirectX::XMMATRIX>& matrices, std::string_view _str);

	void Render(struct HS* _Hlsl, struct MH* _Mesh);
	void CollisionRender(struct HS* _Hlsl, struct MH* _Mesh);


#pragma region "게터"

	struct HS* GetHlsl(std::string_view _str);

	std::shared_ptr<class FScene> GetScene(std::string_view _str);
	//.
#pragma endregion

#pragma region " 빌린거 "
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

private:
	HWND m_hwnd = NULL;

private:
	friend class SwapChain;
	friend class DepthView;
};