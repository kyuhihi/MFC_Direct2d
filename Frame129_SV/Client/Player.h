#pragma once

#include "Include.h"
#include "Obj.h"
#include "Device.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	const INFO& Get_Info(void) { return m_tInfo; }

	virtual HRESULT	Initialize(void) override;
	virtual int	Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	enum PLAYER { P_DOWN, P_LR, P_UP, P_IDLE, P_END };

	void Move_Route(void);

	CDevice* m_pDevice;

	const TEXINFO* m_pTexInfo;

	int m_RenCount;
	DWORD m_dwRenCount;

	float m_fLR;
	PLAYER m_ePlayer;
	//PLAYER m_eLPlayer;

	float m_fSpeed;
	float m_fAngle;

	D3DXMATRIX matScale;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
};

