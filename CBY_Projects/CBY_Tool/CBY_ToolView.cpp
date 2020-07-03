
// CBY_ToolView.cpp: CCBYToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "CBY_Tool.h"
#endif

#include "CBY_ToolDoc.h"
#include "CBY_ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCBYToolView

IMPLEMENT_DYNCREATE(CCBYToolView, CView)

BEGIN_MESSAGE_MAP(CCBYToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCBYToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CCBYToolView 생성/소멸

CCBYToolView::CCBYToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CCBYToolView::~CCBYToolView()
{
}

BOOL CCBYToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CCBYToolView 그리기

void CCBYToolView::OnDraw(CDC* /*pDC*/)
{
	CCBYToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CCBYToolView 인쇄


void CCBYToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCBYToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CCBYToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CCBYToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CCBYToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCBYToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCBYToolView 진단

#ifdef _DEBUG
void CCBYToolView::AssertValid() const
{
	CView::AssertValid();
}

void CCBYToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCBYToolDoc* CCBYToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCBYToolDoc)));
	return (CCBYToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CCBYToolView 메시지 처리기
void CCBYToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	pApp->m_Sample.ResizeDevice(cx, cy);
}

LRESULT CCBYToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CCBYToolApp* pApp = (CCBYToolApp*)AfxGetApp();
	pApp->m_Sample.WndProc(m_hWnd, message, wParam, lParam);
	return CView::WindowProc(message, wParam, lParam);
}