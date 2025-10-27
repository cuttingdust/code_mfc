// F:\thirty\MyFile\mfc_code\src\mfc_dlg\src\CDlg.cpp: 实现文件
//

#include "afxdialogex.h"
#include "CDlg.h"
#include "resource.h"

// CDlg 对话框

IMPLEMENT_DYNAMIC(CDlg, CDialogEx)

CDlg::CDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLGDEMO, pParent), m_edit_cs(_T(""))
{
}

CDlg::~CDlg()
{
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON1, m_btn_OK);
    DDX_Control(pDX, IDC_EDIT1, m_edit_ctl);
    DDX_Text(pDX, IDC_EDIT1, m_edit_cs);
}

void CDlg::OnBnClickedButton1()
{
    /// windows api 的使用
    // ::MessageBox(NULL, TEXT("Hello"), TEXT("CMyDlg的Button"), MB_OK);

    // HWND hEdit = ::GetDlgItem(m_hWnd, IDC_EDIT1);
    // ::SetWindowText(hEdit, TEXT("我是Edit"));

    /// 控制
    // m_edit_ctl.SetWindowText(TEXT("我是CEdit"));

    /// 变量
    m_edit_cs = TEXT("我是CS类型的CEdit");
    UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON1, &CDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlg 消息处理程序
