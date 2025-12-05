// OrgView.cpp: 实现文件
//

#include "pch.h"
#include "OrgView.h"


// OrgView

IMPLEMENT_DYNCREATE(OrgView, CView)

OrgView::OrgView()
{

}

OrgView::~OrgView()
{
}

BEGIN_MESSAGE_MAP(OrgView, CView)
END_MESSAGE_MAP()


// OrgView 绘图

void OrgView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// OrgView 诊断

#ifdef _DEBUG
void OrgView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void OrgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// OrgView 消息处理程序
