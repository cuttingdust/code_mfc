#include "_AFXWIN.h"
#include "_afximpl.h"
CCmdTarget::CCmdTarget()
{
}

const AFX_MSGMAP* CCmdTarget::GetMessageMap() const
{
    return &CCmdTarget::messageMap;
}

const AFX_MSGMAP CCmdTarget::messageMap = { .pBaseMap = NULL, .pEntries = &CCmdTarget::_messageEntries[0] };

const AFX_MSGMAP_ENTRY CCmdTarget::_messageEntries[] = { { 0, 0, 0, 0, 0, (AFX_PMSG)0 } };

/// 支持运行期信息识别
IMPLEMENT_DYNCREATE(CCmdTarget, CObject)
