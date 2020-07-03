#pragma once
#include "resource.h"

// CBYMapForm 대화 상자

class CBYMapForm : public CFormView
{
	DECLARE_DYNAMIC(CBYMapForm)

public:
	CBYMapForm();		// 표준 생성자입니다.
	virtual ~CBYMapForm();
	static CBYMapForm* CreateOne(CWnd* pParent);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBYMapForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_Width;
	int m_Height;
	int m_Size;
	int m_Quality;
	afx_msg void OnBnClickedButton1();
	CString m_szTextur;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void RadioCtrl(UINT id);
	UINT m_nRadio;
	UINT m_radian;
	afx_msg void OnBnClickedButton5();
};
