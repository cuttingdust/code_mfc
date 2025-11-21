#ifndef __AFXWIN_H
#define __AFXWIN_H

#include "_afx.h"
#include "_AFXSTAT.h"

/// CWinThread的定义：对MFC框架推进应用程序的一种封装姿态
/// 1）创建线程对象
/// 2）保存和设置线程的属性
/// 3）对线程进行操作

extern CThreadSlotData* _afxThreadData;
typedef UINT(__cdecl* AFX_THREADPROC)(LPVOID);
class CWinThread : public CObject
{
    DELCARE_DYNCREATE(CWinThread)
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
    ~CWinThread() override;
    virtual void Delete();
    void         CommonConstruct();
    CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam);
    AFX_THREADPROC m_pfnThreadProc; /// 线程函数的地址
    LPVOID         m_pThreadParams; /// 用户传递给线程的参数


    /// 执行线程的初始化
    virtual int  Run();
    virtual BOOL PreTranslateMessage(MSG* msg);
    virtual BOOL PumpMessage();
    virtual BOOL OnIdle(LONG lCount);
    virtual BOOL isIdleMessage(MSG* pMsg);

    /// 线程实例的初始化和终止
    virtual BOOL InitInstance();

    virtual int ExitInstance();

    MSG m_msgCur;
};

CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL,
                           UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);


CWinThread* AfxGetThread();


void AfxEndThread(UINT nExitCode, BOOL bDelete = TRUE);

//////////////////////////////////////////////////////////////////


///CWinApp类
class CWinApp : public CWinThread
{
    DELCARE_DYNCREATE(CWinApp)
public:
    CWinApp();
    ~CWinApp() override;

    /// WinMain的4个参数
    HINSTANCE m_hInstance;
    HINSTANCE m_hPreveInstance;
    LPTSTR    m_lpCmdLine;
    int       m_nCmdShow;

public:
    HCURSOR LoadCursor(UINT nIDResource) const;
    HICON   LoadIcon(UINT nIDResource) const;

public:
    virtual BOOL InitApplication();
    BOOL         InitInstance() override;
    BOOL         ExitInstance() override;
    int          Run() override;
};

__inline HCURSOR CWinApp::LoadCursor(UINT nIDResource) const
{
    return ::LoadCursor(AfxGetModuleState()->m_hCurrentResourceHandle, (LPCTSTR)nIDResource);
}

__inline HICON CWinApp::LoadIcon(UINT nIDResource) const
{
    return ::LoadIcon(AfxGetModuleState()->m_hCurrentResourceHandle, (LPCTSTR)nIDResource);
}

CWinApp*          AfxGetApp();
__inline CWinApp* AfxGetApp()
{
    return AfxGetModuleState()->m_pCurrentWinApp;
}

BOOL AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);

#endif // !__AFXWIN_H
