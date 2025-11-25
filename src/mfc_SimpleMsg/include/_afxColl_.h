#ifndef _AFXCOLL__H
#define _AFXCOLL__H

#include "_afxplex_.h"

/**
 * @class CMapPtrToPtr
 * @brief MFC 指针到指针的映射表类
 * 
 * 该类实现了基于哈希表的指针到指针的映射，使用链地址法解决哈希冲突。
 * 这是 MFC 集合类库的核心组件，用于高效存储和检索键值对。
 * 
 * 主要特性：
 * 1. 基于哈希表的快速查找
 * 2. 链地址法解决哈希冲突
 * 3. 内存池管理提高分配效率
 * 4. 动态哈希表扩容支持
 * 
 * @note 常用于 MFC 内部的句柄映射（如 HWND 到 CWnd* 的映射）
 */
class CMapPtrToPtr
{
    /// 维护 m_hWnd 到 CWnd 的结构映射
protected:
    /**
     * @struct CAssoc
     * @brief 哈希表节点结构，存储键值对和链表信息
     * 
     * 每个 CAssoc 节点包含一个键值对，并通过链表连接解决哈希冲突。
     */
    struct CAssoc
    {
        CAssoc* pNext; ///< 指向下一个节点的指针（链表结构，用于解决哈希冲突）
        void*   key;   ///< 键（指针类型），通常是 Windows 句柄如 HWND
        void*   value; ///< 值（指针类型），通常是对应的 MFC 对象指针如 CWnd*
    };

public:
    /**
     * @brief 构造函数
     * @param nBlockSize 内存块大小，指定每次分配多少个 CAssoc 节点
     * 
     * 初始化哈希表和相关成员变量，设置内存池的块大小。
     */
    CMapPtrToPtr(int nBlockSize = 10);

    /// @name 属性访问
    /// @{
    /**
     * @brief 获取映射表中元素的数量
     * @return 当前存储的键值对数量
     */
    int GetCount() const;

    /**
     * @brief 检查映射表是否为空
     * @return 如果为空返回 TRUE，否则返回 FALSE
     */
    BOOL IsEmpty() const;
    /// @}

    /// @name 查找操作
    /// @{
    /**
     * @brief 查找指定键对应的值
     * @param key 要查找的键
     * @param rValue 用于返回找到的值的引用
     * @return 如果找到返回 TRUE，否则返回 FALSE
     * 
     * 通过哈希表快速定位键对应的值，使用链地址法处理哈希冲突。
     */
    BOOL Lookup(void* key, void*& rValue);
    /// @}

    /// @name 操作函数
    /// @{
    /**
     * @brief 下标运算符，用于访问或插入键值对
     * @param key 要访问的键
     * @return 返回对应值的引用
     * 
     * 如果键不存在，会自动创建新的键值对。
     * 这是实现映射表类字典式访问的关键操作。
     */
    void*& operator[](void* key);

    /**
     * @brief 设置键值对
     * @param key 要设置的键
     * @param newValue 要设置的值
     * 
     * 如果键已存在，则更新对应的值；如果不存在，则插入新的键值对。
     */
    void SetAt(void* key, void* newValue);

    /**
     * @brief 移除指定键的映射
     * @param key 要移除的键
     * @return 如果成功移除返回 TRUE，如果键不存在返回 FALSE
     * 
     * 从哈希表中移除指定的键值对，并释放对应的节点内存。
     */
    BOOL RemoveKey(void* key);

    /**
     * @brief 移除所有映射关系
     * 
     * 清空整个哈希表，释放所有节点内存，重置为初始状态。
     */
    void RemoveAll();
    /// @}

    /// @name 哈希表管理
    /// @{
    /**
     * @brief 获取哈希表的大小
     * @return 哈希表的桶数量
     */
    UINT GetHashTableSize() const;

