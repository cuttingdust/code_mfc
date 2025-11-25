#ifndef WINHAND__H
#define WINHAND__H

#include "_afxColl_.h"

class CObject;

/**
 * @class CHandleMap
 * @brief MFC 句柄映射管理类
 * 
 * 该类负责维护 Windows 句柄（HWND、HDC 等）与 MFC 对象（CWnd、CDC 等）之间的映射关系。
 * 这是 MFC 实现 Windows 句柄与 C++ 对象关联的核心基础设施。
 * 
 * 主要功能：
 * 1. 建立句柄与对象的永久关联
 * 2. 通过句柄查找对应的 MFC 对象
 * 3. 管理句柄映射的生命周期
 * 
 * @note 该类使用 CMapPtrToPtr 作为底层存储结构，提供高效的句柄查找功能。
 */
class CHandleMap
{
private:
    CMapPtrToPtr m_permanetMap; ///< 永久映射表：存储句柄到 MFC 对象的指针映射

public:
    /**
     * @brief 通过句柄查找永久关联的 MFC 对象
     * @param h 要查找的 Windows 句柄（HWND、HDC 等）
     * @return 返回与句柄关联的 CObject 派生类对象指针，如果未找到返回 NULL
     * 
     * @note 此方法只查找通过 SetPermanet 建立的永久关联，不会创建临时对象
     */
    CObject* LookupPermanet(HANDLE h);

    /**
     * @brief 建立句柄与 MFC 对象的永久关联
     * @param h 要关联的 Windows 句柄
     * @param permOb 要关联的 MFC 对象指针
     * 
     * @note 此操作建立双向关联：
     *       1. 可以通过句柄找到对象
     *       2. 对象内部也会保存句柄值
     *       这是 MFC 消息路由机制的基础
     */
    void SetPermanet(HANDLE h, CObject* permOb);

    /**
     * @brief 移除句柄的映射关系
     * @param h 要移除的 Windows 句柄
     * 
     * @note 当窗口销毁或对象分离时调用此方法，断开句柄与对象的关联
     *       但不会销毁实际的 Windows 句柄或 MFC 对象
     */
    void RemoveHandle(HANDLE h);

    /**
     * @brief 通过句柄获取 MFC 对象（查找或创建临时对象）
     * @param h 要查找的 Windows 句柄
     * @return 返回与句柄关联的 CObject 派生类对象指针
     * 
     * @note 此方法首先查找永久关联的对象，如果找不到：
     *       1. 在完整 MFC 实现中会创建临时对象包装该句柄
     *       2. 在当前简化实现中，行为与 LookupPermanet 相同
     */
    CObject* FromHandle(HANDLE h);
};

/**
 * @brief 内联实现：通过句柄查找永久关联的 MFC 对象
 * 
 * 使用 CMapPtrToPtr 的 operator[] 进行查找，如果键不存在会返回 NULL。
 * 这是 MFC 消息路由的关键步骤：将 Windows 消息与对应的 C++ 对象关联。
 * 
 * @param h 要查找的 Windows 句柄
 * @return 关联的 MFC 对象指针，未找到时返回 NULL
 */
__inline CObject* CHandleMap::LookupPermanet(HANDLE h)
{
    return (CObject*)m_permanetMap[h]; ///< 类型转换：void* -> CObject*
}

/**
 * @brief 内联实现：建立句柄与对象的永久关联
 * 
 * 将句柄和对象指针存入映射表中，建立双向关联。
 * 这是 MFC 对象与 Windows 资源绑定的核心操作。
 * 
 * @param h 要关联的 Windows 句柄
 * @param permOb 要关联的 MFC 对象指针
 */
__inline void CHandleMap::SetPermanet(HANDLE h, CObject* permOb)
{
    m_permanetMap[h] = permOb; ///< 存储指针关联
}

/**
 * @brief 内联实现：移除句柄的映射关系
 * 
 * 当 CWnd 对象销毁或调用 Detach() 时，需要从映射表中移除关联，
 * 避免悬空指针和内存访问错误。
 * 
 * @param h 要移除的 Windows 句柄
 */
__inline void CHandleMap::RemoveHandle(HANDLE h)
{
    m_permanetMap.RemoveKey(h); ///< 从映射表中移除指定键
}

/**
 * @brief 内联实现：通过句柄获取 MFC 对象
 * 
 * 在当前简化实现中，此方法与 LookupPermanet 功能相同。
 * 在完整 MFC 实现中，如果找不到永久关联的对象，会创建临时对象。
 * 
 * @param h 要查找的 Windows 句柄
 * @return 关联的 MFC 对象指针
 */
__inline CObject* CHandleMap::FromHandle(HANDLE h)
{
    return LookupPermanet(h); ///< 委托给 LookupPermanet 方法
}

#endif // WINHAND__H
