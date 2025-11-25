#ifndef __AFXTLS_H
#define __AFXTLS_H

#include <Windows.h>

class CSimpleList
{
public:
    CSimpleList(int nNextOffset = 0);

    void Construct(int nNextOffset);
    /// 提供一个对外的接口，供其他对象添加、删除，遍历节点
    BOOL IsEmpty() const;

    void AddHead(void* p);

    void RemoveAll();

    void* GetHead() const;

    void* GetNext(void* p) const;

    BOOL Remove(void* p);

    void*  m_pHead       = NULL; ///< 链表中的头指针
    size_t m_nNextOffset = NULL; ///< 数据结构中pNext的偏移量
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

//////////////////////////////////////////////////////////////////

class CNoTrackObject
{
public:
    void* operator new(size_t nSize);

    void operator delete(void* p);

    virtual ~CNoTrackObject()
    {
    }
};

//////////////////////////////////////////////////////////////////

/// 仿造MFC工作者线程的封装技巧，管理mfc应用程序的线程局部存储技术，使得MFC可以推进HINSTANCE的实例
struct CSlotData;
struct CThreadData;

/// 这个结构相当于MFC中，帮助维护线程的数据结构，相当于统一管理的每一个同学的小纸条
class CThreadSlotData
{
public:
    CThreadSlotData();

    /// \brief 线程局部存储(TLS)槽位分配函数
    /// \return
    int AllocSlot();

    /// \brief 线程局部存储(TLS)槽位释放函数
    /// \param nSlot
    void FreeSlot(int nSlot);

    void* GetThreadValue(int nSlot);

    void SetValue(int nSlot, void* pValue);

    void DeleteValues(HINSTANCE hInst, BOOL bAll = FALSE);

    DWORD m_tlsIndex = -1; ///< 基于TLS进行管理

    int m_nAlloc = 0; ///<  m_pSlotData所指向的数组的大小
    int m_nRover = 1; ///< 为了能够快速找到空闲的槽位而设定的值
    int m_nMax   = 0; ///< pData所指向的数组的大小

    CSlotData*                     m_pSlotData = NULL;
    CTypedSimpleList<CThreadData*> m_list;
    CRITICAL_SECTION               m_cs;

    void* operator new(size_t, void* p)
    {
        return p;
    }

    /// \brief 删除线程数据
    /// \param pData
    /// \param hInst 模块句柄
    void DeleteValues(CThreadData* pData, HINSTANCE hInst);

    ~CThreadSlotData();
};

//////////////////////////////////////////////////////////////////
class CThreadLocalObject
{
public:
    /// \brief 获取或创建数据
    /// \param pfnCreateObject
    /// \return
    CNoTrackObject* GetData(CNoTrackObject* (*pfnCreateObject)());

    /// \brief 仅获取数据（不创建）
    /// \return
    CNoTrackObject* GetDataNA();

    DWORD m_nSlot;

    ~CThreadLocalObject();
};

//////////////////////////////////////////////////////////////////

template <class TYPE>
class CThreadLocal : public CThreadLocalObject
{
public:
    TYPE* GetData()
    {
        TYPE* pData = (TYPE*)CThreadLocalObject::GetData(&CreateObject);
        return pData;
    }

    TYPE* GetDataNA()
    {
        TYPE* pData = (TYPE*)CThreadLocalObject::GetDataNA();
        return pData;
    }

    operator TYPE*()
    { /// 赋值就是获取数据
        return GetData();
    }

    TYPE* operator->()
    {
        return GetData();
    }

public:
    static CNoTrackObject* CreateObject()
    {
        return new TYPE;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////

#define THREAD_LOCAL(class_name, ident_name)        CThreadLocal<class_name> ident_name;
#define EXTERN_THREAD_LOCAL(class_name, ident_name) extern THREAD_LOCAL(class_name, ident_name)

#endif // !__AFXTLS_H
