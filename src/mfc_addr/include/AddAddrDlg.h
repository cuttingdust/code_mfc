#pragma once
#include "afxdialogex.h"

#include <memory>


// AddAddrDlg 对话框

class AddAddrDlg : public CDialogEx
{
    DECLARE_DYNAMIC(AddAddrDlg)

public:
    AddAddrDlg(CWnd* pParent = nullptr); // 标准构造函数
    virtual ~AddAddrDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_DLG_ADD_ADDR
    };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnOk();

public:
    CString GetAddAddress() const;

    CString GetAddUser() const;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};
