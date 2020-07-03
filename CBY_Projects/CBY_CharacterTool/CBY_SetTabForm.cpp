// CBY_SetTabForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_SetTabForm.h"
#include "afxdialogex.h"


// CBY_SetTabForm 대화 상자

IMPLEMENT_DYNAMIC(CBY_SetTabForm, CFormView)

CBY_SetTabForm::CBY_SetTabForm()
	: CFormView(IDD_CBY_SetTabForm)
{
}

CBY_SetTabForm::~CBY_SetTabForm()
{
}

CBY_SetTabForm* CBY_SetTabForm::CreateOne(CWnd* pParent)
{
	CBY_SetTabForm* pForm = new CBY_SetTabForm;
	CRect rect = CRect(0, 0, 500, 500);
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		rect, pParent, 0, NULL);
	
	return pForm;
}


BOOL CBY_SetTabForm::SetDiaWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	//CRect rect;
	m_Tab.SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
	//m_Tab.GetWindowRect(&rect);
	m_StateSet.SetWindowPos(pWndInsertAfter, x, y+25, cx, cy, nFlags);
	m_MTRSet.SetWindowPos(pWndInsertAfter, x, y+25, cx, cy, nFlags);
	m_BoneSet.SetWindowPos(pWndInsertAfter, x, y + 25, cx, cy, nFlags);
	m_ObjSet.SetWindowPos(pWndInsertAfter, x, y + 25, cx, cy, nFlags);
	m_ObjMtr.SetWindowPos(pWndInsertAfter, x, y + 25, cx, cy, nFlags);
	m_ObjSocket.SetWindowPos(pWndInsertAfter, x, y + 25, cx, cy, nFlags);
	//this->ShowWindow(SW_HIDE);
	return TRUE;
}

void CBY_SetTabForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CBY_SetTabForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBY_SetTabForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CBY_SetTabForm 메시지 처리기



void CBY_SetTabForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//this->ShowWindow(SW_HIDE);
	if (m_pWnd != NULL)
	{
		m_pWnd->ShowWindow(SW_HIDE);
		m_pWnd = NULL;
	}

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
	{
		m_StateSet.ShowWindow(SW_SHOW);
		m_pWnd = &m_StateSet;
	}
	break;

	case 1:
	{
		m_MTRSet.ShowWindow(SW_SHOW);
		m_pWnd = &m_MTRSet;
	}
	break;

	case 2:
	{
		m_BoneSet.ShowWindow(SW_SHOW);
		m_pWnd = &m_BoneSet;
	}
	break;

	case 3:
	{
		m_ObjSet.ShowWindow(SW_SHOW);
		m_pWnd = &m_ObjSet;
	}
	break;

	case 4:
	{
		m_ObjMtr.ShowWindow(SW_SHOW);
		m_pWnd = &m_ObjMtr;
	}
	break;

	case 5:
	{
		m_ObjSocket.ShowWindow(SW_SHOW);
		m_pWnd = &m_ObjSocket;
	}
	break;
	}
	*pResult = 0;
}


void CBY_SetTabForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rect;

	m_Tab.DeleteAllItems();
	m_Tab.InsertItem(0, _T("FRAME"));
	m_Tab.InsertItem(1, _T("MTR"));
	m_Tab.InsertItem(2, _T("Bone"));
	m_Tab.InsertItem(3, _T("Object"));
	m_Tab.InsertItem(4, _T("Obj_mtr"));
	m_Tab.InsertItem(5, _T("Obj_Sock"));

	m_Tab.SetCurSel(0);
	m_Tab.GetWindowRect(&rect);

	BOOL rt = TRUE;

	rt = m_StateSet.Create(IDD_CBY_CharSetFrom, &m_Tab);
	//rt = pForm->m_StateSet.Create(IDD_CBY_CharSetFrom, pForm);
	m_StateSet.MoveWindow(rect.left, rect.top + 25, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
	rt = m_StateSet.ShowWindow(SW_SHOW);

	rt = m_MTRSet.Create(IDD_DIALOG1, &m_Tab);
	//rt = pForm->m_BoneSet.Create(IDD_DIALOG1, pForm);
	m_MTRSet.MoveWindow(rect.left, rect.top + 25, rect.Width(), rect.Height(), SWP_NOZORDER);
	rt = m_MTRSet.ShowWindow(SW_HIDE);


	rt = m_BoneSet.Create(IDD_DIALOG2, &m_Tab);
	//rt = pForm->m_BoneSet.Create(IDD_DIALOG1, pForm);
	m_BoneSet.MoveWindow(rect.left, rect.top + 25, rect.Width(), rect.Height(), SWP_NOZORDER);
	rt = m_BoneSet.ShowWindow(SW_HIDE);

	rt = m_ObjSet.Create(IDD_DIALOG3, &m_Tab);
	//rt = pForm->m_BoneSet.Create(IDD_DIALOG1, pForm);
	m_ObjSet.MoveWindow(rect.left, rect.top + 25, rect.Width(), rect.Height(), SWP_NOZORDER);
	rt = m_ObjSet.ShowWindow(SW_HIDE);
	
	rt = m_ObjMtr.Create(IDD_DIALOG4, &m_Tab);
	//rt = pForm->m_BoneSet.Create(IDD_DIALOG1, pForm);
	m_ObjMtr.MoveWindow(rect.left, rect.top + 25, rect.Width(), rect.Height(), SWP_NOZORDER);
	rt = m_ObjMtr.ShowWindow(SW_HIDE);

	rt = m_ObjSocket.Create(IDD_DIALOG5, &m_Tab);
	//rt = pForm->m_BoneSet.Create(IDD_DIALOG1, pForm);
	m_ObjSocket.MoveWindow(rect.left, rect.top + 25, rect.Width(), rect.Height(), SWP_NOZORDER);
	rt = m_ObjSocket.ShowWindow(SW_HIDE);

	
	m_pWnd = &m_StateSet;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
