#include "_AFXTLS.h"

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
