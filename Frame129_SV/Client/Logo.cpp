#include "stdafx.h"
#include "Logo.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "Device.h"

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, CLogo::ImgLoadThreadFunc, (LPVOID)this, 0, nullptr);


	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ImgPath Load failed");
		return E_FAIL;
	}

	return S_OK;
}

void CLogo::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::STAGE);
		return;
	}
}

void CLogo::Late_Update_Scene()
{
}

void CLogo::Render_Scene()
{
#pragma region 백그라운드
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Logo", L"BackGround", 0);

	if (nullptr == pTexInfo)
		return;

	float		fX = pTexInfo->tImgInfo.Width / 2.f;
	float		fY = pTexInfo->tImgInfo.Height / 2.f;

	D3DXMATRIX	matWorld, matTrans, matScale;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 6.f, 6.f, 1.f);
	D3DXMatrixTranslation(&matTrans, WINCX / 8.f, WINCY / 2.f, 0.f);
	matWorld = matScale * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
#pragma endregion

#pragma region 로고 타이틀
	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Logo", L"Button", 0);

	if (nullptr == pTexInfo)
		return;

	fX = pTexInfo->tImgInfo.Width / 2.f;
	fY = pTexInfo->tImgInfo.Height / 2.f;

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 3.f, 0.f);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fX, fY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
#pragma endregion

	wstring	wstrFullPath = CTextureMgr::Get_Instance()->Get_String();

	D3DXMatrixTranslation(&matTrans, 50.f, 500.f, 0.f);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		wstrFullPath.c_str(),
		wstrFullPath.length(),
		nullptr,
		0,
		D3DCOLOR_ARGB(255, 0, 0, 0));
}

void CLogo::Release_Scene()
{
}

size_t __stdcall CLogo::ImgLoadThreadFunc(LPVOID lpParam)
{
	return 0;
}