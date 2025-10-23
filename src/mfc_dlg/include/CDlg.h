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
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    CButton      m_btn_OK;
    CEdit        m_edit_ctl;
    CString      m_edit_cs;
};
