#include "MFCEduView.h"

IMPLEMENT_DYNCREATE(CMFCEduView, CView)
BEGIN_MESSAGE_MAP(CMFCEduView, CView)
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CMFCEduView::CMFCEduView()
{
}

CMFCEduDoc *CMFCEduView::GetDocument()
{
    return (CMFCEduDoc *)m_pDocument;
}

void CMFCEduView::OnDraw(CDC *pDC)
{
    CMFCEduDoc *pDoc = GetDocument();
}

BOOL CMFCEduView::PreCreateWindow(CREATESTRUCT &cs)
{
    return CView::PreCreateWindow(cs);
}

CMFCEduView::~CMFCEduView()
{
}

void CMFCEduView::OnLButtonDown(UINT nFlags, CPoint point)
{
    AfxMessageBox(TEXT("Hello MFC Doc/View"));
}
