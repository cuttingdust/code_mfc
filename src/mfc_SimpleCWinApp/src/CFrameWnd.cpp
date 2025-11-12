#include "CFrameWnd.h"
#include "global.h"


/// 窗口类的成员函数---------------------------------------------------------------------------

int CFrameWnd::RegisterWindow()
{
    WNDCLASS wc;
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = lpszClassName;
    return ::RegisterClass(&wc);
}

void CFrameWnd::Create(LPCTSTR lpClassName, LPCTSTR lpWindowName)
{
    RegisterWindow();
    hInst = hInstance;
    hWnd  = CreateWindow(lpszClassName, lpWindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL,
                         NULL, hInstance, NULL);
}

void CFrameWnd::ShowWindow(int nCmdShow)
{
    ::ShowWindow(hWnd, nCmdShow);
}

void CFrameWnd::UpdateWindow()
{
    ::UpdateWindow(hWnd);
}
