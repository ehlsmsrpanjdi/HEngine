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

#pragma region "리소스"
	void CreateHlsl(std::shared_ptr<class EngineFile> _fileManager);
	void CreateMesh(std::vector<std::shared_ptr<struct FMesh>>& _AllMesh);
	void CreateAllCBuffer();
	void CreateConstantBuffer(std::string _str);
	void UpdateConstantBuffer(const XMMATRIX& _transform, std::string_view _str);

	void CreateTexture(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filename);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRV;


	void Render(struct HS* _Material, struct MH* _Mesh);

#pragma endregion

#pragma region "게터"

	struct HS* GetHlsl(std::string_view _str);

	struct MH* GetMesh(std::string_view _str);

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