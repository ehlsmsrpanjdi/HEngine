#pragma once
#include <Windows.h>
#include <functional>
#include "EngineHelper/EngineTransform.h"


class EngineWindow {
public:
	friend class EngineCore;


	EngineWindow() {

	}

	~EngineWindow() {
		// BackBuffer HDC ����
		if (BackBuffer) {
			DeleteDC(BackBuffer);
			BackBuffer = nullptr;
		}

		// MainBuffer HDC ����
		if (MainBuffer) {
			DeleteDC(MainBuffer);
			MainBuffer = nullptr;
		}

		// HWND ���ҽ� ����
		if (hWnd) {
			DestroyWindow(hWnd);
			hWnd = nullptr;
		}

		// HINSTANCE�� �ý����� �ڵ����� �����ϹǷ� �߰����� ������ �ʿ� �����ϴ�.
		hInst = NULL;
	}

	const HWND GetHWND() {
		return hWnd;
	}

	const HDC GetHDC() {
		return MainBuffer;
	}

	const HDC GetBack() {
		return BackBuffer;
	}

private:
	BOOL WindowRegister(HINSTANCE hInstance);
	BOOL WindowOpen();
	void WindowStart(std::function<void(void)> _Start, std::function<void(void)> _End);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static void SetWindowSize(Int2D _int2D) {
		WindowSize.X = _int2D.X;
		WindowSize.Y = _int2D.Y;
	}

	static void SetWindowSize(int _x, int _y) {
		WindowSize.X = _x;
		WindowSize.Y = _y;
	}


	HINSTANCE hInst = NULL;
	HWND hWnd = nullptr;
	HDC MainBuffer = nullptr;
	HDC BackBuffer = nullptr;
	static BOOL WindowEnd;
	static Int2D WindowSize;
};