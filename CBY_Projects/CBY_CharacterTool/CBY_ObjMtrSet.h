#pragma once


// CBY_ObjMtrSet 대화 상자

class CBY_ObjMtrSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_ObjMtrSet)

public:
	CBY_ObjMtrSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_ObjMtrSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iState;
	bool m_bBoneAni;
	CListBox m_StateList;
	CListBox m_MTRList;
	CString m_StateName;
	CString m_MTRName;
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedmtrAdd();
	afx_msg void OnBnClickedStateAdd();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedView();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
};
