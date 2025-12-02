#pragma once
#include "stdafx.h"
class CStroke : public CObject
{
public:
    CStroke(UINT nPenWidth);

protected:
    CStroke();
    DECLARE_SERIAL(CStroke)
protected:
    UINT m_nPenWidth;

public:
    CArray<CPoint, CPoint> m_pointArray;

public:
    BOOL DrawStroke(CDC *pDC);

public:
    void Serialize(CArchive &ar) override;
};
