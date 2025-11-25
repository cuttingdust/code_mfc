#ifndef TESTCOREMFC_H
#define TESTCOREMFC_H

#include "_AFXWIN.h"

class CMyApp : public CWinApp
{
public:
    BOOL InitInstance() override;
};

class CMainWindow : public CWnd
{
public:
    CMainWindow();

protected:
    wchar_t m_szText[1024]; /// 客户区的文本缓冲区
    RECT    m_rcInfo;

protected:
    void         PostNcDestroy() override;
    afx_msg BOOL OnCreate(LPCREATESTRUCT);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nID);
    DECLARE_MESSAGE_MAP();
};


#endif // TESTCOREMFC_H
