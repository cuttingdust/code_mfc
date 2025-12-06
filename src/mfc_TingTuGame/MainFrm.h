
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

class CLockableSplitterWnd : public CSplitterWnd
{
    DECLARE_DYNAMIC(CLockableSplitterWnd)

public:
    CLockableSplitterWnd() : m_bLocked(TRUE), m_bOnSplitter(FALSE)
    {
    }

    void LockSplitter(BOOL bLock = TRUE)
    {
        m_bLocked = bLock;
    }
    BOOL IsSplitterLocked() const
    {
        return m_bLocked;
    }

protected:
    BOOL m_bLocked;
    BOOL m_bOnSplitter;

    // 重写虚函数
    virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
    virtual void OnInvertTracker(const CRect& rect);

    // 消息处理函数
    afx_msg BOOL    OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg LRESULT OnNcHitTest(CPoint point);

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////


class CMainFrame : public CFrameWnd
{
protected: // 仅从序列化创建
    CMainFrame() noexcept;
    DECLARE_DYNCREATE(CMainFrame)

    // 特性
protected:
    CLockableSplitterWnd m_wndSplitter;

public:
    // 操作
public:
    // 重写
public:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    void         LockSplitterBars(BOOL bLock);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    // 实现
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected: // 控件条嵌入成员
    CToolBar m_wndToolBar;

    // 生成的消息映射函数
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};
