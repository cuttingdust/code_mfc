#pragma once
#include "stdafx.h"

class CMainFrame : public CFrameWnd
{
protected:
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)
public:
    BOOL PreCreateWindow(CREATESTRUCT& cs) override;

public:
    ~CMainFrame() override;

protected:
    DECLARE_MESSAGE_MAP();
};
