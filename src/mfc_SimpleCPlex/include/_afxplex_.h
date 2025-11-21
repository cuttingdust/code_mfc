#pragma once

#include <windows.h>

struct CPlex
{
    CPlex* pNext; /// 向我们组织的内存块，用链表串联的指针

    /// 数据区
    void* data()
    {
        return this + 1;
    }

    /// 用于申请内存,CbElement大小的空间有nMax
    static CPlex* Create(CPlex*& pHead, UINT nMax, UINT cbElement);

    /// 释放以当前对象为首地址的内存链表
    void FreeDataChain();
};
