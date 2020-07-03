#pragma once
#include "resource.h"

// CBY_CharacterFrom 대화 상자

class CBY_CharacterFrom : public CFormView
{
	DECLARE_DYNAMIC(CBY_CharacterFrom)

public:
	CBY_CharacterFrom();   // 표준 생성자입니다.
	virtual ~CBY_CharacterFrom();
	static CBY_CharacterFrom* CreateOne(CWnd* pParent);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_CharacterFrom };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_szSkinName;
	CString m_szMTRName;
	CListBox m_SkinList;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedSkinAdd();
	afx_msg void OnLbnSelchangeList1();
};
