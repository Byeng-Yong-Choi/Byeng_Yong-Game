#pragma once
#include "CBY_ObjView.h"

// CBY_ObjPane

class CBY_ObjPane : public CDockablePane
{
	DECLARE_DYNAMIC(CBY_ObjPane)
public:
	CBY_ObjPane();
	virtual ~CBY_ObjPane();
	CBY_ObjView* m_WndForm;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


