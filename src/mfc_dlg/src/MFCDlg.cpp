#include <afxwin.h>
// #include <resource.h>
#include <CDlg.h>

#define BTN_ID1 10002

class MyFstApp : CWinApp /// ==>一个实例，Windows应用程序实例
{
public:
    BOOL InitInstance() override;
};

class MainWindow : public CFrameWnd /// ==>这是一个用C++对象表明的Windows窗口
{
public:
    MainWindow();

    afx_msg void HandleButton();

    DECLARE_MESSAGE_MAP()

private:
    CButton *pBtn = nullptr;
};

BEGIN_MESSAGE_MAP(MainWindow, CFrameWnd)
ON_BN_CLICKED(BTN_ID1, HandleButton)

END_MESSAGE_MAP()

void MainWindow::HandleButton()
{
    // ::MessageBox(NULL, TEXT("Hello"), TEXT("MFC"), MB_OK);
    // ::DialogBoxParam(AfxGetInstanceHandle(), (LPCTSTR)IDD_DLGDEMO, NULL, NULL, NULL);

    CDlg dlg;
    dlg.DoModal();
}

MyFstApp theApp;

BOOL MyFstApp::InitInstance()
{
    /// 创建一个框架窗口
    MainWindow *pMain = new MainWindow();
    /// 显示
    pMain->ShowWindow(m_nCmdShow);
    pMain->UpdateWindow();
    this->m_pMainWnd = pMain; /// 建立Window对象和App对象的关联关系
    return TRUE;
}

MainWindow::MainWindow()
{
    CFrameWnd::Create(NULL, TEXT("我的第一个MFC程序"));

    HWND hBtn = ::CreateWindow(TEXT("Button"), TEXT("按钮"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 45, 160, 65,
                               this->m_hWnd, reinterpret_cast<HMENU>(10001), AfxGetInstanceHandle(), NULL);

    CRect r(300, 450, 500, 600);
    pBtn = new CButton;
    pBtn->Create(TEXT("CButton"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, r, this, BTN_ID1);
}
