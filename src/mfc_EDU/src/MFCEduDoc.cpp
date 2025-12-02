#include "MFCEduDoc.h"

IMPLEMENT_DYNCREATE(CMFCEduDoc, CDocument)

/// Doc参与消息循环，也能进行用户行为与操作系统的交互
BEGIN_MESSAGE_MAP(CMFCEduDoc, CDocument)
END_MESSAGE_MAP()

CMFCEduDoc::CMFCEduDoc()
{
}

BOOL CMFCEduDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }
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
}

CMFCEduDoc::~CMFCEduDoc()
{
}
