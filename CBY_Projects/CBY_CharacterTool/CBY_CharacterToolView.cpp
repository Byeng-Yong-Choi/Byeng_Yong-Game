
// CBY_CharacterToolView.cpp: CCBYCharacterToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "CBY_CharacterTool.h"
#endif

#include "CBY_CharacterToolDoc.h"
#include "CBY_CharacterToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCBYCharacterToolView

IMPLEMENT_DYNCREATE(CCBYCharacterToolView, CView)

BEGIN_MESSAGE_MAP(CCBYCharacterToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCBYCharacterToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCBYCharacterToolView 생성/소멸

CCBYCharacterToolView::CCBYCharacterToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CCBYCharacterToolView::~CCBYCharacterToolView()
{
}

BOOL CCBYCharacterToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CCBYCharacterToolView 그리기

void CCBYCharacterToolView::OnDraw(CDC* /*pDC*/)
{
	CCBYCharacterToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CCBYCharacterToolView 인쇄


void CCBYCharacterToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCBYCharacterToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CCBYCharacterToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CCBYCharacterToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CCBYCharacterToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCBYCharacterToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCBYCharacterToolView 진단

#ifdef _DEBUG
void CCBYCharacterToolView::AssertValid() const
{
	CView::AssertValid();
}

void CCBYCharacterToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCBYCharacterToolDoc* CCBYCharacterToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCBYCharacterToolDoc)));
	return (CCBYCharacterToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CCBYCharacterToolView 메시지 처리기
void CCBYCharacterToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	pApp->m_main.ResizeDevice(cx, cy);
	pApp->m_main.SetClientCenter(cx, cy);
}

LRESULT CCBYCharacterToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CCBYCharacterToolApp* pApp = (CCBYCharacterToolApp*)AfxGetApp();
	pApp->m_main.WndProc(m_hWnd, message, wParam, lParam);
	return CView::WindowProc(message, wParam, lParam);
}