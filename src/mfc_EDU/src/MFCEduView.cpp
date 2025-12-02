#include "MFCEduView.h"

IMPLEMENT_DYNCREATE(CMFCEduView, CView)
BEGIN_MESSAGE_MAP(CMFCEduView, CView)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
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
    CMFCEduDoc                        *pDoc       = GetDocument();
    CTypedPtrList<CObList, CStroke *> &strokeList = pDoc->m_strokeList;
    POSITION                           pos        = strokeList.GetHeadPosition();
    while (pos != NULL)
    {
        CStroke *pStroke = strokeList.GetNext(pos);
        pStroke->DrawStroke(pDC);
    }
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
    // AfxMessageBox(TEXT("Hello MFC Doc/View"));
    m_pStrokeCur = GetDocument()->NewStroke();
    m_pStrokeCur->m_pointArray.Add(point);

    SetCapture(); /// 捕捉鼠标，直到抬起
    m_ptPrev = point;
    return;
}

void CMFCEduView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (GetCapture() != this)
    {
        return;
    }

    CMFCEduDoc *pDoc = GetDocument();

    CClientDC dc(this);
    CPen     *pOldPen = dc.SelectObject(pDoc->getCurrentPen());

    dc.MoveTo(m_ptPrev);
    dc.LineTo(point);
    dc.SelectObject(pOldPen);
    m_pStrokeCur->m_pointArray.Add(point);
    ReleaseCapture();
    return;
}

void CMFCEduView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (GetCapture() != this)
    {
        return;
    }

    CClientDC dc(this);
    m_pStrokeCur->m_pointArray.Add(point);

    /// 从上一个点按下鼠标之后的拖动轨迹，必须画出
    CPen *pOldPen = dc.SelectObject(GetDocument()->getCurrentPen());
    dc.MoveTo(m_ptPrev);
    dc.LineTo(point);
    dc.SelectObject(pOldPen);
    m_ptPrev = point;

    return;
}
