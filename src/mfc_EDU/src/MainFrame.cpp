#include "MainFrame.h"

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
    if (!CFrameWnd::PreCreateWindow(cs))
    {
        return FALSE;
    }
    /*cs.cx = 600;
	cs.cy = 480;*/

    return TRUE;
}

CMainFrame::~CMainFrame()
{
}

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
