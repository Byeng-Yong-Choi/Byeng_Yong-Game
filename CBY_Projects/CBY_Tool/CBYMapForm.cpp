// CBYMapForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_Tool.h"
#include "CBYMapForm.h"
#include "afxdialogex.h"


// CBYMapForm 대화 상자

IMPLEMENT_DYNAMIC(CBYMapForm, CFormView)


CBYMapForm::CBYMapForm() : CFormView(IDD_CBYMapForm)
, m_Width(0)
, m_Height(0)
, m_Size(0)
, m_Quality(0)
, m_szTextur(_T(""))
, m_nRadio(3)
, m_radian(0)
{

}

CBYMapForm::~CBYMapForm()
{

}

CBYMapForm* CBYMapForm::CreateOne(CWnd* pParent)
{
	CBYMapForm* pForm = new CBYMapForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

void CBYMapForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Width);
	DDX_Text(pDX, IDC_EDIT2, m_Height);
	DDX_Text(pDX, IDC_EDIT3, m_Size);
	DDX_Text(pDX, IDC_EDIT4, m_Quality);
	DDX_Text(pDX, IDC_EDIT5, m_szTextur);
	DDX_Radio(pDX, IDC_RADIO1, (int&)m_nRadio);
	DDX_Text(pDX, IDC_EDIT6, m_radian);
}


BEGIN_MESSAGE_MAP(CBYMapForm, CFormView)
	ON_BN_CLICKED(IDOK, &CBYMapForm::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CBYMapForm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBYMapForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBYMapForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CBYMapForm::OnBnClickedButton5)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO5, &CBYMapForm::RadioCtrl)
END_MESSAGE_MAP()


// CBYMapForm 메시지 처리기

void CBYMapForm::OnBnClickedOk()
{
	UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	pApp->m_Sample.Create(m_Width, m_Height, m_Size, m_Quality, m_szTextur.GetBuffer());
}


void CBYMapForm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(FALSE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//selFileName += dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		//AfxMessageBox(selFileName);
		m_szTextur = selFileName;
		UpdateData(FALSE);
	}
}


void CBYMapForm::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	pApp->m_Sample.Tile(m_szTextur.GetBuffer(),m_Quality);

}


void CBYMapForm::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	pApp->m_Sample.Splatting(m_szTextur.GetBuffer(), m_Quality);
}


void CBYMapForm::RadioCtrl(UINT id)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	int i;
	switch (m_nRadio)
	{
	case 0:
	{
		i = 1;
	}
	break;

	case 1:
	{
		i = -1;
	}
	break;

	case 2:
	{
		i = 0;
	}
	break;

	case 3:
	{
		i = 3;
	}
	break;

	case 4:
	{
		i = 4;
	}
	break;
	}

	pApp->m_Sample.SetHeightMode(i);
}


void CBYMapForm::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	pApp->m_Sample.SetRadian(m_radian);
}
