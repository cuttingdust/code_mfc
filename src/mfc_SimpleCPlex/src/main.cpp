#include "_afxplex_.h"
#include <iostream>

struct CMyData
{
    int nSomeData;
    int nSomeMoreData;
};

int main()
{
    CPlex* pBlocks = NULL;

    /// 分配第一个内存块
    CPlex*   pBlock1 = CPlex::Create(pBlocks, 10, sizeof(CMyData));
    CMyData* pData1  = (CMyData*)pBlock1->data();

    /// 使用第一个块的数据
    for (int i = 0; i < 10; i++)
    {
        // (pData1 + i)->nSomeData = i;
        pData1[i].nSomeData = i;
    }

    /// 分配第二个内存块
    CPlex*   pBlock2 = CPlex::Create(pBlocks, 10, sizeof(CMyData));
    CMyData* pData2  = (CMyData*)pBlock2->data();

    /// 使用第二个块的数据
    for (int i = 0; i < 10; i++)
    {
        pData2[i].nSomeData = i + 10; /// 从10开始
    }

    /// 遍历所有块的数据
    CPlex* pCurrent   = pBlocks;
    int    blockIndex = 0;
    while (pCurrent != NULL)
    {
        CMyData* pData = (CMyData*)pCurrent->data();
        std::cout << "块" << blockIndex << "的数据: ";
        for (int i = 0; i < 10; i++)
        {
            std::cout << pData[i].nSomeData << " ";
        }
        std::cout << std::endl;

        pCurrent = pCurrent->pNext;
        blockIndex++;
    }

    /// 释放所有内存块
    pBlocks->FreeDataChain();
    return 0;
}
