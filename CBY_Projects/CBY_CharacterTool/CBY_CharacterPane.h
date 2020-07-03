#pragma once
#include "CBY_CharacterFrom.h"

// CBY_CharacterPane

class CBY_CharacterPane : public CDockablePane
{
	DECLARE_DYNAMIC(CBY_CharacterPane)

public:
	CBY_CharacterPane();
	virtual ~CBY_CharacterPane();
	CBY_CharacterFrom* m_WndForm;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


