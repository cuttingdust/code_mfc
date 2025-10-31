#include "CMyThread.h"

#include "CThreadWnd.h"

BOOL CMyThread::InitInstance()
{
    // TODO: 在此添加专用代码和/或调用基类
    CThreadWnd *pDlg = new CThreadWnd();
    pDlg->DoModal();
    return CWinThread::InitInstance();
}

int CMyThread::ExitInstance()
{
    // TODO: 在此添加专用代码和/或调用基类

    return CWinThread::ExitInstance();
}
