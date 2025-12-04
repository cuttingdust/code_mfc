// COrigView.cpp: 实现文件
//

#include "pch.h"
#include "COrigView.h"
#include "MFCMultiView.h"


// COrigView

IMPLEMENT_DYNCREATE(COrigView, CView)

COrigView::COrigView()
{
}

COrigView::~COrigView()
{
}

BEGIN_MESSAGE_MAP(COrigView, CView)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// COrigView 绘图

void COrigView::OnDraw(CDC* pDC)
{
    CMFCMultiViewDoc* pDoc = (CMFCMultiViewDoc*)GetDocument();
    // TODO:  在此添加绘制代码
    for (int i = 0; i < pDoc->m_RectArg.GetSize(); i++)
    {
        pDC->Rectangle(pDoc->m_RectArg[i]);
    }
}


// COrigView 诊断

#ifdef _DEBUG
void COrigView::AssertValid() const
{
    CView::AssertValid();
}

#ifndef _WIN32_WCE
void COrigView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif
#endif //_DEBUG


// COrigView 消息处理程序

void COrigView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CMFCMultiViewDoc* pDoc = (CMFCMultiViewDoc*)GetDocument();
    int               r    = rand() % 50 + 5;
    CRect             Ret(point.x - r, point.y - r, point.x + r, point.y + r);

    pDoc->m_RectArg.Add(Ret);
    InvalidateRect(Ret, FALSE);
    pDoc->UpdateAllViews(this);

    CView::OnLButtonDown(nFlags, point);
}
