#include "SwapChain.h"
#include "GraphicsEngine.h"
#include "GraphicDevice.h"

SwapChain::SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
    ID3D11Device* device = GraphicsEngine::get()->GetDevice()->Get();

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = 0;

    if (GraphicsEngine::get()->m4xMsaaQuality > 0) {
        desc.SampleDesc.Count = 4;
        desc.SampleDesc.Quality = GraphicsEngine::get()->m4xMsaaQuality - 1;
    }

    // Create the swap chain for the window indicated by HWND parameter
    HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);
    if (FAILED(hr)) {
        return false;
    }

    if (GraphicsEngine::get()->m_dxgi_factory) {
        GraphicsEngine::get()->m_dxgi_factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
    }

    // Get the back buffer color and create its render target view
    ID3D11Texture2D* buffer = nullptr;
    hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
    if (FAILED(hr)) {
        return false;
    }

    hr = device->CreateRenderTargetView(buffer, nullptr, &m_rtv);
    buffer->Release();
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

bool SwapChain::present(bool vsync)
{
    HRESULT hr = m_swap_chain->Present(vsync ? 1 : 0, 0);
    return SUCCEEDED(hr);
}

void SwapChain::release()
{
    if (m_swap_chain != nullptr) {
        m_swap_chain->Release();
        m_swap_chain = nullptr;
    }
    if (m_rtv != nullptr) {
        m_rtv->Release();
        m_rtv = nullptr;
    }
}

SwapChain::~SwapChain()
{
    release();
}