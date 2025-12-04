
// MFCMultiViewView.cpp: CMFCMultiViewView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCMultiView.h"
#endif

#include "MFCMultiViewDoc.h"
#include "MFCMultiViewView.h"
#include "COrigView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCMultiViewView

IMPLEMENT_DYNCREATE(CMFCMultiViewView, CView)

BEGIN_MESSAGE_MAP(CMFCMultiViewView, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCMultiViewView 构造/析构

CMFCMultiViewView::CMFCMultiViewView() noexcept
{
    // TODO: 在此处添加构造代码
}

CMFCMultiViewView::~CMFCMultiViewView()
{
}

BOOL CMFCMultiViewView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

// CMFCMultiViewView 绘图

void CMFCMultiViewView::OnDraw(CDC* pDC)
{
    CMFCMultiViewDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    for (int i = 0; i < pDoc->m_RectArg.GetSize(); i++)
    {
        pDC->Rectangle(pDoc->m_RectArg[i]);
    }
}


// CMFCMultiViewView 打印

BOOL CMFCMultiViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // 默认准备
    return DoPreparePrinting(pInfo);
}

void CMFCMultiViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加额外的打印前进行的初始化过程
}

void CMFCMultiViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: 添加打印后进行的清理过程
}


// CMFCMultiViewView 诊断

#ifdef _DEBUG
void CMFCMultiViewView::AssertValid() const
{
    CView::AssertValid();
}

void CMFCMultiViewView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMFCMultiViewDoc* CMFCMultiViewView::GetDocument() const // 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCMultiViewDoc)));
    return (CMFCMultiViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCMultiViewView 消息处理程序
