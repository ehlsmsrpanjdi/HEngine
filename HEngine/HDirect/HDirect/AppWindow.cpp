#include "AppWindow.h"
#include "GraphicDevice.h"
#include "SwapChain.h"
#include "DeviceContext.h"
AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicDevice::GetEngine()->init();
	m_SwapChain = new SwapChain();
	m_SwapChain->init(m_hwnd, getClientWindowRect());
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicDevice::GetEngine()->GetContext()->clearRenderTargetColor(m_SwapChain, 1,1,1,1);
	m_SwapChain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	GraphicDevice::GetEngine()->release();
	m_SwapChain->release();
}
