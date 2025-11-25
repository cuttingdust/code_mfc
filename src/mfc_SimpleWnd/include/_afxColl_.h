#ifndef _AFXCOLL__H
#define _AFXCOLL__H

#include "_afxplex_.h"

class CMapPtrToPtr
{
    /// 维护m_hWnd到Cwnd的结构
protected:
    struct CAssoc
    {
        CAssoc* pNext; /// 指向下一个节点的指针（链表结构）
        void*   key;   ///  键（指针类型）
        void*   value; /// 值（指针类型）
    };

public:
    CMapPtrToPtr(int nBlockSize = 10);

    /// 属性
    int  GetCount() const;
    BOOL IsEmpty() const;
    /// 查找
    BOOL Lookup(void* key, void*& rValue);

    /// 操作
    /// 对添加key生成对应的value
    void*& operator[](void* key);

    /// 添加
    void SetAt(void* key, void* newValue);

    ///移除
    BOOL RemoveKey(void* key);
    void RemoveAll();

    /// 希望，我们的查询尽可能的快，因此，我们在这个类中，介绍Hash
    UINT GetHashTableSize() const;
    void InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE);
    UINT HashKey(void* key) const;

    /// 要有好用new
    CAssoc* NewAssoc();
    /// 要有好用delete
    void FreeAssoc(CAssoc* pAssoce);
    /// 根据key找到对应的CAssoc的value
    CAssoc* GetAssocAt(void* key, UINT& nHashs) const;

    void PrintAsciiArt() const;

public:
    ~CMapPtrToPtr();

protected:
    /// m_pHashTable(CAssoc** 类型)
    /// │
    /// ├── [0] → NULL
    /// ├── [1] → CAssoc → CAssoc → NULL
    /// ├── [2] → NULL
    /// ├── [3] → CAssoc → NULL
    /// ├── [4] → NULL
    /// ├── [5] → CAssoc → CAssoc → CAssoc → NULL
    /// ├── ...
    /// └── [16] → NULL

    CAssoc** m_pHashTable; /// 我们需要维护一个HashTable

    /// 形成拉链法查找CAssoc
    int m_nHashTableSize;

    /// 用Cpelx串联CAssoc
    struct CPlex* m_pBlocks;    /// m_pBlocks是CPlex的首地址
    int           m_nBlockSize; /// 当前分配空间中可以有多少个CAssoc结构
    CAssoc*       m_pFreeList;  /// 预留空间中有多少没有被使用的CAssoc结构的头指针
    int           m_nCount;     /// 我们用来记录程序中一共使用了多少个CAssoc
};

inline int CMapPtrToPtr::GetCount() const
{
    return m_nCount;
}


inline BOOL CMapPtrToPtr::IsEmpty() const
{
    return m_nCount == 0;
}


inline void CMapPtrToPtr::SetAt(void* key, void* newValue)
{
    (*this)[key] = newValue;
}


inline UINT CMapPtrToPtr::GetHashTableSize() const
{
    return m_nHashTableSize;
}
//////////////////////////////////////////////////////////////////


#endif // _AFXCOLL__H
