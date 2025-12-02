#include "Edu.h"
#include "MFCEduDoc.h"
#include "MFCEduView.h"
#include "MainFrame.h"

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    enum
    {
        IDD = IDD_ABOUTBOX
    };
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMFCEduApp, CWinApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

CMFCEduApp::CMFCEduApp()
{
}

CMFCEduApp theApp;

BOOL CMFCEduApp::InitInstance()
{
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(IDR_MAINFRAME, RUNTIME_CLASS(CMFCEduDoc), RUNTIME_CLASS(CMainFrame),
                                          RUNTIME_CLASS(CMFCEduView));

    AddDocTemplate(pDocTemplate);

    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    if (!ProcessShellCommand(cmdInfo))
    {
        return FALSE;
    }

    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

void CMFCEduApp::OnAppAbout()
{
    CAboutDlg dlg;
    dlg.DoModal();
}
