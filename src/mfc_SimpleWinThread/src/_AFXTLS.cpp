#include "_AFXTLS.h"
#include <cstdio>

/**
 * @brief 在链表头部添加节点
 * @param p 要添加的节点指针
 */
void CSimpleList::AddHead(void *p)
{
    /// 将新节点的next指针指向当前头节点
    *GetNextPtr(p) = m_pHead;
    /// 更新头节点为新节点
    m_pHead = p;
}

/**
 * @brief 从链表中移除指定节点
 * @param p 要移除的节点指针
 * @return 成功移除返回TRUE，否则返回FALSE
 */
BOOL CSimpleList::Remove(void *p)
{
    if (p == NULL)
        return FALSE;

    /// 假定移除失败
    BOOL bResult = FALSE;

    /// 情况1：要移除的是头节点
    if (p == m_pHead)
    {
        /// 将头节点指向下一个节点
        m_pHead = *GetNextPtr(p);
        bResult = TRUE;
    }
    else
    {
        /// 情况2：要移除的是中间或尾部节点
        void *pText = m_pHead;

        /// 遍历链表，找到p的前一个节点
        while (pText != NULL && *GetNextPtr(pText) != p)
        {
            pText = *GetNextPtr(pText);
        }

        /// 如果找到了前一个节点
        if (pText != NULL)
        {
            /// 将前一个节点的next指针指向p的下一个节点
            *GetNextPtr(pText) = *GetNextPtr(p);
            bResult            = TRUE;
        }
    }
    return bResult;
}

/////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief 重载new操作符，使用GlobalAlloc分配内存（不参与内存跟踪）
 * @param nSize 要分配的内存大小
 * @return 分配的内存指针
 */
void *CNoTrackObject::operator new(size_t nSize)
{
    /// 申请一块GMEM_FIXED（固定内存）和GMEM_ZEROINIT（初始化为0）的内存
    /// GlobalAlloc是Windows操作系统直接分配的内存，不参与MFC的内存跟踪
    void *p = ::GlobalAlloc(GPTR, nSize); /// GPTR = GMEM_FIXED | GMEM_ZEROINIT

    /// 调试用：验证内存分配
    // printf("CNoTrackObject::operator new - 分配 %zu 字节内存\n", nSize);
    return p;
}


/**
 * @brief 重载delete操作符，释放GlobalAlloc分配的内存
 * @param p 要释放的内存指针
 */
