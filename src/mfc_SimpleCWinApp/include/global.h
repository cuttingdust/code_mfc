#pragma once

/// 需要包含的头文件---------------------------------------------------------------------------
#include <Windows.h>

/// 定义全局变量和函数------------------------------------------------------------------------
extern HINSTANCE      hInst;
extern HINSTANCE      hInstance;
extern MSG            msg;
extern wchar_t        lpszClassName[];
extern const wchar_t *ShowText;
/// 声明函数原型--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); ///窗口函数
void             OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void             OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void             OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
