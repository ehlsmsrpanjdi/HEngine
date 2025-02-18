#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#include "AppWindow.h"
#include "EngineHelper/EngineKey.h"
#include "EngineHelper/EngineTime.h"
#include "EngineHelper/Vector3D.h"
#include "EngineHelper/Matrix4x4.h"
#include "HDirect/GraphicsEngine.h"
#include "memory"
#include "HDirect/SwapChain.h"
#include "HDirect/DeviceContext.h"
#include <DirectXMath.h>
#include "EngineHelper/EngineFile.h"


AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	KeyManager = std::make_shared<EngineKey>(EngineKey());
	KeyManager->Init();
	TimeManager = std::make_shared<EngineTime>(EngineTime());
	TimeManager->Init();
	FileManager = std::make_shared<EngineFile>(EngineFile());



	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->init(m_hwnd, rc);

	GraphicsEngine::get()->getSwapChain()->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	


	GraphicsEngine::get()->CreateHlsl();

}


void AppWindow::onUpdate()
{
	Window::onUpdate();
	KeyManager->Update();
	TimeManager->Update();

	if (EngineKey::IsDown('Z') == true) {
		int a = 0;
	}

	GraphicsEngine::get()->ClearRenderTargetView(0, 0.3f, 0.4f, 1.f);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->getSwapChain()->present(false);



}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::OnSize()
{
	GraphicsEngine::get()->ResizeBuffers();
}
