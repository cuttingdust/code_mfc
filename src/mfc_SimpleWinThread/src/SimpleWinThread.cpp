#include "_AFXTLS.h"

#include <stdio.h>

/// 带组合的节点
struct MyThreadData
{
    MyThreadData *pNext;
    int           nSomeData;
};

int main(int argc, char *argv[])
{
    {
        /// 使用CSimpleList的单元测试
        // MyThreadData *pData;
        // CSimpleList   list;
        //
        // list.Construct(offsetof(MyThreadData, pNext));
        //
        // /// 向list里面添加节点
        // for (int i = 0; i < 10; i++)
        // {
        //     pData            = new MyThreadData;
        //     pData->nSomeData = i;
        //     list.AddHead(pData);
        // }
        //
        // /// 遍历
        // pData = (MyThreadData *)list.GetHead();
        // while (pData != NULL)
        // {
        //     MyThreadData *pNextData = pData->pNext;
        //     printf("%d\n", pData->nSomeData);
        //     delete pData;
        //     pData = pNextData;
        // }
    }

    {
        MyThreadData                    *pData;
        CTypedSimpleList<MyThreadData *> list;

        list.Construct(offsetof(MyThreadData, pNext));
        ///向list里面添加节点
        for (int i = 0; i < 10; i++)
        {
            pData            = new MyThreadData;
            pData->nSomeData = i;
            list.AddHead(pData);
        }

        /// 遍历
        pData = list; /// 调用（）运算符重载，从而实现了getHead的方法
        while (pData != NULL)
        {
            MyThreadData *pNextData = pData->pNext;
            printf("%d\n", pData->nSomeData);
            delete pData;
            pData = pNextData;
        }
    }

    getchar();
    return 0;
}
