// F:\thirty\MyFile\mfc_code\src\mfc_dlg\src\CDlg.cpp: 实现文件
//

#include "afxdialogex.h"
#include "CDlg.h"
#include "resource.h"

// CDlg 对话框

IMPLEMENT_DYNAMIC(CDlg, CDialogEx)

CDlg::CDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_DLGDEMO, pParent)
{
}

CDlg::~CDlg()
{
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg, CDialogEx)
END_MESSAGE_MAP()


// CDlg 消息处理程序
