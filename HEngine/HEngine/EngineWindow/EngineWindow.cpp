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
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);  // 기본 애플리케이션 아이콘
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);    // 기본 화살표 커서
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;        // 메뉴 없음
    wcex.lpszClassName = EngineName;   // 클래스 이름 (필요에 따라 변경)
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); // 작은 아이콘도 기본 아이콘 사용

    if (!RegisterClassExW(&wcex)) {
        return FALSE;
    }

    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    return TRUE;
}

BOOL EngineWindow::WindowOpen()
{

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);  // 화면 가로 해상도
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 화면 세로 해상도

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

    // 기본 메시지 루프입니다:
    while (true)
    {
        // 메시지를 대기하지 않고, 비어있는 메시지 큐도 처리할 수 있도록 PeekMessage 사용
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // WM_QUIT 메시지가 들어오면 루프 종료
            if (msg.message == WM_QUIT)
                break;

            // 가속기 처리
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                // 메시지 번역 및 디스패치
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            // 메세지 루프의 데드타임이라는 곳에서 실행됩니다.
            if (nullptr != _Start)
            {
                _Start();
            }
        }
        // 메시지가 없으면 추가적인 작업을 할 수 있다.
        // 예를 들어, 애플리케이션이 메시지 큐에 대기하지 않고 작업을 수행할 수 있다.
        // 이 부분에 다른 비동기적인 작업이나 렌더링 등을 넣을 수 있다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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

