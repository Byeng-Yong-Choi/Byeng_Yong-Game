#pragma once
#include "CBY_MeshSkin.h"

// CBY_BoneSet 대화 상자

class CBY_BoneSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBY_BoneSet)

public:
	CBY_BoneSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBY_BoneSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Selectstr;
	std::vector<HTREEITEM> m_hRoot;
	std::vector<HTREEITEM> m_hChild;
	CTreeCtrl m_TreeCtrl;
	afx_msg void OnBnClickedButton1();
	void TreeChildSet(HTREEITEM parent, CBY_MeshSkin* obj);
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString m_XSize;
	CString m_YSize;
	CString m_ZSize;
	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedColBoxRender();
	afx_msg void RadioCtrl(UINT id);
	int m_iDirection;
	afx_msg void OnBnClickedColBoxAllRender();
	afx_msg void OnBnClickedButton8();
	int m_dirRadio;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
};
