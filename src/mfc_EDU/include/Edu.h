#pragma once
#include "stdafx.h"
#include "Resource.h"

class CMFCEduApp : public CWinApp
{
public:
    CMFCEduApp();

public:
    BOOL         InitInstance() override;
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};
