#pragma once
#include "stdafx.h"
#include "MFCEduDoc.h"

class CMFCEduView : public CView
{
protected:
    CMFCEduView();
    DECLARE_DYNCREATE(CMFCEduView)
public:
    CMFCEduDoc* GetDocument();

public:
    void OnDraw(CDC* pDC) override;
    BOOL PreCreateWindow(CREATESTRUCT& cs) override;

public:
    ~CMFCEduView() override;

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()
};
