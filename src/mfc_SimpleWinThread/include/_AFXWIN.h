#ifndef __AFXWIN_H
#define __AFXWIN_H

#include "_AFXSTAT.h"


/// CWinThread的定义：对MFC框架推进应用程序的一种封装姿态
/// 1）创建线程对象
/// 2）保存和设置线程的属性
/// 3）对线程进行操作

extern CThreadSlotData* _afxThreadData;
typedef UINT(__cdecl* AFX_THREADPROC)(LPVOID);
class CWinThread
{
public:
    /// 线程对象的初始化
    CWinThread();

    BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0, LPSECURITY_ATTRIBUTES lpSec = NULL);

    /// 保存线程的对象的特有属性
    BOOL   m_bAutoDelete;
    HANDLE m_hThread;
    operator HANDLE() const
    {
        return this == NULL ? NULL : m_hThread;
    }
    DWORD m_nThreadID;
    int   GetThreadPriority()
    {
        return ::GetThreadPriority(m_hThread);
    }
    BOOL SetThreadPriority(int nPriority)
    {
        return ::SetThreadPriority(m_hThread, nPriority);
    }
    /// 对线程的操作
    DWORD SuspendThread()
    {
        return ::SuspendThread(m_hThread);
    }
    DWORD ResumeThread()
    {
        return ::ResumeThread(m_hThread);
    }

public:
    virtual ~CWinThread();
    virtual void Delete();
    void         CommonConstruct();
    CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam);
    AFX_THREADPROC m_pfnThreadProc; /// 线程函数的地址
    LPVOID         m_pThreadParams; /// 用户传递给线程的参数
};

CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL,
                           UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);


CWinThread* AfxGetThread();


void AfxEndThread(UINT nExitCode, BOOL bDelete = TRUE);

#endif // !__AFXWIN_H
