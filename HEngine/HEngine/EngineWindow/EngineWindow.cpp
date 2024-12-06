#include "pch.h"
#include "EngineWindow.h"
#define IDC_SMENGINE 1001

Int2D EngineWindow::WindowSize = Int2D();
BOOL EngineWindow::WindowEnd = FALSE;


BOOL EngineWindow::WindowRegister(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = EngineWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);  // �⺻ ���ø����̼� ������
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);    // �⺻ ȭ��ǥ Ŀ��
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;        // �޴� ����
    wcex.lpszClassName = EngineName;   // Ŭ���� �̸� (�ʿ信 ���� ����)
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); // ���� �����ܵ� �⺻ ������ ���

    if (!RegisterClassExW(&wcex)) {
        return FALSE;
    }

    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    return TRUE;
}

BOOL EngineWindow::WindowOpen()
{

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // ȭ�� ���� �ػ�
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // ȭ�� ���� �ػ�

    hWnd = CreateWindowW(
        EngineName,
        EngineTitle,
        WS_OVERLAPPEDWINDOW,
        screenWidth / 4,     // x
        screenHeight / 4,                         // y
        WindowSize.X,     // nWidth
        WindowSize.Y,                         // nHeight    
        nullptr,
        nullptr,
        hInst,
        nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MainBuffer = GetDC(hWnd);

    return TRUE;
}

BOOL EngineWindow::WindowStart(std::function<void(void)> _Start, std::function<void(void)> _End) {


    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_SMENGINE));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    while (true)
    {
        // �޽����� ������� �ʰ�, ����ִ� �޽��� ť�� ó���� �� �ֵ��� PeekMessage ���
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // WM_QUIT �޽����� ������ ���� ����
            if (msg.message == WM_QUIT)
                break;

            // ���ӱ� ó��
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                // �޽��� ���� �� ����ġ
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            // �޼��� ������ ����Ÿ���̶�� ������ ����˴ϴ�.
            if (nullptr != _Start)
            {
                _Start();
            }
        }
        // �޽����� ������ �߰����� �۾��� �� �� �ִ�.
        // ���� ���, ���ø����̼��� �޽��� ť�� ������� �ʰ� �۾��� ������ �� �ִ�.
        // �� �κп� �ٸ� �񵿱����� �۾��̳� ������ ���� ���� �� �ִ�.
    }

    if (nullptr != _End && WindowEnd == TRUE) {
        _End();
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK EngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        WindowEnd = TRUE;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK EngineWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

