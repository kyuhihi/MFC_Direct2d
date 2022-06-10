#include "stdafx.h"
#include "Test.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTest)

CTest::CTest()
	: m_bStart(false)
	, m_pDevice(CDevice::Get_Instance())
	, m_pTexInfo(nullptr)
	, m_RenCount(0)
	, m_dwRenCount(GetTickCount())
	, m_fLR(1.f)
	, m_ePlayer(P_END)
	, m_eLPlayer(P_END)
{
	for (int i = 0; i < 12; ++i)
	{
		m_pStand[i] = nullptr;
	}

	for (int i = 0; i < 13; ++i)
	{
		m_pWalk[i] = nullptr;
	}
}


CTest::~CTest()
{
	Release();
}

void CTest::Initialize(void)
{
	// 대기 텍스쳐
	/*if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Stand/AKIHA_AKI00_00%d.png", TEX_MULTI, L"Player", L"Stand", 12)))
	{
		AfxMessageBox(L"Stand Image Insert failed");
		return;
	}*/
	// 걷기 텍스쳐
	/*if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Walk/AKIHA_AKI26_00%d.png", TEX_MULTI, L"Player", L"Walk", 13)))
	{
		AfxMessageBox(L"Walk Image Insert failed");
		return;
	}*/
	// 달리기 텍스쳐
	/*if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png", TEX_MULTI, L"Player", L"Dash", 11)))
	{
		AfxMessageBox(L"Dash Image Insert failed");
		return;
	}*/

	// 대기 세팅
	/*for (int i = 0; i < 12; ++i)
	{
		Set_Stand(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Stand", i), i);
	}*/
	// 걷기 세팅
	/*for (int i = 0; i < 13; ++i)
	{
		Set_Walk(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Walk", i), i);
	}*/
	// 달리기 세팅
	/*for (int i = 0; i < 11; ++i)
	{
		Set_Dash(CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Dash", i), i);
	}*/

	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };

	m_fSpeed = 2.5f;
	m_fAngle = 0.f;

	m_bStart = true;
}

int CTest::Update(void)
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

void CTest::LateUpdate(void)
{
	if (m_dwRenCount + 100 < GetTickCount())
	{
		m_RenCount += 1;

		switch (m_ePlayer)
		{
		case CTest::P_DOWN:
			if (3 < m_RenCount)
			{
				m_RenCount = 0;
			}
			break;

		case CTest::P_LR:
			if (3 < m_RenCount)
			{
				m_RenCount = 0;
			}
			break;

		case CTest::P_UP:
			if (3 < m_RenCount)
			{
				m_RenCount = 0;
			}
			break;

		case CTest::P_IDLE:
			if (0 < m_RenCount)
			{
				m_RenCount = 0;
			}
			break;
		}
		m_dwRenCount = GetTickCount();
	}
}

void CTest::Render(void)
{
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, m_fLR, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matRotZ * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	switch (m_ePlayer)
	{
	case CTest::P_DOWN:
		m_pDevice->Get_Sprite()->Draw(m_pStand[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pStand[m_RenCount]->tImgInfo.Width / 2), (m_pStand[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CTest::P_LR:
		m_pDevice->Get_Sprite()->Draw(m_pWalk[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pWalk[m_RenCount]->tImgInfo.Width / 2), (m_pWalk[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CTest::P_UP:
		m_pDevice->Get_Sprite()->Draw(m_pDash[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pDash[m_RenCount]->tImgInfo.Width / 2), (m_pDash[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case CTest::P_IDLE:
		m_pDevice->Get_Sprite()->Draw(m_pIDLE[m_RenCount]->pTexture,
			nullptr,
			&D3DXVECTOR3((m_pIDLE[m_RenCount]->tImgInfo.Width / 2), (m_pIDLE[m_RenCount]->tImgInfo.Height / 2), 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}
}

void CTest::Release(void)
{
}

void CTest::Key_Input(void)
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
