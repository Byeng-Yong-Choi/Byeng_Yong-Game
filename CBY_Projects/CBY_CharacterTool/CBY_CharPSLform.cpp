// CBY_CharPSLform.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_CharPSLform.h"
#include "afxdialogex.h"


// CBY_CharPSLform 대화 상자

IMPLEMENT_DYNAMIC(CBY_CharPSLform, CFormView)

CBY_CharPSLform::CBY_CharPSLform()
	: CFormView(IDD_CBY_CharPSLform)
{

}

CBY_CharPSLform::~CBY_CharPSLform()
{
}

CBY_CharPSLform* CBY_CharPSLform::CreateOne(CWnd* pParent)
{
	CBY_CharPSLform* pForm = new CBY_CharPSLform;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

void CBY_CharPSLform::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBY_CharPSLform, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_CharPSLform::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_CharPSLform::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_CharPSLform::OnBnClickedLoad)
END_MESSAGE_MAP()


// CBY_CharPSLform 메시지 처리기


void CBY_CharPSLform::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	pApp->m_main.SetCharacterPlay();
}


void CBY_CharPSLform::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	CString selFileName;
	CFileDialog dlg(FALSE, L"txt", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"txt Files(*.txt)|*.txt| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		pApp->m_main.Save(selFileName.GetBuffer());
		UpdateData(FALSE);
	}
}


void CBY_CharPSLform::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	CString selFileName;
	CFileDialog dlg(TRUE, L"txt", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"txt Files(*.txt)|*.txt| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		pApp->m_main.Load(selFileName.GetBuffer());
		UpdateData(FALSE);
	}
}
