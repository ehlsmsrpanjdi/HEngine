#include "AppWindow.h"
#include "EngineHelper/EngineKey.h"
#include "EngineHelper/EngineTime.h"
#include "EngineHelper/Vector3D.h"
#include "EngineHelper/Matrix4x4.h"

struct vertex {
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


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

	vertex list[] =
	{
		//X - Y - Z
		{Vector3D(-0.5f,-0.5f,0.0f),    Vector3D(-0.32f,-0.11f,0.0f),   Vector3D(0,0,0), Vector3D(0,1,0) }, // POS1
		{Vector3D(-0.5f,0.5f,0.0f),     Vector3D(-0.11f,0.78f,0.0f),   Vector3D(1,1,0), Vector3D(0,1,1) }, // POS2
		{ Vector3D(0.5f,-0.5f,0.0f),     Vector3D(0.75f,-0.73f,0.0f), Vector3D(0,0,1),  Vector3D(1,0,0) },// POS2
		{ Vector3D(0.5f,0.5f,0.0f),     Vector3D(0.88f,0.77f,0.0f),    Vector3D(1,1,1), Vector3D(0,0,1) }
	};

}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	KeyManager->Update();
	TimeManager->Update();

	if (EngineKey::IsDown('Z') == true) {
		int a = 0;
	}
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}