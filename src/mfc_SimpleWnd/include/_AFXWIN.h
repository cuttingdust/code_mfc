#ifndef __AFXWIN_H
#define __AFXWIN_H

#include "_afx.h"
#include "_AFXSTAT.h"
#include "_afxColl_.h"

/// 首先实现CCmdTarget为所有有需要控制消息循环的类作为基类
class CCmdTarget : public CObject
{
    DELCARE_DYNCREATE(CCmdTarget)
public:
    CCmdTarget();
};

class CWnd : public CCmdTarget
{
    DELCARE_DYNCREATE(CWnd)
public:
    CWnd();
    virtual ~CWnd();

    HWND m_hWnd;
    operator HWND() const
    {
        return m_hWnd;
    }
    HWND GetSafeHwnd()
    {
        return this == NULL ? NULL : m_hWnd;
    }

    /// 窗口句柄映射
    static CWnd* FromeHandle(HWND hWnd);
    static CWnd* FormHandlePermanet(HWND hWnd);
    BOOL         Attach(HWND hWndNew);
    HWND         Detach();

    virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD, const RECT&, CWnd*, UINT, LPVOID lpParam = NULL);
    /// create
    BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y,
                  int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL);

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void PostNcDestroy();
    virtual void PreSubClassWindow(); /// 窗口子类化

    static const MSG* GetCurrentMessage();

protected:
    /// 默认的消息函数地址
    WNDPROC          m_pfnSuper;
    virtual WNDPROC* GetSuperWndProcAddr();

    /// 对消息进行默认处理的相关函数
    LRESULT         Default();
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

    virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lparam);
    /// 分发消息，由于消息是一个Windows SDK的全局函数，为了实现C/C++的混合编程
    /// 所以使用友元函数
    friend LRESULT AfxCallWndProc(CWnd*, HWND, UINT, WPARAM, LPARAM);
    /// 钩子
    friend LRESULT __stdcall _AfxCbtFilterHook(int, WPARAM, LPARAM);
};

typedef void (CWnd::*AFX_PMSGW)(void);

/// 注册用户程序自己的窗口类辅助函数

LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor = 0, HBRUSH hbrBackground = 0, HICON hIcon = 0);

BOOL AfxRegisterClass(WNDCLASS* lpWndClass);
BOOL AfxEndDeferRegisterClass(LONG fToRegister);

WNDPROC AfxGetAfxWndProc();

void AfxHookWindowCreate(CWnd* pWnd);
BOOL AfxUnhookWindowCreate();


//////////////////////////////////////////////////////////////////


/// CWinThread的定义：对MFC框架推进应用程序的一种封装姿态
/// 1）创建线程对象
/// 2）保存和设置线程的属性
/// 3）对线程进行操作

extern CThreadSlotData* _afxThreadData;
typedef UINT(__cdecl* AFX_THREADPROC)(LPVOID);
class CWinThread : public CCmdTarget
{
    DELCARE_DYNCREATE(CWinThread)
public:
    /// 线程对象的初始化
    CWinThread();

    BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0, LPSECURITY_ATTRIBUTES lpSec = NULL);

    CWnd* m_pMainWnd;
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
