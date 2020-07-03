#pragma once


// CBY_ObjSet 대화 상자

class CBY_ObjSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_ObjSet)

public:
	CBY_ObjSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_ObjSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_ObjName;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedmtrbutton();
	CString m_mtrname;
};
