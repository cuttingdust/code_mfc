#include "Cutter.h"

#include "CDirDialog.h"
#include "resource.h"

#include <afxdlgs.h>

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
    CMainDialog dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    CWinApp::InitInstance();

    /// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
    /// 而不是启动应用程序的消息泵。

    return FALSE;
}

CMainDialog::CMainDialog(CWnd *pParentWnd) : CDialog(IDD_FILECUTTER_DIALOG, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
ON_BN_CLICKED(IDC_SOURCEBROWSER, &CMainDialog::OnSourceBrowser)
ON_BN_CLICKED(IDC_DESTBROWSER, &CMainDialog::OnDestBrowser)
ON_BN_CLICKED(IDC_START, &CMainDialog::OnStart)
ON_BN_CLICKED(IDC_STOP, &CMainDialog::OnStop)
ON_BN_CLICKED(IDC_SELECTSPLIT, &CMainDialog::OnSelect)
/// 自定义消息:WM_CUTTERSTART：是在我们线程的辅助类里面的
ON_MESSAGE(WM_CUTTERSTART, &CMainDialog::OnCutterStart)
ON_MESSAGE(WM_CUTTERSTATUS, &CMainDialog::OnCutterStatus)
ON_MESSAGE(WM_CUTTERSTOP, &CMainDialog::OnCutterStop)
END_MESSAGE_MAP()

void CMainDialog::UIControl()
{
    BOOL bIsWorking = m_pCutter->IsRunning();
    GetDlgItem(IDC_SELECTSPLIT)->EnableWindow(!bIsWorking);
    GetDlgItem(IDC_UNIT)->EnableWindow(!bIsWorking);

    GetDlgItem(IDC_START)->EnableWindow(!bIsWorking);
    GetDlgItem(IDC_STOP)->EnableWindow(bIsWorking);

    if (bIsWorking)
    {
        return;
    }

    BOOL bSplit = ((CButton *)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
    if (bSplit)
    {
        GetDlgItem(IDC_START)->SetWindowText(TEXT("分割"));
        GetDlgItem(IDC_SOURCETITLE)->SetWindowText(TEXT("请选择要分割的文件"));
        GetDlgItem(IDC_DESTTITLE)->SetWindowText(TEXT("请选择要保存的文件夹"));
        GetDlgItem(IDC_UNIT)->EnableWindow(TRUE);
    }

    GetDlgItem(IDC_STATUSTEXT)->SetWindowText(TEXT("状态显示"));
    m_Progress.SetPos(0);
}

BOOL CMainDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_pCutter = new CFileCutter(m_hWnd);

    ((CButton *)GetDlgItem(IDC_SELECTSPLIT))->SetCheck(1);

    ((CComboBox *)GetDlgItem(IDC_UNIT))->AddString(TEXT("1"));
    ((CComboBox *)GetDlgItem(IDC_UNIT))->AddString(TEXT("10"));
    ((CComboBox *)GetDlgItem(IDC_UNIT))->AddString(TEXT("20"));
    ((CComboBox *)GetDlgItem(IDC_UNIT))->SetCurSel(0);

    m_Progress.SubclassWindow(*GetDlgItem(IDC_PROGRESS));
    UIControl();
    return TRUE;
}

void CMainDialog::OnCancel()
{
    BOOL bExit = TRUE;
    if (m_pCutter->IsRunning())
    {
        if (MessageBox(TEXT("工作尚未完成，确定退出码?"), NULL, MB_YESNO) == IDYES)
        {
            bExit = FALSE;
        }
    }

    if (bExit)
    {
        delete m_pCutter;
        CDialog::OnCancel();
    }
}

void CMainDialog::OnSourceBrowser()
{
    BOOL bSplit = ((CButton *)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
    if (bSplit)
    {
        CFileDialog sourceFile(TRUE);
        /// 选入源文件
        if (sourceFile.DoModal())
        {
            GetDlgItem(IDC_EDITSOURCE)->SetWindowText(sourceFile.GetPathName());

            /// 设置默认的切割目录 C://a.txt
            /// C://就是默认目录
            CString strDef = sourceFile.GetPathName();
            strDef         = strDef.Left(strDef.ReverseFind('.'));
            GetDlgItem(IDC_EDITDEST)->SetWindowText(strDef);
        }
    }
}

void CMainDialog::OnDestBrowser()
{
    CDirDialog destFolder;
    if (destFolder.DoBrowser(*this) == IDOK)
    {
        GetDlgItem(IDC_EDITDEST)->SetWindowText(destFolder.GetPath());
    }
}

void CMainDialog::OnStart()
{
    CString strSource, strDest;

    /// 检查输入
    GetDlgItem(IDC_EDITSOURCE)->GetWindowText(strSource);
    GetDlgItem(IDC_EDITDEST)->GetWindowTextW(strDest);

    if (strSource.IsEmpty() || strDest.IsEmpty())
    {
        MessageBox(TEXT("路径不能为空"));
        return;
    }

    BOOL bSplit = ((CButton *)GetDlgItem(IDC_SELECTSPLIT))->GetCheck();
    if (bSplit)
    {
        CString str;
        GetDlgItem(IDC_UNIT)->GetWindowText(str);
        m_pCutter->StartSplit(strDest, strSource, _wtoi(str) * 1024 * 1024);
    }
}

void CMainDialog::OnStop()
{
    m_pCutter->SuspendCutter();
    if (MessageBox(TEXT("确定要终止吗？"), NULL, MB_YESNO) == IDOK)
    {
        m_pCutter->StopCutter();
    }
    else
    {
        m_pCutter->ResumeCutter();
    }
}

void CMainDialog::OnSelect()
{
    UIControl();
}

LRESULT CMainDialog::OnCutterStart(WPARAM wParam, LPARAM lParam)
{
    int nTotalFiles = wParam;
    m_Progress.SetRange(0, nTotalFiles);
    UIControl();
    return 0;
}

LRESULT CMainDialog::OnCutterStatus(WPARAM wParam, LPARAM lParam)
{
    int nCompleted = (int)lParam;
    m_Progress.SetPos(nCompleted);
    /// 显示状态
    CString s;
    s.Format(TEXT("完成了%d个文件"), nCompleted);
    GetDlgItem(IDC_STATUSTEXT)->SetWindowText(s);
    return 0;
}

LRESULT CMainDialog::OnCutterStop(WPARAM wParam, LPARAM lParam)
{
    int nErrorCode = wParam;
    switch (nErrorCode)
    {
        case CFileCutter::exitSuccess:
            MessageBox(TEXT("操作成功"));
            break;
        case CFileCutter::exitSourceErr:
            MessageBox(TEXT("源文件出错"));
            break;
        case CFileCutter::exitDestErr:
            MessageBox(TEXT("目标文件出错"));
            break;
        case CFileCutter::exitUserForce:
            MessageBox(TEXT("用户终止"));
            break;
    }
    UIControl();
    return 0;
}
