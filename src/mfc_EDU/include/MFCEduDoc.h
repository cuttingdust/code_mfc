#pragma once
#include "stdafx.h"

class CMFCEduDoc : public CDocument
{
protected:
    CMFCEduDoc();
    DECLARE_DYNCREATE(CMFCEduDoc)
public:
    BOOL OnNewDocument() override;
    void Serialize(CArchive& ar) override;

public:
    ~CMFCEduDoc() override;

protected:
    DECLARE_MESSAGE_MAP()
};
