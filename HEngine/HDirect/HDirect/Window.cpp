#include "Window.h"
#define EngineName "UDirectX"

Window* window = NULL;


Window::Window()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE: {
		//When Window Create
		window->setHWND(hwnd);
		window->onCreate();

		break;
	}
	case WM_DESTROY: {
		//When Window Destroy
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default: {
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}


		   return NULL;


	}
	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = EngineName;
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;


	if (!::RegisterClassEx(&wc)) {
		return false;
	}

	if (!window) {
		window = this;
	}

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, EngineName, "DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd) {
		return false;
	}

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);


	m_is_run = true;

	return true;
}

bool Window::broadcase() {

	MSG msg;
	window->onUpdate();


	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	Sleep(0);

	return false;
}

bool Window::release()
{

	if (m_hwnd) {
		if (!DestroyWindow(m_hwnd)) {
			return false;
		}
	}
	return false;
}

bool Window::isRun()
{
	return m_is_run;
}

Window::~Window()
{
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}
