
// TingTuGameDoc.cpp: CTingTuGameDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TingTuGame.h"
#endif

#include "TingTuGameDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTingTuGameDoc

IMPLEMENT_DYNCREATE(CTingTuGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CTingTuGameDoc, CDocument)
END_MESSAGE_MAP()


// CTingTuGameDoc 构造/析构

CTingTuGameDoc::CTingTuGameDoc() noexcept
{
    // TODO: 在此添加一次性构造代码

    m_BMP_WIDTH  = 300;
    m_BMP_HEIGHT = 250;

    hnums = 3;
    vnums = 2;

    m_blankPos.row = hnums;
    m_blankPos.col = vnums;

    HBITMAP hBitmap = ::LoadBitmap(AfxGetApp()->m_hInstance, (LPCTSTR)IDB_BITMAP1);
    m_bitmap.Attach(hBitmap);
    BITMAP bmp;
    GetObject(m_bitmap.m_hObject, sizeof(BITMAP), (LPSTR)&bmp);

    m_bmpWidth  = bmp.bmWidth;
    m_bmpHeight = bmp.bmHeight;

    /// 按照定义的行列数进行切分
    m_cellWidth  = m_bmpWidth / hnums;
    m_cellHeight = m_bmpHeight / vnums;

    /// 地图初始化
    MapInitial();

    /// 打乱图像，将空白起点放在右下角
    m_blankPos.row = hnums;
    m_blankPos.col = vnums;

    GameMixMove();

    UpdateAllViews(NULL);
}

CTingTuGameDoc::~CTingTuGameDoc()
{
    DeleteObject(m_bitmap);
}

BOOL CTingTuGameDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: 在此添加重新初始化代码
    // (SDI 文档将重用该文档)

    return TRUE;
}


// CTingTuGameDoc 序列化

void CTingTuGameDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: 在此添加存储代码
    }
    else
    {
        // TODO: 在此添加加载代码
    }
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CTingTuGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CTingTuGameDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // 从文档数据设置搜索内容。
    // 内容部分应由“;”分隔

    // 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
    SetSearchContent(strSearchContent);
}

void CTingTuGameDoc::SetSearchContent(const CString& value)
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

// CTingTuGameDoc 诊断

#ifdef _DEBUG
void CTingTuGameDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CTingTuGameDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/// 函数功能：初始化游戏地图（拼图网格），为每个格子设置初始的行列坐标
void CTingTuGameDoc::MapInitial()
{
    /// 假设hnums是水平方向格子数，vnums是垂直方向格子数
    /// 通常数组下标从1开始，可能是为了简化与界面坐标的对应关系
    for (int i = 1; i <= hnums; i++) /// 遍历每一行
    {
        for (int j = 1; j <= vnums; j++) /// 遍历每一列
        {
            m_map[i][j].row = i; /// 设置该格子的行坐标
            m_map[i][j].col = j; /// 设置该格子的列坐标
        }
    }
    /// 初始化后，m_map数组中的每个元素(row, col)与它的数组索引(i, j)一致
    /// 例如，m_map[2][3]的值为(row=2, col=3)，表示这是原始状态下位于第2行第3列的拼图块
}

/// 函数功能：通过执行300次随机移动来打乱拼图（即"洗牌"操作）
void CTingTuGameDoc::GameMixMove()
{
    /// 用当前时间初始化随机数种子，确保每次运行生成的随机序列不同
    srand(time(nullptr));

    /// 执行300次随机移动来充分打乱拼图
    for (int i = 1; i < 300; i++)
    {
        /// 生成1-4的随机数，对应四个移动方向
        switch (int temp = rand() % 4 + 1)
        {
            case 1:
                GameMove(VK_RIGHT); /// 随机尝试将空白格右移
                break;
            case 2:
                GameMove(VK_LEFT); /// 随机尝试将空白格左移
                break;
            case 3:
                GameMove(VK_UP); /// 随机尝试将空白格上移
                break;
            case 4:
                GameMove(VK_DOWN); /// 随机尝试将空白格下移
                break;
        }
    }
}

/// 函数功能：根据键盘方向键输入移动空白格，实现拼图块的滑动
/// 参数nChar：虚拟键码（VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN）
void CTingTuGameDoc::GameMove(UINT nChar)
{
    pos tempPos; // 记录要与空白格交换的邻居位置
    switch (nChar)
    {
        case VK_LEFT:
            if (m_blankPos.row > 1)
            {
                tempPos.row = m_blankPos.row - 1;
                tempPos.col = m_blankPos.col;

                // 1. 保存邻居格子的值
                pos neighborValue = m_map[tempPos.row][tempPos.col];
                // 2. 将邻居格子的值移到空白格（空白格被填充）
                m_map[m_blankPos.row][m_blankPos.col] = neighborValue;
                // 3. 关键：将空白格自身的“正确坐标”移到邻居格（邻居格变成新空白格）
                m_map[tempPos.row][tempPos.col].row = tempPos.row;
                m_map[tempPos.row][tempPos.col].col = tempPos.col;

                // 4. 更新空白格位置记录
                m_blankPos = tempPos;
            }
            break;
        case VK_RIGHT:
            if (m_blankPos.row < hnums)
            {
                tempPos.row = m_blankPos.row + 1;
                tempPos.col = m_blankPos.col;

                pos neighborValue                     = m_map[tempPos.row][tempPos.col];
                m_map[m_blankPos.row][m_blankPos.col] = neighborValue;
                m_map[tempPos.row][tempPos.col].row   = tempPos.row;
                m_map[tempPos.row][tempPos.col].col   = tempPos.col;

                m_blankPos = tempPos;
            }
            break;
        case VK_UP:
            if (m_blankPos.col > 1)
            {
                tempPos.row = m_blankPos.row;
                tempPos.col = m_blankPos.col - 1;

                pos neighborValue                     = m_map[tempPos.row][tempPos.col];
                m_map[m_blankPos.row][m_blankPos.col] = neighborValue;
                m_map[tempPos.row][tempPos.col].row   = tempPos.row;
                m_map[tempPos.row][tempPos.col].col   = tempPos.col;

                m_blankPos = tempPos;
            }
            break;
        case VK_DOWN:
            if (m_blankPos.col < vnums)
            {
                tempPos.row = m_blankPos.row;
                tempPos.col = m_blankPos.col + 1;

                pos neighborValue                     = m_map[tempPos.row][tempPos.col];
                m_map[m_blankPos.row][m_blankPos.col] = neighborValue;
                m_map[tempPos.row][tempPos.col].row   = tempPos.row;
                m_map[tempPos.row][tempPos.col].col   = tempPos.col;

                m_blankPos = tempPos;
            }
            break;
    }
}

BOOL CTingTuGameDoc::IsWin()
{
    for (int i = 1; i <= hnums; i++)
    {
        for (int j = 1; j <= vnums; j++)
        {
            if (i == m_blankPos.row && j == m_blankPos.col)
            {
                continue;
            }

            /// 检查当前格子存储的坐标是否等于它应该所在的位置
            if (m_map[i][j].row != i || m_map[i][j].col != j)
            {
                return FALSE; /// 只要有一个不对，就没赢
            }
        }
    }
    /// 所有非空白格都归位了
    return TRUE;
}


void CTingTuGameDoc::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    GameMove(nChar);

    CObArray HintArray; /// 做更新提示
    UpdateAllViews(NULL, 0, &HintArray);

    if (IsWin())
    {
        MessageBox(NULL, TEXT("闯关成功"), TEXT("拼图游戏"), MB_OK);
    }
}
// CTingTuGameDoc 命令
