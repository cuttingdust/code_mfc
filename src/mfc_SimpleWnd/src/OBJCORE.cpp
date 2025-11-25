#include "_afx.h"

CObject* CRuntimeClass::CreateObject()
{
    if (m_pfnCreateObject == NULL)
        return NULL;
    /// 否则调用创建类的函数
    return (*m_pfnCreateObject)();
}

BOOL CRuntimeClass::IsDerivedFrom(const CRuntimeClass* pBaseClass) const
{
    const CRuntimeClass* pClassThis = this;
    while (pClassThis != NULL)
    {
        if (pClassThis == pBaseClass)
        {
            return TRUE;
        }
        pClassThis = pClassThis->m_pBaseClass;
    }
    return FALSE;
}

const struct CRuntimeClass CObject::classCObject = { .m_lpszClassName   = "CObject",       ///< 类名
                                                     .m_nObjectSize     = sizeof(CObject), ///< 大小
                                                     .m_wShcema         = 0xffff,          ///< 版本
                                                     .m_pfnCreateObject = NULL,            ///< 不支持动态创建
                                                     .m_pBaseClass      = NULL,            ///< 没有父类
                                                     .m_pNextClass      = NULL };

CRuntimeClass* CObject::GetRuntimeClass() const
{
    return ((CRuntimeClass*)&(CObject::classCObject));
}

BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const
{
    CRuntimeClass* pClassThis = GetRuntimeClass();
    return pClassThis->IsDerivedFrom(pClassThis);
}
