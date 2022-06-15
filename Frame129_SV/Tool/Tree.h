#pragma once
#include "Include.h"

class CToolView;
class CTree
{
public:
	CTree();
	~CTree();
public:
	void	Initialize(void);
	void	Update(void);
	void	Render(void);
	void	Mini_Render(void);
	void	Release(void);
public:
	void Set_MainView(CToolView* _pMainView) { m_pMainView = _pMainView; }
	bool Set_MouseTree(D3DXVECTOR3 _vPos, int _byDrawID);
	void Add_Tree(BYTE _NewTreeType);
	void Set_MouseReset() { m_pMouseTree.vPos = { -50.f,-50.f,0.f }; }
	void Set_Ratio(D3DXMATRIX* _pOut, const float& _fX, const float& _fY, const float& _fZ);

private:
	vector<TILE*> m_vecTree;
	TILE		m_pMouseTree;

	CToolView* m_pMainView = nullptr;
};

