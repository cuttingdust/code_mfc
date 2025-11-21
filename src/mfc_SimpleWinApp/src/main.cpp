#include "_AFXWIN.h"

class CMyApp : public CWinApp
{
public:
    BOOL InitInstance() override;
    BOOL ExitInstance() override;
};

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
    ::MessageBox(NULL, TEXT("主线程开始执行!"), TEXT("CMyApp::Init"), MB_OK);
    return FALSE; /// 不要进入消息循环
}

int CMyApp::ExitInstance()
{
    ::MessageBox(NULL, TEXT("主线程将要退出"), TEXT("CMyApp::Exit"), MB_OK);
    return 0;
}
