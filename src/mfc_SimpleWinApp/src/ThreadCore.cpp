#include "_AFXWIN.h"

#include <process.h>

/// 我们希望提供一个CWinThread的入口点函数AfxThreadEntry
/// 如何提供父线程与子线程的交互
/// 通过将CWinthread对象组合进入CreateThread，
/// 来完成CWinthread的入口点

struct _AFX_THREAD_STARTUP
{
    CWinThread* pThread = NULL; ///< 新线程的CWinthread对象
    HANDLE      hEvent;         ///< 在线程初始化以后触发
    HANDLE      hEvent2;        ///< 创建线程从CreateThread返回时触发
    BOOL        bError;         ///< 看看线程是否能够初始化成功
};

UINT __stdcall _AfxThreadEntry(void* pParam)
{
    _AFX_THREAD_STARTUP* pStartup = (_AFX_THREAD_STARTUP*)pParam;
    CWinThread*          pThread  = pStartup->pThread;
    try
    {
        AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
        pState->m_pCurrentWinThread     = pThread;
    }
    catch (...)
    {
        pStartup->bError = TRUE;
        ::SetEvent(pStartup->hEvent);
        AfxEndThread((UINT)-1, FALSE);
    }

    HANDLE hEvent2 = pStartup->hEvent2;

    /// 保存了pStartup所指向的空间，我们就可以允许父线程返回
    ::SetEvent(pStartup->hEvent);

    /// 等待父线程中的CWinThread：：CreateThread代码执行完毕
    ::WaitForSingleObject(hEvent2, INFINITE);
    ::CloseHandle(hEvent2);

    /// 调用用户的线程函数
    DWORD nRes = 0;
    if (pThread->m_pfnThreadProc)
    {
        nRes = (*pThread->m_pfnThreadProc)(pThread->m_pThreadParams);
    }

    /// 结束线程
    AfxEndThread(nRes);
    return 0;
}

CWinThread::CWinThread() = default;

BOOL CWinThread::CreateThread(DWORD dwCreateFlags, UINT nStackSize, LPSECURITY_ATTRIBUTES lpSec)
{
    _AFX_THREAD_STARTUP startup;
    memset(&startup, 0, sizeof(startup));

    startup.pThread = this;
    startup.hEvent  = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);

    m_hThread = (HANDLE)_beginthreadex(lpSec, nStackSize, &_AfxThreadEntry, &startup, dwCreateFlags | CREATE_SUSPENDED,
                                       (UINT*)&m_nThreadID);

    if (m_hThread == NULL)
    {
        return FALSE;
    }

    /// 恢复线程执行
    ResumeThread();
    ::WaitForSingleObject(startup.hEvent, INFINITE);
    ::CloseHandle(startup.hEvent);

    if (dwCreateFlags & CREATE_SUSPENDED)
    {
        ::SuspendThread(m_hThread);
    }

    if (startup.bError)
    {
        ::WaitForSingleObject(m_hThread, INFINITE);
        ::CloseHandle(m_hThread);
        m_hThread = NULL;
        ::CloseHandle(startup.hEvent2);
        return FALSE;
    }

    ::SetEvent(startup.hEvent2);
    return TRUE;
}

CWinThread::~CWinThread()
{
    /// 关闭线程句柄，释放系统资源
    if (m_hThread != NULL)
    {
        CloseHandle(m_hThread);
    }

    /// 获取当前线程的模块线程状态
    AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();

    /// 关键清理：防止悬空指针
    ///
    /// 场景分析：
    /// 1. 正常情况：线程已结束，pState->m_pCurrentWinThread 应该为 NULL
    /// 2. 异常情况：线程对象被提前删除，但线程还在运行
    /// 3. 自引用情况：当前线程正在销毁自己的 CWinThread 对象
    ///
    /// 这个检查的作用：
    /// - 如果当前销毁的对象正是线程状态中记录的对象
    /// - 需要将其置为 NULL，避免后续访问已销毁的对象
    /// - 防止出现悬空指针导致的崩溃或未定义行为
    if (pState->m_pCurrentWinThread == this)
    {
        /// 为什么要这样做？
        ///
        /// 考虑以下场景：
        /// 线程A正在执行，其线程状态中记录: m_pCurrentWinThread = 对象X
        /// 如果对象X被意外销毁，但线程A还在运行并尝试访问线程状态
        /// 就会访问到已销毁的内存，导致崩溃
        ///
        /// 这个清理确保：
        /// 1. 对象销毁时，如果它还在被线程状态引用，就解除引用
        /// 2. 避免其他代码通过 AfxGetThread() 获取到已销毁的对象
        /// 3. 特别是对于自动删除的线程对象(m_bAutoDelete = TRUE)
        pState->m_pCurrentWinThread = NULL;
    }
}

