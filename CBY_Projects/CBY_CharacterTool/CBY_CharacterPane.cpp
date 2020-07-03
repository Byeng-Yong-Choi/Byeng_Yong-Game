// CBY_CharacterPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_CharacterPane.h"


// CBY_CharacterPane

IMPLEMENT_DYNAMIC(CBY_CharacterPane, CDockablePane)

CBY_CharacterPane::CBY_CharacterPane()
{

}

CBY_CharacterPane::~CBY_CharacterPane()
{
}


BEGIN_MESSAGE_MAP(CBY_CharacterPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CBY_CharacterPane 메시지 처리기
void CBY_CharacterPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_WndForm)
	{
		m_WndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int CBY_CharacterPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_WndForm = CBY_CharacterFrom::CreateOne(this);
	return 0;
}


int CBY_CharacterPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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

