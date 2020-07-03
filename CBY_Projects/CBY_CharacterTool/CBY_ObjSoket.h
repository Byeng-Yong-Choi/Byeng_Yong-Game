#pragma once


// CBY_ObjSoket 대화 상자

class CBY_ObjSoket : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_ObjSoket)

public:
	CBY_ObjSoket(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_ObjSoket();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Selectstr;
	std::vector<HTREEITEM> m_hRoot;
	std::vector<HTREEITEM> m_hChild;
	void TreeChildSet(HTREEITEM parent, CBY_MeshSkin* obj);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_BoneList;
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CString m_Yaw;
	CString m_Pitch;
	CString m_MoveX;
	CString m_MoveY;
	CString m_MoveZ;
	CString m_Scale;
	CString m_Roll;
};
