#include "AppWindow.h"
#include "GraphicDevice.h"
#include "SwapChain.h"
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
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	GraphicDevice::GetEngine()->release();
	m_SwapChain->release();
}
