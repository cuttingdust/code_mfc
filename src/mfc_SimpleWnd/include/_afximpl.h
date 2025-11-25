#ifndef __AFXIMPL_H__
#define __AFXIMPL_H__
#include "_AFXWIN.h"
/// 窗口类的类型标识
#define AFX_WND_REG            (0x0001)
#define AFX_WNDFRAMEORVIEW_REG (0x002)

#define AFX_WND            (L"Wnd")
#define AFX_WNDFRAMEORVIEW (L"FrameOrView")

/// 我们会为MFC注册窗口的时候提供类名
extern const TCHAR _afxWnd[];
extern const TCHAR _afxWndFrameOrView[];

#endif // !__AFXIMPL_H__
