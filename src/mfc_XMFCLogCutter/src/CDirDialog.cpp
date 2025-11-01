#include "CDirDialog.h"

CDirDialog::CDirDialog()
{
    memset(&m_bi, 0, sizeof(m_bi));
    m_bi.hwndOwner      = NULL;
    m_bi.pidlRoot       = NULL;
    m_bi.pszDisplayName = m_szDisplay;
    m_bi.lpszTitle      = NULL;
    m_bi.ulFlags        = BIF_RETURNONLYFSDIRS;
    m_szPath[0]         = '\0';
}

CDirDialog::~CDirDialog()
{
}

BOOL CDirDialog::DoBrowser(HWND hWndParent, LPCTSTR pszTitle)
{
    if (pszTitle == NULL)
        m_bi.lpszTitle = TEXT("选择目标文件夹");
    else
        m_bi.lpszTitle = pszTitle;

    m_bi.hwndOwner     = hWndParent;
    LPITEMIDLIST pItem = ::SHBrowseForFolder(&m_bi);
    if (pItem != 0)
    {
        ::SHGetPathFromIDList(pItem, m_szPath);
        return TRUE;
    }
    return FALSE;
}

LPCTSTR CDirDialog::GetPath() const
{
    return m_szPath;
}
