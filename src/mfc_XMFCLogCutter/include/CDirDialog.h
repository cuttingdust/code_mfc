/**
 * @file   CDirDialog.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-31
 */

#ifndef CDIRDIALOG_H
#define CDIRDIALOG_H

#include <ShlObj.h>

class CDirDialog
{
public:
    CDirDialog();
    virtual ~CDirDialog();

public:
    BOOL DoBrowser(HWND hWndParent, LPCTSTR pszTitle = NULL);


    /// \brief 取得用户选择目录名称
    /// \return
    LPCTSTR GetPath() const;

protected:
    BROWSEINFOW m_bi;

    wchar_t m_szDisplay[MAX_PATH];
    wchar_t m_szPath[MAX_PATH];
};


#endif // CDIRDIALOG_H
