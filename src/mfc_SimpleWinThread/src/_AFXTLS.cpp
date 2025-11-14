#include "_AFXTLS.h"

#include <cstdio>

void CSimpleList::AddHead(void *p)
{
    *GetNextPtr(p) = m_pHead;
    m_pHead        = p;
}

BOOL CSimpleList::Remove(void *p)
{
    if (p == NULL)
        return FALSE;
    /// 假定移除失败
    BOOL bResult = FALSE;
    if (p == m_pHead)
    {
        m_pHead = *GetNextPtr(p);
        bResult = TRUE;
    }
    else
    {
        void *pText = m_pHead;
        while (pText != NULL && *GetNextPtr(pText) != p)
        {
            pText = *GetNextPtr(pText);
        }
        if (pText != NULL)
        {
            *GetNextPtr(pText) = *GetNextPtr(p);
            bResult            = TRUE;
        }
    }
    return bResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
void *CNoTrackObject::operator new(size_t nSize)
{
    /// 申请一块GMEM-FIXED和GMEM_ZEROPOINT的内存，Windows操作系统直接分配的内存，
    void *p = ::GlobalAlloc(GPTR, nSize); /// GMEM_FIXED | GMEM_ZEROINIT
    /// 验证一下，内存分配
    // printf("%s\n", __FUNCTION__);
    return p;
}

void CNoTrackObject::operator delete(void *p)
{
    if (p != NULL)
    {
        ::GlobalFree(p);
        // printf("%s\n", __FUNCTION__);
    }
}

///////////////////////////////////////////////////////////////////////////////////

struct CSlotData
{
    DWORD     dwFlags; ///< 槽有没有被使用
    HINSTANCE hInst;   ///< 模块句柄
};

struct CThreadData : public CNoTrackObject
{
    CThreadData *pNext  = nullptr;
    int          nCount = 0;
    LPVOID      *pData  = nullptr;
};

#define SLOT_USED 0x01

CThreadSlotData::CThreadSlotData()
{
    m_list.Construct(offsetof(CThreadData, pNext));

    m_nMax      = 0;
    m_nAlloc    = 0;
    m_nRover    = 1;
    m_pSlotData = NULL;

    m_tlsIndex = ::TlsAlloc();
    ::InitializeCriticalSection(&m_cs);
}

int CThreadSlotData::AllocSlot()
{
    ::EnterCriticalSection(&m_cs);
    int nAlloc = m_nAlloc;
    int nSlot  = m_nRover; /// m_nRover快速定位的
    if (nSlot >= nAlloc || m_pSlotData[nSlot].dwFlags & SLOT_USED)
    {
        for (nSlot = 1; nSlot < nAlloc && m_pSlotData[nSlot].dwFlags & SLOT_USED; nSlot++)
        {
        }
        /// 当槽位不足时，每次增加32个槽位
        int     nNewAlloc = nAlloc + 32;
        HGLOBAL hSlotData;
        if (m_pSlotData == NULL)
        {
            hSlotData = ::GlobalAlloc(GMEM_MOVEABLE, nNewAlloc * sizeof(CSlotData));
        }
        else
        {
            /// 扩容
            hSlotData = ::GlobalHandle(m_pSlotData);
            ::GlobalUnlock(hSlotData);
            hSlotData = ::GlobalReAlloc(hSlotData, nNewAlloc * sizeof(CSlotData), GMEM_MOVEABLE);
        }
        CSlotData *pSlotData = (CSlotData *)::GlobalLock(hSlotData);

        /// 将我们得到的新空间初始化为0
        memset(pSlotData + m_nAlloc, 0, (nNewAlloc - nAlloc) * sizeof(CSlotData));
        m_nAlloc    = nNewAlloc;
        m_pSlotData = pSlotData;
    }

    /// 调整一下当前对象的nMax值
    if (nSlot >= m_nMax)
    {
        m_nMax = nSlot + 1;
    }

    m_pSlotData[nSlot].dwFlags |= SLOT_USED;
    m_nRover = nSlot + 1;
    ::LeaveCriticalSection(&m_cs);

    return nSlot; /// 槽号已经获得，可以将模块句柄放入
}

void CThreadSlotData::FreeSlot(int nSlot)
{
    ::EnterCriticalSection(&m_cs);
    CThreadData *pData = m_list; /// 取链表第一个
    while (pData != NULL)
    {
        if (nSlot < pData->nCount)
        {
            delete (CNoTrackObject *)pData->pData[nSlot];
            pData->pData[nSlot] = NULL;
        }
        pData = pData->pNext;
    }

    /// 标记此槽号可以使用
    m_pSlotData[nSlot].dwFlags &= ~SLOT_USED;
    ::LeaveCriticalSection(&m_cs);
}

void *CThreadSlotData::GetThreadValue(int nSlot)
{
    CThreadData *pData = (CThreadData *)::TlsGetValue(m_tlsIndex);
    if (pData == NULL || nSlot >= pData->nCount)
    {
        return NULL;
    }

    return pData->pData[nSlot];
}

void CThreadSlotData::SetValue(int nSlot, void *pValue)
{
    CThreadData *pData = (CThreadData *)::TlsGetValue(m_tlsIndex);

    /// 为线程私有数据申请空间

    if ((pData == NULL || nSlot >= pData->nCount) && pValue != NULL)
    {
        if (pData == NULL)
        {
            pData         = new CThreadData;
            pData->nCount = 0;
            pData->pData  = NULL;

            /// 将新申请的内存添加到全局列表中
            ::EnterCriticalSection(&m_cs);
            m_list.AddHead(pData);
            ::LeaveCriticalSection(&m_cs);
        }

        if (pData->pData == NULL)
        {
            pData->pData = (void **)::GlobalAlloc(LMEM_FIXED, m_nMax * sizeof(LPVOID));
        }
        else
        {
            pData->pData = (void **)::GlobalReAlloc(pData->pData, m_nMax * sizeof(LPVOID), LMEM_MOVEABLE);
        }

        memset(pData->pData + pData->nCount, 0, (m_nMax - pData->nCount) * sizeof(LPVOID));

        pData->nCount = m_nMax;
        ::TlsSetValue(m_tlsIndex, pData);
    }
    pData->pData[nSlot] = pValue;
}

void CThreadSlotData::DeleteValues(HINSTANCE hInst, BOOL bAll)
{
    ::EnterCriticalSection(&m_cs);
    if (!bAll)
    {
        CThreadData *pData = (CThreadData *)::TlsGetValue(m_tlsIndex);
        if (pData != NULL)
        {
            DeleteValues(pData, hInst);
        }
    }
    else
    {
        CThreadData *pData = m_list.GetHead();
        while (pData != NULL)
        {
            CThreadData *pNextData = pData->pNext;
            DeleteValues(pData, hInst);
            pData = pNextData;
        }
    }
    ::LeaveCriticalSection(&m_cs);
}

void CThreadSlotData::DeleteValues(CThreadData *pData, HINSTANCE hInst)
{
    BOOL bDelete = TRUE;
    for (int i = 1; i < pData->nCount; i++)
    {
        if (hInst == NULL || m_pSlotData[i].hInst == hInst)
        {
            /// 模块句柄匹配成功，可以删除
            delete (CNoTrackObject *)pData->pData[i];
            pData->pData[i] = NULL;
        }
        else
        {
            if (pData->pData[i] != NULL)
            {
                bDelete = FALSE;
            }
        }
    }
    if (bDelete)
    {
        ::EnterCriticalSection(&m_cs);
        m_list.Remove(pData);
        ::LeaveCriticalSection(&m_cs);
        ::LocalFree(pData->pData);
        delete pData;

        /// 清除TLS索引
        ::TlsSetValue(m_tlsIndex, NULL);
    }
}

CThreadSlotData::~CThreadSlotData()
{
    CThreadData *pData = m_list; /// 取链表头
    while (pData != NULL)
    {
        CThreadData *pDataNext = pData->pNext;
        DeleteValues(pData, NULL);
        pData = pData->pNext;
    }
    if (m_tlsIndex != (DWORD)-1)
    {
        ::TlsFree(m_tlsIndex);
    }

    if (m_pSlotData != NULL)
    {
        HGLOBAL hSlotData = ::GLOBALHANDLE(m_pSlotData);
        ::GlobalUnlock(hSlotData);
        ::GlobalFree(m_pSlotData);
    }
    ::DeleteCriticalSection(&m_cs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// CThreadSlotData
BYTE             afxThreadData[sizeof(CThreadSlotData)];
CThreadSlotData *_afxThreadData; /// 定义全局的变量，进行内存分配

CNoTrackObject *CThreadLocalObject::GetData(CNoTrackObject *(*pfnCreateObject)())
{
    if (m_nSlot == 0)
    {
        if (_afxThreadData == NULL)
        {
            _afxThreadData = new (afxThreadData) CThreadSlotData;
            m_nSlot        = _afxThreadData->AllocSlot();
        }
    }

    CNoTrackObject *pValue = (CNoTrackObject *)_afxThreadData->GetThreadValue(m_nSlot);
    if (pValue == NULL)
    {
        pValue = (*pfnCreateObject)();
        /// 线程私有数据保存新创建的对象
        _afxThreadData->SetValue(m_nSlot, pValue);
    }
    return pValue;
}

CNoTrackObject *CThreadLocalObject::GetDataNA()
{
    /// No Allocation
    if (m_nSlot == 0 || _afxThreadData == 0)
    {
        return NULL;
    }
    return (CNoTrackObject *)_afxThreadData->GetThreadValue(m_nSlot);
}

CThreadLocalObject::~CThreadLocalObject()
{
    if (m_nSlot != 0 && _afxThreadData != NULL)
    {
        _afxThreadData->FreeSlot(m_nSlot);
        m_nSlot = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
