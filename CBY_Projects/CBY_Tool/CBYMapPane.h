#pragma once
#include "CBYMapForm.h"

// CBYMapPane

class CBYMapPane : public CDockablePane
{
	DECLARE_DYNAMIC(CBYMapPane)

public:
	CBYMapPane();
	virtual ~CBYMapPane();
	CBYMapForm* m_WndForm;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


