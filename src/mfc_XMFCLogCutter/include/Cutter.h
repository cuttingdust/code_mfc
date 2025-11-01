#include <afxwin.h>
#include <afxcmn.h>

#include "CFileCutter.h"

class CMyApp : public CWinApp
{
public:
    BOOL InitInstance() override;
};

class CMainDialog : public CDialog
{
public:
    CMainDialog(CWnd* pParentWnd = NULL);

protected:
    /// 进度条
    CProgressCtrl m_Progress;

    /// 辅助类：CFileCutter--To Do---
    CFileCutter* m_pCutter;

    /// 由于我们需要实时更新工作者线程和主线程
    /// 动态的控制程序界面的函数
    void UIControl();

protected:
    BOOL OnInitDialog() override;
    void OnCancel() override;

    afx_msg void OnSourceBrowser(); /// 选择源文件的按钮响应函数
    afx_msg void OnDestBrowser();   /// 选择目的文件夹的按钮响应函数

    afx_msg void OnStart();
    afx_msg void OnStop();
    afx_msg void OnSelect();

    /// 自定义消息
    afx_msg LRESULT OnCutterStart(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCutterStatus(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCutterStop(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};
