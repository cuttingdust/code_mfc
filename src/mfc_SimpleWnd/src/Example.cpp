#include "Example.h"

#include <iostream>
CMyApp theApp;

BOOL CMyApp::InitInstance()
{
    m_pMainWnd = new CMyWnd;
    ::ShowWindow(*m_pMainWnd, this->m_nCmdShow);
    ::UpdateWindow(*m_pMainWnd);
    return TRUE;
}

CMyWnd::CMyWnd()
{
    LPCTSTR lpszClassName =
            AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE + 1));

    CreateEx(WS_EX_CLIENTEDGE, lpszClassName, L"这是我们创建的窗口CWnd", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);
}

LRESULT CMyWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCDESTROY)
    {
        ::PostQuitMessage(0);
        delete this;
        return 0; //Cwnd对象就要被销毁了
    }
    return Default();
}
