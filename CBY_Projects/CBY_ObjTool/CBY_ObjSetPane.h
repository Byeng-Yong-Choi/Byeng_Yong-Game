#pragma once
#include "CBY_ObjTrans.h"

// CBY_ObjSetPane

class CBY_ObjSetPane : public CDockablePane
{
	DECLARE_DYNAMIC(CBY_ObjSetPane)
public:
	CBY_ObjSetPane();
	virtual ~CBY_ObjSetPane();
	CBY_ObjTrans* m_WndForm;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


