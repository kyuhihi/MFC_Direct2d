#include "stdafx.h"
#include "Terrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ToolView.h"


CTerrain::CTerrain()
	: m_pMouseTile()
	, m_iMouseID(35)
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize(void)
{
	// 타일 텍스쳐
	/*if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Tile Image Insert failed");
		return;
	}*/
	// 스타듀타일
	//if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/STile%d.png", TEX_MULTI, L"Terrain", L"STile", 36)))
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/STile%02d.png", TEX_MULTI, L"Terrain", L"STile", 36)))
	{
		AfxMessageBox(L"Tile Image Insert failed");
		return;
	}

	// 타일 세팅
	/*for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float fX = (TILECX * j) + (i % 2) * (TILECX  / 2.f);
			float fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY, 0.f };
			pTile->byDrawID = 2;
			pTile->byOption = 0;

			m_vecTile.push_back(pTile);
		}
	}*/
	// 스타듀타일 세팅
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float fX = STILECX * j;
			float fY = STILECY * i;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)STILECX, (float)STILECY, 0.f };
			pTile->byDrawID = 3;
			pTile->byOption = 0;

			m_vecTile.push_back(pTile);

		}
	}


}

void CTerrain::Update(void)
{
}

void CTerrain::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	RECT	rc{};

	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);
	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0), iter->vPos.y - m_pMainView->GetScrollPos(1), iter->vPos.z);

		matWorld = matScale *  matTrans;

		Set_Ratio(&matWorld, fX, fY, 0.f);


		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"STile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float fX = STILECX / 2.f;
		float fY = STILECY / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			0,
			D3DCOLOR_ARGB(255, 0, 0, 0));

		++iIndex;
//===================================================
		if (m_pMouseTile.vPos.x!= -50.f) 
		{
			const TEXINFO*	pTexInfo2;
			if (iter->vPos == m_pMouseTile.vPos)
			{
				pTexInfo2 = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"STile", m_iMouseID);
			}
			else
				continue;
			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture,
				nullptr,
				&D3DXVECTOR3(fX, fY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
//====================================================
		

	}
}

void CTerrain::Mini_Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	//TCHAR szBuf[MIN_STR] = L"";
	//int iIndex = 0;

	/*RECT rc{};

	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float fX = WINCX / rc.right - rc.left;
	float fX = WINCY / rc.bottom - rc.top;*/

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f , 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x, iter->vPos.y, iter->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.45f, 0.45f, 0.45f);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"STile", iter->byDrawID);

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

		/*CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			0,
			D3DCOLOR_ARGB(255, 0, 0, 0));*/

		//++iIndex;
	}
}

void CTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::Tile_Change(const D3DXVECTOR3 & _vPos, const int & _byDrawID)
{
	int iIndex = Get_TileIndex(_vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = _byDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & _vPos)
{
	/*for (size_t iIndex = 0; iIndex < m_vecTile.size(); ++iIndex)
	{
		if (Picking_Dot(_vPos, iIndex))
		{
			return iIndex;
		}
	}*/

	for (size_t iIndex = 0; iIndex < m_vecTile.size(); ++iIndex)
	{
		if (Picking(_vPos, iIndex))
		{
			return iIndex;
		}
	}
	
	return -1;
}

bool CTerrain::Picking(const D3DXVECTOR3 & _vPos, const int & _iIndex)
{
	if ((STILECX / 2) >= abs(m_vecTile[_iIndex]->vPos.x - _vPos.x) && (STILECY / 2) >= abs(m_vecTile[_iIndex]->vPos.y - _vPos.y))
	{
		return true;
	}
	return false;
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3 & _vPos, const int & _iIndex)
{
	// 법선벡터를 통한 마우스 피킹.

	D3DXVECTOR3	vPoint[4] = {

		{ m_vecTile[_iIndex]->vPos.x, m_vecTile[_iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[_iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[_iIndex]->vPos.y, 0.f },
		{ m_vecTile[_iIndex]->vPos.x, m_vecTile[_iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[_iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[_iIndex]->vPos.y, 0.f }

	}; // 마름모의 각 끝점

	D3DXVECTOR3	vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	}; // 빗변의 Dir을 구함

	D3DXVECTOR3		vNormal[4] = {

		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	}; // Dir x와 y값을 서로 바꾸고 x자리에 -붙임

	D3DXVECTOR3 vMouseDir[4] =
	{
		_vPos - vPoint[0],
		_vPos - vPoint[1],
		_vPos - vPoint[2],
		_vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}

	return true;
}

bool CTerrain::Set_MouseTile(D3DXVECTOR3 _vPos, int  _byDrawID)
{
	for (auto& iter :m_vecTile)
	{
		if ((STILECX / 2) >= abs(iter->vPos.x - _vPos.x) && (STILECY / 2) >= abs(iter->vPos.y - _vPos.y))
		{
			m_pMouseTile.vPos = { iter->vPos };
			return true;
		}
		else
			continue;
	}	

	return false;

}

void CTerrain::Set_Ratio(D3DXMATRIX* _pOut, const float& _fX, const float& _fY, const float& _fZ)
{
	// X
	_pOut->_11 *= _fX;
	_pOut->_21 *= _fX;
	_pOut->_31 *= _fX;
	_pOut->_41 *= _fX;
	// Y
	_pOut->_12 *= _fY;
	_pOut->_22 *= _fY;
	_pOut->_32 *= _fY;
	_pOut->_42 *= _fY;
	// Z

}
