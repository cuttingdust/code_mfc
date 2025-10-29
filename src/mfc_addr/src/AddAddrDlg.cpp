// AddAddrDlg.cpp: 实现文件
//

#include "MFCAddr.h"
#include "afxdialogex.h"
#include "AddAddrDlg.h"

class AddAddrDlg::PImpl
{
public:
    PImpl(AddAddrDlg *owenr);
    ~PImpl() = default;

public:
    AddAddrDlg *owenr_ = nullptr;
    CString     add_user_;
    CString     add_address_;
};

AddAddrDlg::PImpl::PImpl(AddAddrDlg *owenr) : owenr_(owenr)
{
}


// AddAddrDlg 对话框

IMPLEMENT_DYNAMIC(AddAddrDlg, CDialogEx)

AddAddrDlg::AddAddrDlg(CWnd *pParent /*=nullptr*/) : CDialogEx(IDD_DLG_ADD_ADDR, pParent)
{
    impl_ = std::make_unique<AddAddrDlg::PImpl>(this);
}

AddAddrDlg::~AddAddrDlg()
{
}

void AddAddrDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddAddrDlg, CDialogEx)
ON_BN_CLICKED(IDC_BTN_OK, &AddAddrDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// AddAddrDlg 消息处理程序
void AddAddrDlg::OnBnClickedBtnOk()
{
    GetDlgItemText(IDC_EDIT_ADDR, impl_->add_address_);
    GetDlgItemText(IDC_EDIT_USER, impl_->add_user_);

    if (impl_->add_address_.IsEmpty() || impl_->add_user_.IsEmpty())
    {
        EndDialog(IDCANCEL);
        return;
    }
    EndDialog(IDOK);
}

CString AddAddrDlg::GetAddUser() const
{
    return impl_->add_user_;
}

CString AddAddrDlg::GetAddAddress() const
{
    return impl_->add_address_;
}
