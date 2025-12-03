/// MFCDrawRect.h : MFCDrawRect 应用程序的主头文件
///
#include "stdafx.h"

#include "resource.h" /// 主符号

/// CMFCDrawRectApp:
/// 有关此类的实现，请参阅 MFCDrawRect.cpp
///

class CMFCDrawRectApp : public CWinApp
{
public:
    CMFCDrawRectApp();


    /// 重写
public:
    BOOL InitInstance() override;
    int  ExitInstance() override;

    /// 实现
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CMFCDrawRectApp theApp;
