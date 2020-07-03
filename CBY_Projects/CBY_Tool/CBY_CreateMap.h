#pragma once


// CBY_CreateMap 대화 상자

class CBY_CreateMap : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_CreateMap)

public:
	CBY_CreateMap(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_CreateMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBY_CreateMap };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
};
