
// MFCDrawRectArchiveView.cpp: CMFCDrawRectArchiveView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCDrawRectArchive.h"
#endif

#include "MFCDrawRectArchiveDoc.h"
#include "MFCDrawRectArchiveView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDrawRectArchiveView

IMPLEMENT_DYNCREATE(CMFCDrawRectArchiveView, CView)

BEGIN_MESSAGE_MAP(CMFCDrawRectArchiveView, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCDrawRectArchiveView 构造/析构

CMFCDrawRectArchiveView::CMFCDrawRectArchiveView() noexcept
{
    // TODO: 在此处添加构造代码
}

CMFCDrawRectArchiveView::~CMFCDrawRectArchiveView()
{
}

BOOL CMFCDrawRectArchiveView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

// CMFCDrawRectArchiveView 绘图

void CMFCDrawRectArchiveView::OnDraw(CDC* pDC)
{
    CMFCDrawRectArchiveDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: 在此处为本机数据添加绘制代码
    pDoc->DrawRect(pDC);
}


// CMFCDrawRectArchiveView 打印

BOOL CMFCDrawRectArchiveView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // 默认准备
    return DoPreparePrinting(pInfo);
}

void CMFCDrawRectArchiveView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加额外的打印前进行的初始化过程
}

void CMFCDrawRectArchiveView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加打印后进行的清理过程
}


// CMFCDrawRectArchiveView 诊断

#ifdef _DEBUG
void CMFCDrawRectArchiveView::AssertValid() const
{
    CView::AssertValid();
}

void CMFCDrawRectArchiveView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

void CMFCDrawRectArchiveView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CMFCDrawRectArchiveDoc* pDoc = GetDocument();
    /// 随机生成一个Rect,同时记录pdoc
    int   r  = rand() % 50 + 5;
    int   x  = point.x - r;
    int   y  = point.y - r;
    int   x1 = point.x + r;
    int   y1 = point.y + r;
    CRect rect(x, y, x1, y1);
    pDoc->NewRect(x, y, x1, y1);
    Invalidate();
    CView::OnLButtonDown(nFlags, point);
}

CMFCDrawRectArchiveDoc* CMFCDrawRectArchiveView::GetDocument() const // 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDrawRectArchiveDoc)));
    return (CMFCDrawRectArchiveDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCDrawRectArchiveView 消息处理程序
