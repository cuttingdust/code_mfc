
// TingTuGameView.cpp: CTingTuGameView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TingTuGame.h"
#endif

#include "TingTuGameDoc.h"
#include "TingTuGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTingTuGameView

IMPLEMENT_DYNCREATE(CTingTuGameView, CView)

BEGIN_MESSAGE_MAP(CTingTuGameView, CView)
END_MESSAGE_MAP()

// CTingTuGameView 构造/析构

CTingTuGameView::CTingTuGameView() noexcept
{
	// TODO: 在此处添加构造代码

}

CTingTuGameView::~CTingTuGameView()
{
}

BOOL CTingTuGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTingTuGameView 绘图

void CTingTuGameView::OnDraw(CDC* /*pDC*/)
{
	CTingTuGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTingTuGameView 诊断

#ifdef _DEBUG
void CTingTuGameView::AssertValid() const
{
	CView::AssertValid();
}

void CTingTuGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTingTuGameDoc* CTingTuGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTingTuGameDoc)));
	return (CTingTuGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CTingTuGameView 消息处理程序
