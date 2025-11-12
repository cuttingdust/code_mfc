#include "global.h"

#include "CWinApp.h"


/// 由CWinApp类派生CMyApp类------------------------------------------------------
class CMyApp : public CWinApp
{
public:
    BOOL InitInstance(int nCmdShow) override; /// 重新定义InitInstance函数
};
/// 重新定义的成员函数InitInstance --------------------------------------------------------
BOOL CMyApp::InitInstance(int nCmdShow)
{
    m_pMainWnd = new CFrameWnd;
    m_pMainWnd->Create(NULL, L"用新的InitInstance函数的程序");
    m_pMainWnd->ShowWindow(nCmdShow);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

/// 程序员定义的MyApp的对象theApp----------------------------------------------------
CMyApp theApp;
/// 主函数------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,     /// 当前实例句柄
                      _In_opt_ HINSTANCE hPrevInstance, /// 前一个实例 总是 NULL（Win16兼容性遗留）
                      _In_ LPWSTR        lpCmdLine,     /// 命令行参数
                      _In_ int           nCmdShow       /// 窗口显示模式
)
{
    /// 忽略未使用的参数
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int ResultCode = -1;
    theApp.InitInstance(nCmdShow);
    return ResultCode = theApp.Run();
}

/// 窗口函数的实现--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_LBUTTONDOWN:
            OnLButtonDown(hWnd, message, wParam, lParam);
            break;
        case WM_PAINT:
            OnPaint(hWnd, message, wParam, lParam);
            break;
        case WM_DESTROY:
            OnDestroy(hWnd, message, wParam, lParam);
            break;
        default:
            return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ShowText = L"Hello!";
    ::InvalidateRect(hWnd, NULL, 1);
}

void OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC         hdc;
    hdc = ::BeginPaint(hWnd, &ps);
    ::TextOut(hdc, 50, 50, ShowText, 6);
    ::EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ::PostQuitMessage(0);
}
