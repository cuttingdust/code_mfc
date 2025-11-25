#include "_AFXWIN.h"
#include "WINHAND_.h"
#include "_afximpl.h"

/**
 * @file _AFXWIN.cpp 
 * @brief MFC 窗口核心实现文件
 * 
 * 该文件实现了 MFC 窗口类的核心功能，包括：
 * 1. 窗口对象生命周期管理
 * 2. 句柄映射机制
 * 3. 消息路由系统
 * 4. 窗口类注册和管理
 * 5. 窗口创建和子类化机制
 */

//////////////////////////////////////////////////////////////////
/// MFC 窗口类名定义
//////////////////////////////////////////////////////////////////

/// 由于我们是自己管理窗口并注册，所以我们给 MFC 的类都自定义名字
const TCHAR _afxWnd[]            = AFX_WND;            ///< 普通窗口类名 - 用于基本的 CWnd 派生类
const TCHAR _afxWndFrameOrView[] = AFX_WNDFRAMEORVIEW; ///< 框架或视图窗口类名 - 用于 CFrameWnd、CView 等

//////////////////////////////////////////////////////////////////
/// CWnd 类实现 - 窗口对象生命周期管理
//////////////////////////////////////////////////////////////////

/**
 * @brief CWnd 构造函数
 * 
 * 初始化窗口对象的核心成员变量，建立空的窗口关联。
 * 此时 CWnd 对象尚未与实际的 Windows 窗口关联。
 */
CWnd::CWnd()
{
    m_hWnd     = NULL; ///< 窗口句柄初始化为空，表示该对象尚未关联实际窗口
    m_pfnSuper = NULL; ///< 父类窗口过程指针初始化为空，用于保存原始窗口过程
}

/**
 * @brief CWnd 析构函数
 * 
 * 负责窗口资源的自动清理，体现 RAII（Resource Acquisition Is Initialization）原则。
 * C++ 对象销毁时自动销毁对应的 Windows 窗口资源。
 */
CWnd::~CWnd()
{
    if (m_hWnd != NULL)
    {
        ::DestroyWindow(m_hWnd); ///< 调用 Windows API 销毁实际窗口
    }
}

//////////////////////////////////////////////////////////////////
/// 句柄映射机制 - MFC 核心基础设施
//////////////////////////////////////////////////////////////////

/**
 * @brief 获取窗口句柄映射表
 * 
 * MFC 使用这个映射表来维护 HWND 和 CWnd 对象之间的双向映射关系。
 * 这是 MFC 能够将 Windows 消息路由到对应 C++ 对象的关键基础设施。
 * 
 * @param bCreate 如果映射表不存在时是否自动创建
 * @return 返回句柄映射表指针，如果不存在且 bCreate=FALSE 则返回 NULL
 */
CHandleMap* afxMapHWND(BOOL bCreate = FALSE)
{
    AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState(); ///< 获取当前线程状态
    if (pState->m_pmapHWND == NULL && bCreate)
    {
        pState->m_pmapHWND = new CHandleMap(); ///< 创建新的句柄映射表
    }
    return pState->m_pmapHWND; ///< 返回映射表指针
}

/**
 * @brief 通过窗口句柄获取对应的 CWnd 对象
 * 
 * 这是 MFC 的核心机制：将 Windows API 的 HWND 与 MFC 的 CWnd 对象关联。
 * 如果永久映射不存在，会创建临时 CWnd 对象进行包装。
 * 
 * @param hWnd 要查找的窗口句柄
 * @return 返回对应的 CWnd 对象指针
 */
CWnd* CWnd::FromeHandle(HWND hWnd)
{
    CHandleMap* pMap = afxMapHWND(TRUE);  ///< 获取或创建句柄映射表
    return (CWnd*)pMap->FromHandle(hWnd); ///< 从映射表中查找或创建对象
}

/**
 * @brief 通过窗口句柄获取永久关联的 CWnd 对象
 * 
 * 与 FromHandle 不同，这里只查找已经永久附加的对象，不会创建临时对象。
 * 
 * @param hWnd 要查找的窗口句柄
 * @return 返回永久关联的 CWnd 对象指针，未找到返回 NULL
 */
CWnd* CWnd::FormHandlePermanet(HWND hWnd)
{
    CHandleMap* pMap = afxMapHWND(); ///< 获取句柄映射表（不自动创建）
    CWnd*       pWnd = NULL;
    if (pMap != NULL)
    {
        pWnd = (CWnd*)pMap->LookupPermanet(hWnd); ///< 在映射表中查找永久关联的对象
    }
    return pWnd;
}

