// CBY_ObjView.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_ObjTool.h"
#include "CBY_ObjView.h"
#include "afxdialogex.h"


// CBY_ObjView 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjView, CFormView)

CBY_ObjView::CBY_ObjView()
	: CFormView(IDD_CBY_ObjView)
	, m_SkinName(_T(""))
	, m_MtrName(_T(""))
{

}

CBY_ObjView* CBY_ObjView::CreateOne(CWnd* pParent)
{
	CBY_ObjView* pForm = new CBY_ObjView;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

CBY_ObjView::~CBY_ObjView()
{
}

void CBY_ObjView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_SkinName);
	DDX_Text(pDX, IDC_EDIT2, m_MtrName);
}


BEGIN_MESSAGE_MAP(CBY_ObjView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CBY_ObjView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CBY_ObjView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CBY_ObjView::OnBnClickedButton5)
END_MESSAGE_MAP()


// CBY_ObjView 메시지 처리기


void CBY_ObjView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"skn", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"skin Files(*.skn)|*.skn| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		m_SkinName = selFileName;
		selFileName = dlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CBY_ObjView::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString selFileName;
	CFileDialog dlg(TRUE, L"mtr", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"mtr Files(*.mtr)|*.mtr| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		m_MtrName = selFileName;
		UpdateData(FALSE);
	}
}


void CBY_ObjView::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CCBYObjToolApp* pApp = (CCBYObjToolApp*)AfxGetApp();
	pApp->m_main.ObjCreate(m_SkinName.GetBuffer(), m_MtrName.GetBuffer());
}
