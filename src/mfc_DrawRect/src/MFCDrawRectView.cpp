#include "MFCDrawRectView.h"

IMPLEMENT_DYNCREATE(CMFCDrawRectView, CView)

BEGIN_MESSAGE_MAP(CMFCDrawRectView, CView)
/// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CMFCDrawRectView::CMFCDrawRectView()
{
}

void CMFCDrawRectView::OnDraw(CDC *pDC)
{
    CMFCDrawRectDoc *pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: 在此处为本机数据添加绘制代码
    for (int i = 0; i < pDoc->m_RectArg.GetSize(); i++)
    {
        pDC->Rectangle(pDoc->m_RectArg[i]);
    }
}

BOOL CMFCDrawRectView::PreCreateWindow(CREATESTRUCT &cs)
{
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

BOOL CMFCDrawRectView::OnPreparePrinting(CPrintInfo *pInfo)
{
    // 默认准备
    return DoPreparePrinting(pInfo);
}

void CMFCDrawRectView::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    // TODO: 添加额外的打印前进行的初始化过程
}

void CMFCDrawRectView::OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    // TODO: 添加打印后进行的清理过程
}

CMFCDrawRectView::~CMFCDrawRectView()
{
}

void CMFCDrawRectView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CMFCDrawRectDoc *pDoc = GetDocument();
    int              r    = rand() % 50 + 5;
    CRect            Ret(point.x - r, point.y - r, point.x + r, point.y + r);

    pDoc->m_RectArg.Add(Ret);
    InvalidateRect(Ret, FALSE);

    CView::OnLButtonDown(nFlags, point);
}

/// CMFCDrawRectView 诊断

#ifdef _DEBUG
void CMFCDrawRectView::AssertValid() const
{
    CView::AssertValid();
}

void CMFCDrawRectView::Dump(CDumpContext &dc) const
{
    CView::Dump(dc);
}

CMFCDrawRectDoc *CMFCDrawRectView::GetDocument() const /// 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDrawRectDoc)));
    return (CMFCDrawRectDoc *)m_pDocument;
}
#endif //_DEBUG
