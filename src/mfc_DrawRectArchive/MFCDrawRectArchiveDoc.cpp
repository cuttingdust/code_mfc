
// MFCDrawRectArchiveDoc.cpp: CMFCDrawRectArchiveDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCDrawRectArchive.h"
#endif

#include "MFCDrawRectArchiveDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCDrawRectArchiveDoc

IMPLEMENT_DYNCREATE(CMFCDrawRectArchiveDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCDrawRectArchiveDoc, CDocument)
END_MESSAGE_MAP()


// CMFCDrawRectArchiveDoc 构造/析构

IMPLEMENT_SERIAL(CRectPos, CObject, 1)
CRectPos::CRectPos(int x, int y, int x1, int y1) : m_x(x), m_y(y), m_x1(x1), m_y1(y1)
{
}

void CRectPos::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_x << m_y << m_x1 << m_y1;
    }
    else
    {
        ar >> m_x >> m_y >> m_x1 >> m_y1;
    }
}

CMFCDrawRectArchiveDoc::CMFCDrawRectArchiveDoc() noexcept
{
    // TODO: 在此添加一次性构造代码
}

CMFCDrawRectArchiveDoc::~CMFCDrawRectArchiveDoc()
{
}

void CMFCDrawRectArchiveDoc::NewRect(int x, int y, int x1, int y1)
{
    CRectPos* pPos = new CRectPos(x, y, x1, y1);
    m_RectList.AddTail(pPos);
    SetModifiedFlag();
}

void CMFCDrawRectArchiveDoc::DrawRect(CDC* pDC)
{
    POSITION pos = m_RectList.GetHeadPosition();
    while (pos != NULL)
    {
        CRectPos* pRectPos = m_RectList.GetNext(pos);
        pDC->Rectangle(pRectPos->m_x, pRectPos->m_y, pRectPos->m_x1, pRectPos->m_y1);
    }
}

BOOL CMFCDrawRectArchiveDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: 在此添加重新初始化代码
    // (SDI 文档将重用该文档)

    return TRUE;
}


// CMFCDrawRectArchiveDoc 序列化

void CMFCDrawRectArchiveDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: 在此添加存储代码
    }
    else
    {
        // TODO: 在此添加加载代码
    }
    m_RectList.Serialize(ar);
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMFCDrawRectArchiveDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    // 修改此代码以绘制文档数据
    dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

    CString strText = _T("TODO: implement thumbnail drawing here");
    LOGFONT lf;

    CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
    pDefaultGUIFont->GetLogFont(&lf);
    lf.lfHeight = 36;

    CFont fontDraw;
    fontDraw.CreateFontIndirect(&lf);

    CFont* pOldFont = dc.SelectObject(&fontDraw);
    dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
    dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMFCDrawRectArchiveDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // 从文档数据设置搜索内容。
    // 内容部分应由“;”分隔

    // 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
    SetSearchContent(strSearchContent);
}

void CMFCDrawRectArchiveDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl* pChunk = nullptr;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != nullptr)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

// CMFCDrawRectArchiveDoc 诊断

#ifdef _DEBUG
void CMFCDrawRectArchiveDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CMFCDrawRectArchiveDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCDrawRectArchiveDoc 命令
