// CBY_CharStateSet.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_CharStateSet.h"
#include "afxdialogex.h"


// CBY_CharStateSet 대화 상자

IMPLEMENT_DYNAMIC(CBY_CharStateSet, CDialog)

CBY_CharStateSet::CBY_CharStateSet(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CBY_CharSetFrom, pParent)
	, m_Radio(0)
	, m_StartFrame(0)
	, m_EndFrame(0)
	, m_StateName(_T(""))
	, m_SliderVal(_T(""))
{

}

CBY_CharStateSet::~CBY_CharStateSet()
{
}

void CBY_CharStateSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, (int&)m_Radio);
	DDX_Text(pDX, IDC_EDIT1, m_StartFrame);
	DDX_Text(pDX, IDC_EDIT2, m_EndFrame);
	DDX_Text(pDX, IDC_EDIT3, m_StateName);
	DDX_Control(pDX, IDC_LIST1, m_StateList);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Text(pDX, IDC_EDIT4, m_SliderVal);
}


BEGIN_MESSAGE_MAP(CBY_CharStateSet, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &CBY_CharStateSet::RadioCtrl)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_CharStateSet::OnBnClickedIdle)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_CharStateSet::OnBnClickedMove)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBY_CharStateSet::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_CharStateSet::OnBnClickedButton3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CBY_CharStateSet::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT4, &CBY_CharStateSet::OnEnChangeEdit4)
END_MESSAGE_MAP()


// CBY_CharStateSet 메시지 처리기
void CBY_CharStateSet::RadioCtrl(UINT id)
{
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	switch (m_Radio)
	{
	case 0:
	{
		m_iState = -1;
	}
	break;

	case 1:
	{
		m_iState = 1;
	}
	break;
	}
	UpdateData(FALSE);
}


void CBY_CharStateSet::OnBnClickedIdle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_StateList.InsertString(m_StateList.GetCount(),m_StateName);
	UpdateData(FALSE);
}


void CBY_CharStateSet::OnBnClickedMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (m_iState == -1)
	{
		
		pApp->m_main.SetCharAction(m_iSelectState, m_StartFrame, m_EndFrame);
		return;
	}
	pApp->m_main.GetCharAction(m_iSelectState);
}

void CBY_CharStateSet::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iSelectState = m_StateList.GetCurSel();
	m_StateList.GetText(m_iSelectState, m_StateSet);
	UpdateData(FALSE);
}


void CBY_CharStateSet::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		int last = pApp->m_main.m_Character->m_Scene.iLastFrame;
		m_Slider.SetRange(0, last);
		m_Slider.SetPos(0);
		m_Slider.SetLineSize(1);
		m_Slider.SetPageSize(10);
	}
	UpdateData(FALSE);
}


void CBY_CharStateSet::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iPos = m_Slider.GetPos();

	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_SliderVal= sPos;

	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.m_Character->SetNoStateFrameTime(iPos, iPos);
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CBY_CharStateSet::OnEnChangeEdit4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iPos = _ttoi(m_SliderVal);

	m_Slider.SetPos(iPos);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	if (pApp->m_main.m_Character)
	{
		pApp->m_main.m_Character->SetNoStateFrameTime(iPos, iPos);
	}
	UpdateData(FALSE);
}
