#ifndef __AFXSTAT_H
#define __AFXSTAT_H

#include "_AFXTLS.h"


class CWinThread;
class AFX_MODULE_THREAD_STATE : public CNoTrackObject
{
    /// 模块线程状态
public:
    /// 指向当前的CWinThread对象
    CWinThread* m_pCurrentWinThread = NULL;
};

EXTERN_THREAD_LOCAL(AFX_MODULE_THREAD_STATE, _afxModuleThreadState)
AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState();

#endif // !__AFXSTAT_H
