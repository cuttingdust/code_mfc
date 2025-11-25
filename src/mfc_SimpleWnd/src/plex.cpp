#include "_afxplex_.h"


/**
 * @brief 创建新的 CPlex 内存块并插入链表头部
 * 
 * @param pHead    [in/out] 内存块链表的头指针引用，新创建的内存块将成为新的头节点
 * @param nMax     [in] 当前内存块能够容纳的最大元素数量
 * @param cbElement [in] 每个元素的大小（字节数）
 * @return CPlex*  返回新创建的内存块指针
 * 
 * @details
 * 这个函数执行以下操作：
 * 1. 分配一块连续内存，包含 CPlex 头信息和数据区
 * 2. 将新内存块插入到链表头部（前插法）
 * 3. 更新链表头指针指向新创建的内存块
 * 
 * 内存布局示意图：
 * ┌─────────────────────────────────────────────┐
 * │ CPlex 头信息                                         │        数据区（nMax个元素）      │
 * │  sizeof(CPlex)                                       │         nMax * cbElement 字节    │
 * └─────────────────────────────────────────────┘
 * 
 * 链表变化示意图：
 * 调用前： pHead → [块A] → [块B] → ... → NULL
 * 调用后： pHead → [新块] → [块A] → [块B] → ... → NULL
 */
CPlex* CPlex::Create(CPlex*& pHead, UINT nMax, UINT cbElement)
{
    /// 计算总内存大小：头信息大小 + 数据区大小
    /// 使用 BYTE 数组确保内存对齐和正确的大小计算
    CPlex* p = (CPlex*)new BYTE[sizeof(CPlex) + nMax * cbElement];

    /// 将新创建的内存块插入链表头部（前插法）
    /// 新块的 next 指针指向原来的头节点
    p->pNext = pHead;
    /// 更新链表头指针，使其指向新创建的内存块
    pHead = p;

    /// 返回新创建的内存块指针，调用者可以使用 data() 方法访问数据区
    return p;
}


/**
 * @brief 释放从当前内存块开始的整个链表
 * 
 * @details
 * 这个函数遍历并释放以当前对象为起点的整个内存块链表。
 * 它会逐个释放每个内存块，直到遇到 NULL 指针。
 * 
 * 释放过程示意图：
 * 
 * 释放前链表：
 * [当前块] → [下一块] → [再下一块] → ... → NULL
 *    ↓          ↓           ↓
 *  开始释放    依次释放     最后释放
 * 
 * 注意：这个函数会释放整个链表，而不仅仅是当前块。
 * 调用者需要确保之后不再使用该链表中的任何内存块。
 */
void CPlex::FreeDataChain()
{
    /// 从当前内存块开始遍历链表
    CPlex* p = this; /// p 指向当前要释放的内存块

    while (p != NULL)
    {
        /// 保存当前块的原始字节指针，用于后续的 delete[] 操作
        /// 必须使用 BYTE* 类型，因为分配时使用的是 new BYTE[]
        BYTE* pBytes = (BYTE*)p;

        /// 在释放当前块之前，先保存下一个块的指针
        /// 这是关键步骤：一旦当前块被释放，p->pNext 就无法访问了
        CPlex* pNext = p->pNext;

        /// 释放当前内存块
        /// 使用 delete[] 因为分配时使用的是 new BYTE[]（数组形式）
        delete[] pBytes;

        /// 移动到链表中的下一个内存块
        p = pNext;

        /// 循环继续，直到 p == NULL（链表结束）
    }

    /// 注意：函数执行完毕后，整个链表的所有内存块都被释放
    /// 调用者应该将相关的头指针设置为 NULL，避免悬空指针
}

/**
 * 使用示例：
 * 
 * CPlex* pHead = NULL;  // 初始化空链表
 * 
 * // 创建第一个内存块，容纳 10 个 int 元素
 * CPlex* pBlock1 = CPlex::Create(pHead, 10, sizeof(int));
 * 
 * // 创建第二个内存块，容纳 20 个 int 元素  
 * CPlex* pBlock2 = CPlex::Create(pHead, 20, sizeof(int));
 * // 此时 pHead 指向 pBlock2，pBlock2->pNext 指向 pBlock1
 * 
 * // 访问数据区
 * int* data = (int*)pBlock1->data();
 * data[0] = 42;  // 在数据区存储数据
 * 
 * // 释放整个链表
 * pHead->FreeDataChain();
 * pHead = NULL;  // 重要：将头指针置为 NULL
 */
