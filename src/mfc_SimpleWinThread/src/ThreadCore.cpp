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
