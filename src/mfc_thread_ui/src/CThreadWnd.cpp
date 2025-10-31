// CThreadWnd.cpp: 实现文件
//

#include "MFCUIThread.h"
#include "afxdialogex.h"
#include "CThreadWnd.h"


// CThreadWnd 对话框

IMPLEMENT_DYNAMIC(CThreadWnd, CDialogEx)

CThreadWnd::CThreadWnd(CWnd* pParent /*=nullptr*/) : CDialogEx(IDD_THREAD_WND, pParent)
{
}

CThreadWnd::~CThreadWnd()
{
}

void CThreadWnd::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CThreadWnd, CDialogEx)
END_MESSAGE_MAP()


// CThreadWnd 消息处理程序
