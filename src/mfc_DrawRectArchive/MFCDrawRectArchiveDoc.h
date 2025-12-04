
// MFCDrawRectArchiveDoc.h: CMFCDrawRectArchiveDoc 类的接口
//


#pragma once

class CRectPos : public CObject
{
    DECLARE_SERIAL(CRectPos)
public:
    int m_x, m_y, m_x1, m_y1;

public:
    CRectPos()
    {
    }
    CRectPos(int x, int y, int x1, int y1);
    virtual void Serialize(CArchive& ar);

public:
    ~CRectPos()
    {
    }
};


class CMFCDrawRectArchiveDoc : public CDocument
{
protected: // 仅从序列化创建
    CMFCDrawRectArchiveDoc() noexcept;
    DECLARE_DYNCREATE(CMFCDrawRectArchiveDoc)

    // 特性
public:
    CTypedPtrList<CObList, CRectPos*> m_RectList;
    void                              NewRect(int x, int y, int x1, int y1);
    void                              DrawRect(CDC* pDC);
    // 操作
public:
    // 重写
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    // 实现
public:
    virtual ~CMFCDrawRectArchiveDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    // 生成的消息映射函数
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // 用于为搜索处理程序设置搜索内容的 Helper 函数
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
