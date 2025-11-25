#include "_AFXWIN.h"

/// CWinApp 构造函数 - MFC 应用程序对象的初始化
/// 这是整个 MFC 应用程序的起点，在 main/WinMain 之前执行
CWinApp::CWinApp()
{
    /// 获取当前模块的全局状态
    /// AFX_MODULE_STATE 包含模块级别的全局信息（如资源、类工厂等）
    AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

    /// 获取当前线程的状态信息
    /// AFX_MODULE_THREAD_STATE 包含线程特定的信息（如消息队列、当前窗口等）
    AFX_MODULE_THREAD_STATE* pThreadState = pModuleState->m_thread;

    /// 设置当前线程的 CWinThread 指针指向 this
    /// 这样 MFC 框架就知道当前线程由哪个 CWinThread 对象管理
    pThreadState->m_pCurrentWinThread = this;

    /// 保存当前线程的句柄和ID
    /// 标志着 MFC 开始正式接管当前线程的生命周期管理
    m_hThread   = ::GetCurrentThread();   /// 线程内核对象句柄
    m_nThreadID = ::GetCurrentThreadId(); /// 线程ID，用于消息投递等操作

    /// 设置当前模块的应用程序对象指针
    /// 这样整个 MFC 框架都能通过 AfxGetApp() 获取到应用程序对象
    pModuleState->m_pCurrentWinApp = this;

    /// 初始化实例句柄为 NULL
    /// 只有在 WinMain 函数开始执行后，Windows 才会为应用程序分配真正的 HINSTANCE
    /// 这个值会在 CWinApp::InitInstance() 中被正确设置
    m_hInstance = NULL;
}


/// CWinApp 析构函数 - 应用程序对象的清理
CWinApp::~CWinApp()
{
    /// 获取当前模块状态
    AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

    /// 如果当前应用程序对象仍然是 this，则清空指针
    /// 防止悬空指针访问
    if (pModuleState->m_pCurrentWinApp == this)
    {
        pModuleState->m_pCurrentWinApp = NULL;
    }

    /// 注意：这里没有删除 m_hThread，因为它只是线程句柄的副本
    /// 实际的线程生命周期由操作系统管理
}


/// InitApplication - 应用程序级别的初始化
/// 在 InitInstance 之前调用，用于整个应用程序的全局初始化
/// 返回值: TRUE - 初始化成功; FALSE - 初始化失败，应用程序终止
BOOL CWinApp::InitApplication()
{
    /// 基类实现为空，派生类可以重写此方法
    /// 典型用途：
    /// - 初始化应用程序级别的全局变量
    /// - 注册应用程序级别的窗口类
    /// - 执行一次性的应用程序配置
    return TRUE;
}


/// InitInstance - 实例级别的初始化
/// 每个应用程序实例都会调用此方法（对于DLL，每个进程都会调用）
/// 返回值: TRUE - 初始化成功; FALSE - 初始化失败，应用程序终止
BOOL CWinApp::InitInstance()
{
    /// 基类实现为空，派生类必须重写此方法
    /// 这是 MFC 应用程序的"真正入口点"
    /// 典型用途：
    /// - 创建主窗口
    /// - 设置主窗口的显示状态
    /// - 执行实例特定的初始化
    /// - 如果返回 FALSE，应用程序会立即退出
    return TRUE;
}


/// ExitInstance - 应用程序实例的清理
/// 在应用程序退出时调用，用于执行清理操作
/// 返回值: 应用程序的退出代码（通常作为进程退出码）
BOOL CWinApp::ExitInstance()
{
    /// 返回最后一条消息的 wParam 作为退出代码
    /// 通常 WM_QUIT 消息的 wParam 包含了应用程序的退出码
    /// 典型用途：
    /// - 释放应用程序分配的资源
    /// - 保存应用程序状态
    /// - 执行其他清理操作
    return m_msgCur.wParam;
}


/// Run - 应用程序的主消息循环
/// 这是 MFC 应用程序的核心消息泵
/// 返回值: 应用程序的退出代码
int CWinApp::Run()
{
    /// 调用基类 CWinThread::Run() 启动主消息循环
    /// CWinThread::Run() 实现了完整的消息泵机制：
    /// 1. 处理 Windows 消息
    /// 2. 在空闲时调用 OnIdle 进行后台处理
    /// 3. 维护应用程序的响应性
    ///
    /// 消息循环的基本流程：
    /// while (应用程序运行) {
    ///     while (有空闲时间 && 没有消息) {
    ///         调用 OnIdle() 进行后台处理
    ///     }
    ///     处理所有待处理的消息
    ///     如果是空闲消息，重置空闲状态
    /// }
    return CWinThread::Run();
}


/// IMPLEMENT_DYNCREATE - MFC 运行时类系统的宏
/// 这个宏实现了 MFC 的动态创建机制
IMPLEMENT_DYNCREATE(CWinApp, CWinThread);

/// IMPLEMENT_DYNCREATE 宏展开后大致实现的功能：
/// 1. 为 CWinApp 创建 CRuntimeClass 静态对象
/// 2. 实现 GetRuntimeClass() 虚函数，返回类的运行时信息
/// 3. 实现动态创建函数 CreateObject()，用于通过类名创建对象
/// 4. 实现 IsKindOf() 功能，支持运行时类型检查
///
/// 这使得以下操作成为可能：
/// - RUNTIME_CLASS(CWinApp)->CreateObject() 动态创建对象
/// - pObj->IsKindOf(RUNTIME_CLASS(CWinApp)) 类型检查
/// - CObject::Serialization 序列化机制  CFile -> CArchive  -> CObject -> CDocument
