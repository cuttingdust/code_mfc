#include "CFileCutter.h"

/// 创建一个内部的工作线程
UINT _CutterEntry(LPVOID lpParam)
{
    /// 得到文件切割对象
    CFileCutter *pCutter = static_cast<CFileCutter *>(lpParam);
    /// 循环处理用户的工作请求
    while (::WaitForSingleObject(pCutter->m_hWorkEvent, INFINITE) == WAIT_OBJECT_0 && !pCutter->m_bExitThread)
    {
        /// 设置m_hEvent
        ::EnterCriticalSection(&pCutter->m_cs);
        pCutter->m_bRunning = TRUE;
        ::LeaveCriticalSection(&pCutter->m_cs);
        if (pCutter->m_bSplit)
        {
            pCutter->DoSplit();
        }
    }
    return 0;
}

CFileCutter::CFileCutter(HWND hWndNotify)
{
    /// 初始化全局变量
    m_hWndNotify  = hWndNotify;
    m_bExitThread = FALSE;
    m_bSuspend    = FALSE;

    m_hWorkEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    /// 创建工作者线程
    m_pThread                = AfxBeginThread(_CutterEntry, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
    m_pThread->m_bAutoDelete = FALSE;
    m_pThread->ResumeThread();

    ::InitializeCriticalSection(&m_cs);
    Reset();
}

CFileCutter::~CFileCutter()
{
    /// 设置结束
    m_bExitThread = TRUE;

    ::EnterCriticalSection(&m_cs);
    m_bContinue = FALSE;
    ::LeaveCriticalSection(&m_cs);

    ::SetEvent(m_hWorkEvent);

    ::WaitForSingleObject(m_pThread->m_hThread, INFINITE);
    ::CloseHandle(m_hWorkEvent);
    ::DeleteCriticalSection(&m_cs);
    delete m_pThread;
}

BOOL CFileCutter::IsRunning() const
{
    return m_bRunning;
}

BOOL CFileCutter::StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFileSize)
{
    if (m_bRunning)
    {
        return FALSE;
    }

    ::EnterCriticalSection(&m_cs);
    m_strSource = lpszSourceFile;
    m_strDest   = lpszDestDir;
    m_uFileSize = uFileSize;
    m_bSplit    = TRUE;
    ::LeaveCriticalSection(&m_cs);

    /// 通知线程开始工作
    ::SetEvent(m_hWorkEvent);
    return TRUE;
}

BOOL CFileCutter::SuspendCutter()
{
    if (!m_bRunning)
    {
        return FALSE;
    }

    if (!m_bSuspend)
    {
        m_pThread->SuspendThread();
        m_bSuspend = TRUE;
    }
    return TRUE;
}

BOOL CFileCutter::ResumeCutter()
{
    if (!m_bRunning)
    {
        return FALSE;
    }

    if (m_bSuspend)
    {
        m_pThread->ResumeThread();
        m_bSuspend = TRUE;
    }
    return TRUE;
}

void CFileCutter::StopCutter()
{
    /// 强制退出
    ::EnterCriticalSection(&m_cs);
    m_bContinue = FALSE;
    ::LeaveCriticalSection(&m_cs);
    ResumeCutter();
}

void CFileCutter::Reset()
{
    ::EnterCriticalSection(&m_cs);
    m_strSource.Empty();
    m_strDest.Empty();
    m_uFileSize = 0;
    m_bSplit    = TRUE;

    m_bContinue = TRUE;
    m_bRunning  = FALSE;
    ::LeaveCriticalSection(&m_cs);
}

void CFileCutter::DoSplit()
{
    int nCompleted = 0;

    CString strSourceFile = m_strSource;
    CString strDestDir    = m_strDest;
    CFile   sourceFile, destFile;

    /// 打开文件
    BOOL bOK = sourceFile.Open(strSourceFile,
                               CFile::modeRead |               ///  只读模式
                                       CFile::shareDenyWrite | ///  禁止其他进程写入
                                       CFile::typeBinary);     ///  二进制模式

    if (!bOK) /// 打开失败处理
    {
        ::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSourceErr, nCompleted);
        return;
    }

    int nPos = -1; /// 开始创建文件夹
                   /// 逐级创建目录（处理多级目录如：C:\A\B\C）
    while ((nPos = strDestDir.Find('\\', nPos + 1)) != -1)
    {
        ::CreateDirectory(strDestDir.Left(nPos), NULL);
    }
    ::CreateDirectory(strDestDir, NULL); /// 创建最终目录

    if (strDestDir.Right(1) != '\\')
    {
        strDestDir += '\\';
    }

    /// 通知UI线程，我们开始分割文件
    int nTotalFiles = sourceFile.GetLength() / m_uFileSize + 1;
    ::PostMessage(m_hWndNotify, WM_CUTTERSTART, nTotalFiles, TRUE);

    /// 工作者线程开始任务
    const int c_page = 4 * 1024;
    char      buff[c_page];
    DWORD     dwRead;

    CString sDestName;
    int     nPreCount = 1;
    UINT    uWriteBytes;
    do
    {
        /// 创建一个目标文件
        sDestName.Format(TEXT("%d"), nPreCount);
        sDestName += sourceFile.GetFileName();
        if (!destFile.Open(strDestDir + sDestName, CFile::modeWrite | CFile::modeCreate))
        {
            ::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitDestErr, nCompleted);
            sourceFile.Close();
        }

        uWriteBytes = 0;
        do
        {
            if (!m_bContinue)
            {
                destFile.Close();
                sourceFile.Close();
                if (!m_bExitThread)
                    ::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitUserForce, nCompleted);
                return;
            }

            /// 进行按4k大小写入
            dwRead = sourceFile.Read(buff, c_page);
            destFile.Write(buff, dwRead);
            uWriteBytes += dwRead;
        }
        while (dwRead > 0 && uWriteBytes < m_uFileSize);
        destFile.Close();

        nCompleted = nPreCount++;
        ::PostMessage(m_hWndNotify, WM_CUTTERSTATUS, 0, nCompleted);
    }
    while (dwRead > 0);
    sourceFile.Close();

    /// 通知用户，工作完成
    ::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSuccess, nCompleted);
}
