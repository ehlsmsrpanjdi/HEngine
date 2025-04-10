#include "Window/AppWindow.h"
#include "EngineHelper/EngineFile.h"
#include "EngineHelper/EngineTime.h"
#include "EngineHelper/EngineKey.h"
#include "HDirect/GraphicsEngine.h"
#include "GameEngine/GameEngine.h"
#include "EngineHelper/EngineDebug.h"
#include "EngineHelper/FBXTool.h"
#include "EngineHelper/AllStruct.h"
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
	if (false == FileManager->MovetoFile("Resource")) {
		EngineDebug::Error("���°�η� �̵���");
	}
	FileManager->AllExtendFileRecursive();

	FBXTool::GetInst().Init();
	
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->init(m_hwnd, rc);
	FBXTool::GetInst().LoadALLFBX(FileManager);
	//GraphicsEngine::get()->CreateTexture(FileManager);
	//GraphicsEngine::get()->CreateMesh(FBXTool::GetInst().GetMesh());
	//GraphicsEngine::get()->CreateHlsl(FileManager);
	GameEngine::get()->Init(rc);
}


void AppWindow::onUpdate()
{
	Window::onUpdate();
	KeyManager->Update();
	
	EngineKey::MouseX = -MouseForceX;
	EngineKey::MouseY = -MouseForceY;
	
	GameEngine::get()->Update(TimeManager->Update());
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