/**
 * @brief 将窗口句柄附加到 CWnd 对象
 * 
 * 建立 HWND 和 CWnd 对象的永久关联，这是 MFC 窗口管理的关键操作。
 * 附加后，该窗口的所有消息都将路由到对应的 CWnd 对象。
 * 
 * @param hWNdNew 要附加的窗口句柄
 * @return 成功返回 TRUE，失败返回 FALSE
 */
BOOL CWnd::Attach(HWND hWNdNew)
{
    if (hWNdNew == NULL)
        return FALSE;
    CHandleMap* pMap = afxMapHWND(TRUE);       ///< 获取或创建句柄映射表
    pMap->SetPermanet(m_hWnd = hWNdNew, this); ///< 建立永久关联并设置成员变量
    return TRUE;
}

/**
 * @brief 从 CWnd 对象分离窗口句柄
 * 
 * 断开 HWND 和 CWnd 对象的关联，但不销毁实际的 Windows 窗口。
 * 常用于窗口重用或手动窗口管理场景。
 * 
 * @return 返回被分离的窗口句柄
 */
HWND CWnd::Detach()
{
    HWND hWnd = m_hWnd; ///< 保存当前窗口句柄
    if (hWnd != NULL)
    {
        CHandleMap* pMap = afxMapHWND();
        if (pMap != NULL)
            pMap->RemoveHandle(hWnd); ///< 从映射表中移除关联
        m_hWnd = NULL;                ///< 清空窗口句柄，表示该对象不再管理任何窗口
    }
    return hWnd; ///< 返回被分离的窗口句柄
}

//////////////////////////////////////////////////////////////////
/// 消息处理机制 - MFC 消息路由核心
//////////////////////////////////////////////////////////////////

/**
 * @brief MFC 的全局窗口过程函数
 * 
 * 所有 MFC 管理的窗口都使用这个窗口过程，这是 MFC 消息系统的入口。
 * 当 Windows 系统向窗口发送消息时，首先进入这个函数。
 * 
 * @param hWnd 接收消息的窗口句柄
 * @param nMsg 消息类型
 * @param wParam 消息参数 1
 * @param lParam 消息参数 2
 * @return 消息处理结果
 */
LRESULT __stdcall AfxWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    /// 通过窗口句柄查找对应的 CWnd 对象
    /// 这是 MFC 将 Windows 消息与 C++ 对象关联的关键步骤
    CWnd* pWnd = CWnd::FormHandlePermanet(hWnd);
    return AfxCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
}

/**
 * @brief MFC 的核心消息处理函数
 * 
 * 这个函数负责将 Windows 消息路由到对应的 CWnd 对象的消息处理系统。
 * 维护线程消息状态，支持消息嵌套处理。
 * 
 * @param pWnd 接收消息的 CWnd 对象指针
 * @param hWnd 窗口句柄
 * @param nMsg 消息类型
 * @param wParam 消息参数 1
 * @param lParam 消息参数 2
 * @return 消息处理结果
 */
LRESULT AfxCallWndProc(CWnd* pWnd, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    _AFX_THREAD_STATE* pThreadState = AfxGetThreadState(); ///< 获取当前线程状态

    /// 保存当前线程的最后一个消息状态
    /// 支持消息嵌套处理（比如在消息处理中又发送消息），保证递归消息处理的正确性
    MSG oldState = pThreadState->m_lastSendMsg;

    /// 更新线程状态中的最后一个消息信息
    /// 设置当前正在处理的消息详细信息，供后续消息处理函数使用
    pThreadState->m_lastSendMsg.hwnd    = hWnd;   ///< 目标窗口句柄
    pThreadState->m_lastSendMsg.message = nMsg;   ///< 消息类型
    pThreadState->m_lastSendMsg.wParam  = wParam; ///< 消息参数 1
    pThreadState->m_lastSendMsg.lParam  = lParam; ///< 消息参数 2

    /// 将消息交给 CWnd 对象的窗口过程处理
    /// 这是 MFC 消息路由的核心：让消息进入 WindowProc
    LRESULT lResult;
    lResult = pWnd->WindowProc(nMsg, wParam, lParam);

    /// 消息处理完毕以后，恢复原来的消息状态
    /// 保证消息嵌套处理的正确性，恢复递归前的消息上下文
    pThreadState->m_lastSendMsg = oldState;
    return lResult;
}

