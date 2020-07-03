#pragma once


// CBY_ObjView 대화 상자

class CBY_ObjView : public CFormView
{
	DECLARE_DYNAMIC(CBY_ObjView)

public:
	CBY_ObjView();   // 표준 생성자입니다.
	virtual ~CBY_ObjView();
	static CBY_ObjView* CreateOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_ObjView };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_SkinName;
	CString m_MtrName;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
};
