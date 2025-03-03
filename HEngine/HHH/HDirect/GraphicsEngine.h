#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>
#include <unordered_map>
#include <memory>

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

	void Presnet(float red, float green, float blue, float alpha, bool _bool);

	~GraphicsEngine();
public:
	bool createD3DDevice();


#pragma region "∞‘≈Õ"
public:
	DeviceContext* GetContext();
	SwapChain* getSwapChain();
	GraphicDevice* GetDevice();
	static GraphicsEngine* get();

#pragma endregion


public:
	void ResizeBuffers();
#pragma region "Ω¶¿Ã¥ı"
public:
	std::unordered_map<std::string, ID3DBlob*> VSBlobMap;
	std::unordered_map<std::string, ID3DBlob*> PSBlobMap;


	std::unordered_map<std::string, ID3D11VertexShader*> VSShader;
	std::unordered_map<std::string, ID3D11PixelShader*> PSShader;

	std::unordered_map<std::string, ID3DBlob*> ErrorBlobMap;

	std::unordered_map<std::string, ID3D11Buffer*> BufferMap;


	std::unordered_map<std::string, ID3D11InputLayout*> LayoutMap;

	void CreateHlsl(class EngineFile* _fileManager);

	void CompileShader(class EngineFile* _fileManager);
	void CreateBuffer();
	void CreateLayout();

	void SetBuffer();

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