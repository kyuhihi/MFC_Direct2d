#include "stdafx.h"
#include "MainGame.h"
#include "Device.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "TimeMgr.h"

CMainGame::CMainGame()
	: m_pGraphicDev(CDevice::Get_Instance())
{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * MIN_STR);
	m_iFps = 0;
}


CMainGame::~CMainGame()
{
	Release();
}


HRESULT CMainGame::Initialize(void)
{
	if (FAILED(m_pGraphicDev->InitDevice()))
	{
		ERR_MSG(L"Client Device Create Failed");
		return E_FAIL;
	}

	CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::LOADING);
	CTimeMgr::Get_Instance()->Initialize();

	return S_OK;
}

void CMainGame::Update(void)
{
	CTimeMgr::Get_Instance()->Update();
	CSceneMgr::Get_Instance()->Update_SceneMgr();
}

void CMainGame::Late_Update(void)
{
	CSceneMgr::Get_Instance()->Late_Update_SceneMgr();
}

void CMainGame::Render(void)
{
	//++m_iFps;

	m_pGraphicDev->Render_Begin();

	CSceneMgr::Get_Instance()->Render_SceneMgr();

	/*swprintf_s(m_szFPS, L"FPS: %d", m_iFps);

	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		m_szFPS,
		lstrlen(m_szFPS),
		nullptr,
		0,
		D3DCOLOR_ARGB(255, 0, 0, 0));*/

	m_pGraphicDev->Render_End();
}

void CMainGame::Release(void)
{
	CTimeMgr::Get_Instance()->Destroy_Instance();
	CObjMgr::Get_Instance()->Destroy_Instance();
	CTextureMgr::Get_Instance()->Destroy_Instance();
	CSceneMgr::Get_Instance()->Destroy_Instance();
	m_pGraphicDev->Destroy_Instance();
}

