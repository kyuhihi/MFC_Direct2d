#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "AStarMgr.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
	: m_pDevice(CDevice::Get_Instance())
	, m_pTexInfo(nullptr)
	, m_ePlayer(P_END)
{
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { STILECX, STILECY, 0.f };

	m_fSpeed = 200.f;
	m_fAngle = 0.f;

	m_tFrame = { 0.f, 4.f };
	m_ePlayer = P_IDLE;
	m_fLR = 1.f;

	return S_OK;
}

int CPlayer::Update(void)
{
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

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		CAStarMgr::Get_Instance()->Astar_Start(m_tInfo.vPos, ::Get_Mouse() - CObj::m_vScroll);
	}

	Move_Route();
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

void CPlayer::Move_Route(void)
{
	list<TILE*>& BestList = CAStarMgr::Get_Instance()->Get_BestList();

	if (!BestList.empty())
	{
		int	iIndex = BestList.front()->iIndex;

		D3DXVECTOR3	vDir = BestList.front()->vPos - m_tInfo.vPos;

		float fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		if (1.2f > vDir.x && 0.8f < vDir.x) // 우
		{
			m_ePlayer = P_LR;
			m_fLR = 1.f;
		}
		else if (-1.2f < vDir.x && -0.8f > vDir.x) // 좌
		{
			m_ePlayer = P_LR;
			m_fLR = -1.f;
		}
		else if (1.2f > vDir.y && 0.8f < vDir.y) // 하
		{
			m_ePlayer = P_DOWN; 
		}
		else if (-1.2f < vDir.y && -0.8f > vDir.y) // 상
		{
			m_ePlayer = P_UP;
		}
		
		m_tInfo.vPos += vDir * m_fSpeed * CTimeMgr::Get_Instance()->Get_TimeDelta();

		// 타일에 가까워졌을 때 다음 길을 찾기 위해 맨 앞에 원소를 제거
		if (3.f >= fDistance)
			BestList.pop_front();

		if (BestList.empty())
			m_ePlayer = P_IDLE;
	}
}
