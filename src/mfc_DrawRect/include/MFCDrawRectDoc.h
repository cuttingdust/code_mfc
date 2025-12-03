#pragma once
#include "stdafx.h"


class CMFCDrawRectDoc : public CDocument
{
protected: /// 仅从序列化创建
    CMFCDrawRectDoc();
    DECLARE_DYNCREATE(CMFCDrawRectDoc)

    /// 特性
public:
    CArray<CRect, CRect&> m_RectArg;
    /// 操作
public:
    /// 重写
public:
    BOOL OnNewDocument() override;
    void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    /// 实现
public:
    ~CMFCDrawRectDoc() override;
#ifdef _DEBUG
    void AssertValid() const override;
    void Dump(CDumpContext& dc) const override;
#endif

protected:
    /// 生成的消息映射函数
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    /// 用于为搜索处理程序设置搜索内容的 Helper 函数
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
