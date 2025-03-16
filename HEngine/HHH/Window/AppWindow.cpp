#include "Window/AppWindow.h"
#include "EngineHelper/EngineFile.h"
#include "EngineHelper/EngineTime.h"
#include "EngineHelper/EngineKey.h"
#include "HDirect/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

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

	FileManager->Move_parent();
	FileManager->MovetoFile("HDirect");
	FileManager->AllExtendFileRecursive(".hlsl");
	
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->init(m_hwnd, rc);
	GraphicsEngine::get()->CreateHlsl(FileManager.get());
	GameEngine::get()->Init(rc);
}


void AppWindow::onUpdate()
{
	Window::onUpdate();
	GameEngine::get()->Update(1.f);
	KeyManager->Update();
	TimeManager->Update();

	if (EngineKey::IsDown('Z') == true) {
		int a = 0;
	}
	GraphicsEngine::get()->Presnet(1.0f,0.5,0.5,1.0f, true);

}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	KeyManager = nullptr;
	TimeManager = nullptr;
	FileManager = nullptr;
	GraphicsEngine::get()->release();
}

void AppWindow::OnSize()
{
	//GraphicsEngine::get()->ResizeBuffers();
}
