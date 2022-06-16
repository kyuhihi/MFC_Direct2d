#include "stdafx.h"
#include "Sheep.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ToolView.h"

CSheep::CSheep()
{
	Set_MouseReset();
}


CSheep::~CSheep()
{
	Release();
}

void CSheep::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/TSheep/SSheep%02d.png", TEX_MULTI, L"TSheep", L"SSheep", 5)))
	{
		AfxMessageBox(L"Sheep Image Insert failed");
		return;
	}
}

void CSheep::Update(void)
{
}

void CSheep::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	RECT	rc{};

	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	if (!m_vecSheep.empty()) {
		for (auto& iter : m_vecSheep)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0), iter->vPos.y - m_pMainView->GetScrollPos(1), iter->vPos.z);

			matWorld = matScale *  matTrans;

			//Set_Ratio(&matWorld, fX, fY, 0.f);
			const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"TSheep", L"SSheep", iter->byDrawID);

			if (nullptr == pTexInfo)
				return;
			float fX = SSHEEPCX / 2.f;
			float fY = SSHEEPCX / 2.f;
			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fX, fY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	//==================================================================================
	if (m_pMouseSheep.vPos.x != -50.f) {
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, m_pMouseSheep.vPos.x - m_pMainView->GetScrollPos(0), m_pMouseSheep.vPos.y - m_pMainView->GetScrollPos(1), m_pMouseSheep.vPos.z);
		matWorld = matScale *  matTrans;

		const TEXINFO*	pTexInfo2;
		float TreefX = STREECX / 2.f;
		float TreefY = STREECY / 2.f;
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		pTexInfo2 = CTextureMgr::Get_Instance()->Get_Texture(L"TSheep", L"SSheep", 4);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture,
			nullptr,
			&D3DXVECTOR3(TreefX, TreefY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//==================================================================================
}

void CSheep::Mini_Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;
	for (auto& iter : m_vecSheep)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x, iter->vPos.y, iter->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.45f, 0.45f, 0.45f);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"TSheep", L"SSheep", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float fX = pTexInfo->tImgInfo.Width / 2.f;
		float fY = pTexInfo->tImgInfo.Height / 2.f;

		//swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CSheep::Release(void)
{
	for_each(m_vecSheep.begin(), m_vecSheep.end(), CDeleteObj());
	m_vecSheep.clear();
	m_vecSheep.shrink_to_fit();
}

bool CSheep::Set_MouseSheep(D3DXVECTOR3 _vPos, int _byDrawID)
{
	m_pMouseSheep.vPos = _vPos;

	return true;
}

void CSheep::Add_Sheep(BYTE _NewTreeType)
{
	TILE* pNewSheep = new TILE;

	pNewSheep->vPos = { m_pMouseSheep.vPos.x, m_pMouseSheep.vPos.y - 20.f , 0.f };
	pNewSheep->vSize = { (float)SSHEEPCX, (float)SSHEEPCX, 0.f };
	pNewSheep->byDrawID = _NewTreeType;
	pNewSheep->byOption = 1;

	m_vecSheep.push_back(pNewSheep);
}

void CSheep::Set_Ratio(D3DXMATRIX * _pOut, const float & _fX, const float & _fY, const float & _fZ)
{
	_pOut->_11 *= _fX;
	_pOut->_21 *= _fX;
	_pOut->_31 *= _fX;
	_pOut->_41 *= _fX;
	// Y
	_pOut->_12 *= _fY;
	_pOut->_22 *= _fY;
	_pOut->_32 *= _fY;
	_pOut->_42 *= _fY;
}
