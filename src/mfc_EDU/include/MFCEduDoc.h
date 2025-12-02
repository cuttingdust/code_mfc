#pragma once
#include "stdafx.h"
#include "Stroke.h"

class CMFCEduDoc : public CDocument
{
protected:
    CMFCEduDoc();
    ~CMFCEduDoc() override;
    DECLARE_DYNCREATE(CMFCEduDoc)
public:
    BOOL OnNewDocument() override;
    void Serialize(CArchive& ar) override;
    //////////////////////
    BOOL OnOpenDocument(LPCTSTR lpszPathName) override;
    void DeleteContents() override;

public:
    CTypedPtrList<CObList, CStroke*> m_strokeList;
    CPen*                            getCurrentPen()
    {
        return &m_penCur;
    }

    CStroke* NewStroke();

protected:
    void InitDocument();

public:
    UINT m_nPenWidth;
    CPen m_penCur;

protected:
    DECLARE_MESSAGE_MAP()
};
