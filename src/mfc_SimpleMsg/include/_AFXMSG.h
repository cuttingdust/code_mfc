#ifndef _AFXMSG_H
#define _AFXMSG_H

/// 消息函数的签名表示
enum AfxSig
{
    AfxSig_end = 0,
    AfxSig_vv  = 1, /// void (void),比如void OnPaint
    AfxSig_vw  = 2, /// void(UINT), void OnTimer()
    AfxSig_is  = 3, /// int (LPSTR) BOOL OnCreate(LPCREATESTRUT)
};


/// 列举若干个自定义的MFC消息
#define ON_WM_CREATE() \
    { WM_CREATE, 0, 0, 0, AfxSig_is, (AFX_PMSG)(AFX_PMSGW)(int(CWnd::*)(LPCREATESTRUCT)) & OnCreate },

#define ON_WM_PAINT() { WM_PAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW)(int(CWnd::*)(HDC)) & OnPaint },


#define ON_WM_CLOSE() { WM_CLOSE, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW)(int(CWnd::*)(void)) & OnClose },

#define ON_WM_DESTROY() { WM_DESTROY, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW)(int(CWnd::*)(void)) & OnDestroy },

#define ON_WM_NCDESTROY() { WM_NCDESTROY, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW)(int(CWnd::*)(void)) & OnNcDestroy },

#define ON_WM_TIMER() { WM_TIMER, 0, 0, 0, AfxSig_vw, (AFX_PMSG)(AFX_PMSGW)(void(CWnd::*)(UINT)) & OnTimer },


#endif // _AFXMSG_H