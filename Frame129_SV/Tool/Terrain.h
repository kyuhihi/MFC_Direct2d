#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void	Initialize(void);
	void	Update(void);
	void	Render(void);
	void	Mini_Render(void);
	void	Release(void);

public:
	void Tile_Change(const D3DXVECTOR3& _vPos, const int& _byDrawID);
	int Get_TileIndex(const D3DXVECTOR3& _vPos);
	bool Picking(const D3DXVECTOR3& _vPos, const int& _iIndex);
	bool Picking_Dot(const D3DXVECTOR3& _vPos, const int& _iIndex);

	bool Set_MouseTile(D3DXVECTOR3 _vPos, int _byDrawID);
	void Set_MouseReset() { m_pMouseTile.vPos = { -50.f,-50.f,0.f }; }

public:
	vector<TILE*>& Get_VecTile(void) { return m_vecTile; }
	void Set_MainView(CToolView* _pMainView) { m_pMainView = _pMainView; }
	void Set_Ratio(D3DXMATRIX* _pOut, const float& _fX, const float& _fY, const float& _fZ);

private:
	vector<TILE*> m_vecTile;
	TILE		m_pMouseTile;

	CToolView* m_pMainView = nullptr;

	int m_iMouseID;
};

