#include "_AFXSTAT.h"

AFX_MODULE_THREAD_STATE *AfxGetModuleThreadState()
{
    return _afxModuleThreadState.GetData();
}

THREAD_LOCAL(AFX_MODULE_THREAD_STATE, _afxModuleThreadState)

/// 我们定义了_afxModuleThreadState：是用来管理线程状态，
/// 并且将来，引入消息循环之后，维护句柄，套接字等系列的成员

_AFX_THREAD_STATE *AfxGetThreadState()
{
    return _afxThreadState.GetData();
}
THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState)

//////////////////////////////////////////////////////////////////

/// 模块状态
AFX_MODULE_STATE  _afxBaseModuleState;
AFX_MODULE_STATE *AfxGetAppModuleState()
{
    return &_afxBaseModuleState;
}

AFX_MODULE_STATE *AfxGetModuleState()
{
    _AFX_THREAD_STATE *pState = _afxThreadState.GetData();
    AFX_MODULE_STATE  *pRes;

    if (pState->m_pModuleState != NULL)
    {
        pRes = pState->m_pModuleState;
    }
    else
    {
        pRes = AfxGetAppModuleState(); /// AfxGetModuleState();
    }
    return pRes;
}
/// 模块-线程状态
