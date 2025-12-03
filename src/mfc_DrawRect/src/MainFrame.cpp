#include "MainFrame.h"
#include "resource.h"


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR, /// 状态行指示器
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;

    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return TRUE;
}

CMainFrame::~CMainFrame()
{
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext &dc) const
{
    CFrameWnd::Dump(dc);
}
#endif //_DEBUG

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,
                               WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
                                       CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("未能创建工具栏\n");
        return -1; /// 未能创建
    }

    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("未能创建状态栏\n");
        return -1; /// 未能创建
    }
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

    // TODO: 如果不需要可停靠工具栏，则删除这三行
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);


    return 0;
}
