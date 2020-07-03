// CBY_CharacterFrom.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_CharacterFrom.h"
#include "afxdialogex.h"


// CBY_CharacterFrom 대화 상자

IMPLEMENT_DYNAMIC(CBY_CharacterFrom, CFormView)

CBY_CharacterFrom::CBY_CharacterFrom()
	: CFormView(IDD_CBY_CharacterFrom)
	, m_szSkinName(_T(""))
	, m_szMTRName(_T(""))
{

}

CBY_CharacterFrom* CBY_CharacterFrom::CreateOne(CWnd* pParent)
{
	CBY_CharacterFrom* pForm = new CBY_CharacterFrom;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

CBY_CharacterFrom::~CBY_CharacterFrom()
{
}

void CBY_CharacterFrom::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szSkinName);
	DDX_Text(pDX, IDC_EDIT2, m_szMTRName);
	DDX_Control(pDX, IDC_LIST1, m_SkinList);
}


BEGIN_MESSAGE_MAP(CBY_CharacterFrom, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_CharacterFrom::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_CharacterFrom::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_CharacterFrom::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_CharacterFrom::OnBnClickedSkinAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBY_CharacterFrom::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CBY_CharacterFrom 메시지 처리기


void CBY_CharacterFrom::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"skn", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"skin Files(*.skn)|*.skn| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//AfxMessageBox(selFileName);
		m_szSkinName = selFileName;
		selFileName = dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		m_SkinList.AddString(selFileName);
		UpdateData(FALSE);
	}
}


void CBY_CharacterFrom::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"mtr", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"mtr Files(*.mtr)|*.mtr| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//AfxMessageBox(selFileName);
		m_szMTRName = selFileName;
		//selFileName = dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		//m_SkinList.AddString(selFileName);
		UpdateData(FALSE);
	}
}


void CBY_CharacterFrom::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	pApp->m_main.Create(m_szSkinName.GetBuffer(), m_szMTRName.GetBuffer());
}


void CBY_CharacterFrom::OnBnClickedSkinAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	pApp->m_main.SkinAdd(m_szSkinName.GetBuffer());
}


void CBY_CharacterFrom::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_SkinList.GetCurSel();
	CString str;
	m_SkinList.GetText(index, str);
}
