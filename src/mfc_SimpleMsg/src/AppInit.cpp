#include "_AFXWIN.h"

/// AfxWinInit - MFC Windows 应用程序初始化函数
/// 这是 MFC 应用程序的 Windows 特定初始化入口，在 AfxWinMain 中调用
///
/// 参数:
///   hInstance      - 当前应用程序实例句柄（来自 WinMain）
///   hPrevInstance  - 前一个应用程序实例句柄（Win16 兼容性，Win32 中通常为 NULL）
///   lpCmdLine      - 命令行参数字符串（不包含程序名）
///   nCmdShow       - 窗口显示方式（如 SW_SHOW、SW_MAXIMIZE 等）
///
/// 返回值:
///   BOOL - 初始化是否成功（TRUE 成功，FALSE 失败）
///
/// 调用时机:
///   在全局 CWinApp 对象构造之后，InitInstance 之前调用
///   负责设置模块状态和应用程序对象的 Windows 特定属性
BOOL AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    /// 获取当前模块的全局状态对象
    /// AFX_MODULE_STATE 包含模块级别的全局信息，如实例句柄、资源句柄、模块线程状态等
    /// 这个对象在整个模块生命周期中管理着 MFC 框架的状态信息
    AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

    /// 设置当前模块的实例句柄
    /// 这个句柄用于：
    ///   - 加载模块资源（图标、字符串、对话框等）
    ///   - 注册窗口类
    ///   - 创建窗口实例
    /// 在 DLL 场景中，这可能是 DLL 的实例句柄而非 EXE 的实例句柄
    pModuleState->m_hCurrentInstanceHandle = hInstance;

    /// 设置当前模块的资源句柄
    /// 默认情况下，实例句柄也作为资源句柄使用
    /// 应用程序可以通过 AfxSetResourceHandle 修改此设置，以加载其他模块的资源
    pModuleState->m_hCurrentResourceHandle = hInstance;

    /// 初始化应用程序状态
    /// 获取全局应用程序对象指针（在 CWinApp 构造函数中设置）
    /// 这个指针指向由 DECLARE_THE_APP 宏定义的全局 theApp 对象
    CWinApp* pApp = AfxGetApp();

    /// 检查应用程序对象是否存在
    /// 正常情况下，全局 theApp 对象应该在调用此函数前已经构造完成
    if (pApp != NULL)
    {
        /// 设置应用程序实例句柄
        /// 这个句柄标识当前运行的应用程序实例
        /// 用于：
        ///   - 创建应用程序的主窗口
        ///   - 加载应用程序资源
        ///   - 与其他实例通信（通过 FindWindow 等）
        pApp->m_hInstance = hInstance;

        /// 设置前一个实例句柄（主要用于 Win16 兼容性）
        /// 在 Win32 系统中，每个进程有独立的地址空间，此参数通常为 NULL
        /// MFC 保留此字段是为了保持与旧版本 Windows 的源代码兼容性
        pApp->m_hPreveInstance = hPrevInstance;

        /// 保存命令行参数字符串
        /// 这个字符串不包含程序名称，只包含实际的命令行参数
        /// 例如：如果用户运行 "myapp.exe -debug -file test.txt"
        /// 那么 lpCmdLine 将包含 "-debug -file test.txt"
        /// 应用程序可以通过解析此字符串来获取启动参数
        pApp->m_lpCmdLine = lpCmdLine;

        /// 保存窗口显示命令
        /// 这个值决定了应用程序主窗口的初始显示状态
        /// 常用值包括：
        ///   SW_SHOW      - 正常显示窗口
        ///   SW_MAXIMIZE  - 最大化显示
        ///   SW_MINIMIZE  - 最小化显示
        ///   SW_HIDE      - 隐藏窗口
        /// 应用程序可以在 InitInstance 中覆盖此设置
        pApp->m_nCmdShow = nCmdShow;
    }
    else
    {
        /// 应用程序对象不存在，这是严重的初始化错误
        /// 通常意味着：
        ///   - 没有定义全局 CWinApp 派生类对象
        ///   - 在对象构造前调用了此函数
        ///   - 内存损坏导致对象丢失
        /// 在实际的 MFC 实现中，这里可能会触发断言或异常
        /// 注意：这里没有返回 FALSE，保持了与原始 MFC 的兼容性
    }

    /// 返回初始化成功
    /// 注意：即使 pApp 为 NULL，也返回 TRUE
    /// 这是因为在 DLL 场景中，可能没有 CWinApp 对象
    /// 实际的应用程序初始化检查在后续的 InitInstance 中进行
    return TRUE;
}

/// 函数执行后的状态变化：
///
/// 调用前状态：
///   - 全局 CWinApp 对象已构造，但 Windows 相关属性未设置
///   - 模块状态中的实例句柄可能为 NULL 或默认值
///   - 应用程序无法正确加载资源或创建窗口
///
/// 调用后状态：
///   - 模块状态中的实例句柄和资源句柄已正确设置
///   - CWinApp 对象的 Windows 属性已初始化
///   - 应用程序准备好执行 InitInstance 进行具体的窗口创建和初始化
///
/// 典型调用序列：
///   WinMain (操作系统调用)
///     ↓
///   AfxWinMain (MFC 入口)
///     ↓
///   AfxWinInit (Windows 初始化) ← 这里
///     ↓
///   pApp->InitApplication (应用程序级初始化)
///     ↓
///   pApp->InitInstance (实例级初始化) ← 派生类重写的地方
///     ↓
///   pApp->Run (消息循环)
///
/// 多模块场景说明：
///   在 DLL 模块中，hInstance 是 DLL 的实例句柄
///   在 EXE 模块中，hInstance 是 EXE 的实例句柄
///   MFC 通过模块状态机制确保每个模块使用正确的实例句柄
///
/// 错误处理：
///   此函数通常不会失败，因为主要工作是设置指针和句柄
///   真正的应用程序初始化检查在 InitInstance 中进行
///   如果 InitInstance 返回 FALSE，应用程序会终止
