#pragma once

#include "CBY_SetTabForm.h"
// CBY_CharSetPane

class CBY_CharSetPane : public CDockablePane
{
	DECLARE_DYNAMIC(CBY_CharSetPane)

public:
	CBY_SetTabForm* m_WndTabForm;

public:
	CBY_CharSetPane();
	virtual ~CBY_CharSetPane();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


