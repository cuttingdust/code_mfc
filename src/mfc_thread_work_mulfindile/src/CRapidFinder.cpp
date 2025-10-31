#include "CRapidFinder.h"

CRapidFinder::CRapidFinder(int nMaxThread) : m_nMaxThread(nMaxThread)
{
    m_nResultCount   = 0;
    m_nThreadCount   = 0;
    m_szMatchName[0] = L'\0';

    /// HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, // 安全属性，通常为NULL
    ///                    BOOL                  bManualReset,      // 手动重置还是自动重置
    ///                    BOOL                  bInitialState,     // 初始状态（有信号/无信号）
    ///                    LPCSTR                lpName             // 事件对象名称
    /// );

    m_listDir.Construct(offsetof(CDirectoryNode, pNext));
    m_hDirEvent  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    ::InitializeCriticalSection(&m_cs);
}

CRapidFinder::~CRapidFinder()
{
    ::CloseHandle(m_hDirEvent);
    ::CloseHandle(m_hExitEvent);
    ::DeleteCriticalSection(&m_cs);
}

BOOL CRapidFinder::CheckFile(LPCTSTR lpszFileName)
{
    const size_t BUFFER_SIZE            = MAX_PATH;
    wchar_t      string[BUFFER_SIZE]    = { 0 };
    wchar_t      strSearch[BUFFER_SIZE] = { 0 };

    /// 使用安全的 Unicode 字符串函数
    wcscpy(string, lpszFileName);
    wcscpy(strSearch, m_szMatchName);

    /// 转换为大写
    _wcsupr(string);
    _wcsupr(strSearch);

    /// 检查是否包含
    return (wcsstr(string, strSearch) != NULL);
}
