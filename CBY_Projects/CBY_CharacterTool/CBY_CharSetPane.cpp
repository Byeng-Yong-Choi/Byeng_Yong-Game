// CBY_CharSetPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_CharSetPane.h"


// CBY_CharSetPane

IMPLEMENT_DYNAMIC(CBY_CharSetPane, CDockablePane)

CBY_CharSetPane::CBY_CharSetPane()
{

}

CBY_CharSetPane::~CBY_CharSetPane()
{
}


BEGIN_MESSAGE_MAP(CBY_CharSetPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CBY_CharSetPane 메시지 처리기


void CBY_CharSetPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_WndTabForm)
	{
		m_WndTabForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		if (m_WndTabForm->m_StateSet)
		{
			m_WndTabForm->SetDiaWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
		}
	}

	//if (m_WndTabForm->m_WndSetForm)
	//{
		//m_WndTabForm->m_WndSetForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	//}
}


int CBY_CharSetPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_WndTabForm = CBY_SetTabForm::CreateOne(this);
	//m_WndTabForm->m_WndSetForm = CBY_CharSetFrom::CreateOne(this);
	return 0;
}


int CBY_CharSetPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(
			pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}