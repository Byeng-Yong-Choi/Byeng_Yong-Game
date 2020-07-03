#pragma once

//#include "CBY_CharSetFrom.h"
// CBY_SetTabForm 대화 상자
#include "CBY_CharStateSet.h"
#include "CBY_MTRStateSet.h"
#include "CBY_BoneSet.h"
#include "CBY_ObjSet.h"
#include "CBY_ObjMtrSet.h"
#include "CBY_ObjSoket.h"

class CBY_SetTabForm : public CFormView
{
	DECLARE_DYNAMIC(CBY_SetTabForm)

public:
	CBY_SetTabForm();   // 표준 생성자입니다.
	virtual ~CBY_SetTabForm();

	static CBY_SetTabForm* CreateOne(CWnd* pParent);

public:
	CBY_CharStateSet m_StateSet;
	CBY_MTRStateSet m_MTRSet;
	CBY_BoneSet m_BoneSet;
	CBY_ObjSet m_ObjSet;
	CBY_ObjMtrSet m_ObjMtr;
	CBY_ObjSoket m_ObjSocket;

	CWnd* m_pWnd;

public:
	BOOL SetDiaWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_SetTabForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};