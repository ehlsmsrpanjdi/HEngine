#pragma once
#include <d3d11.h>
#include <map>
#include <iostream>
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

public:
	DeviceContext* getImmediateDeviceContext();
	SwapChain* getSwapChain();

public:
	void ResizeBuffers();
	void ClearRenderTargetView(float red, float green, float blue, float alpha);



#pragma region "Ω¶¿Ã¥ı"
public:
	std::map<std::string, ID3DBlob*> BlobMap;
	
	void CompileShader();
#pragma endregion



public:
	IndexBuffer* createIndexBuffer();
	VertexBuffer* createVertexBuffer();
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);
	ConstantBuffer* createConstantBuffer();
public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	void releaseCompiledShader();

public:
	static GraphicsEngine* get();

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