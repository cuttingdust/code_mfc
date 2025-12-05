
// TingTuGameView.h: CTingTuGameView 类的接口
//

#pragma once
#include "TingTuGameDoc.h"

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
};

#ifndef _DEBUG // TingTuGameView.cpp 中的调试版本
inline CTingTuGameDoc* CTingTuGameView::GetDocument() const
{
    return reinterpret_cast<CTingTuGameDoc*>(m_pDocument);
}
#endif
