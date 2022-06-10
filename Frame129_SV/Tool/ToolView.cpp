
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CToolView ����/�Ҹ�

CToolView::CToolView()
	: m_pDevice(CDevice::Get_Instance())
	, m_iID(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
		AfxMessageBox(L"m_pDevice ���� ����");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

	ZeroMemory(bKeyState, sizeof(bKeyState));

	m_Test = CTest::Get_Instance();

	// ��� �ؽ���
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Stand/AKIHA_AKI00_00%d.png", TEX_MULTI, L"Player", L"Stand", 12)))
	{
		AfxMessageBox(L"Stand Image Insert failed");
		return;
	}
	// �ȱ� �ؽ���
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Walk/AKIHA_AKI26_00%d.png", TEX_MULTI, L"Player", L"Walk", 13)))
	{
		AfxMessageBox(L"Walk Image Insert failed");
		return;
	}
	// �޸��� �ؽ���
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png", TEX_MULTI, L"Player", L"Dash", 11)))
	{
		AfxMessageBox(L"Dash Image Insert failed");
		return;
	}

	// ��� ����
	for (int i = 0; i < 12; ++i)
	{
		m_Test->Set_Stand(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Stand", i), i);
	}
	// �ȱ� ����
	for (int i = 0; i < 13; ++i)
	{
		m_Test->Set_Walk(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Walk", i), i);
	}
	// �޸��� ����
	for (int i = 0; i < 11; ++i)
	{
		m_Test->Set_Dash(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Dash", i), i);
	}
}

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	m_pDevice->Render_Begin();

	m_pTerrain->Render();

	m_Test->Render();

	m_pDevice->Render_End();
}

#pragma region ���ʿ�
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}


BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����
#pragma endregion ���ʿ�

// CToolView �μ�

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
