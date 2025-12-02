#include "MFCEduDoc.h"

/// Doc参与消息循环，也能进行用户行为与操作系统的交互
BEGIN_MESSAGE_MAP(CMFCEduDoc, CDocument)
END_MESSAGE_MAP()

CMFCEduDoc::CMFCEduDoc()
{
}

CMFCEduDoc::~CMFCEduDoc()
{
}

BOOL CMFCEduDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }
    InitDocument();
    return TRUE;
}

void CMFCEduDoc::Serialize(CArchive &ar)
{
    if (ar.IsStoring())
    {
    }
    else
    {
    }
    m_strokeList.Serialize(ar);
}

BOOL CMFCEduDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
    {
        return FALSE;
    }
    InitDocument();
    return TRUE;
}

void CMFCEduDoc::DeleteContents()
{
    while (!m_strokeList.IsEmpty())
    {
        delete m_strokeList.RemoveHead();
    }
    CDocument::DeleteContents();
}


CStroke *CMFCEduDoc::NewStroke()
{
    CStroke *pStrokeItem = new CStroke(m_nPenWidth);
    m_strokeList.AddTail(pStrokeItem);
    SetModifiedFlag();
    return pStrokeItem;
}

void CMFCEduDoc::InitDocument()
{
    m_nPenWidth = 2;
    if (&m_penCur != NULL)
    {
        m_penCur.DeleteObject();
        m_penCur.CreatePen(PS_SOLID, m_nPenWidth, RGB(0, 0, 0));
    }
}


IMPLEMENT_DYNCREATE(CMFCEduDoc, CDocument)
