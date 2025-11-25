#pragma once
#include "afxdialogex.h"


// CDlg 对话框

class CDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CDlg)

public:
    CDlg(CWnd* pParent = nullptr); // 标准构造函数
    virtual ~CDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_DLGDEMO
    };
#endif

protected:
    void    DoDataExchange(CDataExchange* pDX) override; // DDX/DDV 支持
    LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    CButton      m_btn_OK;
    CEdit        m_edit_ctl;
    CString      m_edit_cs;
    afx_msg void OnClose();
};
