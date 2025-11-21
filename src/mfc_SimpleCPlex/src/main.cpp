#include "_afxplex_.h"
#include "_afxColl_.h"

#include <iostream>

struct CMyData
{
    int nSomeData;
    int nSomeMoreData;
};

int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    {
        /// 使用CPlex分配和管理内存块的示例
        // CPlex* pBlocks = NULL;
        //
        // /// 分配第一个内存块
        // CPlex*   pBlock1 = CPlex::Create(pBlocks, 10, sizeof(CMyData));
        // CMyData* pData1  = (CMyData*)pBlock1->data();
        //
        // /// 使用第一个块的数据
        // for (int i = 0; i < 10; i++)
        // {
        //     // (pData1 + i)->nSomeData = i;
        //     pData1[i].nSomeData = i;
        // }
        //
        // /// 分配第二个内存块
        // CPlex*   pBlock2 = CPlex::Create(pBlocks, 10, sizeof(CMyData));
        // CMyData* pData2  = (CMyData*)pBlock2->data();
        //
        // /// 使用第二个块的数据
        // for (int i = 0; i < 10; i++)
        // {
        //     pData2[i].nSomeData = i + 10; /// 从10开始
        // }
        //
        // /// 遍历所有块的数据
        // CPlex* pCurrent   = pBlocks;
        // int    blockIndex = 0;
        // while (pCurrent != NULL)
        // {
        //     CMyData* pData = (CMyData*)pCurrent->data();
        //     std::cout << "块" << blockIndex << "的数据: ";
        //     for (int i = 0; i < 10; i++)
        //     {
        //         std::cout << pData[i].nSomeData << " ";
        //     }
        //     std::cout << std::endl;
        //
        //     pCurrent = pCurrent->pNext;
        //     blockIndex++;
        // }
        //
        // /// 释放所有内存块
        // pBlocks->FreeDataChain();
    }

    {
        setlocale(LC_ALL, "chs");

        CMapPtrToPtr map;
        char         szDay[][20] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
        /// 用[]重载
        map[szDay[0]] = (void *)"周日";
        map[szDay[1]] = (void *)"周1";
        map[szDay[2]] = (void *)"周2";
        map[szDay[3]] = (void *)"周3";
        map[szDay[4]] = (void *)"周4";
        map[szDay[5]] = (void *)"周5";
        map[szDay[6]] = (void *)"周6";
        map.PrintAsciiArt();

        printf("%s:%s\n", szDay[4], (char *)map[szDay[4]]);
    }

    return 0;
}
