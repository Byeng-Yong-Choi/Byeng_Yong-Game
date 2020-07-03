// CBY_ObjMtrSet.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_ObjMtrSet.h"
#include "afxdialogex.h"


// CBY_ObjMtrSet 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjMtrSet, CDialogEx)

CBY_ObjMtrSet::CBY_ObjMtrSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_StateName(_T(""))
	, m_MTRName(_T(""))
	, m_iState(-1)
{
	m_bBoneAni = false;
}

CBY_ObjMtrSet::~CBY_ObjMtrSet()
{
}

void CBY_ObjMtrSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_StateList);
	DDX_Control(pDX, IDC_LIST2, m_MTRList);
	DDX_Text(pDX, IDC_EDIT1, m_StateName);
	DDX_Text(pDX, IDC_EDIT2, m_MTRName);
}


BEGIN_MESSAGE_MAP(CBY_ObjMtrSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_ObjMtrSet::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_ObjMtrSet::OnBnClickedmtrAdd)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_ObjMtrSet::OnBnClickedStateAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_ObjMtrSet::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON4, &CBY_ObjMtrSet::OnBnClickedView)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBY_ObjMtrSet::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CBY_ObjMtrSet::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CBY_ObjMtrSet 메시지 처리기


void CBY_ObjMtrSet::OnBnClickedSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString selFileName;
	CFileDialog dlg(TRUE, L"mtr", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"mtr Files(*.mtr)|*.mtr| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//AfxMessageBox(selFileName);
		m_MTRName = selFileName;
		//selFileName = dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		//m_SkinList.AddString(selFileName);
		UpdateData(FALSE);
	}
}


void CBY_ObjMtrSet::OnBnClickedmtrAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.ObjectAniMTRAdd(m_MTRName.GetBuffer(), m_bBoneAni);
		m_MTRList.InsertString(m_MTRList.GetCount(), m_MTRName);
	}

	UpdateData(FALSE);
}


void CBY_ObjMtrSet::OnBnClickedStateAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_StateList.InsertString(m_StateList.GetCount(), m_StateName);
	UpdateData(FALSE);
}


void CBY_ObjMtrSet::OnBnClickedApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_iState != -1)
	{
		CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
		if (pApp->m_main.m_Character && m_MTRList.GetCount() > 0)
		{
			pApp->m_main.ObjectAniMTRApply(m_MTRName.GetBuffer(), m_iState);
		}
	}
}


void CBY_ObjMtrSet::OnBnClickedView()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CBY_ObjMtrSet::OnLbnSelchangeList1()
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


void CBY_ObjMtrSet::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int Select = m_MTRList.GetCurSel();
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character && m_MTRList.GetCount() > 0)
	{
		m_MTRList.GetText(Select, m_MTRName);
		pApp->m_main.ObjectAniMTRAdd(m_MTRName.GetBuffer(), m_bBoneAni);
	}
	UpdateData(FALSE);
}
