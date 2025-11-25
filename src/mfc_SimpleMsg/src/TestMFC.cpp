#include "TestCoreMFC.h"

#define IDT_TIMER 10001

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
    m_pMainWnd = new CMainWindow;
    ::ShowWindow(*m_pMainWnd, m_nCmdShow);
    ::UpdateWindow(*m_pMainWnd);
    return TRUE;
}

CMainWindow::CMainWindow()
{
    m_szText[0]           = '\0';
    LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW),
                                                (HBRUSH)(COLOR_3DFACE + 1), NULL);

    CreateEx(WS_EX_CLIENTEDGE, lpszClassName, L"CoreMFC演示", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300,
             230, NULL, NULL);
    CenterWindow();
}

BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lp)
{
    ::GetClientRect(m_hWnd, &m_rcInfo);
    m_rcInfo.left   = 30;
    m_rcInfo.top    = 20;
    m_rcInfo.right  = m_rcInfo.right - 30;
    m_rcInfo.bottom = m_rcInfo.bottom - 30;

    ::SetTimer(m_hWnd, IDT_TIMER, 500, NULL);
    return TRUE;
}

void CMainWindow::OnTimer(UINT nID)
{
    if (nID == IDT_TIMER)
    {
        SYSTEMTIME time;
        ::GetLocalTime(&time);
        m_szText[0] = '\0';
        wchar_t szBuff[128];
        wsprintf(szBuff, L"当前的时间是%.2d:%.2d:%.2d \n", time.wHour, time.wMinute, time.wSecond);
        wcscat(m_szText, szBuff);
        ::InvalidateRect(m_hWnd, &m_rcInfo, TRUE);
    }
}

void CMainWindow::OnPaint()
{
    PAINTSTRUCT ps;
    HDC         hdc = ::BeginPaint(m_hWnd, &ps);
    /// 绘制
    ::DrawText(hdc, m_szText, wcslen(m_szText), &m_rcInfo, 0);
    ::EndPaint(m_hWnd, &ps);
}

void CMainWindow::PostNcDestroy()
{
    delete this;
}
