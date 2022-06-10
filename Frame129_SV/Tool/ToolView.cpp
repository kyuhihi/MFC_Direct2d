
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "MiniView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)


	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pDevice(CDevice::Get_Instance())
	, m_iID(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	SetScrollSizes(MM_TEXT, CSize(STILECX * TILEX, STILECY * TILEY));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();


	RECT rcWnd{};

	pMainFrm->GetWindowRect(&rcWnd);

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);


	RECT rcMainView{};

	GetClientRect(&rcMainView);


	float fRowFrm = float(rcWnd.right - rcMainView.right);
	float fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	g_hWnd = m_hWnd;


	if (FAILED(m_pDevice->Initialize()))
	{
		AfxMessageBox(L"m_pDevice 생성 실패");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

	ZeroMemory(bKeyState, sizeof(bKeyState));

	m_Test = CTest::Get_Instance();

	// 대기 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Stand/AKIHA_AKI00_00%d.png", TEX_MULTI, L"Player", L"Stand", 12)))
	{
		AfxMessageBox(L"Stand Image Insert failed");
		return;
	}
	// 걷기 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Walk/AKIHA_AKI26_00%d.png", TEX_MULTI, L"Player", L"Walk", 13)))
	{
		AfxMessageBox(L"Walk Image Insert failed");
		return;
	}
	// 달리기 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png", TEX_MULTI, L"Player", L"Dash", 11)))
	{
		AfxMessageBox(L"Dash Image Insert failed");
		return;
	}

	// 대기 세팅
	for (int i = 0; i < 12; ++i)
	{
		m_Test->Set_Stand(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Stand", i), i);
	}
	// 걷기 세팅
	for (int i = 0; i < 13; ++i)
	{
		m_Test->Set_Walk(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Walk", i), i);
	}
	// 달리기 세팅
	for (int i = 0; i < 11; ++i)
	{
		m_Test->Set_Dash(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Dash", i), i);
	}
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pDevice->Render_Begin();

	m_pTerrain->Render();

	m_Test->Render();

	m_pDevice->Render_End();
}

#pragma region 불필요
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}


BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기
#pragma endregion 불필요

// CToolView 인쇄

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f), m_iID);

	Invalidate(false);

	//
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	CMiniView* pMini = dynamic_cast<CMiniView*>(pMainFrm->Get_SecondSplitter().GetPane(0, 0));

	pMini->Invalidate(false);
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
}

bool CToolView::Key_Down(int _iKey)
{
	if (!bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		bKeyState[_iKey] = !bKeyState[_iKey];

		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			bKeyState[i] = !bKeyState[i];
	}

	return false;
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		OnLButtonDown(nFlags, point);
	}

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		OnRButtonDown(nFlags, point);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete<CTerrain*>(m_pTerrain);
	CTextureMgr::Get_Instance()->Destroy_Instance();
	m_pDevice->Destroy_Instance();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
