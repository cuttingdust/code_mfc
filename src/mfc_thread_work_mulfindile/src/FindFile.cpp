#include <CRapidFinder.h>

UINT FinderEntry(LPVOID lpParam)
{
    CRapidFinder*   pFinder = (CRapidFinder*)lpParam;
    CDirectoryNode* pNode   = NULL; /// m_listDir就是从pNode中获取
    BOOL            bActive = TRUE; /// 线程状态的bool变量

    /// 只要m_listdir有目录
    while (true)
    {
        /// 取出新目录
        ::EnterCriticalSection(&pFinder->m_cs);
        if (pFinder->m_listDir.IsEmpty())
        {
            bActive = FALSE;
        }
        else
        {
            pNode = pFinder->m_listDir.GetHead();
            pFinder->m_listDir.Remove(pNode);
        }
        ::LeaveCriticalSection(&pFinder->m_cs);

        /// bActive指示了当前线程的工作状态，如果m_listDir队列当前状态为空
        /// 那么，我们当前线程先等待
        if (!bActive)
        {
            ::EnterCriticalSection(&pFinder->m_cs);
            pFinder->m_nThreadCount--;
            if (pFinder->m_nThreadCount == 0)
            {
                ::LeaveCriticalSection(&pFinder->m_cs);
                break;
            }
            ::LeaveCriticalSection(&pFinder->m_cs);

            /// 进入等待状态
            ResetEvent(pFinder->m_hDirEvent);
            ::WaitForSingleObject(pFinder->m_hDirEvent, INFINITE);

            /// 此时，当前线程再度获得CPU的推进机会
            ::EnterCriticalSection(&pFinder->m_cs);
            pFinder->m_nThreadCount++; /// 当前的活动进程数量+1
            ::LeaveCriticalSection(&pFinder->m_cs);
            bActive = TRUE;
            continue;
        }

        /// 实现基于pNode的目录查找
        WIN32_FIND_DATA fileData;
        HANDLE          hFindFile;
        /// "D:\"
        if (pNode->szDir[wcslen(pNode->szDir) - 1] != '\\')
        {
            wcscat(pNode->szDir, L"\\");
        }
        wcscat(pNode->szDir, L"*.*");
        hFindFile = ::FindFirstFile(pNode->szDir, &fileData);

        if (hFindFile != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (fileData.cFileName[0] == '.')
                    continue;
                if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    /// 添加进入m_listDir
                    CDirectoryNode* p = new CDirectoryNode;
                    wcsncpy(p->szDir, pNode->szDir, wcslen(pNode->szDir) - 3);
                    wcscat(p->szDir, fileData.cFileName);

                    ::EnterCriticalSection(&pFinder->m_cs);
                    pFinder->m_listDir.AddHead(p);
                    ::LeaveCriticalSection(&pFinder->m_cs);
                    /// 置信一个事件
                    ::SetEvent(pFinder->m_hDirEvent);
                }
                else
                {
                    if (pFinder->CheckFile(fileData.cFileName))
                    {
                        ::EnterCriticalSection(&pFinder->m_cs);
                        ::InterlockedIncrement((long*)&pFinder->m_nResultCount);
                        ::LeaveCriticalSection(&pFinder->m_cs);
                        wprintf(L"%ls\n", fileData.cFileName);
                    }
                }
            }
            while (::FindNextFile(hFindFile, &fileData));
        }
        /// 此节点的保存的目录已经全部搜索完毕
        delete pNode;
        pNode = NULL;
    }
    ::SetEvent(pFinder->m_hDirEvent);
    /// 判断当前线程是否是最后一个结束循环的线程
    if (::WaitForSingleObject(pFinder->m_hDirEvent, 0) != WAIT_TIMEOUT)
    {
        /// 通知主线程，最后一个搜索线程也结束了
        ::SetEvent(pFinder->m_hExitEvent);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    CRapidFinder*   pFinder = new CRapidFinder(64);
    CDirectoryNode* pNode   = new CDirectoryNode;

    /// 测试数据，查询d盘下所有的stdafx
    const wchar_t sz_path[] = LR"(D:\test\)";
    wchar_t       sz_file[] = L"marginLine_48";

    /// 对我们的CRapider信息进行设置
    wcscpy(pNode->szDir, sz_path);
    pFinder->m_listDir.AddHead(pNode);
    wcscpy(pFinder->m_szMatchName, sz_file);

    /// 创建辅助线程
    pFinder->m_nThreadCount = pFinder->m_nMaxThread;
    /// 创建了辅助线程，并等待查找结束
    for (int i = 0; i < pFinder->m_nMaxThread; i++)
    {
        AfxBeginThread(FinderEntry, pFinder);
    }
    ::WaitForSingleObject(pFinder->m_hExitEvent, INFINITE);
    /// 打印出结果
    printf("一共找到了同名文件: %d\n", pFinder->m_nResultCount);
    delete pFinder;

    getchar();
    return 0;
}
