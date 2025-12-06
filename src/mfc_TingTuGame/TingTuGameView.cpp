
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
ON_WM_LBUTTONDOWN()
ON_WM_KEYDOWN()
ON_WM_SHOWWINDOW()
ON_WM_SETFOCUS()
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

void CTingTuGameView::OnDraw(CDC* pDC)
{
    CTingTuGameDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: 在此处为本机数据添加绘制代码
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap memBmp;
    memBmp.CreateCompatibleBitmap(pDC, pDoc->m_bmpWidth, pDoc->m_bmpHeight);
    memDC.SelectObject(&memBmp);

    /// 将图像绘制到membmp:首先：我们修改的pos的对应游戏map的数据结构，我们将这个数据结构对应的cell图像绘入view
    /// step1:绘制打乱的方块
    DrawGameBlocks(&memDC);
    /// step2:绘制空白的方块
    DrawBlankBlock(&memDC);
    //step3:绘制边界线
    DrawLines(&memDC);

    //整幅考入我们的屏幕
    pDC->BitBlt(0, 0, pDoc->m_bmpWidth, pDoc->m_bmpHeight, &memDC, 0, 0, SRCCOPY);
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

void CTingTuGameView::OnUpdate(CView*, LPARAM, CObject*)
{
    // TODO: 在此添加专用代码和/或调用基类
    Invalidate(FALSE);
}


void CTingTuGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CTingTuGameDoc* pDoc = GetDocument();
    /// 计算一下鼠标点击所在的cell方块的位置
    pos DownPos;
    DownPos.row = point.x / pDoc->m_cellWidth + 1;
    DownPos.col = point.y / pDoc->m_cellHeight + 1;

    /// 根据当前空白方块位置将鼠标点击相邻方块的情况
    /// 转换成键盘输入提交给Document
    if (abs(DownPos.row - pDoc->m_blankPos.row) + abs(DownPos.col - pDoc->m_blankPos.col) == 1)
    {
        if (DownPos.row - pDoc->m_blankPos.row == 1)
        {
            pDoc->OnKeyDown(VK_RIGHT, 0, 0);
        }
        if (DownPos.row - pDoc->m_blankPos.row == -1)
        {
            pDoc->OnKeyDown(VK_LEFT, 0, 0);
        }
        if (DownPos.col - pDoc->m_blankPos.col == 1)
        {
            pDoc->OnKeyDown(VK_DOWN, 0, 0);
        }
        if (DownPos.col - pDoc->m_blankPos.col == -1)
        {
            pDoc->OnKeyDown(VK_UP, 0, 0);
        }
    }
}

void CTingTuGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CTingTuGameDoc* pDoc = GetDocument();
    pDoc->OnKeyDown(nChar, nRepCnt, nFlags);
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTingTuGameView::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CView::OnShowWindow(bShow, nStatus);
}

void CTingTuGameView::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);
}


//////////////////////////////////////////////////////////////////
//绘制方块间的信息


/// 函数功能：在指定目标位置绘制一个拼图方块，其图像来源于源图像中的特定位置
/// 参数pDC: 设备上下文，用于在窗口上绘图
/// 参数destPos: 目标位置（在游戏网格中的行列坐标）
/// 参数srcPos: 源位置（在完整原图中的行列坐标，用于从原图中截取对应方块）
void CTingTuGameView::DrawCellBlock(CDC* pDC, pos destPos, pos srcPos)
{
    /// 获取文档对象指针，文档中存储了游戏数据和位图资源
    CTingTuGameDoc* pDoc = GetDocument();

    /// 创建与当前设备上下文（pDC）兼容的内存设备上下文，用于操作位图
    CDC memdc;
    memdc.CreateCompatibleDC(pDC);
    /// 将游戏原图（m_bitmap）选入内存设备上下文，以便进行后续的位块传输
    memdc.SelectObject(pDoc->m_bitmap);

    /// 关键操作：将原图中指定位置的方块图像，绘制到游戏网格的指定格子中
    pDC->BitBlt((destPos.row - 1) * pDoc->m_cellWidth,  /// 目标矩形左上角X坐标（像素）
                (destPos.col - 1) * pDoc->m_cellHeight, /// 目标矩形左上角Y坐标（像素）
                pDoc->m_cellWidth,                      /// 要传输的矩形宽度（一个方块的宽度）
                pDoc->m_cellHeight,                     /// 要传输的矩形高度（一个方块的高度）
                &memdc,                                 /// 源设备上下文（包含原图的内存DC）
                (srcPos.row - 1) * pDoc->m_cellWidth,   /// 源位图矩形左上角X坐标（像素）
                (srcPos.col - 1) * pDoc->m_cellHeight,  /// 源位图矩形左上角Y坐标（像素）
                SRCCOPY                                 /// 光栅操作码：直接将源复制到目标
    );
}

