
// MFCAddrDlg.cpp: 实现文件
//

#include "framework.h"
#include "MFCAddr.h"
#include "MFCAddrDlg.h"
#include "AddAddrDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCAddrDlg 对话框


CMFCAddrDlg::CMFCAddrDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_MFCADDR_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAddrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCAddrDlg, CDialogEx)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BTN_ADD_ADDR, &CMFCAddrDlg::OnBnClickedBtnAddAddr)
ON_LBN_DBLCLK(IDC_LIST_ADDR, &CMFCAddrDlg::OnLDoubleClickedList)
END_MESSAGE_MAP()


// CMFCAddrDlg 消息处理程序

BOOL CMFCAddrDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);  // 设置大图标
    SetIcon(m_hIcon, FALSE); // 设置小图标

    // TODO: 在此添加额外的初始化代码

    return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCAddrDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int   cxIcon = GetSystemMetrics(SM_CXICON);
        int   cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCAddrDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMFCAddrDlg::OnBnClickedBtnAddAddr()
{
    AddAddrDlg addDlg;
    if (addDlg.DoModal() == IDOK)
    {
        CListBox* pList = static_cast<CListBox*>(GetDlgItem(IDC_LIST_ADDR));
        pList->AddString(addDlg.GetAddAddress() + ";" + addDlg.GetAddUser());
    }
}

void CMFCAddrDlg::OnLDoubleClickedList()
{
    CListBox* pList = static_cast<CListBox*>(GetDlgItem(IDC_LIST_ADDR));
    CString   strTemp;
    pList->GetText(pList->GetCurSel(), strTemp);
    CString output;
    AfxExtractSubString(output, strTemp, 0, ';');
    TRACE("0---->%S", output);

    ::ShellExecute(NULL, TEXT("open"), output, NULL, NULL, SW_SHOW);
    AfxExtractSubString(output, strTemp, 1, ';');
    HGLOBAL p = NULL;
    TRACE("0---->%S", output);

    if (OpenClipboard()) /// 获取剪贴板的独占访问权
    {
        EmptyClipboard();
        USES_CONVERSION;
        char szName[256];
        strcpy(szName, T2A(output));   /// 将Unicode字符串转换为ANSI

        HGLOBAL hMem = GlobalAlloc(GHND, sizeof(szName));  /// 分配全局内存
        char*   pmem = (char*)GlobalLock(hMem); /// 锁定内存获取指针
        memcpy(pmem, szName, sizeof(szName)); /// // 复制数据
        GlobalUnlock(hMem);                     /// 解锁内存
        SetClipboardData(CF_TEXT, hMem);  /// 将数据放到剪贴板
        CloseClipboard(); /// 释放剪贴板控制权
        p = GlobalFree(hMem);
    }
}