/**
 * @brief CWnd 窗口过程函数
 * 
 * 派生类应该重写这个函数来处理特定的窗口消息。
 * 这是消息到达具体窗口对象后的第一站。
 * 
 * @param message 消息类型
 * @param wParam 消息参数 1
 * @param lParam 消息参数 2
 * @return 消息处理结果
 */
LRESULT CWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    /// 只处理必要的消息，其他都传递给默认处理
    switch (message)
    {
        case WM_CLOSE:
            /// 直接销毁窗口
            ::DestroyWindow(m_hWnd);
            return 0;

        case WM_DESTROY:
            /// 如果是主窗口，退出程序
            ::PostQuitMessage(0);
            return 0;
    }

    /// 其他所有消息（包括移动、缩放、系统命令等）都交给默认处理
    return DefWindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////
/// 窗口类注册机制 - MFC 窗口类管理
//////////////////////////////////////////////////////////////////

/**
 * @brief 通用的窗口类注册函数
 * 
 * 封装 Windows API 的 RegisterClass，提供错误处理和重复注册检查。
 * 
 * @param lpWndClass 要注册的窗口类结构指针
 * @return 注册成功返回 TRUE，失败返回 FALSE
 */
BOOL AfxRegisterClass(WNDCLASS* lpWndClass)
{
    WNDCLASS wndclass;
    /// 检查窗口类是否已经注册，避免重复注册
    if (GetClassInfo(lpWndClass->hInstance, lpWndClass->lpszClassName, &wndclass))
    {
        return TRUE; ///< 已注册，直接返回成功
    }
    else
    {
        /// 调用 Windows API 注册窗口类
        if (!::RegisterClass(lpWndClass))
        {
            return FALSE; ///< 注册失败
        }
    }
    return TRUE; ///< 注册成功
}

/**
 * @brief 注册 MFC 窗口类
 * 
 * 根据提供的窗口样式参数动态生成并注册窗口类。
 * 支持自定义光标、背景刷、图标等窗口属性。
 * 
 * @param nClassStyle 窗口类样式
 * @param hCursor 光标句柄
 * @param hbrBackground 背景刷句柄
 * @param hIcon 图标句柄
 * @return 返回注册的窗口类名，失败返回 NULL
 */
LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
    /// 使用线程状态的临时缓冲区存储生成的窗口类名
    /// 避免内存分配，提高性能
    LPTSTR lpszName = AfxGetThreadState()->m_szTempClassName;

    HINSTANCE hInst = AfxGetModuleState()->m_hCurrentInstanceHandle;

    /// 根据参数生成唯一的窗口类名
    /// 类名基于参数哈希，避免重复注册相同的窗口类
    if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
        wsprintf(lpszName, L"Afx:%d:%d", (int)hIcon, nClassStyle); ///< 简单参数情况
    else
        wsprintf(lpszName, L"Afx:%d:%d:%d:%d", (int)hInst, nClassStyle, (int)hCursor, (int)hbrBackground,
                 (int)hIcon); ///< 复杂参数情况

    WNDCLASS wc = { 0 };
    /// 检查窗口类是否已经注册
    if (::GetClassInfo(hInst, lpszName, &wc))
    {
        return lpszName; ///< 如果已注册，直接返回类名
    }

    /// 填充窗口类结构
    wc.hInstance     = hInst;           ///< 模块实例句柄
    wc.style         = nClassStyle;     ///< 窗口类样式
    wc.hCursor       = hCursor;         ///< 光标资源
    wc.hbrBackground = hbrBackground;   ///< 背景刷
    wc.hIcon         = hIcon;           ///< 图标资源
    wc.lpszClassName = lpszName;        ///< 窗口类名
    wc.lpfnWndProc   = ::DefWindowProc; ///< 暂时使用默认窗口过程，后续 MFC 会子类化

    if (!AfxRegisterClass(&wc))
    {
        return NULL; ///< 注册失败返回 NULL
    }
    return lpszName; ///< 注册成功返回窗口类名
}

/**
 * @brief 延迟注册 MFC 内置窗口类
 * 
 * 根据标志位注册不同类型的 MFC 内置窗口类。
 * 这是 MFC 框架初始化时调用的内部函数。
 * 
 * @param fToRegister 注册标志位，指定要注册的窗口类类型
 * @return 注册成功返回 TRUE，失败返回 FALSE
 */
