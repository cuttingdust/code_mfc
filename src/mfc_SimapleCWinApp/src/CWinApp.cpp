#include "global.h"
#include "CWinApp.h"

/// 定义全局变量（只在一个文件中定义）
HINSTANCE      hInst           = nullptr;
HINSTANCE      hInstance       = nullptr;
MSG            msg             = {};
wchar_t        lpszClassName[] = L"window_class";
const wchar_t *ShowText        = L"初始文本";

/// 应用程序类的成员函数-------------------------------------------------------------------
BOOL CWinApp::InitInstance(int nCmdShow)
{
    m_pMainWnd = new CFrameWnd;
    m_pMainWnd->Create(NULL, L"封装的Windows程序");
    m_pMainWnd->ShowWindow(nCmdShow);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

int CWinApp::Run()
{
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

CWinApp::~CWinApp()
{
    delete m_pMainWnd;
}
