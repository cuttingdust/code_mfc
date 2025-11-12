/**
 * @file   CWinApp.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-11-12
 */

#ifndef CWINAPP_H
#define CWINAPP_H

#include "CFrameWnd.h"

/// 应用程序类----------------------------------------------------------------------------------
class CWinApp
{
public:
    CFrameWnd *m_pMainWnd;

public:
    virtual BOOL InitInstance(int nCmdShow);
    int          Run();
    virtual ~CWinApp();
};


#endif // CWINAPP_H