void CWinThread::Delete()
{
    if (m_bAutoDelete)
    {
        delete this;
    }
}

void CWinThread::CommonConstruct()
{
    m_hThread     = NULL;
    m_nThreadID   = 0;
    m_bAutoDelete = TRUE;
}

CWinThread::CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam)
{
    m_pfnThreadProc = pfnThreadProc;
    m_pThreadParams = pParam;
    CommonConstruct();
}

/// 不从完善没有实现的虚函数方法
/// CWinThread::Run() - MFC 线程主消息循环
/// 这是 MFC 框架的核心消息泵实现，负责处理消息和空闲时间处理
int CWinThread::Run()
{
    /// bIdle: 标识当前是否处于空闲状态
    /// lIdleCount: 空闲计数器，用于控制不同优先级的空闲处理
    BOOL bIdle      = TRUE; /// 初始化为 TRUE，表示开始时空闲
    LONG lIdleCount = 0;    /// 空闲计数从 0 开始

    /// 无限循环，直到收到 WM_QUIT 消息
    for (;;)
    {
        /// 阶段1: 空闲处理循环
        /// 当没有消息且处于空闲状态时，执行 OnIdle 处理
        while (bIdle && !::PeekMessage(&m_msgCur, NULL, 0, 0, PM_NOREMOVE))
        {
            /// 调用 OnIdle 进行后台处理
            /// OnIdle 返回值: TRUE 表示还有工作要做，FALSE 表示空闲处理完成
            /// lIdleCount 参数:
            ///   - 0: 高优先级空闲任务
            ///   - 1: 低优先级空闲任务
            ///   - >=2: 更低优先级的空闲任务
            if (!OnIdle(lIdleCount++))
            {
                /// OnIdle 返回 FALSE，表示没有更多空闲工作要做
                bIdle = FALSE; /// 退出空闲状态
            }

            /// 注意: PeekMessage 使用 PM_NOREMOVE 只是检查而不取出消息
            /// 这样可以在不破坏消息队列的情况下检测是否有新消息到达
        }

        /// 阶段2: 消息处理循环
        do
        {
            /// 泵送一条消息（从队列取出、翻译、分发）
            if (!PumpMessage())
            {
                /// PumpMessage 返回 FALSE 表示收到 WM_QUIT 消息
                /// 调用 ExitInstance 清理并退出线程
                return ExitInstance();
            }

            /// 检查刚处理的消息是否是"空闲消息"
            /// 空闲消息通常是不重要的用户输入，如鼠标移动、定时器消息等
            /// 这些消息处理后可以立即返回空闲状态
            if (isIdleMessage(&m_msgCur))
            {
                bIdle      = TRUE; /// 重新进入空闲状态
                lIdleCount = 0;    /// 重置空闲计数器
            }
        }
        /// 使用 PeekMessage 检查是否还有更多消息需要处理
        /// PM_NOREMOVE 确保只是检查而不实际取出消息
        while (::PeekMessage(&m_msgCur, NULL, 0, 0, PM_NOREMOVE));

        /// 当消息队列为空时，退出内层循环，回到外层循环
        /// 外层循环会重新进入空闲处理阶段
    }

    // 理论上不会执行到这里
    // ASSERT(FALSE);
    return 0;
}

