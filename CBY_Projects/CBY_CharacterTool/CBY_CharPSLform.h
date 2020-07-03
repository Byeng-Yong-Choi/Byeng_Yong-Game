#pragma once


// CBY_CharPSLform 대화 상자

class CBY_CharPSLform : public CFormView
{
	DECLARE_DYNAMIC(CBY_CharPSLform)

public:
	CBY_CharPSLform();   // 표준 생성자입니다.
	virtual ~CBY_CharPSLform();
	static CBY_CharPSLform* CreateOne(CWnd* pParent);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_CharPSLform };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};
