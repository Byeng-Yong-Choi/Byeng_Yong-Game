// CBY_Save.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_Tool.h"
#include "CBY_Save.h"
#include "afxdialogex.h"


// CBY_Save 대화 상자

IMPLEMENT_DYNAMIC(CBY_Save, CDialogEx)

CBY_Save::CBY_Save(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBY_Save, pParent)
{

}

CBY_Save::~CBY_Save()
{
}

void CBY_Save::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBY_Save, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_Save::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CBY_Save::OnBnClickedButton4)
END_MESSAGE_MAP()


// CBY_Save 메시지 처리기


void CBY_Save::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString selFileName;
	CFileDialog dlg(
		FALSE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);
	//UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//selFileName += dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		pApp->m_Sample.SaveFile(selFileName.GetBuffer(), 0);
		AfxMessageBox(selFileName);
	}
}


void CBY_Save::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString selFileName;
	CFileDialog dlg(
		FALSE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);
	//UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//selFileName += dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		pApp->m_Sample.SaveFile(selFileName.GetBuffer(), 1);
		AfxMessageBox(selFileName);
	}
}
