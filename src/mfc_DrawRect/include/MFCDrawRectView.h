#pragma once

#include "MFCDrawRectDoc.h"
#include "stdafx.h"

class CMFCDrawRectView : public CView
{
protected: /// 仅从序列化创建
    CMFCDrawRectView();
    DECLARE_DYNCREATE(CMFCDrawRectView)

    /// 特性
public:
    CMFCDrawRectDoc* GetDocument() const;

public:
    /// 操作
public:
    /// 重写
public:
    void OnDraw(CDC* pDC) override; /// 重写以绘制该视图
    BOOL PreCreateWindow(CREATESTRUCT& cs) override;

protected:
    BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
    void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
    void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

    /// 实现
public:
    ~CMFCDrawRectView() override;
#ifdef _DEBUG
    void AssertValid() const override;
    void Dump(CDumpContext& dc) const override;
#endif

protected:
    /// 生成的消息映射函数
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG /// MFCDrawRectView.cpp 中的调试版本
inline CMFCDrawRectDoc* CMFCDrawRectView::GetDocument() const
{
    return reinterpret_cast<CMFCDrawRectDoc*>(m_pDocument);
}
#endif
