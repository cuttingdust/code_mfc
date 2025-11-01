/**
 * @file   CFileCutter.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-31
 */

#ifndef CFILECUTTER_H
#define CFILECUTTER_H

#include <afxwin.h>

/// 定义自定义消息
#define WM_CUTTERSTART  (WM_USER + 100) /// wParam, nTotal
#define WM_CUTTERSTOP   (WM_USER + 101) /// wParam=nExitCode,lParam=nCompleteCount
#define WM_CUTTERSTATUS (WM_USER + 102) /// lpParam=完成的数量

class CFileCutter
{
public:
    CFileCutter(HWND hWndNotify);
    ~CFileCutter();
    /// 工作码
    enum ExitCode
    {
        exitSuccess,
        exitUserForce,
        exitSourceErr,
        exitDestErr
    }; /// 我们永远不能轻信的多线程的推进是可靠的
public:
    BOOL IsRunning() const;

    BOOL StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFileSize);

    BOOL SuspendCutter();

    BOOL ResumeCutter();

    void StopCutter();

protected:
    /// \brief 设置状态
    void Reset();

    /// \brief 切割文件函数
    void DoSplit();

    UINT friend _CutterEntry(LPVOID lpParam);

    /// 参数信息
    CString m_strSource;
    CString m_strDest;
    UINT    m_uFileSize;
    BOOL    m_bSplit;

    /// 状态标志
    BOOL m_bContinue;
    BOOL m_bRunning;

    /// 同步
    CRITICAL_SECTION m_cs;

private:
    HWND        m_hWndNotify; /// 接受UI线程和当前工作线程的窗口句柄
    HANDLE      m_hWorkEvent; /// 通知开始工作线程的事件对象
    CWinThread* m_pThread;    /// 工作线程
    BOOL        m_bSuspend;
    BOOL        m_bExitThread;
};


#endif // CFILECUTTER_H
