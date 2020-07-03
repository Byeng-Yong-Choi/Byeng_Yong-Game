#pragma once


// CBY_Save 대화 상자

class CBY_Save : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_Save)

public:
	CBY_Save(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_Save();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_Save };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
};
