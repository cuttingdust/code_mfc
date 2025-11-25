#include "_AFXWIN.h"

/// MFC 应用程序主执行流程 - 通常位于 AfxWinMain 函数中
/// 这是 MFC 应用程序的完整启动和运行流程
/// wWinMain - MFC Windows 应用程序的入口点
/// 这是 Unicode 版本的 WinMain，由 Windows 操作系统调用启动应用程序
///
/// 参数:
///   _In_ HINSTANCE hInstance      - 当前应用程序实例的句柄，用于标识这个运行中的程序实例
///   _In_opt_ HINSTANCE hPrevInstance - 前一个实例句柄，在 Win32 中总是 NULL（仅为 Win16 兼容性保留）
///   _In_ LPWSTR lpCmdLine         - 命令行参数字符串（Unicode 版本），不包含程序名
///   _In_ int nCmdShow             - 窗口显示方式标志，如 SW_SHOW、SW_MAXIMIZE、SW_MINIMIZE 等
///
/// 返回值:
///   int - 应用程序退出代码，0 表示成功，非 0 表示错误
///
/// 调用流程:
///   操作系统 → wWinMain → MFC 初始化 → 应用程序初始化 → 消息循环 → 返回退出码
int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,     ///< 当前实例句柄 - Windows 分配给每个进程的唯一标识
                      _In_opt_ HINSTANCE hPrevInstance, ///< 前一个实例句柄 - Win32中恒为NULL，保持Win16兼容性
                      _In_ LPWSTR        lpCmdLine,     ///< 命令行参数字符串 - 例如: L"-debug -file test.txt"
                      _In_ int           nCmdShow       ///< 窗口显示模式 - 控制主窗口初始显示状态
)
{
    /// 初始化返回值，默认为 -1 表示失败
    /// 这个值将在各个阶段被更新，最终作为进程退出码返回给操作系统
    int nRet = -1;

    /// 获取当前线程对象指针
    /// CWinThread 管理线程的消息循环和状态
    /// 对于主线程，这通常返回 CWinApp 对象（因为 CWinApp 继承自 CWinThread）
    CWinThread* pThread = AfxGetThread();

    /// 获取应用程序对象指针
    /// 返回全局的 CWinApp 派生类实例（在应用程序中定义为 theApp）
    /// 这个对象在进入 wWinMain 之前就已经构造完成
    CWinApp* pApp = AfxGetApp();

    /// ====================================================================
    /// 阶段 1: MFC Windows 框架初始化
    /// ====================================================================
    /// AfxWinInit 设置 Windows 特定的环境：
    ///   - 初始化模块状态中的实例句柄和资源句柄
    ///   - 设置 CWinApp 对象的 Windows 相关属性
    ///   - 准备 MFC 框架运行所需的 Windows 环境
    ///
    /// 如果失败，意味着 MFC 框架无法正常启动，直接跳转到清理流程
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
        goto InitFailure;

    /// ====================================================================
    /// 阶段 2: 应用程序全局初始化
    /// ====================================================================
    /// InitApplication 执行应用程序级别的一次性初始化：
    ///   - 注册应用程序范围的窗口类
    ///   - 初始化全局应用程序数据
    ///   - 执行只需要进行一次的配置
    ///
    /// 注意：在 DLL 场景中，每个进程都会调用此函数
    /// 如果失败，表示应用程序级别的设置有问题
    if (pApp != NULL && !pApp->InitApplication())
        goto InitFailure;

    /// ====================================================================
    /// 阶段 3: 线程实例初始化（核心初始化阶段）
    /// ====================================================================
    /// InitInstance 是每个实例必须执行的初始化：
    ///   - 创建应用程序的主窗口
    ///   - 设置主窗口的显示状态
    ///   - 加载应用程序特定的资源
    ///   - 执行实例特有的配置
    ///
    /// 这是派生类必须重写的关键虚函数！
    /// 如果返回 FALSE，表示应用程序实例初始化失败
    if (!pThread->InitInstance())
    {
        /// 实例初始化失败，调用 ExitInstance 进行清理
        /// 保存退出代码后跳转到清理流程
        nRet = pThread->ExitInstance();
        goto InitFailure;
    }

    /// ====================================================================
    /// 阶段 4: 主消息循环 - 应用程序的"心脏"
    /// ====================================================================
    /// Run() 启动主消息循环，这是应用程序与用户交互的核心：
    ///   - 从消息队列获取 Windows 消息
    ///   - 翻译和分发消息到相应的窗口过程
    ///   - 在消息空闲时调用 OnIdle 进行后台处理
    ///   - 循环直到收到 WM_QUIT 消息
    ///
    /// Run() 的返回值通常来自 WM_QUIT 消息的 wParam
    /// 这个值将作为整个应用程序的退出代码
    nRet = pThread->Run();

/// ====================================================================
/// 清理标签：所有失败路径都会跳转到这里
/// ====================================================================
InitFailure:
    /// 返回应用程序退出代码给操作系统
    /// 可能的返回值：
    ///   - 大于 0: Run() 正常执行后的退出码（来自 WM_QUIT）
    ///   - 0: ExitInstance() 返回的退出码
    ///   - -1: 初始化阶段失败
    return nRet;
}

/// 完整的应用程序启动流程总结：
///
/// 1. 全局 CWinApp 对象构造（在进入 wWinMain 之前）
///    ↓
/// 2. wWinMain 被操作系统调用
///    ↓
/// 3. AfxWinInit()    - MFC Windows 环境初始化
///    ↓
/// 4. InitApplication() - 应用程序全局初始化（可选重写）
///    ↓
/// 5. InitInstance()    - 实例初始化（必须重写！创建主窗口）
///    ↓
/// 6. Run()            - 主消息循环（应用程序运行）
///    ↓
/// 7. ExitInstance()   - 实例清理（如果 InitInstance 失败）
///    ↓
/// 8. 返回退出码给操作系统
///
/// 关键设计特点：
/// - 使用 goto 进行集中错误处理，避免深层嵌套
/// - 清晰的阶段划分，每个阶段职责单一
/// - 模板方法模式：框架控制流程，派生类定制行为
/// - 资源安全：任何阶段失败都能正确清理
///
/// 典型派生类实现示例：
///
/// class CMyApp : public CWinApp {
/// public:
///     virtual BOOL InitInstance() override {
///         // 创建主窗口
///         CMainFrame* pFrame = new CMainFrame;
///         if (!pFrame->LoadFrame(IDR_MAINFRAME))
///             return FALSE;
///
///         // 设置主窗口并显示
///         m_pMainWnd = pFrame;
///         pFrame->ShowWindow(m_nCmdShow);
///         pFrame->UpdateWindow();
///
///         return TRUE;
///     }
/// };
///
/// // 全局应用程序对象
/// CMyApp theApp;
