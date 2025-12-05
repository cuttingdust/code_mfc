
// MFCArchiveDemoView.cpp: CMFCArchiveDemoView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCArchiveDemo.h"
#endif

#include "MFCArchiveDemoDoc.h"
#include "MFCArchiveDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCArchiveDemoView

IMPLEMENT_DYNCREATE(CMFCArchiveDemoView, CView)

BEGIN_MESSAGE_MAP(CMFCArchiveDemoView, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCArchiveDemoView::OnFilePrintPreview)
ON_WM_CONTEXTMENU()
ON_WM_RBUTTONUP()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCArchiveDemoView 构造/析构

CMFCArchiveDemoView::CMFCArchiveDemoView() noexcept
{
    // TODO: 在此处添加构造代码
}

CMFCArchiveDemoView::~CMFCArchiveDemoView()
{
}

BOOL CMFCArchiveDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

// CMFCArchiveDemoView 绘图

void CMFCArchiveDemoView::OnDraw(CDC* pDC)
{
    CMFCArchiveDemoDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: 在此处为本机数据添加绘制代码
    int index;
    index = pDoc->m_index;
    for (int i = 1; i <= index; i++)
    {
        pDC->Ellipse(pDoc->points[i].x - 2, pDoc->points[i].y - 2, pDoc->points[i].x + 2, pDoc->points[i].y + 2);
    }
}


// CMFCArchiveDemoView 打印


void CMFCArchiveDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL CMFCArchiveDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // 默认准备
    return DoPreparePrinting(pInfo);
}

void CMFCArchiveDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加额外的打印前进行的初始化过程
}

void CMFCArchiveDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加打印后进行的清理过程
}

void CMFCArchiveDemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CMFCArchiveDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCArchiveDemoView 诊断

#ifdef _DEBUG
void CMFCArchiveDemoView::AssertValid() const
{
    CView::AssertValid();
}

void CMFCArchiveDemoView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMFCArchiveDemoDoc* CMFCArchiveDemoView::GetDocument() const // 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCArchiveDemoDoc)));
    return (CMFCArchiveDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCArchiveDemoView 消息处理程序

void CMFCArchiveDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CMFCArchiveDemoDoc* pDoc = GetDocument();
    if (pDoc->m_index == 100)
        return;

    /// 接受鼠标
    pDoc->points[pDoc->m_index] = point;
    pDoc->m_index++;
    pDoc->SetModifiedFlag(); /// 设置文档修改标志
    Invalidate();

    CView::OnLButtonDown(nFlags, point);
}
