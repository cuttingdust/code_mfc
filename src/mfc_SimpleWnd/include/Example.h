#include "_AFXWIN.h"

class CMyApp : public CWinApp
{
public:
    BOOL InitInstance() override;
};

class CMyWnd : public CWnd
{
public:
    CMyWnd();
    LRESULT WindowProc(UINT, WPARAM, LPARAM) override;
};
