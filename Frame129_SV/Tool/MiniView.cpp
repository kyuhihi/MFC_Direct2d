// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Tree.h"
#include "Sheep.h"
// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

	CTerrain* pTerrain = pView->Get_Terrain();
	CTree* pTree = pView->Get_Tree();
	CSheep* pSheep = pView->Get_Sheep();
	pTerrain->Mini_Render();
	pTree->Mini_Render();
	pSheep->Mini_Render();
	CDevice::Get_Instance()->Render_End(m_hWnd);
}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.
