#pragma once


// CBY_CharStateSet 대화 상자

class CBY_CharStateSet : public CDialog
{
	DECLARE_DYNAMIC(CBY_CharStateSet)

public:
	CBY_CharStateSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_CharStateSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_CharSetFrom };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString m_StateSet;
	int m_iSelectState;

public:
	int m_iState;
	int m_iCharType;
	UINT m_Radio;
	afx_msg void RadioCtrl(UINT id);
	DWORD m_StartFrame;
	DWORD m_EndFrame;
	afx_msg void OnBnClickedIdle();
	afx_msg void OnBnClickedMove();
	CString m_StateName;
	CListBox m_StateList;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton3();
	CSliderCtrl m_Slider;
	CString m_SliderVal;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit4();
};