/// 函数功能：绘制空白方块（即拼图中空缺的那一格）
/// 参数pDC: 设备上下文，用于在窗口上绘图
void CTingTuGameView::DrawBlankBlock(CDC* pDC)
{
    CTingTuGameDoc* pDoc = GetDocument();

    /// 计算空白格在窗口客户区中的矩形位置（像素坐标）
    RECT rc;
    /// 左上角坐标：根据空白格在网格中的逻辑位置（m_blankPos）和每个格子的大小计算
    rc.left = (pDoc->m_blankPos.row - 1) * pDoc->m_cellWidth;
    rc.top  = (pDoc->m_blankPos.col - 1) * pDoc->m_cellHeight;
    /// 右下角坐标：左上角坐标加上一个格子的宽高
    rc.right  = rc.left + pDoc->m_cellWidth;
    rc.bottom = rc.top + pDoc->m_cellHeight;

    /// 使用灰色填充该矩形区域，视觉上表示“空白格”
    pDC->FillSolidRect(&rc, RGB(153, 153, 153)); /// 填充为灰色（RGB值153, 153, 153）
}

/// 函数功能：绘制游戏网格线（分隔各个拼图方块的红色线条）
/// 参数pDC: 设备上下文，用于在窗口上绘图
void CTingTuGameView::DrawLines(CDC* pDC)
{
    /// 创建用于画线的红色画笔
    CPen linePen;
    linePen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); /// 实线，宽度3像素，红色

    /// 将新画笔选入设备上下文，并保存旧画笔以便之后恢复
    CPen*           pOldPen = pDC->SelectObject(&linePen);
    CTingTuGameDoc* pDoc    = GetDocument();

    int i = 0;
    /// 绘制垂直网格线（列之间的分隔线）
    for (i = 1; i < pDoc->hnums; i++) /// 循环次数为列数减一，因为n列有n-1条内部分隔线
    {
        /// 计算当前线的X坐标（第i列右侧），Y坐标从顶部到底部
        pDC->MoveTo(i * pDoc->m_cellWidth - 1, 0);                 /// 移动到线条起点（减去1像素可能为了视觉效果）
        pDC->LineTo(i * pDoc->m_cellWidth - 1, pDoc->m_bmpHeight); /// 画线到终点（整个位图高度）
    }

    /// 绘制水平网格线（行之间的分隔线）
    for (i = 1; i < pDoc->vnums; i++) /// 循环次数为行数减一
    {
        /// 计算当前线的Y坐标（第i行下侧），X坐标从最左到最右
        pDC->MoveTo(0, i * pDoc->m_cellHeight - 1);
        pDC->LineTo(pDoc->m_bmpWidth, i * pDoc->m_cellHeight - 1);
    }

    /// 恢复设备上下文原来的画笔，这是良好的GDI编程习惯，避免资源泄漏
    pDC->SelectObject(pOldPen);
}

/// 函数功能：绘制游戏中的所有非空白方块
/// 参数pDC: 设备上下文，用于在窗口上绘图
/// 此函数遍历游戏网格，根据每个格子存储的坐标信息，绘制对应的拼图块
void CTingTuGameView::DrawGameBlocks(CDC* pDC)
{
    CTingTuGameDoc* pDoc = GetDocument();

    /// 遍历游戏网格的每一个位置（i, j）
    for (int i = 1; i <= pDoc->hnums; i++) /// i 表示当前网格列
    {
        for (int j = 1; j <= pDoc->vnums; j++) /// j 表示当前网格行
        {
            pos srcPos, destPos;

            /// 【核心逻辑】决定方块图像的来源和去向：
            /// 1. srcPos（源位置）：从文档的m_map数组中读取。该坐标指示此网格位置(i, j)
            ///    当前应该显示原图中哪个位置(row, col)的方块。这是游戏逻辑（移动、打乱）的结果。
            /// 2. destPos（目标位置）：就是当前网格位置(i, j)本身。
            srcPos.row  = pDoc->m_map[i][j].row; /// 此格应该显示的原图行
            srcPos.col  = pDoc->m_map[i][j].col; /// 此格应该显示的原图列
            destPos.row = i;                     /// 此格在游戏网格中的列索引
            destPos.col = j;                     /// 此格在游戏网格中的行索引

            /// 调用DrawCellBlock函数，将原图中srcPos位置的图像绘制到网格的destPos位置
            DrawCellBlock(pDC, destPos, srcPos);
        }
    }
    /// 注意：此函数绘制所有方块，但不包括空白格。空白格通常需要单独调用DrawBlankBlock绘制。
}


// CTingTuGameView 消息处理程序
