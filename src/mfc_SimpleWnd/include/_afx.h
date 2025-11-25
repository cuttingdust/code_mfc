#ifndef _AFX_H
#define _AFX_H

#include <Windows.h>
#define VERSION 0xFFFF

class CObject;

struct CRuntimeClass
{
    /// 属性
    LPCSTR m_lpszClassName; ///< 类名
    int    m_nObjectSize;   ///< 类的大小
    UINT   m_wShcema;       ///< 版本
    CObject*(__stdcall* m_pfnCreateObject)();
    CRuntimeClass* m_pBaseClass;

    CObject* CreateObject();
    BOOL     IsDerivedFrom(const CRuntimeClass* pBaseClass) const;

    /// 内部实现可维护的单链表
    CRuntimeClass* m_pNextClass;
};

class CObject
{
public:
    virtual CRuntimeClass* GetRuntimeClass() const;
    virtual ~CObject();

    BOOL IsKindOf(const CRuntimeClass* pClass) const;

public:
    static const CRuntimeClass classCObject; ///< 标识类的静态成员
};

inline CObject::~CObject() {};


/// RUNTIME_CLASS: 提取公因式
#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)&class_name::class##class_name)
//CRuntimeClass* &CObject::classCObject;
//CRuntimeClass* &CWnd::classCWnd;

#define DECLARE_DYNAMIC(class_name)               \
public:                                           \
    static const CRuntimeClass class##class_name; \
    virtual CRuntimeClass* GetRuntimeClass() const;

/// 动态类型识别的代码，统一抽象成宏，从而减少代码的编写

#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew)                                    \
    const CRuntimeClass class_name::class##class_name = { #class_name, sizeof(class class_name),       wSchema, \
                                                          pfnNew,      RUNTIME_CLASS(base_class_name), NULL };  \
    CRuntimeClass*      class_name::GetRuntimeClass() const                                                     \
    {                                                                                                           \
        return RUNTIME_CLASS(class_name);                                                                       \
    }

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, VERSION, NULL)

/// 支持动态创建的宏

#define DELCARE_DYNCREATE(class_name) \
    DECLARE_DYNAMIC(class_name)       \
    static CObject* __stdcall CreateObject();

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
    CObject* __stdcall class_name::CreateObject()        \
    {                                                    \
        return new class_name;                           \
    }                                                    \
    IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, VERSION, class_name::CreateObject)


#endif // _AFX_H
