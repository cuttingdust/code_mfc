#ifndef __AFXSTAT_H
#define __AFXSTAT_H

#include "_AFXTLS.h"


class CHandleMap;
class CWinThread;
class AFX_MODULE_THREAD_STATE : public CNoTrackObject
{
    /// 模块线程状态
public:
    /// 指向当前的CWinThread对象
    CWinThread* m_pCurrentWinThread = NULL;

    /// 窗口句柄映射
    CHandleMap* m_pmapHWND;
};

EXTERN_THREAD_LOCAL(AFX_MODULE_THREAD_STATE, _afxModuleThreadState)
AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState();

//////////////////////////////////////////////////////////////////

/// 模块状态的获取
struct CRuntimeClass;
class CWinApp;
class AFX_MODULE_STATE : public CNoTrackObject
{
public:
    CTypedSimpleList<CRuntimeClass*> m_listClass; /// 对于应用程序中的类信息使用链表管理

    CWinApp*  m_pCurrentWinApp;         /// 当前的CWinApp对象的指针
    HINSTANCE m_hCurrentInstanceHandle; /// 当前模块的实例句柄
    HINSTANCE m_hCurrentResourceHandle; /// 包含资源的实例句柄

    THREAD_LOCAL(AFX_MODULE_THREAD_STATE, m_thread)
};

AFX_MODULE_STATE* AfxGetAppModuleState();
AFX_MODULE_STATE* AfxGetModuleState();

//////////////////////////////////////////////////////////////////


/// 线程状态
class CWnd;
class _AFX_THREAD_STATE : public CNoTrackObject
{
public:
    AFX_MODULE_STATE* m_pModuleState;

    /// 注册窗口类的缓冲区
    TCHAR m_szTempClassName[96];

    /// 创建窗口的辅助
    CWnd* m_pWndInit;          /// 初始化Cwnd对象的指针
    HHOOK m_hHookOldCbtFilter; /// 钩子句柄

    MSG m_lastSendMsg;
};

EXTERN_THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState)
_AFX_THREAD_STATE* AfxGetThreadState();

#endif // !__AFXSTAT_H
