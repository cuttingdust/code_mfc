#include "Stroke.h"

CStroke::CStroke(UINT nPenWidth) : m_nPenWidth(nPenWidth)
{
}

CStroke::CStroke()
{
}

BOOL CStroke::DrawStroke(CDC *pDC)
{
    CPen penStroke;
    if (!penStroke.CreatePen(PS_SOLID, m_nPenWidth, RGB(0, 0, 0)))
    {
        return FALSE;
    }

    CPen *pOldpen = pDC->SelectObject(&penStroke);
    pDC->MoveTo(m_pointArray[0]);
    for (int i = 0; i < m_pointArray.GetSize(); i++)
    {
        pDC->LineTo(m_pointArray[i]);
    }
    pDC->SelectObject(pOldpen);
    return TRUE;
}

void CStroke::Serialize(CArchive &ar)
{
    if (ar.IsStoring())
    {
        ar << (WORD)m_nPenWidth;
        m_pointArray.Serialize(ar);
    }
    else
    {
        WORD w;
        ar >> w;
        m_nPenWidth = w;
        m_pointArray.Serialize(ar);
    }
}

IMPLEMENT_SERIAL(CStroke, CObject, 1)
