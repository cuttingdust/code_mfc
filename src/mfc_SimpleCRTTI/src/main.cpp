#include "_afx.h"
#include <stdio.h>

class CBoy : public CObject
{
public:
    virtual CRuntimeClass *GetRuntimeClass()
    {
        return (CRuntimeClass *)&classCBoy;
    }
    static const CRuntimeClass classCBoy;
    static CObject *__stdcall CreateObject()
    {
        return new CBoy();
    }
};

const CRuntimeClass CBoy::classCBoy = { .m_lpszClassName   = "CBoy",
                                        .m_nObjectSize     = sizeof(CBoy),
                                        .m_wShcema         = 0xffff,
                                        .m_pfnCreateObject = &CBoy::CreateObject,
                                        .m_pBaseClass      = (CRuntimeClass *)&CObject::classCObject,
                                        .m_pNextClass      = NULL };

// class CGirl
// {
// public:
//     const int *GetRuntimeClass()
//     {
//         return &classCGirl;
//     }
//     static const int classCGirl;
// };
//
// const int CGirl::classCGirl = 1;
//

class CGirl : public CObject
{
    DECLARE_DYNAMIC(CGirl)
};

IMPLEMENT_DYNAMIC(CGirl, CObject)

class CStudent : public CObject
{
    DELCARE_DYNCREATE(CStudent)
};
IMPLEMENT_DYNCREATE(CStudent, CObject)

int main(int argc, char *argv[])
{
    {
        // CObject *pMyObj = new CBoy;
        // if (pMyObj->IsKindOf(RUNTIME_CLASS(CBoy)))
        // {
        //     printf("TRUE");
        //     delete pMyObj;
        // }
        // else
        // {
        //     delete pMyObj;
        // }
    }

    {
        // /// 如果不使用new操作符，我们也能够创建出对象
        // /// 必然是通过CRuntimeClass这个结构实现的
        // CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CBoy);
        // CObject       *pObj          = pRuntimeClass->CreateObject();
        // if (pObj != NULL && pObj->IsKindOf(RUNTIME_CLASS(CBoy)))
        // {
        //     printf("这是一个CBoy的对象");
        // }
    }

    {
        // CObject *pObj = new CGirl;
        // if (pObj->IsKindOf(RUNTIME_CLASS(CGirl)))
        // {
        //     CGirl *pMyGirl = (CGirl *)pObj;
        //     printf("这是一个CGirl");
        //     delete pMyGirl;
        // }
        // else
        // {
        //     delete pObj;
        // }
    }

    {
        CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CStudent);
        CObject       *pObj          = pRuntimeClass->CreateObject();
        if (pObj != NULL && pObj->IsKindOf(RUNTIME_CLASS(CStudent)))
        {
            printf("创建成功");
            delete pObj;
        }
    }

    getchar();
    return 0;
}
