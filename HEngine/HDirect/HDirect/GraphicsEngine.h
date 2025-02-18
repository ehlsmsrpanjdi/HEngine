#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>
#include <unordered_map>

class SwapChain;
class DeviceContext;
class GraphicsEngine
{
public:
	GraphicsEngine();
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init(HWND _hwnd, RECT rc);
	//Release all the resources loaded
	bool release();
	~GraphicsEngine();
public:
	SwapChain* createSwapChain();
	DeviceContext* createDeviceContext();
	bool createD3DDevice();
	class DepthView* createDepthView();


#pragma region "∞‘≈Õ"
public:
	DeviceContext* getImmediateDeviceContext();
	SwapChain* getSwapChain();
	static GraphicsEngine* get();

#pragma endregion


public:
	void ResizeBuffers();
	void ClearRenderTargetView(float red, float green, float blue, float alpha);



#pragma region "Ω¶¿Ã¥ı"
public:
	std::unordered_map<std::string, ID3DBlob*> VSBlobMap;
	std::unordered_map<std::string, ID3DBlob*> PSBlobMap;


	std::unordered_map<std::string, ID3D11VertexShader*> VSShader;
	std::unordered_map<std::string, ID3D11PixelShader*> PSShader;

	std::unordered_map<std::string, ID3DBlob*> ErrorBlobMap;

	std::unordered_map<std::string, ID3D11Buffer*> BufferMap;


	std::unordered_map<std::string, ID3D11InputLayout*> LayoutMap;

	void CreateHlsl();

	void CompileShader();
	void CreateBuffer();
	void CreateLayout();

#pragma endregion



public:
	UINT m4xMsaaQuality = 0;

private:
	DeviceContext* m_imm_device_context = nullptr;
	SwapChain* m_SwapChain = nullptr;
	DepthView* m_DepthView = nullptr;
private:
	ID3D11Device* m_d3d_device = nullptr;
	D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL();
private:
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;
	ID3D11DeviceContext* m_imm_context = nullptr;

private:
	HWND m_hwnd = NULL;

private:
	friend class SwapChain;
	friend class DepthView;
};