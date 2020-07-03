#pragma once


// CBY_ObjectLoad 대화 상자

class CBY_ObjectLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_ObjectLoad)

public:
	CBY_ObjectLoad(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_ObjectLoad();
	void LoadResource(CString str);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_ObjectLoad };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
};
