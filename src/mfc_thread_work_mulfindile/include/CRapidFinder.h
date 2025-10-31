#ifndef RAPIDFIND_H
#define RAPIDFIND_H

#include <afxwin.h>

struct CDirectoryNode : public CNoTrackObject
{
    CDirectoryNode* pNext;           /// 链表指针
    wchar_t         szDir[MAX_PATH]; /// 目录路径
};

class CRapidFinder
{
public:
    explicit CRapidFinder(int nMaxThread);
    virtual ~CRapidFinder();

    BOOL CheckFile(LPCTSTR lpszFileName);

public:
    int m_nResultCount; ///< 结果的数量
    int m_nThreadCount; ///< 活动线程的数量

    CTypedSimpleList<CDirectoryNode*> m_listDir; /// 待搜索目录队列
    CRITICAL_SECTION                  m_cs;      /// 临界区

    const int m_nMaxThread;
    wchar_t   m_szMatchName[MAX_PATH]; ///< 要搜索的文件

    HANDLE m_hDirEvent; ///< 我们向m_listDir添加新的目录，授信 9停止，1个人，m_listDir为空  /// 目录事件，通知线程有新目录可处理
    HANDLE m_hExitEvent; ///< 各个搜索线程是否已经结束  // 退出事件，通知主线程所有工作已完成
};


#endif // RAPIDFIND_H
