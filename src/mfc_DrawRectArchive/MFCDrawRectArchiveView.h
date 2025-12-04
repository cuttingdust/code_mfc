
// MFCDrawRectArchiveView.h: CMFCDrawRectArchiveView 类的接口
//

#pragma once


class CMFCDrawRectArchiveView : public CView
{
protected: // 仅从序列化创建
    CMFCDrawRectArchiveView() noexcept;
    DECLARE_DYNCREATE(CMFCDrawRectArchiveView)

    // 特性
public:
    CMFCDrawRectArchiveDoc* GetDocument() const;

    // 操作
public:
    // 重写
public:
    virtual void OnDraw(CDC* pDC); // 重写以绘制该视图
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    // 实现
public:
    virtual ~CMFCDrawRectArchiveView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    // 生成的消息映射函数
protected:
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG // MFCDrawRectArchiveView.cpp 中的调试版本
inline CMFCDrawRectArchiveDoc* CMFCDrawRectArchiveView::GetDocument() const
{
    return reinterpret_cast<CMFCDrawRectArchiveDoc*>(m_pDocument);
}
#endif
