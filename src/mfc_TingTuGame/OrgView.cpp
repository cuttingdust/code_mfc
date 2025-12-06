// OrgView.cpp: 实现文件
//

#include "pch.h"
#include "OrgView.h"

#include "TingTuGameDoc.h"


// OrgView

IMPLEMENT_DYNCREATE(OrgView, CView)

OrgView::OrgView()
{
}

OrgView::~OrgView()
{
}

BEGIN_MESSAGE_MAP(OrgView, CView)
END_MESSAGE_MAP()


// OrgView 绘图

void OrgView::OnDraw(CDC* pDC)
{
    CTingTuGameDoc* pDoc = (CTingTuGameDoc*)GetDocument();
    // TODO:  在此添加绘制代码
    CDC memdc;
    memdc.CreateCompatibleDC(pDC);
    memdc.SelectObject(pDoc->m_bitmap);

    //
    pDC->BitBlt(0, 0, pDoc->m_bmpWidth, pDoc->m_bmpHeight, &memdc, 0, 0, SRCCOPY);
}


// OrgView 诊断

#ifdef _DEBUG
void OrgView::AssertValid() const
{
    CView::AssertValid();
}

#ifndef _WIN32_WCE
void OrgView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif
#endif //_DEBUG

void OrgView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (pHint != NULL)
    {
        if (pHint->IsKindOf(RUNTIME_CLASS(CObArray)))
            return;
    }
    Invalidate(TRUE);
}


// OrgView 消息处理程序
