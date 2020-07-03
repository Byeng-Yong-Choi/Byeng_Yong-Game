#pragma once


// CBY_ObjTrans 대화 상자

class CBY_ObjTrans : public CFormView
{
	DECLARE_DYNAMIC(CBY_ObjTrans)

public:
	CBY_ObjTrans();   // 표준 생성자입니다.
	virtual ~CBY_ObjTrans();
	static CBY_ObjTrans* CreateOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_ObjTrans };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_szSkinName;
	CString m_szMtrName;
	afx_msg void OnBnClickedButtonSkin1();
	afx_msg void OnBnClickedButtonMtr3();
	afx_msg void OnBnClickedButtonSkinTrans2();
	afx_msg void OnBnClickedButtonMtrTrans4();
	afx_msg void OnBnClickedButtonSkinBLoad5();
	afx_msg void OnBnClickedButtonMtrBLoad6();
	afx_msg void OnBnClickedButton7();
};
