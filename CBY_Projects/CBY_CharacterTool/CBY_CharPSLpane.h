#pragma once

#include "CBY_CharPSLform.h"
// CBY_CharPSLpane

class CBY_CharPSLpane : public CDockablePane
{
	DECLARE_DYNAMIC(CBY_CharPSLpane)

public:
	CBY_CharPSLform* m_WndSetForm;
	CBY_CharPSLpane();
	virtual ~CBY_CharPSLpane();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


