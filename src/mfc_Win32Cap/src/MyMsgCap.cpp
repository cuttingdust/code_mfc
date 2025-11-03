#include <Windows.h>

#include <string>

static std::wstring wstr;
struct MSGMAP_ENTRY
{
    UINT nMessage;
    LONG (*pfn)(HWND, UINT, WPARAM, LPARAM);
};
LONG OnCreate(HWND, UINT, WPARAM, LPARAM);
LONG OnPaint(HWND, UINT, WPARAM, LPARAM);
LONG OnLButtonDown(HWND, UINT, WPARAM, LPARAM);
/// 定义一个对应
struct MSGMAP_ENTRY _messageEntries[] = {
    { .nMessage = WM_CREATE, .pfn = OnCreate },
    { .nMessage = WM_PAINT, .pfn = OnPaint },
    { .nMessage = WM_LBUTTONDOWN, .pfn = OnLButtonDown },
};

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t    wszClassName[] = L"MainWClass";
    WNDCLASSEX wndclass;

    wndclass.cbSize        = sizeof(wndclass);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = MainWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = ::LoadIcon(hInstance, IDI_APPLICATION);
    wndclass.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = wszClassName;
    wndclass.hIconSm       = NULL;

    ::RegisterClassEx(&wndclass);

    /// 创建主窗口
    /// 创建主窗口
    HWND hWnd = ::CreateWindowEx(0,                         /// 扩展样式
                                 wszClassName,              /// 窗口类名
                                 TEXT("我的第一个GUI程序"), /// 窗口标题
                                 WS_OVERLAPPEDWINDOW,       /// 窗口样式
                                 CW_USEDEFAULT,             /// x坐标
                                 CW_USEDEFAULT,             /// y坐标
                                 CW_USEDEFAULT,             /// 宽度
                                 CW_USEDEFAULT,             /// 高度
                                 NULL,                      /// 父窗口
                                 NULL,                      /// 菜单
                                 hInstance,                 /// 实例句柄
                                 NULL);                     /// 创建参数
    if (NULL == hWnd)
    {
        ::MessageBox(NULL, TEXT("创建窗口失败"), TEXT("错误"), MB_OK);
        return -1;
    }

    /// 显示窗口
    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return msg.wParam;
}


/// 工程学观点：隔离变化，C++封装，封装变化
/// 提取公因式，合并同类项
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    for (int i = 0; i < (sizeof(_messageEntries) / sizeof(_messageEntries[0])); i++)
    {
        if (message == _messageEntries[i].nMessage)
        {
            return ((*_messageEntries[i].pfn)(hWnd, message, wparam, lparam));
        }
    }

    // return 0; ///  错误！窗口会行为异常
    /// 窗口无法正常关闭（×按钮失效）
    ///
    /// 无法移动、调整大小
    ///
    /// 失去焦点、激活状态异常
    ///
    /// 可能无法重绘非客户区

    return ::DefWindowProc(hWnd, message, wparam, lparam);
}


LONG OnCreate(HWND hWnd, UINT nMsg, WPARAM wparam, LPARAM lparam)
{
    MessageBox(hWnd, TEXT("Hello MsgCap"), TEXT("myCap"), MB_OK);
    return ::DefWindowProc(hWnd, nMsg, wparam, lparam);
}

LONG OnPaint(HWND hWnd, UINT nMsg, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HDC         hdc = ::BeginPaint(hWnd, &ps);

    ::SetTextColor(hdc, RGB(255, 0, 0));
    ::SetBkColor(hdc, ::GetSysColor(COLOR_3DFACE));
    ::TextOut(hdc, 0, 0, wstr.c_str(), wstr.length());
    ::EndPaint(hWnd, &ps);
    return ::DefWindowProc(hWnd, nMsg, wparam, lparam);
}

LONG OnLButtonDown(HWND hWnd, UINT nMsg, WPARAM wparam, LPARAM lparam)
{
    wchar_t szPoint[56];
    wsprintf(szPoint, L"X=%d,Y=%d", LOWORD(lparam), HIWORD(lparam));
    wstr = szPoint;

    ::InvalidateRect(hWnd, NULL, 1);
    return 0;
}
