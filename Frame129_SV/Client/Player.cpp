#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"

CPlayer::CPlayer()
	: m_bStart(false)
	, m_pDevice(CDevice::Get_Instance())
	, m_pTexInfo(nullptr)
	, m_RenCount(0)
	, m_dwRenCount(GetTickCount())
	, m_fLR(1.f)
	, m_ePlayer(P_END)
	, m_eLPlayer(P_END)
{
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };

	m_fSpeed = 0.5f;
	m_fAngle = 0.f;

	m_bStart = true;

	for (int i = 0; i < 2; ++i)
		m_pIDLE[i] = CTextureMgr::Get_Instance()->Get_Texture(L"Cat", L"IDLE", i);

	for (int i = 0; i < 4; ++i)
	{
		m_pStand[i] = CTextureMgr::Get_Instance()->Get_Texture(L"Cat", L"Down", i);
		m_pWalk[i] = CTextureMgr::Get_Instance()->Get_Texture(L"Cat", L"Right", i);
		m_pDash[i] = CTextureMgr::Get_Instance()->Get_Texture(L"Cat", L"Up", i);
	}

	return S_OK;
}

int CPlayer::Update(void)
{
	m_ePlayer = P_DOWN;

	Key_Input();

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);

	return 0;
}

void CPlayer::Late_Update(void)
{
	m_RenCount += 1;

	switch (m_ePlayer)
	{
	case CPlayer::P_DOWN:
		if (3 < m_RenCount)
		{
			m_RenCount = 0;
		}
		break;

	case CPlayer::P_LR:
		if (3 < m_RenCount)
		{
			m_RenCount = 0;
		}
		break;

	case CPlayer::P_UP:
		if (3 < m_RenCount)
		{
			m_RenCount = 0;
		}
		break;

	case CPlayer::P_IDLE:
		if (0 < m_RenCount)
		{
			m_RenCount = 0;
		}
		break;
	}
}

void CPlayer::Render(void)
{
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, m_fLR, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	switch (m_ePlayer)
	{
	case CPlayer::P_DOWN:
		m_pDevice->Get_Sprite()->Draw(m_pStand[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pStand[m_RenCount]->tImgInfo.Width / 2), (m_pStand[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CPlayer::P_LR:
		m_pDevice->Get_Sprite()->Draw(m_pWalk[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pWalk[m_RenCount]->tImgInfo.Width / 2), (m_pWalk[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CPlayer::P_UP:
		m_pDevice->Get_Sprite()->Draw(m_pDash[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pDash[m_RenCount]->tImgInfo.Width / 2), (m_pDash[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CPlayer::P_IDLE:
		m_pDevice->Get_Sprite()->Draw(m_pIDLE[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pIDLE[m_RenCount]->tImgInfo.Width / 2), (m_pIDLE[m_RenCount]->tImgInfo.Height / 2), 0.f),
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
	else 
	{
		m_RenCount = 0;
		m_ePlayer = P_IDLE;
		
	}
}
