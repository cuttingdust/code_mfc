/**
 * @file   CFrameWnd.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-11-12
 */

#ifndef CFRAMEWND_H
#define CFRAMEWND_H

#include <Windows.h>

class CFrameWnd
{
public:
    HWND hWnd;

public:
    int  RegisterWindow();
    void Create(LPCTSTR lpClassName, LPCTSTR lpWindowName);
    void ShowWindow(int nCmdShow);
    void UpdateWindow();
};


#endif // CFRAMEWND_H
