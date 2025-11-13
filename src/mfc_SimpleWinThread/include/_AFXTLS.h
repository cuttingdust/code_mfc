#ifndef __AFXTLS_H
#define __AFXTLS_H

#include <Windows.h>

#include <stddef.h>

#define VERSION 0xFFFF

class CSimpleList
{
public:
    CSimpleList(int nNextOffset = 0);
    void Construct(int nNextOffset);
    /// 提供一个对外的接口，供其他对象添加、删除，遍历节点
    BOOL  IsEmpty() const;
    void  AddHead(void* p);
    void  RemoveAll();
    void* GetHead() const;
    void* GetNext(void* p) const;
    BOOL  Remove(void* p);

    void*  m_pHead;       ///< 链表中的头指针
    size_t m_nNextOffset; ///< 数据结构中pNext的偏移量
    void** GetNextPtr(void* p) const;
};

inline CSimpleList::CSimpleList(int nNextOffset)
{
    m_pHead       = NULL;
    m_nNextOffset = nNextOffset;
}

inline void CSimpleList::Construct(int nNextOffset)
{
    m_nNextOffset = nNextOffset;
}

inline BOOL CSimpleList::IsEmpty() const
{
    return m_pHead == NULL;
}

inline void CSimpleList::RemoveAll()
{
    m_pHead = NULL;
}

inline void* CSimpleList::GetHead() const
{
    return m_pHead;
}

inline void* CSimpleList::GetNext(void* preElement) const
{
    return *GetNextPtr(preElement);
}

inline void** CSimpleList::GetNextPtr(void* p) const
{
    return (void**)((BYTE*)p + m_nNextOffset);
}

/////////////////////////////////////////////////////////////////////////////////
template <class TYPE>
class CTypedSimpleList : public CSimpleList /// 模板原则 组合大于继承  除了类型检查 就是语言限制派生
{
public:
    CTypedSimpleList(int nNextOffset = 0) : CSimpleList(nNextOffset)
    {
    }
    void AddHead(TYPE p)
    {
        CSimpleList::AddHead((void*)p);
    }
    TYPE GetHead()
    {
        return (TYPE)CSimpleList::GetHead();
    }
    TYPE GetNext(TYPE p)
    {
        return (TYPE)CSimpleList::GetNext(p);
    }
    BOOL Remove(TYPE p)
    {
        return CSimpleList::Remove(p);
    }
    operator TYPE()
    {
        return (TYPE)CSimpleList::GetHead();
    }
}; /// 使用CTypedSimplelist将简单链表泛型化


#endif // !__AFXTLS_H
