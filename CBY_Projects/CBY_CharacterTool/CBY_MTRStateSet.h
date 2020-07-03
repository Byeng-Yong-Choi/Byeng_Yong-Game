#pragma once


// CBY_MTRStateSet 대화 상자

class CBY_MTRStateSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_MTRStateSet)

public:
	CBY_MTRStateSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_MTRStateSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	bool m_bBoneAni;
	CListBox m_StateList;
	CListBox m_mtrlist;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
	CString m_State;
	CString m_Mtr;
	int m_iState;
	afx_msg void OnBnClickedStateBt();
	afx_msg void OnBnClickedMTRbt();
	afx_msg void OnBnClickedfind();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
