
// TingTuGameDoc.h: CTingTuGameDoc 类的接口
//


#pragma once


class CTingTuGameDoc : public CDocument
{
protected: // 仅从序列化创建
    CTingTuGameDoc() noexcept;
    DECLARE_DYNCREATE(CTingTuGameDoc)

    // 特性
public:
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
    virtual ~CTingTuGameDoc();
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


    /// 方块位置结构
    struct pos
    {
        int row = 0;
        int col = 0;
    };

public:
    BOOL IsWin();

    void MapInitial();
    void GameMixMove();
    void GameMove(UINT nChar);

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
    UINT    gamePicID_;
    CBitmap m_bitmap;
    int     hnums = 0;  ///< 系统游戏的标记
    int     vnums = 0;  ///< 纵向的方块数
    pos     m_blankPos; ///< 空白的方块位置

    pos m_map[100][100];  ///< 核心地图
    int m_cellWidth  = 0; ///< 每一个方块的宽度和高度
    int m_cellHeight = 0;
    int m_bmpWidth; ///< 当前图片的尺寸
    int m_bmpHeight;

    int m_BMP_WIDTH; ///< 图片的导入
    int m_BMP_HEIGHT;
};
