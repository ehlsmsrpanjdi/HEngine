#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "iostream"
#include <d3dcompiler.h>
#include <cassert>
#include "DepthView.h"
void PrintSupportedDisplayModes(IDXGIOutput* output)
{
	// 지원되는 디스플레이 모드를 가져올 수 있는 크기 확인
	UINT numModes = 0;
	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr);

	if (numModes == 0)
	{
		std::cout << "지원되는 R8G8B8A8_UNORM 포맷 디스플레이 모드가 없습니다." << std::endl;
		return;
	}

	// 디스플레이 모드 배열
	DXGI_MODE_DESC* modes = new DXGI_MODE_DESC[numModes];

	// 디스플레이 모드 목록 가져오기
	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, modes);

	std::cout << "지원되는 R8G8B8A8_UNORM 포맷 디스플레이 모드:" << std::endl;
	for (UINT i = 0; i < numModes; ++i)
	{
		const DXGI_MODE_DESC& mode = modes[i];
		std::cout << "해상도: " << mode.Width << "x" << mode.Height
			<< ", 갱신률: " << mode.RefreshRate.Numerator << "/" << mode.RefreshRate.Denominator
			<< ", 비율: " << (float)mode.RefreshRate.Numerator / mode.RefreshRate.Denominator << "Hz" << std::endl;
	}

	delete[] modes;
}

void PrintAdapterDescription(const wchar_t* description)
{
	char buffer[128]; // 변환된 문자열을 저장할 버퍼
	WideCharToMultiByte(CP_ACP, 0, description, -1, buffer, sizeof(buffer), nullptr, nullptr);
	std::cout << buffer;
}

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init(HWND _hwnd)
{
	m_hwnd = _hwnd;
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}

	m4xMsaaQuality;
	m_d3d_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);



	m_imm_device_context = new DeviceContext(m_imm_context);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	UINT index = 0;
	UINT Aindex = 0;

	while (m_dxgi_factory->EnumAdapters(index, &m_dxgi_adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		m_dxgi_adapter->GetDesc(&adapterDesc);

		std::cout << "Adapter " << index << ": ";
		PrintAdapterDescription(adapterDesc.Description);
		std::cout << std::endl;
		std::cout << "  - VRAM: " << (adapterDesc.DedicatedVideoMemory / (1024 * 1024)) << " MB" << std::endl;
		std::cout << "  - Vendor ID: " << adapterDesc.VendorId << std::endl;

		IDXGIOutput* t_output = nullptr;
		m_dxgi_adapter->EnumOutputs(Aindex, &t_output);
		if (t_output) {
			PrintSupportedDisplayModes(t_output);
		}

		m_dxgi_adapter->Release();  // 해제
		index++;
	}


	return true;
}


bool GraphicsEngine::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_imm_device_context->release();


	m_d3d_device->Release();
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DepthView* GraphicsEngine::createDepthView()
{
	return new DepthView();
}


DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader();

	if (!vs->init(shader_byte_code, byte_code_size))
	{
		vs->release();
		return nullptr;
	}

	return vs;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader();

	if (!ps->init(shader_byte_code, byte_code_size))
	{
		ps->release();
		return nullptr;
	}

	return ps;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob)m_blob->Release();
}



GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}