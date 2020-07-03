// CBY_ObjTrans.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_ObjTool.h"
#include "CBY_ObjTrans.h"
#include "afxdialogex.h"


// CBY_ObjTrans 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjTrans, CFormView)

CBY_ObjTrans::CBY_ObjTrans()
	: CFormView(IDD_CBY_ObjTrans)
	, m_szSkinName(_T(""))
	, m_szMtrName(_T(""))
{

}
CBY_ObjTrans* CBY_ObjTrans::CreateOne(CWnd* pParent)
{
	CBY_ObjTrans* pForm = new CBY_ObjTrans;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

CBY_ObjTrans::~CBY_ObjTrans()
{
}

void CBY_ObjTrans::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szSkinName);
	DDX_Text(pDX, IDC_EDIT2, m_szMtrName);
}


BEGIN_MESSAGE_MAP(CBY_ObjTrans, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_ObjTrans::OnBnClickedButtonSkin1)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_ObjTrans::OnBnClickedButtonMtr3)
	ON_BN_CLICKED(IDC_BUTTON2, &CBY_ObjTrans::OnBnClickedButtonSkinTrans2)
	ON_BN_CLICKED(IDC_BUTTON4, &CBY_ObjTrans::OnBnClickedButtonMtrTrans4)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_ObjTrans::OnBnClickedButtonSkinBLoad5)
	ON_BN_CLICKED(IDC_BUTTON6, &CBY_ObjTrans::OnBnClickedButtonMtrBLoad6)
	ON_BN_CLICKED(IDC_BUTTON7, &CBY_ObjTrans::OnBnClickedButton7)
END_MESSAGE_MAP()


// CBY_ObjTrans 메시지 처리기


void CBY_ObjTrans::OnBnClickedButtonSkin1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"skn", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"skin Files(*.skn)|*.skn| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		m_szSkinName = selFileName;
		selFileName = dlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CBY_ObjTrans::OnBnClickedButtonMtr3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"mtr", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"mtr Files(*.mtr)|*.mtr| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		m_szMtrName = selFileName;
		UpdateData(FALSE);
	}
}


void CBY_ObjTrans::OnBnClickedButtonSkinTrans2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYObjToolApp* pApp = (CCBYObjToolApp*)AfxGetApp();
	pApp->m_main.SkinParser(m_szSkinName.GetBuffer());
}


void CBY_ObjTrans::OnBnClickedButtonMtrTrans4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYObjToolApp* pApp = (CCBYObjToolApp*)AfxGetApp();
	pApp->m_main.MtrParser(m_szMtrName.GetBuffer());
}


void CBY_ObjTrans::OnBnClickedButtonSkinBLoad5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYObjToolApp* pApp = (CCBYObjToolApp*)AfxGetApp();
	pApp->m_main.SkinBLoad(m_szSkinName.GetBuffer());
}


void CBY_ObjTrans::OnBnClickedButtonMtrBLoad6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYObjToolApp* pApp = (CCBYObjToolApp*)AfxGetApp();
	pApp->m_main.MtrBLoad(m_szMtrName.GetBuffer());
}


void CBY_ObjTrans::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYObjToolApp* pApp = (CCBYObjToolApp*)AfxGetApp();
	pApp->m_main.TestCreate(m_szSkinName.GetBuffer(), m_szMtrName.GetBuffer());
}
