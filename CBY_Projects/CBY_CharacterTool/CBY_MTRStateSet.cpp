// CBY_MTRStateSet.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_MTRStateSet.h"
#include "afxdialogex.h"


// CBY_MTRStateSet 대화 상자

IMPLEMENT_DYNAMIC(CBY_MTRStateSet, CDialogEx)

CBY_MTRStateSet::CBY_MTRStateSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_State(_T(""))
	, m_Mtr(_T(""))
	, m_iState(-1)
{
	m_bBoneAni = false;
}

CBY_MTRStateSet::~CBY_MTRStateSet()
{
}

void CBY_MTRStateSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_StateList);
	DDX_Control(pDX, IDC_LIST2, m_mtrlist);
	DDX_Text(pDX, IDC_EDIT1, m_State);
	DDX_Text(pDX, IDC_EDIT2, m_Mtr);
}


BEGIN_MESSAGE_MAP(CBY_MTRStateSet, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBY_MTRStateSet::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CBY_MTRStateSet::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_MTRStateSet::OnBnClickedStateBt)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_MTRStateSet::OnBnClickedMTRbt)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_MTRStateSet::OnBnClickedfind)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_MTRStateSet::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CBY_MTRStateSet::OnBnClickedButton4)
END_MESSAGE_MAP()


// CBY_MTRStateSet 메시지 처리기


void CBY_MTRStateSet::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int Select = m_StateList.GetCurSel();
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character && m_StateList.GetCount() > 0)
	{
		m_iState = Select;
	}
	UpdateData(FALSE);
}


void CBY_MTRStateSet::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int Select = m_mtrlist.GetCurSel();
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character && m_mtrlist.GetCount()>0)
	{
		m_mtrlist.GetText(Select, m_Mtr);
		pApp->m_main.AniMTRAdd(m_Mtr.GetBuffer(),m_bBoneAni);
	}
	UpdateData(FALSE);
}


void CBY_MTRStateSet::OnBnClickedStateBt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_StateList.InsertString(m_StateList.GetCount(), m_State);
	UpdateData(FALSE);

}


void CBY_MTRStateSet::OnBnClickedMTRbt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.AniMTRAdd(m_Mtr.GetBuffer(), m_bBoneAni);
		m_mtrlist.InsertString(m_mtrlist.GetCount(), m_Mtr);
	}
	
	UpdateData(FALSE);
}


void CBY_MTRStateSet::OnBnClickedfind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString selFileName;
	CFileDialog dlg(TRUE, L"mtr", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"mtr Files(*.mtr)|*.mtr| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//AfxMessageBox(selFileName);
		m_Mtr = selFileName;
		//selFileName = dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		//m_SkinList.AddString(selFileName);
		UpdateData(FALSE);
	}
}


void CBY_MTRStateSet::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_iState != -1)
	{
		CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
		if (pApp->m_main.m_Character && m_mtrlist.GetCount() > 0)
		{
			pApp->m_main.AniMTRApply(m_Mtr.GetBuffer(),m_iState);
		}
	}
}


void CBY_MTRStateSet::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character && m_mtrlist.GetCount() > 0)
	{
		pApp->m_main.GetCharAction(m_iState);
	}
}
