#pragma once
#include "Include.h"

class CToolView;
class CSheep
{
public:
	CSheep();
	~CSheep();
public:
	void	Initialize(void);
	void	Update(void);
	void	Render(void);
	void	Mini_Render(void);
	void	Release(void);
public:
	void Set_MainView(CToolView* _pMainView) { m_pMainView = _pMainView; }
	bool Set_MouseSheep(D3DXVECTOR3 _vPos, int _byDrawID);
	void Add_Sheep(BYTE _NewTreeType);
	void Set_MouseReset() { m_pMouseSheep.vPos = { -50.f,-50.f,0.f }; }
	void Set_Ratio(D3DXMATRIX* _pOut, const float& _fX, const float& _fY, const float& _fZ);
	vector<TILE*>& Get_VecSheep(void) { return m_vecSheep; }
private:
	vector<TILE*> m_vecSheep;
	TILE		m_pMouseSheep;

	CToolView* m_pMainView = nullptr;
};
