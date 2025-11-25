#include "_afxColl_.h"

#include <iomanip>
#include <iostream>


/// 构造函数
CMapPtrToPtr::CMapPtrToPtr(int nBlockSize)
{
    m_pHashTable     = NULL;       /// 哈希表指针，初始为NULL
    m_nHashTableSize = 17;         /// 初始哈希表大小，选择质数以减少哈希冲突
    m_pBlocks        = NULL;       /// 内存块链表头指针
    m_nBlockSize     = nBlockSize; /// 每个内存块包含的关联结构数量
    m_nCount         = 0;          /// 当前存储的键值对数量
    m_pFreeList      = NULL;       /// 空闲关联结构链表
}

/// 查找键对应的值
BOOL CMapPtrToPtr::Lookup(void* key, void*& rValue)
{
    UINT nHash;
    /// 获取指定键的关联结构
    CAssoc* pAssoc = GetAssocAt(key, nHash);
    if (pAssoc == NULL)
    {
        return FALSE; /// 未找到键
    }

    rValue = pAssoc->value; /// 通过引用返回找到的值
    return TRUE;
}

/// 重载下标操作符，用于访问或插入键值对
void*& CMapPtrToPtr::operator[](void* key)
{
    UINT    nHash;
    CAssoc* pAssoc;

    /// 尝试查找已存在的键
    if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
    {
        /// 键不存在，需要创建新的关联

        /// 如果哈希表未初始化，先初始化
        if (m_pHashTable == NULL)
        {
            InitHashTable(m_nHashTableSize);
        }

        /// 创建新的关联结构
        pAssoc      = NewAssoc();
        pAssoc->key = key; /// 设置键

        /// 把新关联插入到哈希表的对应桶中（链地址法解决冲突）
        /// 新节点插入到链表头部
        pAssoc->pNext       = m_pHashTable[nHash];
        m_pHashTable[nHash] = pAssoc;
    }

    /// 返回值的引用，允许直接修改
    return pAssoc->value;
}

/// 移除指定键的关联
BOOL CMapPtrToPtr::RemoveKey(void* key)
{
    if (m_pHashTable == NULL)
    {
        return FALSE; /// 哈希表未初始化，直接返回失败
    }

    /// 计算哈希值，找到对应的桶
    CAssoc** ppAssocPre;
    ppAssocPre = &m_pHashTable[HashKey(key) % m_nHashTableSize];

    CAssoc* pAssoc;
    /// 遍历链表查找要删除的节点
    for (pAssoc = *ppAssocPre; pAssoc != NULL; pAssoc = pAssoc->pNext)
    {
        if (pAssoc->key == key)
        {
            /// 从链表中移除节点
            *ppAssocPre = pAssoc->pNext; /// 前驱节点指向后继节点

            FreeAssoc(pAssoc); /// 释放关联结构
            return TRUE;       /// 成功删除
        }
        ppAssocPre = &pAssoc->pNext; /// 移动到下一个节点的指针位置
    }
    return FALSE; /// 未找到要删除的键
}

/// 移除所有关联
void CMapPtrToPtr::RemoveAll()
{
    /// 释放哈希表数组
    if (m_pHashTable != NULL)
    {
        delete[] m_pHashTable;
        m_pHashTable = NULL;
    }

    /// 重置所有状态
    m_nCount    = 0;
    m_pFreeList = NULL;

    /// 释放所有内存块
    if (m_pBlocks != NULL)
    {
        m_pBlocks->FreeDataChain();
        m_pBlocks = NULL;
    }
}

/// 初始化哈希表
void CMapPtrToPtr::InitHashTable(UINT nHashSize, BOOL bAllocNow)
{
    /// 如果已有哈希表，先释放
    if (m_pHashTable != NULL)
    {
        delete[] m_pHashTable;
        m_pHashTable = NULL;
    }

    if (bAllocNow)
    {
        /// 申请哈希表数组空间
        m_pHashTable = new CAssoc*[nHashSize];
        /// 初始化所有桶为空
        memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
    }
    m_nHashTableSize = nHashSize; /// 更新哈希表大小
}