BOOL AfxEndDeferRegisterClass(LONG fToRegister)
{
    WNDCLASS wndclass;
    memset(&wndclass, 0, sizeof(wndclass)); ///< 清零初始化

    /// 设置通用的窗口类属性
    wndclass.lpfnWndProc = ::DefWindowProc;                               ///< 默认窗口过程
    wndclass.hInstance   = AfxGetModuleState()->m_hCurrentInstanceHandle; ///< 实例句柄
    wndclass.hCursor     = ::LoadCursor(NULL, IDC_ARROW);                 ///< 默认箭头光标

    BOOL bResult = FALSE;

    /// 注册普通窗口类
    if (fToRegister & AFX_WND_REG)
    {
        wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; ///< 支持重绘和双击
        wndclass.lpszClassName = _afxWnd;                              ///< 使用预定义的窗口类名
        bResult                = AfxRegisterClass(&wndclass);
    }
    /// 注册框架或视图窗口类
    else if (fToRegister & AFX_WNDFRAMEORVIEW_REG)
    {
        wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; ///< 支持重绘和双击
        wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);           ///< 默认窗口背景色
        wndclass.lpszClassName = _afxWndFrameOrView;                   ///< 使用预定义的框架视图类名
        bResult                = AfxRegisterClass(&wndclass);
    }
    return bResult;
}

//////////////////////////////////////////////////////////////////
/// 窗口创建钩子机制 - MFC 窗口初始化核心
//////////////////////////////////////////////////////////////////

/**
 * @brief 获取 MFC 的全局窗口过程函数指针
 * 
 * 这个函数用于窗口子类化，将窗口过程替换为 MFC 的窗口过程。
 * 
 * @return 返回 MFC 全局窗口过程的地址
 */
WNDPROC AfxGetAfxWndProc()
{
    return &AfxWndProc; ///< 返回 MFC 全局窗口过程的地址
}

/**
 * @brief CBT（Computer-Based Training）钩子过程
 * 
 * 这个钩子用于拦截窗口创建过程，实现 MFC 的窗口初始化机制。
 * 在窗口创建完成但尚未显示时，完成 MFC 特有的初始化工作。
 * 
 * @param code 钩子代码
 * @param wParam 附加参数
 * @param lParam 附加参数
 * @return 钩子处理结果
 */
LRESULT __stdcall _AfxCbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
{
    _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

    /// 只处理窗口创建消息（HCBT_CREATEWND）
    /// 其他消息直接传递给下一个钩子
    if (code != HCBT_CREATEWND)
    {
        return ::CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code, wParam, lParam);
    }

    /// 得到正在初始化的窗口句柄和 CWnd 的对象指针
    /// wParam 参数包含新创建的窗口句柄
    HWND hWnd = (HWND)wParam;

    /// 从线程状态中获取正在初始化的 CWnd 对象
    /// 这个指针在 AfxHookWindowCreate 中设置
    CWnd* pWndInit = pThreadState->m_pWndInit;

    /// 将 hWnd 关联到 pWndInit 指向的 CWnd 对象中
    /// 这是 MFC 窗口创建机制的核心：在窗口创建时建立 HWND 与 CWnd 的关联
    if (pWndInit != NULL)
    {
        /// 步骤 1：附加窗口句柄到 CWnd 对象
        /// 建立 HWND 和 CWnd 对象的永久关联，这是 MFC 消息路由的基础
        pWndInit->Attach(hWnd);

        /// 步骤 2：允许子类化窗口的事件先完成
        /// 这是虚函数，派生类可以重写以进行自定义初始化
        pWndInit->PreSubClassWindow();

        /// 步骤 3：改变窗口的默认回调函数（窗口子类化）
        /// 获取存储原始窗口过程指针的地址
        WNDPROC* pOldWndProc = pWndInit->GetSuperWndProcAddr();

        /// 获取 MFC 的全局窗口过程
        WNDPROC afxWndProc = AfxGetAfxWndProc();

        /// 将窗口过程替换为 MFC 的窗口过程
        /// 这是窗口子类化的关键步骤，从此该窗口的所有消息都由 MFC 处理
        WNDPROC oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (DWORD_PTR)afxWndProc);

        /// 如果原始窗口过程与 MFC 窗口过程不同，保存原始窗口过程
        /// 这样可以在需要时调用原始窗口过程，实现消息链的传递
        if (oldWndProc != afxWndProc)
            *pOldWndProc = oldWndProc;

        /// 清空线程状态中的初始化窗口指针
        /// 表示这个窗口已经初始化完成，可以处理下一个窗口创建
        pThreadState->m_pWndInit = NULL;
    }

    /// 继续传递消息给下一个钩子
    /// 保证其他安装的 CBT 钩子也能正常处理窗口创建消息
    return ::CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code, wParam, lParam);
}

