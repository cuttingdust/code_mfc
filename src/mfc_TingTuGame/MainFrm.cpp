
// MainFrm.cpp: CMainFrame 类的实现
//
#include "pch.h"
#include "framework.h"
#include "TingTuGame.h"

#include "MainFrm.h"

#include "OrgView.h"
#include "TingTuGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CLockableSplitterWnd, CSplitterWnd)

BEGIN_MESSAGE_MAP(CLockableSplitterWnd, CSplitterWnd)
ON_WM_SETCURSOR()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_NCHITTEST()
END_MESSAGE_MAP()

void CLockableSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
    if (m_bLocked && pDC != NULL)
    {
        // 锁定状态：绘制简单的灰色分割条
        CBrush br(GetSysColor(COLOR_BTNFACE));
        pDC->FillRect(rect, &br);

        // 绘制中间的分割线
        CPen  pen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
        CPen* pOldPen = pDC->SelectObject(&pen);

        if (rect.Width() < rect.Height())
        {
            // 垂直分割条
            int xMid = rect.left + rect.Width() / 2;
            pDC->MoveTo(xMid, rect.top + 2);
            pDC->LineTo(xMid, rect.bottom - 2);
        }
        else
        {
            // 水平分割条
            int yMid = rect.top + rect.Height() / 2;
            pDC->MoveTo(rect.left + 2, yMid);
            pDC->LineTo(rect.right - 2, yMid);
        }

        pDC->SelectObject(pOldPen);
    }
    else
    {
        // 解锁状态：使用默认绘制
        CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
    }
}

void CLockableSplitterWnd::OnInvertTracker(const CRect& rect)
{
    if (!m_bLocked)
    {
        CSplitterWnd::OnInvertTracker(rect);
    }
    // 锁定时不显示拖动跟踪矩形
}

BOOL CLockableSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bLocked && m_bOnSplitter)
    {
        // 当鼠标在分割条上时，始终返回箭头光标
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        return TRUE;
    }

    return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CLockableSplitterWnd::OnNcHitTest(CPoint point)
{
    if (m_bLocked)
    {
        // 转换到客户端坐标
        ScreenToClient(&point);

        // 简单判断是否在分割条区域
        int cxBorder = GetSystemMetrics(SM_CXBORDER);

        // 检查是否在垂直分割条位置（根据你的布局，分割条在x=300处）
        if (point.x >= 300 - cxBorder && point.x <= 300 + cxBorder)
        {
            return HTCLIENT; // 告诉系统这是客户端区域，不是分割条
        }
    }

    return CSplitterWnd::OnNcHitTest(point);
}

void CLockableSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_bLocked)
    {
        int splitterPos = 300;

        if (point.x >= splitterPos - 5 && point.x <= splitterPos + 5)
        {
            // 点击在分割条上，但不处理
            m_bOnSplitter = TRUE; // 保持状态
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
            return;
        }
        else
        {
            m_bOnSplitter = FALSE;
        }
    }

    CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CLockableSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bLocked)
    {
        // 检查鼠标是否在分割条上
        BOOL bNowOnSplitter = FALSE;

        // 获取分割条位置（根据你的布局，第一个窗格宽300）
        int splitterPos = 300; // 这个值应该与你的 CSize(300, 0) 一致
        int cxBorder    = GetSystemMetrics(SM_CXEDGE);

        // 检查是否在分割条区域（前后各5像素的范围）
        if (point.x >= splitterPos - 5 && point.x <= splitterPos + 5)
        {
            bNowOnSplitter = TRUE;
        }

        // 如果状态改变，强制设置光标
        if (bNowOnSplitter != m_bOnSplitter)
        {
            m_bOnSplitter = bNowOnSplitter;

            if (m_bOnSplitter)
            {
                // 在分割条上：强制设置为箭头光标
                ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
                SetCapture(); // 捕获鼠标，确保能持续控制光标
            }
            else
            {
                // 不在分割条上：释放捕获
                ReleaseCapture();
            }
        }

        // 如果一直在分割条上，持续设置箭头光标
        if (m_bOnSplitter)
        {
            ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        }
    }
    else
    {
        // 解锁状态：使用默认行为
        m_bOnSplitter = FALSE;
        CSplitterWnd::OnMouseMove(nFlags, point);
    }
}

void CLockableSplitterWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (!m_bLocked)
    {
        CSplitterWnd::OnLButtonUp(nFlags, point);
    }
    // 锁定时不处理
}

//////////////////////////////////////////////////////////////////


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
ON_WM_CREATE()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
    // TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

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
        return -1; // 未能创建
    }

    // TODO: 如果不需要可停靠工具栏，则删除这三行
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);


    return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    // return m_wndSplitter.Create(this, 2, 2,    // TODO: 调整行数和列数
    //                             CSize(10, 10), // TODO: 调整最小窗格大小
    //                             pContext);
    m_wndSplitter.CreateStatic(this, 1, 2);
    m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTingTuGameView), CSize(300, 0), pContext);
    m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(OrgView), CSize(0, 0), pContext);
    /// 将第一个作为当前激活状态的视图
    m_wndSplitter.SetActivePane(0, 0);
    return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式
    cs.cx = 600;
    cs.cy = 580;
    // cs.style     = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_SYSMENU;
    /// 修改这一行：去掉 WS_THICKFRAME 以移除可缩放的边框，去掉 WS_MAXIMIZEBOX 禁用最大化
    cs.style     = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU | WS_MINIMIZEBOX;
    cs.dwExStyle = WS_EX_ACCEPTFILES;
    return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序
