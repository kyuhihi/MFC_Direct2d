
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Device.h"
#include "Terrain.h"
#include "Tree.h"

class CToolDoc;
class CToolView : public CScrollView 
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CTerrain* Get_Terrain(void) { return m_pTerrain; }
	CTree* Get_Tree(void) { return m_pTree; }

	void Set_ID(int _iID) {
		ResetBool();
		m_pTree->Set_MouseReset();
		m_bBoolMgr[BOOL_TILE] = true;
		m_iID = _iID;
	}
	void Set_TreeID(int _iID) {
		ResetBool();
		//m_pTerrain->Set_MouseReset();
		m_bBoolMgr[BOOL_TREE] = true;
		m_iTreeType = _iID;
	}
	void ResetBool() { for (int i = 0; i < BOOL_END; ++i) m_bBoolMgr[i] = false; }
	bool Key_Down(int _iKey);

private:
	CDevice* m_pDevice;
	CTerrain* m_pTerrain;
	CTree*	 m_pTree;

	bool bKeyState[VK_MAX];
	bool m_bBoolMgr[BOOL_END];

	int m_iID;

	int	 m_iTreeType;
// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

