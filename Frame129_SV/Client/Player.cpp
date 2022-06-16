#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"

CPlayer::CPlayer()
	: m_pDevice(CDevice::Get_Instance())
	, m_pTexInfo(nullptr)
	, m_fLR(1.f)
	, m_ePlayer(P_END)
	//, m_eLPlayer(P_END)
{
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { STILECX, STILECY, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };

	m_fSpeed = 0.2f;
	m_fAngle = 0.f;

	m_tFrame = { 0.f, 4.f };

	return S_OK;
}

int CPlayer::Update(void)
{
	Key_Input();

	// vec mat
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CObj::m_vScroll.x, m_tInfo.vPos.y + CObj::m_vScroll.y, 0.f);
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);

	// texture mat
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, m_fLR, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 2.5f + CObj::m_vScroll.x, m_tInfo.vPos.y - 18.f + CObj::m_vScroll.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	return 0;
}

void CPlayer::Late_Update(void)
{
	Move_Frame();
}

void CPlayer::Render(void)
{
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	switch (m_ePlayer)
	{
	case CPlayer::P_DOWN:
		m_wstrObjKey = L"Cat";
		m_wstrStateKey = L"Down";
		m_pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
		m_pDevice->Get_Sprite()->Draw(m_pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pTexInfo->tImgInfo.Width / 2), (m_pTexInfo->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CPlayer::P_LR:
		m_wstrObjKey = L"Cat";
		m_wstrStateKey = L"Right";
		m_pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
		m_pDevice->Get_Sprite()->Draw(m_pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pTexInfo->tImgInfo.Width / 2), (m_pTexInfo->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CPlayer::P_UP:
		m_wstrObjKey = L"Cat";
		m_wstrStateKey = L"Up";
		m_pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
		m_pDevice->Get_Sprite()->Draw(m_pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pTexInfo->tImgInfo.Width / 2), (m_pTexInfo->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CPlayer::P_IDLE:
		m_wstrObjKey = L"Cat";
		m_wstrStateKey = L"IDLE";
		m_pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
		m_pDevice->Get_Sprite()->Draw(m_pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pTexInfo->tImgInfo.Width / 2), (m_pTexInfo->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}
}

void CPlayer::Release(void)
{
}

void CPlayer::Key_Input(void)
{
	if (GetAsyncKeyState('W'))
	{
		m_ePlayer = P_UP;
		m_fAngle = D3DXToRadian(0.f);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		return;
	}
	else if (GetAsyncKeyState('S'))
	{
		m_ePlayer = P_DOWN;
		m_fAngle = D3DXToRadian(180.f);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
		return;
	}
	else if (GetAsyncKeyState('D'))
	{
		m_ePlayer = P_LR;
		m_fLR = 1.f;
		m_fAngle = D3DXToRadian(90.f);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}
	else if (GetAsyncKeyState('A'))
	{
		m_ePlayer = P_LR;
		m_fLR = -1.f;
		m_fAngle = D3DXToRadian(270.f);
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}
	else if (GetAsyncKeyState(VK_LBUTTON))
	{
		
	}
	else 
	{
		m_RenCount = 0;
		m_ePlayer = P_IDLE;		
	}
}
