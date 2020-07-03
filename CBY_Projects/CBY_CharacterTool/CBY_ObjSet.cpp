// CBY_ObjSet.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_ObjSet.h"
#include "afxdialogex.h"


// CBY_ObjSet 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjSet, CDialogEx)

CBY_ObjSet::CBY_ObjSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_ObjName(_T(""))
	, m_mtrname(_T(""))
{

}

CBY_ObjSet::~CBY_ObjSet()
{
}

void CBY_ObjSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ObjName);
	DDX_Text(pDX, IDC_EDIT2, m_mtrname);
}


BEGIN_MESSAGE_MAP(CBY_ObjSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_ObjSet::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_ObjSet::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_ObjSet::OnBnClickedmtrbutton)
END_MESSAGE_MAP()


// CBY_ObjSet 메시지 처리기


void CBY_ObjSet::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"skn", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"skn Files(*.skn)|*.skn| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//AfxMessageBox(selFileName);
		m_ObjName = selFileName;
		selFileName = dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		UpdateData(FALSE);
	}
}


void CBY_ObjSet::OnBnClickedAdd()
{
	UpdateData(TRUE);
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	pApp->m_main.ObjCreate(m_ObjName.GetBuffer(),m_mtrname.GetBuffer());
}



void CBY_ObjSet::OnBnClickedmtrbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"skn", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"mtr Files(*.mtr)|*.mtr| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//AfxMessageBox(selFileName);
		m_mtrname = selFileName;
		selFileName = dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		UpdateData(FALSE);
	}
}