/**
 * @brief 安装窗口创建钩子
 * 
 * 这个函数在创建 MFC 窗口前调用，用于拦截窗口创建过程。
 * 是 MFC 窗口创建流程的准备工作。
 * 
 * @param pWnd 要初始化的 CWnd 对象指针
 */
void AfxHookWindowCreate(CWnd* pWnd)
{
    _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

    /// 如果已经在初始化这个窗口，直接返回
    /// 避免重复设置，保证线程安全
    if (pThreadState->m_pWndInit == pWnd)
        return;

    /// 如果钩子还没有安装，安装 CBT 钩子
    /// 每个线程只需要安装一次 CBT 钩子
    if (pThreadState->m_hHookOldCbtFilter == NULL)
    {
        /// 安装线程专用的 CBT 钩子，只监视当前线程的窗口创建
        /// WH_CBT 钩子可以拦截窗口创建、激活、销毁等系统事件
        pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT, _AfxCbtFilterHook, NULL, ::GetCurrentThreadId());
    }

    /// 设置当前正在初始化的窗口对象
    /// 这个指针将在 CBT 钩子过程中使用，用于关联 HWND 和 CWnd 对象
    pThreadState->m_pWndInit = pWnd;
}

/**
 * @brief 卸载窗口创建钩子
 * 
 * 窗口创建完成后调用这个函数清理钩子资源。
 * 同时检查窗口创建是否成功完成。
 * 
 * @return 窗口创建成功返回 TRUE，可能失败返回 FALSE
 */
BOOL AfxUnhookWindowCreate()
{
    _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

    /// 如果钩子存在，卸载钩子
    /// 每个线程的 CBT 钩子在窗口创建完成后立即卸载
    if (pThreadState->m_hHookOldCbtFilter != NULL)
    {
        ::UnhookWindowsHookEx(pThreadState->m_hHookOldCbtFilter);
        pThreadState->m_hHookOldCbtFilter = NULL;
    }

    /// 检查是否还有未完成的窗口初始化
    /// 如果 m_pWndInit 不为 NULL，说明窗口创建可能失败了
    /// 因为成功的窗口创建会在 CBT 钩子中清空这个指针
    if (pThreadState->m_pWndInit != NULL)
    {
        pThreadState->m_pWndInit = NULL;
        return FALSE; ///< 返回 FALSE 表示窗口创建可能失败
    }

    return TRUE; ///< 返回 TRUE 表示窗口创建成功完成
}

//////////////////////////////////////////////////////////////////
/// 窗口过程支持函数
//////////////////////////////////////////////////////////////////

/**
 * @brief 获取父类窗口过程指针的地址
 * 
 * 这个函数用于存储原始窗口过程，支持消息链的传递。
 * 在窗口子类化时，需要保存原始窗口过程以便后续调用。
 * 
 * @return 返回成员变量 m_pfnSuper 的地址
 */
WNDPROC* CWnd::GetSuperWndProcAddr()
{
    return &m_pfnSuper; ///< 返回成员变量 m_pfnSuper 的地址
}

/**
 * @brief 调用默认的窗口过程处理当前消息
 * 
 * 使用最近保存的消息参数调用默认窗口过程。
 * 这是实现消息传递链的关键函数。
 * 
 * @return 默认窗口过程的处理结果
 */
LRESULT CWnd::Default()
{
    /// 以最近的一个消息作为参数调用默认窗口函数
    /// 使用线程状态中保存的最后一个消息参数
    _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
    return DefWindowProc(pThreadState->m_lastSendMsg.message, pThreadState->m_lastSendMsg.wParam,
                         pThreadState->m_lastSendMsg.lParam);
}

/**
 * @brief 默认窗口过程处理函数
 * 
 * 这是 MFC 消息路由的终点，负责将消息传递给原始窗口过程或系统默认处理。
 * 实现了消息处理链的传递机制。
 * 
 * @param message 消息类型
 * @param wParam 消息参数 1
 * @param lParam 消息参数 2
 * @return 窗口过程处理结果
 */
