#pragma once
#include <Windows.h>


class Window {
public:
	Window();
	bool init();
	bool broadcase();
	bool release();
	bool isRun();
	~Window();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void SetSize(int _x, int _y);
	virtual void OnSize();

	int MouseX = 0;
	int MouseY = 0;

	int MouseForceX = 0;
	int MouseForceY = 0;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	HWND m_hwnd = NULL;
	bool m_is_run = false;

	int WindowX = 1024;
	int WindowY = 768;

	
};

