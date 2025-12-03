#include "MFCDrawRectDoc.h"

#ifndef SHARED_HANDLERS
#include "MFCDrawRect.h"
#endif

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMFCDrawRectDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCDrawRectDoc, CDocument)
END_MESSAGE_MAP()

CMFCDrawRectDoc::CMFCDrawRectDoc()
{
    m_RectArg.SetSize(256, 256);
}

BOOL CMFCDrawRectDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    /// TODO: 在此添加重新初始化代码
    /// (SDI 文档将重用该文档)

    return TRUE;
}

void CMFCDrawRectDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        /// TODO: 在此添加存储代码
    }
    else
    {
        /// TODO: 在此添加加载代码
    }
}

CMFCDrawRectDoc::~CMFCDrawRectDoc()
{
}

#ifdef SHARED_HANDLERS

/// 缩略图的支持
void CMFCDrawRectDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    /// 修改此代码以绘制文档数据
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

/// 搜索处理程序的支持
void CMFCDrawRectDoc::InitializeSearchContent()
{
    CString strSearchContent;
    /// 从文档数据设置搜索内容。
    /// 内容部分应由“;”分隔

    /// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
    SetSearchContent(strSearchContent);
}

void CMFCDrawRectDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl* pChunk = NULL;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != NULL)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

/// CMFCDrawRectDoc 诊断

#ifdef _DEBUG
void CMFCDrawRectDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CMFCDrawRectDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}

#endif

/// CMFCDrawRectDoc 命令