LRESULT CWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    /// 如果保存了父类的窗口过程，优先调用父类的窗口过程
    /// 这是窗口子类化中消息传递的标准做法
    if (m_pfnSuper != NULL)
    {
        return ::CallWindowProc(m_pfnSuper, m_hWnd, message, wParam, lParam);
    }

    /// 否则获取超类窗口过程
    /// 超类窗口过程是通过 GetSuperWndProcAddr 获取的原始窗口过程
    WNDPROC pfnWndProc;
    if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
    {
        /// 如果没有超类窗口过程，调用系统默认窗口过程
        /// 这是消息处理链的最终终点
        return ::DefWindowProc(m_hWnd, message, wParam, lParam);
    }
    else
    {
        /// 调用超类窗口过程
        /// 继续传递消息给原始窗口过程
        return ::CallWindowProc(pfnWndProc, m_hWnd, message, wParam, lParam);
    }
}

//////////////////////////////////////////////////////////////////
/// 窗口创建和管理 - MFC 窗口对象创建接口
//////////////////////////////////////////////////////////////////

/**
 * @brief 创建窗口的简化版本
 * 
 * 这个版本提供了常用的默认参数，简化了窗口创建过程。
 * 主要用于创建子窗口和控件。
 * 
 * @param lpszClassName 窗口类名
 * @param lpszWindowName 窗口标题
 * @param dwStyle 窗口样式
 * @param rect 窗口位置和大小
 * @param pParentWnd 父窗口指针
 * @param nID 控件 ID
 * @param lpParam 创建参数
 * @return 创建成功返回 TRUE，失败返回 FALSE
 */
BOOL CWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                  UINT nID, LPVOID lpParam)
{
    /// 调用扩展版本的 Create，提供默认参数
    /// WS_CHILD 样式表示创建子窗口，这是控件和子窗口的默认样式
    return CreateEx(0, lpszClassName, lpszWindowName, dwStyle | WS_CHILD, rect.left, rect.top, rect.right - rect.left,
                    rect.bottom - rect.top, pParentWnd->GetSafeHwnd(), (HMENU)nID, (LPVOID)lpParam);
}

/**
 * @brief 创建窗口的扩展版本
 * 
 * 这个函数提供了完整的窗口创建参数，支持所有 Windows 窗口样式。
 * 包含完整的 MFC 窗口初始化流程。
 * 
 * @param dwExStyle 扩展窗口样式
 * @param lpszClassName 窗口类名
 * @param lpszWindowName 窗口标题
 * @param dwStyle 窗口样式
 * @param x 窗口 X 坐标
 * @param y 窗口 Y 坐标
 * @param nWidth 窗口宽度
 * @param nHeight 窗口高度
 * @param hWndParent 父窗口句柄
 * @param nIDorHMenu 菜单句柄或控件 ID
 * @param lpParam 创建参数
 * @return 创建成功返回 TRUE，失败返回 FALSE
 */
BOOL CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y,
                    int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam)
{
    CREATESTRUCT cs;
    /// 填充 CREATESTRUCT 结构，这是 CreateWindowEx 的参数
    /// CREATESTRUCT 包含了创建窗口所需的所有信息
    cs.dwExStyle      = dwExStyle;                                     ///< 扩展窗口样式
    cs.lpszClass      = lpszClassName;                                 ///< 窗口类名
    cs.lpszName       = lpszWindowName;                                ///< 窗口标题
    cs.style          = dwStyle;                                       ///< 窗口样式
    cs.x              = x;                                             ///< 窗口 X 坐标
    cs.y              = y;                                             ///< 窗口 Y 坐标
    cs.cx             = nWidth;                                        ///< 窗口宽度
    cs.cy             = nHeight;                                       ///< 窗口高度
    cs.hwndParent     = hWndParent;                                    ///< 父窗口句柄
    cs.hMenu          = nIDorHMenu;                                    ///< 菜单句柄或控件 ID
    cs.hInstance      = AfxGetModuleState()->m_hCurrentInstanceHandle; ///< 应用程序实例句柄
    cs.lpCreateParams = lpParam;                                       ///< 创建参数

    /// 调用虚函数，允许派生类修改创建参数
    /// 这是模板方法模式的应用，派生类可以自定义窗口创建行为
    if (!PreCreateWindow(cs))
    {
        /// 如果预创建失败，调用清理函数
        /// PostNcDestroy 用于清理资源，即使窗口创建失败也可能需要清理
        PostNcDestroy();
        return FALSE;
    }

    /// 安装窗口创建钩子，准备拦截窗口创建过程
    /// 这是 MFC 窗口创建机制的关键步骤，确保窗口创建时能正确初始化 MFC 设施
    AfxHookWindowCreate(this);

    /// 调用 Windows API 创建窗口
    /// 在窗口创建过程中，CBT 钩子会被触发，完成 MFC 的初始化
    HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
                                 cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

    /// 卸载窗口创建钩子
    /// 如果返回 FALSE，说明窗口创建可能失败了
    if (!AfxUnhookWindowCreate())
    {
        /// 窗口创建失败，调用清理函数
        /// 可能是窗口创建过程中出现了问题
        PostNcDestroy();
    }

    /// 检查窗口是否创建成功
    if (hWnd == NULL)
        return FALSE; ///< 窗口创建失败
    return TRUE;      ///< 窗口创建成功
}

//////////////////////////////////////////////////////////////////
/// 可重写虚函数 - 提供给派生类的扩展点
//////////////////////////////////////////////////////////////////

/**
 * @brief 窗口创建前的预处理函数
 * 
 * 派生类可以重写这个函数来修改窗口创建参数。
 * 比如改变窗口样式、设置窗口类名等。
 * 
 * @param cs 窗口创建参数结构引用
 * @return 允许创建返回 TRUE，禁止创建返回 FALSE
 */
BOOL CWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    /// 如果窗口类名为空，设置默认的 MFC 窗口类名
    /// 提供合理的默认值，简化派生类的实现
    if (cs.lpszClass == NULL)
    {
        cs.lpszClass = _afxWnd;
    }
    return TRUE; ///< 默认实现总是返回 TRUE
}

/**
 * @brief 窗口非客户区销毁后的清理函数
 * 
 * 在窗口的非客户区被销毁后调用，用于资源清理。
 * 派生类可以重写这个函数进行自定义资源清理。
 */
void CWnd::PostNcDestroy()
{
    /// 默认实现为空，派生类可以重写
    /// 注意：这个函数在窗口销毁过程中调用，不应进行复杂的操作
}

/**
 * @brief 窗口子类化前的预处理函数
 * 
 * 在窗口被子类化之前调用，允许进行自定义初始化。
 * 这是窗口完全初始化前的最后一个扩展点。
 */
void CWnd::PreSubClassWindow()
{
    /// 默认实现为空，派生类可以重写
    /// 常用于设置窗口的特殊属性或初始化成员变量
}

//////////////////////////////////////////////////////////////////
/// 消息处理支持函数
//////////////////////////////////////////////////////////////////

/**
 * @brief 获取当前正在处理的消息
 * 
 * 这个函数用于在消息处理函数中获取当前消息的详细信息。
 * 比如在 ON_WM_PAINT 处理函数中获取 PaintStruct。
 * 
 * @return 返回当前消息的 MSG 结构指针
 */
const MSG* CWnd::GetCurrentMessage()
{
    _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

    /// 补充消息的时间戳和鼠标位置信息
    /// 这些信息在原始消息结构中没有，需要额外获取
    pThreadState->m_lastSendMsg.time = ::GetMessageTime(); ///< 消息时间戳
    DWORD dw                         = ::GetMessagePos();  ///< 消息位置（屏幕坐标）
    pThreadState->m_lastSendMsg.pt.x = LOWORD(dw);         ///< 鼠标 X 坐标
    pThreadState->m_lastSendMsg.pt.y = HIWORD(dw);         ///< 鼠标 Y 坐标

    /// 返回当前消息的指针
    /// 这个指针指向线程状态中保存的消息副本
    return &pThreadState->m_lastSendMsg;
}

//////////////////////////////////////////////////////////////////
/// 运行时类支持
//////////////////////////////////////////////////////////////////

/**
 * @brief MFC 运行时类支持宏
 * 
 * 这个宏为 CWnd 类添加运行时类型信息支持，包括：
 * 1. 动态创建支持（CreateObject）
 * 2. 运行时类型识别（IsKindOf）
 * 3. 序列化支持
 * 这是 MFC 对象模型的基础设施
 */
IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)
