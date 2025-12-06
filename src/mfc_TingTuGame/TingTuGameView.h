
// TingTuGameView.h: CTingTuGameView 类的接口
//

#pragma once
#include "TingTuGameDoc.h"

struct pos
{
    int row; ///< 方块所在的行
    int col; ///< 方块所在的列
};

class CTingTuGameView : public CView
{
protected: // 仅从序列化创建
    CTingTuGameView() noexcept;
    DECLARE_DYNCREATE(CTingTuGameView)

    // 特性
public:
    CTingTuGameDoc* GetDocument() const;

    // 操作
public:
    void DrawCellBlock(CDC* pDC, pos destsite, pos srcsite);
    void DrawBlankBlock(CDC* pDC);
    void DrawLines(CDC* pDC);
    void DrawGameBlocks(CDC* pDC);

    BOOL m_bFirstRun;
    // 重写
public:
    virtual void OnDraw(CDC* pDC); // 重写以绘制该视图
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    // 实现
public:
    virtual ~CTingTuGameView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    // 生成的消息映射函数
protected:
    DECLARE_MESSAGE_MAP()
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG // TingTuGameView.cpp 中的调试版本
inline CTingTuGameDoc* CTingTuGameView::GetDocument() const
{
    return reinterpret_cast<CTingTuGameDoc*>(m_pDocument);
}
#endif
