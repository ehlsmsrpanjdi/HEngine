#include "AppWindow.h"
#include "EngineHelper/EngineKey.h"
#include "EngineHelper/EngineTime.h"

AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	KeyManager = new EngineKey();
	KeyManager->Init();

	TimeManager = new EngineTime();
	TimeManager->Init();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	KeyManager->Update();
	TimeManager->Update();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}