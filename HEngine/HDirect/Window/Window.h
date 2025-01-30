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

protected:
	HWND m_hwnd = NULL;
	bool m_is_run = false;;
};

