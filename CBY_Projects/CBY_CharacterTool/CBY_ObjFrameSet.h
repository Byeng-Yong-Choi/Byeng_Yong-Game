#pragma once


// CBY_ObjFrameSet 대화 상자

class CBY_ObjFrameSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_ObjFrameSet)

public:
	CBY_ObjFrameSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_ObjFrameSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_CharSetFrom };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
