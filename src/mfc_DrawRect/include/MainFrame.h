#pragma once

#include "stdafx.h"

class CMainFrame : public CFrameWnd
{
protected: /// 仅从序列化创建
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    /// 特性
public:
    /// 操作
public:
    /// 重写
public:
    BOOL PreCreateWindow(CREATESTRUCT& cs) override;

    /// 实现
public:
    ~CMainFrame() override;
#ifdef _DEBUG
    void AssertValid() const override;
    void Dump(CDumpContext& dc) const override;
#endif

protected: /// 控件条嵌入成员
    CToolBar   m_wndToolBar;
    CStatusBar m_wndStatusBar;

    /// 生成的消息映射函数
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};
