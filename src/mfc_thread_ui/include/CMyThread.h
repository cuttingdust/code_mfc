#pragma once
#include <afxwin.h>
class CMyThread : public CWinThread
{
public:
    virtual BOOL InitInstance();
    virtual int  ExitInstance();
};
