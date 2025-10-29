
// MFCCheckBoxDlg.cpp: 实现文件
//

#include "framework.h"
#include "MFCCheckBox.h"
#include "MFCCheckBoxDlg.h"
#include "afxdialogex.h"

#include <istream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_ABOUTBOX
    };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCCheckBoxDlg 对话框


CMFCCheckBoxDlg::CMFCCheckBoxDlg(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_MFCCHECKBOX_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCCheckBoxDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // DDX_Control(pDX, IDC_CHK_STICK, m_chk_btn);
}

BEGIN_MESSAGE_MAP(CMFCCheckBoxDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
// ON_BN_CLICKED(IDC_CHK_STICK, &CMFCCheckBoxDlg::OnBnClickedChkStick)
// ON_BN_CLICKED(IDC_ADDTOLIST, &CMFCCheckBoxDlg::OnBnClickedAddToList)
ON_BN_CLICKED(IDC_BTN_DISPLAY, &CMFCCheckBoxDlg::OnBnDisplayCurText)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMFCCheckBoxDlg 消息处理程序

BOOL CMFCCheckBoxDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL    bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);  // 设置大图标
    SetIcon(m_hIcon, FALSE); // 设置小图标

    // TODO: 在此添加额外的初始化代码

    return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCCheckBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCCheckBoxDlg::OnPaint()
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
HCURSOR CMFCCheckBoxDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMFCCheckBoxDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // AfxMessageBox(TEXT("hello"));

    CListBox* pListBox = static_cast<CListBox*>(GetDlgItem(IDC_LISTBOX));
    pListBox->AddString(_T("11111"));

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCCheckBoxDlg::OnBnDisplayCurText()
{
    {
        // CListBox* pListBox = static_cast<CListBox*>(GetDlgItem(IDC_LISTBOX));
        // CStatic*  pStatic  = static_cast<CStatic*>(GetDlgItem(IDC_TXT_DISPLAY));
        //
        // CString strDisplay;
        // int     nCount = pListBox->GetSelCount(); /// 获取选中项数量
        //
        // if (nCount > 0)
        // {
        //     /// 分配数组来存储选中项的索引
        //     CArray<int, int> arrSelections;
        //     arrSelections.SetSize(nCount);
        //     pListBox->GetSelItems(nCount, arrSelections.GetData());
        //
        //     /// 遍历所有选中项
        //     for (int i = 0; i < nCount; i++)
        //     {
        //         CString strItem;
        //         pListBox->GetText(arrSelections[i], strItem);
        //
        //         if (i > 0)
        //             strDisplay += _T(", ");
        //         strDisplay += strItem;
        //     }
        //
        //     pStatic->SetWindowText(strDisplay);
        // }
        // else
        // {
        //     pStatic->SetWindowText(_T("请先选择项目"));
        // }
    }

    {
        CListBox* pListBox = static_cast<CListBox*>(GetDlgItem(IDC_LISTBOX));
        CStatic*  pStatic  = static_cast<CStatic*>(GetDlgItem(IDC_TEXT_DISPLAY));

        int nIndex = pListBox->GetCurSel();

        if (nIndex != LB_ERR)
        {
            CString strText;
            pListBox->GetText(nIndex, strText);

            pStatic->SetWindowText(strText);
        }
        else
        {
            pStatic->SetWindowText(_T("请先选择一个项目"));
        }
    }
}


// void CMFCCheckBoxDlg::OnBnClickedChkStick()
// {
//     if (m_chk_btn.GetCheck())
//     {
//         SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
//     }
//     else
//     {
//         /// 取消置顶
//         SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
//     }
// }

// void CMFCCheckBoxDlg::OnBnClickedAddToList()
// {
//     {
//         TCHAR ws[256];
//         reinterpret_cast<CComboBox*>(GetDlgItem(IDC_STRLIST))->GetWindowText(ws, 256);
//         TRACE("---%S\n", ws); /// 只在vs 显示， 不在控制台显示
//     }
//     {
//         CComboBox* pCmb = static_cast<CComboBox*>(GetDlgItem(IDC_STRLIST));
//         pCmb->AddString(TEXT("444")); /// 添加List
//     }
// }