/// CWinThread::PreTranslateMessage - 消息预处理函数
/// 在消息被翻译和分发之前进行预处理
/// 返回值: TRUE - 消息已处理，不再继续传递; FALSE - 继续正常处理
BOOL CWinThread::PreTranslateMessage(MSG* pMsg)
{
    /// 首先检查是否是线程消息（无窗口消息）
    if (pMsg->hwnd == NULL)
    {
        /// 处理线程消息（如 WM_TIMER, WM_QUIT 等）
        // return PreTranslateThreadMessage(pMsg);
    }

    /// 遍历所有模块，让模块有机会预处理消息
    /// 这允许插件或扩展模块拦截和处理消息
    // _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
    // for (CWinApp* pApp = AfxGetApp(); pApp != NULL; pApp = pApp->m_pNextWinThread)
    // {
    // if (pApp->PreTranslateMessage(pMsg))
    // {
    // return TRUE; /// 应用程序已处理此消息
    // }
    // }

    /// 检查是否是对话框或菜单的加速键消息
    // CFrameWnd* pFrameWnd = GetMainWnd();
    // if (pFrameWnd != NULL && pFrameWnd->PreTranslateMessage(pMsg))
    // {
    // return TRUE; // 主框架窗口已处理此消息
    // }

    /// 遍历所有窗口，让它们有机会预处理消息
    /// 从最顶层的窗口开始（模态对话框等）
    HWND hWnd = pMsg->hwnd;
    // CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

    /// 向上遍历窗口父子链
    // while (pWnd != NULL)
    // {
    // if (pWnd->PreTranslateMessage(pMsg))
    // {
    // return TRUE; // 窗口已处理此消息
    // }

    /// 移动到父窗口
    // hWnd = ::GetParent(hWnd);
    // pWnd = CWnd::FromHandlePermanent(hWnd);
    // }

    /// 最后检查加速键表
    // if (pFrameWnd != NULL)
    // {
    // HACCEL hAccel = pFrameWnd->GetDefaultAccelerator();
    // if (hAccel != NULL && ::TranslateAccelerator(pFrameWnd->m_hWnd, hAccel, pMsg))
    // {
    // return TRUE; /// 加速键已处理
    // }
    // }

    /// 消息未被任何预处理程序处理，继续正常流程
    // return FALSE;
    // }

    return TRUE;
}


BOOL CWinThread::PumpMessage()
{
    if (!::GetMessage(&m_msgCur, NULL, 0, 0))
    {
        return FALSE; /// 收到 WM_QUIT，返回 FALSE
    }

    /// 预处理消息（加速键等）
    if (!PreTranslateMessage(&m_msgCur))
    {
        ::TranslateMessage(&m_msgCur); /// 翻译键盘消息
        ::DispatchMessage(&m_msgCur);  /// 分派到窗口过程
    }

    return TRUE; /// 成功处理消息
}

/// OnIdle 的典型实现示例
BOOL CWinThread::OnIdle(LONG lCount)
{
    if (lCount == 0)
    {
        /// 高优先级任务：更新UI状态等
        // UpdateCommandUI();
        return TRUE; // 还有工作，请求再次调用
    }
    else if (lCount == 1)
    {
        /// 低优先级任务：清理临时资源等
        // CleanupTempObjects();
        return TRUE; // 还有工作
    }
    else
    {
        /// 没有更多空闲工作
        return FALSE;
    }
}

BOOL CWinThread::isIdleMessage(MSG* pMsg)
{
    switch (pMsg->message)
    {
        case WM_MOUSEMOVE:
        case WM_NCMOUSEMOVE:
        case WM_PAINT:
        case WM_TIMER:
        case 0x0118:     /// WM_SYSTIMER (caret blink)
            return TRUE; /// 这些是不重要的"空闲消息"

        default:
            return FALSE; /// 重要消息，处理完后不立即进入空闲
    }
}

BOOL CWinThread::InitInstance()
{
    return FALSE;
}

int CWinThread::ExitInstance()
{
    int nRet = m_msgCur.lParam;
    return nRet;
}

/// 这种方式管理线程 最大有点就是利用 TLS技术 实现了 不需要 在意线程竞争
CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nPriority, UINT nStackSize,
                           DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
    /// 为新线程创建一个CWinThread的对象
    CWinThread* pThread = new CWinThread(pfnThreadProc, pParam);

    /// 创建线程,同时挂起线程
    if (!pThread->CreateThread(dwCreateFlags | CREATE_SUSPENDED, nStackSize, lpSecurityAttrs))
    {
        pThread->Delete();
        return NULL;
    }

    /// 设置优先级
    pThread->SetThreadPriority(nPriority);
    if (!(dwCreateFlags & CREATE_SUSPENDED))
    {
        pThread->ResumeThread();
    }

    return pThread;
}

CWinThread* AfxGetThread()
{
    AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
    return pState->m_pCurrentWinThread;
}

void AfxEndThread(UINT nExitCode, BOOL bDelete)
{
    /// 释放当前CWinthread的对象空间
    AFX_MODULE_THREAD_STATE* pState  = AfxGetModuleThreadState();
    CWinThread*              pThread = pState->m_pCurrentWinThread;
    if (pThread != NULL)
    {
        if (bDelete)
        {
            pThread->Delete();
        }
        pState->m_pCurrentWinThread = NULL;
    }

    /// 释放线程局部存储
    if (_afxThreadData != NULL)
    {
        _afxThreadData->DeleteValues(NULL, FALSE);
    }
    _endthreadex(nExitCode);
}

IMPLEMENT_DYNCREATE(CWinThread, CObject)

//////////////////////////////////////////////////////////////////
