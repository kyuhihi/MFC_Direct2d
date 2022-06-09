#pragma once

#include "Include.h"
#include "Device.h"
#include "ToolView.h"

class CTest
{
	DECLARE_SINGLETON(CTest)

private:
	CTest();
	~CTest();

public:
	const INFO& Get_Info(void) { return m_tInfo; }

	void Set_TexInfo(const TEXINFO* _pTexInfo) { m_pTexInfo = _pTexInfo; }
	void Set_Stand(const TEXINFO* _pTexInfo, int _i) { m_pStand[_i] = _pTexInfo; }
	void Set_Walk(const TEXINFO* _pTexInfo, int _i) { m_pWalk[_i] = _pTexInfo; }
	void Set_Dash(const TEXINFO* _pTexInfo, int _i) { m_pDash[_i] = _pTexInfo; }

	void Initialize(void);
	int Update(void);
	void LateUpdate(void);
	void Render(void);
	void Release(void);

private:
	enum PLAYER { P_STAND, P_WALK, P_DASH, P_ATTACK, P_END };
	enum DIRECTION { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_END };

	void Key_Input(void);

	CDevice* m_pDevice;

	const TEXINFO* m_pTexInfo;
	const TEXINFO* m_pStand[12];
	const TEXINFO* m_pWalk[13];
	const TEXINFO* m_pDash[11];

	int m_RenCount;
	DWORD m_dwRenCount;

	float m_fLR;
	PLAYER m_ePlayer;
	PLAYER m_eLPlayer;

	float m_fSpeed;
	float m_fAngle;
	bool m_bStart;

	INFO m_tInfo;
	RECT m_tRect;

	D3DXMATRIX matScale;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;

	CToolView* m_pMainView = nullptr;
};

