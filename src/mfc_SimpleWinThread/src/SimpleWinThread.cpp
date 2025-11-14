#include "_AFXTLS.h"

#include <stdio.h>

#include <process.h>

/// 带组合的节点
struct MyThreadData : public CNoTrackObject
{
    MyThreadData *pNext;
    int           nSomeData;
};

struct CMyThreadData : public CNoTrackObject
{
    int nSomeData;
};

CThreadLocal<CMyThreadData> g_myThreadData;

void        show();
UINT WINAPI ThreadFunc(LPVOID lpParam)
{
    g_myThreadData->nSomeData = (int)lpParam;
    show();
    return 0;
}

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
        // MyThreadData                    *pData;
        // CTypedSimpleList<MyThreadData *> list;
        //
        // list.Construct(offsetof(MyThreadData, pNext));
        // ///向list里面添加节点
        // for (int i = 0; i < 10; i++)
        // {
        //     pData            = new MyThreadData;
        //     pData->nSomeData = i;
        //     list.AddHead(pData);
        // }
        //
        // /// 遍历
        // pData = list; /// 调用（）运算符重载，从而实现了getHead的方法
        // while (pData != NULL)
        // {
        //     MyThreadData *pNextData = pData->pNext;
        //     printf("%d\n", pData->nSomeData);
        //     delete pData;
        //     pData = pNextData;
        // }
    }

    {
        /// 测试MFC工作者线程
        HANDLE h[10];
        UINT   uID;
        /// 启10个线程
        for (int i = 0; i < 10; i++)
        {
            h[i] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, (void *)i, 0, &uID);
        }
        ::WaitForMultipleObjects(10, h, TRUE, INFINITE);
        for (int i = 0; i < 10; i++)
        {
            ::CloseHandle(h[i]);
        }
    }


    getchar();
    return 0;
}


void show()
{
    int nData = g_myThreadData->nSomeData;
    printf("Thread id:%d,nSomeData=%d\n", ::GetCurrentThreadId(), nData);
}
