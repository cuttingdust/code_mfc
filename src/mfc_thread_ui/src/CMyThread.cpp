#include "CMyThread.h"

#include "CThreadWnd.h"

BOOL CMyThread::InitInstance()
{
    // TODO: �ڴ����ר�ô����/����û���
    CThreadWnd *pDlg = new CThreadWnd();
    pDlg->DoModal();
    return CWinThread::InitInstance();
}

int CMyThread::ExitInstance()
{
    // TODO: �ڴ����ר�ô����/����û���

    return CWinThread::ExitInstance();
}
