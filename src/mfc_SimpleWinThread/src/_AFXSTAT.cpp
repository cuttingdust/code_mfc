#include "_AFXSTAT.h"

AFX_MODULE_THREAD_STATE *AfxGetModuleThreadState()
{
    return _afxModuleThreadState.GetData();
}

THREAD_LOCAL(AFX_MODULE_THREAD_STATE, _afxModuleThreadState)

/// 我们定义了_afxModuleThreadState：是用来管理线程状态，
/// 并且将来，引入消息循环之后，维护句柄，套接字等系列的成员