void CNoTrackObject::operator delete(void *p)
{
    if (p != NULL)
    {
        ::GlobalFree(p);
        // printf("CNoTrackObject::operator delete - 释放内存\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 槽位数据结构，描述每个TLS槽位的状态
 */
struct CSlotData
{
    DWORD     dwFlags; ///< 槽位标志位，SLOT_USED表示槽位已被使用
    HINSTANCE hInst;   ///< 模块句柄，标识哪个模块在使用这个槽位
};

/**
 * @brief 线程私有数据结构，每个线程都有一个实例
 */
struct CThreadData : public CNoTrackObject
{
    CThreadData *pNext  = nullptr; ///< 指向下一个线程数据的指针（形成链表）
    int          nCount = 0;       ///< 当前线程使用的槽位数量
    LPVOID *     pData  = nullptr; ///< 线程私有数据数组指针
};

#define SLOT_USED 0x01 ///< 槽位使用标志位


/**
 * @brief 线程槽位数据管理类的构造函数
 */
CThreadSlotData::CThreadSlotData()
{
    /// 初始化链表，指定pNext成员在CThreadData结构中的偏移量
    m_list.Construct(offsetof(CThreadData, pNext));

    /// 初始化成员变量
    m_nMax      = 0;    ///< 当前最大槽位索引
    m_nAlloc    = 0;    ///< 已分配的槽位数量
    m_nRover    = 1;    ///< 快速查找空闲槽位的起始位置
    m_pSlotData = NULL; ///< 槽位数据数组指针

    /// 申请一个TLS索引，用于存储线程私有数据
    m_tlsIndex = ::TlsAlloc();
    /// 初始化临界区，用于线程同步
    ::InitializeCriticalSection(&m_cs);
}


/**
 * @brief 分配一个新的线程槽位
 * @return 分配的槽位索引（从1开始，0保留不用）
 */
int CThreadSlotData::AllocSlot()
{
    ::EnterCriticalSection(&m_cs); /// 进入临界区，保证线程安全

    int nAlloc = m_nAlloc; /// 当前已分配的槽位数量
    int nSlot  = m_nRover; /// 从快速查找位置开始寻找空闲槽位

    /// 如果快速查找位置超出范围或已被使用，则从头开始查找
    if (nSlot >= nAlloc || m_pSlotData[nSlot].dwFlags & SLOT_USED)
    {
        /// 从槽位1开始查找第一个空闲槽位（槽位0保留）
        for (nSlot = 1; nSlot < nAlloc && m_pSlotData[nSlot].dwFlags & SLOT_USED; nSlot++)
        {
            /// 循环直到找到空闲槽位或到达末尾
        }
    }

    /// 如果没有找到空闲槽位，需要扩容
    if (nSlot >= nAlloc)
    {
        /// 当槽位不足时，每次增加32个槽位
        int     nNewAlloc = nAlloc + 32;
        HGLOBAL hSlotData;

        if (m_pSlotData == NULL)
        {
            /// 第一次分配：申请新的可移动内存
            hSlotData = ::GlobalAlloc(GMEM_MOVEABLE, nNewAlloc * sizeof(CSlotData));
        }
        else
        {
            /// 扩容：重新分配更大的内存块
            hSlotData = ::GlobalHandle(m_pSlotData); /// 获取现有内存的句柄
            ::GlobalUnlock(hSlotData);               /// 解锁现有内存
            hSlotData = ::GlobalReAlloc(hSlotData, nNewAlloc * sizeof(CSlotData), GMEM_MOVEABLE);
        }

        /// 锁定内存获取指针
        CSlotData *pSlotData = (CSlotData *)::GlobalLock(hSlotData);

        /// 将新扩展的内存空间初始化为0
        memset(pSlotData + m_nAlloc, 0, (nNewAlloc - nAlloc) * sizeof(CSlotData));
        m_nAlloc    = nNewAlloc; /// 更新已分配槽位数量
        m_pSlotData = pSlotData; /// 更新槽位数据指针
    }

    /// 调整当前最大使用槽位索引
    if (nSlot >= m_nMax)
    {
        m_nMax = nSlot + 1;
    }

    /// 标记该槽位为已使用
    m_pSlotData[nSlot].dwFlags |= SLOT_USED;
    /// 更新快速查找位置为下一个槽位
    m_nRover = nSlot + 1;

    ::LeaveCriticalSection(&m_cs); /// 离开临界区

    return nSlot; /// 返回分配的槽位索引，调用者可以将模块句柄放入
}


/**
 * @brief 释放指定的线程槽位
 * @param nSlot 要释放的槽位索引
 */
void CThreadSlotData::FreeSlot(int nSlot)
{
    ::EnterCriticalSection(&m_cs); /// 进入临界区

    /// 遍历所有线程的数据，清理该槽位的数据
    CThreadData *pData = m_list.GetHead(); /// 取链表第一个线程数据
    while (pData != NULL)
    {
        /// 如果该线程使用了这个槽位
        if (nSlot < pData->nCount)
        {
            /// 删除槽位中的数据对象
            delete (CNoTrackObject *)pData->pData[nSlot];
            pData->pData[nSlot] = NULL; /// 置空指针
        }
        pData = pData->pNext; /// 移动到下一个线程数据
    }

    /// 标记此槽位为未使用，可供重新分配
    m_pSlotData[nSlot].dwFlags &= ~SLOT_USED;

    ::LeaveCriticalSection(&m_cs); /// 离开临界区
}


/**
 * @brief 获取当前线程在指定槽位的数据
 * @param nSlot 槽位索引
 * @return 槽位中的数据指针，如果不存在返回NULL
 */
void *CThreadSlotData::GetThreadValue(int nSlot)
{
    /// 从TLS中获取当前线程的私有数据
    CThreadData *pData = (CThreadData *)::TlsGetValue(m_tlsIndex);

    /// 如果线程数据不存在或槽位超出范围，返回NULL
    if (pData == NULL || nSlot >= pData->nCount)
    {
        return NULL;
    }

    /// 返回指定槽位的数据
    return pData->pData[nSlot];
}


/**
 * @brief 设置当前线程在指定槽位的数据
 * @param nSlot 槽位索引
 * @param pValue 要设置的数据指针
 */
void CThreadSlotData::SetValue(int nSlot, void *pValue)
{
    /// 从TLS中获取当前线程的私有数据
    CThreadData *pData = (CThreadData *)::TlsGetValue(m_tlsIndex);

    /// 如果线程数据不存在或槽位超出范围，并且要设置的值不为NULL，则需要分配空间
    if ((pData == NULL || nSlot >= pData->nCount) && pValue != NULL)
    {
        /// 如果线程数据不存在，创建新的线程数据
        if (pData == NULL)
        {
            pData         = new CThreadData; /// 创建新的线程数据对象
            pData->nCount = 0;               /// 初始槽位数量为0
            pData->pData  = NULL;            /// 初始数据指针为NULL

            /// 将新申请的线程数据添加到全局链表中（用于后续清理）
            ::EnterCriticalSection(&m_cs);
            m_list.AddHead(pData); /// 添加到链表头部
            ::LeaveCriticalSection(&m_cs);
        }

        /// 如果数据数组不存在，需要分配
        if (pData->pData == NULL)
        {
            /// 分配固定内存用于数据数组
            pData->pData = (void **)::GlobalAlloc(LMEM_FIXED, m_nMax * sizeof(LPVOID));
        }
        else
        {
            /// 重新分配更大的内存空间
            pData->pData = (void **)::GlobalReAlloc(pData->pData, m_nMax * sizeof(LPVOID), LMEM_MOVEABLE);
        }

        /// 将新扩展的内存空间初始化为0
        memset(pData->pData + pData->nCount, 0, (m_nMax - pData->nCount) * sizeof(LPVOID));

        /// 更新线程的槽位数量
        pData->nCount = m_nMax;
        /// 将线程数据保存回TLS
        ::TlsSetValue(m_tlsIndex, pData);
    }

    /// 设置指定槽位的数据
    pData->pData[nSlot] = pValue;
}


/**
 * @brief 删除指定模块或所有模块的线程数据
 * @param hInst 模块句柄，NULL表示所有模块
 * @param bAll 是否删除所有线程的数据，FALSE只删除当前线程
 */
void CThreadSlotData::DeleteValues(HINSTANCE hInst, BOOL bAll)
{
    ::EnterCriticalSection(&m_cs); /// 进入临界区

    if (!bAll)
    {
        /// 只删除当前线程的数据
        CThreadData *pData = (CThreadData *)::TlsGetValue(m_tlsIndex);
        if (pData != NULL)
        {
            DeleteValues(pData, hInst);
        }
    }
    else
    {
        /// 删除所有线程的数据
        CThreadData *pData = m_list.GetHead(); /// 从链表头开始
        while (pData != NULL)
        {
            CThreadData *pNextData = pData->pNext; /// 保存下一个节点
            DeleteValues(pData, hInst);            /// 删除当前线程数据
            pData = pNextData;                     /// 移动到下一个节点
        }
    }

    ::LeaveCriticalSection(&m_cs); /// 离开临界区
}


/**
 * @brief 删除指定线程数据对象中的值
 * @param pData 线程数据对象
 * @param hInst 模块句柄，NULL表示所有模块
 */
void CThreadSlotData::DeleteValues(CThreadData *pData, HINSTANCE hInst)
{
    BOOL bDelete = TRUE; /// 标记是否应该删除整个线程数据对象

    /// 遍历线程的所有槽位（从1开始，0保留）
    for (int i = 1; i < pData->nCount; i++)
    {
        /// 如果模块句柄匹配或hInst为NULL（删除所有）
        if (hInst == NULL || m_pSlotData[i].hInst == hInst)
        {
            /// 模块句柄匹配成功，可以删除该槽位的数据
            delete (CNoTrackObject *)pData->pData[i];
            pData->pData[i] = NULL; /// 置空指针
        }
        else
        {
            /// 如果该槽位仍有数据，则不能删除整个线程数据对象
            if (pData->pData[i] != NULL)
            {
                bDelete = FALSE;
            }
        }
    }

    /// 如果所有槽位都为空，删除整个线程数据对象
    if (bDelete)
    {
        ::EnterCriticalSection(&m_cs);
        m_list.Remove(pData); /// 从全局链表中移除
        ::LeaveCriticalSection(&m_cs);

        ::LocalFree(pData->pData); /// 释放数据数组内存
        delete pData;              /// 删除线程数据对象

        /// 清除TLS索引中的值
        ::TlsSetValue(m_tlsIndex, NULL);
    }
}


/**
 * @brief 析构函数，清理所有资源
 */
CThreadSlotData::~CThreadSlotData()
{
    /// 遍历所有线程数据对象并清理
    CThreadData *pData = m_list.GetHead(); /// 取链表头
    while (pData != NULL)
    {
        CThreadData *pDataNext = pData->pNext; /// 保存下一个节点
        DeleteValues(pData, NULL);             /// 删除当前线程的所有数据
        pData = pDataNext;                     /// 移动到下一个节点
    }

    /// 释放TLS索引
    if (m_tlsIndex != (DWORD)-1)
    {
        ::TlsFree(m_tlsIndex);
    }

    /// 释放槽位数据数组
    if (m_pSlotData != NULL)
    {
        HGLOBAL hSlotData = ::GlobalHandle(m_pSlotData);
        ::GlobalUnlock(hSlotData); /// 解锁内存
        ::GlobalFree(hSlotData);   /// 释放内存
    }

    /// 删除临界区
    ::DeleteCriticalSection(&m_cs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// 全局线程槽位数据管理对象
BYTE             afxThreadData[sizeof(CThreadSlotData)]; ///< 预分配的内存空间
CThreadSlotData *_afxThreadData = NULL;                  ///< 全局的线程槽位数据管理对象指针


/**
 * @brief 获取线程本地对象的数据
 * @param pfnCreateObject 创建对象的函数指针
 * @return 线程本地对象的指针
 */
CNoTrackObject *CThreadLocalObject::GetData(CNoTrackObject *(*pfnCreateObject)())
{
    /// 如果槽位还未分配，先分配槽位
    if (m_nSlot == 0)
    {
        /// 如果全局线程数据管理对象还未创建，先创建
        if (_afxThreadData == NULL)
        {
            /// 使用placement new在预分配内存上创建对象
            _afxThreadData = new(afxThreadData) CThreadSlotData;
            /// 分配一个新的槽位
            m_nSlot = _afxThreadData->AllocSlot();
        }
    }

    /// 获取当前线程在该槽位的数据
    CNoTrackObject *pValue = (CNoTrackObject *)_afxThreadData->GetThreadValue(m_nSlot);

    /// 如果数据不存在，创建新的对象
    if (pValue == NULL)
    {
        pValue = (*pfnCreateObject)(); /// 调用创建函数创建新对象
        /// 将新创建的对象保存到线程私有数据中
        _afxThreadData->SetValue(m_nSlot, pValue);
    }
    return pValue;
}


/**
 * @brief 获取线程本地对象的数据（不创建新对象）
 * @return 线程本地对象的指针，如果不存在返回NULL
 */
CNoTrackObject *CThreadLocalObject::GetDataNA()
{
    /// No Allocation - 不进行分配
    if (m_nSlot == 0 || _afxThreadData == NULL)
    {
        return NULL;
    }
    return (CNoTrackObject *)_afxThreadData->GetThreadValue(m_nSlot);
}

/**
 * @brief 析构函数，释放槽位资源
 */
CThreadLocalObject::~CThreadLocalObject()
{
    /// 如果槽位已分配且全局管理对象存在，释放槽位
    if (m_nSlot != 0 && _afxThreadData != NULL)
    {
        _afxThreadData->FreeSlot(m_nSlot);
        m_nSlot = 0; /// 重置槽位索引
    }
}

///////////////////////////////////////////////////////////////////////////////
