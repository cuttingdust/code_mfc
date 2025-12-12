
// VideoDlg.h: 头文件
//

#pragma once
#include <opencv2/opencv.hpp>


// CVideoDlg 对话框
class CVideoDlg : public CDialogEx
{
    // 构造
public:
    CVideoDlg(CWnd* pParent = nullptr); // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum
    {
        IDD = IDD_VIDEO_DIALOG
    };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL    OnInitDialog();
    afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void    OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedShowImg();
    afx_msg void OnBnClickedOpenImg();
    afx_msg void OnBnClickedOpenVideo();

    /// 播放视频
    cv::VideoCapture m_video;
    CWnd*            pWin = NULL;
    CDC*             dc   = NULL;
    HDC              hdc  = NULL;
    afx_msg void     OnTimer(UINT_PTR nIDEvent);
    CSliderCtrl      m_play;
    afx_msg void     OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    /// 是否按下滑动条
    bool m_bPress = false;
};