    /**
     * @brief 初始化哈希表
     * @param nHashSize 哈希表大小（桶的数量）
     * @param bAllocNow 是否立即分配内存
     * 
     * 设置哈希表的大小，可以选择立即分配内存或延迟分配。
     * 较大的哈希表可以减少冲突，提高查找效率。
     */
    void InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE);

    /**
     * @brief 计算键的哈希值
     * @param key 要计算哈希值的键
     * @return 计算得到的哈希值（在哈希表大小范围内）
     * 
     * 将指针地址转换为哈希值，用于确定键在哈希表中的位置。
     */
    UINT HashKey(void* key) const;
    /// @}

    /// @name 内存管理
    /// @{
    /**
     * @brief 分配新的关联节点
     * @return 新分配的 CAssoc 节点指针
     * 
     * 从内存池中分配一个新的节点，如果内存池为空则分配新的内存块。
     */
    CAssoc* NewAssoc();

    /**
     * @brief 释放关联节点
     * @param pAssoc 要释放的节点指针
     * 
     * 将节点返回到空闲链表，供后续重用，提高内存使用效率。
     */
    void FreeAssoc(CAssoc* pAssoc);

    /**
     * @brief 根据键查找对应的关联节点
     * @param key 要查找的键
     * @param nHash 返回计算得到的哈希值
     * @return 找到的关联节点指针，如果未找到返回 NULL
     * 
     * 在哈希表中查找指定键对应的节点，包括遍历链表处理冲突。
     */
    CAssoc* GetAssocAt(void* key, UINT& nHash) const;
    /// @}

    /**
     * @brief 打印哈希表的 ASCII 艺术图（调试用）
     * 
     * 可视化显示哈希表的结构，包括每个桶的链表长度，用于调试和分析性能。
     */
    void PrintAsciiArt() const;

public:
    /**
     * @brief 析构函数
     * 
     * 释放所有分配的内存，包括哈希表、内存块和所有节点。
     */
    ~CMapPtrToPtr();

protected:
    /**
     * @brief 哈希表结构示意图
     * 
     * m_pHashTable (CAssoc** 类型)
     * │
     * ├── [0] → NULL
     * ├── [1] → CAssoc → CAssoc → NULL        (哈希冲突，链表长度为2)
     * ├── [2] → NULL
     * ├── [3] → CAssoc → NULL                 (链表长度为1)
     * ├── [4] → NULL
     * ├── [5] → CAssoc → CAssoc → CAssoc → NULL (哈希冲突，链表长度为3)
     * ├── ...
     * └── [nHashTableSize-1] → NULL
     * 
     * 使用链地址法解决哈希冲突，每个桶是一个 CAssoc 链表。
     */

    CAssoc** m_pHashTable; ///< 哈希表指针数组，每个元素指向一个 CAssoc 链表

    int m_nHashTableSize; ///< 哈希表大小（桶的数量），影响查找性能和内存使用

    /// @name 内存池管理
    /// @{
    struct CPlex* m_pBlocks;    ///< 内存块链表的首地址，用于批量分配 CAssoc 节点
    int           m_nBlockSize; ///< 每个内存块中包含的 CAssoc 节点数量
    CAssoc*       m_pFreeList;  ///< 空闲节点链表的头指针，用于快速分配可用节点
    /// @}

    int m_nCount; ///< 当前映射表中存储的键值对数量
};

//////////////////////////////////////////////////////////////////
/// 内联函数实现
//////////////////////////////////////////////////////////////////

/**
 * @brief 内联实现：获取映射表中元素的数量
 * 
 * 直接返回内部计数器，时间复杂度 O(1)。
 * 
 * @return 当前存储的键值对数量
 */
inline int CMapPtrToPtr::GetCount() const
{
    return m_nCount;
}

/**
 * @brief 内联实现：检查映射表是否为空
 * 
 * 通过检查计数器是否为 0 来判断是否为空。
 * 
 * @return 如果为空返回 TRUE，否则返回 FALSE
 */
inline BOOL CMapPtrToPtr::IsEmpty() const
{
    return m_nCount == 0;
}

/**
 * @brief 内联实现：设置键值对
 * 
 * 通过下标运算符实现设置操作，提供更简洁的接口。
 * 
 * @param key 要设置的键
 * @param newValue 要设置的值
 */
inline void CMapPtrToPtr::SetAt(void* key, void* newValue)
{
    (*this)[key] = newValue; ///< 委托给 operator[] 实现
}

/**
 * @brief 内联实现：获取哈希表的大小
 * 
 * 返回哈希表的桶数量，用于分析哈希表的负载因子。
 * 
 * @return 哈希表的桶数量
 */
inline UINT CMapPtrToPtr::GetHashTableSize() const
{
    return m_nHashTableSize;
}

//////////////////////////////////////////////////////////////////

#endif // _AFXCOLL__H