/// 哈希函数：将指针转换为哈希值
UINT CMapPtrToPtr::HashKey(void* key) const
{
    /// 将指针右移4位作为哈希值
    /// 这样可以忽略指针的低位对齐信息，提高分布性
    return ((UINT)(void*)(DWORD)key) >> 4;
}

/// 创建新的关联结构
CMapPtrToPtr::CAssoc* CMapPtrToPtr::NewAssoc()
{
    /// 检查空闲链表是否为空
    if (m_pFreeList == NULL)
    {
        /// 使用CPlex分配新的内存块
        /// 创建包含m_nBlockSize个CAssoc结构的内存块
        CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CAssoc));

        /// 获取内存块中的数据区域
        CAssoc* pAssoc = (CAssoc*)newBlock->data();

        /// 移动到内存块中最后一个CAssoc结构
        pAssoc += m_nBlockSize - 1;

        /// 从后往前将所有的CAssoc结构添加到空闲链表
        for (int i = m_nBlockSize - 1; i >= 0; i--, pAssoc--)
        {
            pAssoc->pNext = m_pFreeList; /// 当前节点指向原空闲链表头
            m_pFreeList   = pAssoc;      /// 更新空闲链表头为当前节点
        }
    }

    /// 从空闲链表中取出一个关联结构
    CAssoc* pAssoc = m_pFreeList;        /// 获取空闲链表头
    m_pFreeList    = m_pFreeList->pNext; /// 空闲链表指向下一个节点
    m_nCount++;                          /// 增加元素计数

    /// 初始化关联结构的字段
    pAssoc->key   = 0;
    pAssoc->value = 0;

    return pAssoc;
}

/// 释放关联结构
void CMapPtrToPtr::FreeAssoc(CAssoc* pAssoc)
{
    /// 将释放的关联结构添加到空闲链表头部
    pAssoc->pNext = m_pFreeList; /// 当前节点指向原空闲链表
    m_pFreeList   = pAssoc;      /// 更新空闲链表头

    m_nCount--; /// 减少元素计数

    /// 如果容器为空，完全清理所有资源
    if (m_nCount == 0)
    {
        RemoveAll();
    }
}

/// 根据键获取关联结构
CMapPtrToPtr::CAssoc* CMapPtrToPtr::GetAssocAt(void* key, UINT& nHashs) const
{
    /// 计算键的哈希值
    nHashs = HashKey(key) % m_nHashTableSize;

    if (m_pHashTable == NULL)
    {
        return NULL; /// 哈希表未初始化
    }

    /// 在对应的哈希桶中线性查找
    CAssoc* pAssoc = m_pHashTable[nHashs];
    while (pAssoc != NULL)
    {
        /// 比较键值（指针比较）
        if (pAssoc->key == key)
        {
            return pAssoc; /// 找到匹配的键
        }
        pAssoc = pAssoc->pNext; /// 移动到链表下一个节点
    }

    return NULL; /// 未找到
}


void CMapPtrToPtr::PrintAsciiArt() const
{
    std::cout << "\n=== Hash Table Debug View ===\n";

    for (UINT i = 0; i < m_nHashTableSize; i++)
    {
        std::cout << "[" << i << "]: ";

        CAssoc* pAssoc = m_pHashTable[i];
        if (pAssoc == NULL)
        {
            std::cout << "NULL\n";
            continue;
        }

        while (pAssoc != NULL)
        {
            const char* keyStr = static_cast<const char*>(pAssoc->key);
            const char* valStr = static_cast<const char*>(pAssoc->value);
            std::cout << "{" << keyStr << ":" << valStr << "}";

            if (pAssoc->pNext != NULL)
            {
                std::cout << " -> ";
            }
            pAssoc = pAssoc->pNext;
        }
        std::cout << "\n";
    }

    std::cout << "--- Summary ---\n";
    std::cout << "Size: " << m_nHashTableSize << " | Count: " << m_nCount << " | Load: " << std::setprecision(3)
              << (float)m_nCount / m_nHashTableSize << "\n\n";
}

/// 析构函数
CMapPtrToPtr::~CMapPtrToPtr()
{
    RemoveAll(); /// 清理所有资源
}
