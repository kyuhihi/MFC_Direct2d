#include "stdafx.h"
#include "MyTree.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "Include.h"

CMyTree::CMyTree()
{
}


CMyTree::~CMyTree()
{
	Release();

}

HRESULT CMyTree::Initialize(void)
{
	if (FAILED(Load_Tree(L"../Data/Tree.dat")))
		return E_FAIL;
	return S_OK;
}

int CMyTree::Update(void)
{
	return 0;
}

void CMyTree::Late_Update(void)
{
}

void CMyTree::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	RECT	rc{};

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	if (!m_vecTree.empty()) {
		for (auto& iter : m_vecTree)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans, iter->vPos.x + CObj::m_vScroll.x, iter->vPos.y + CObj::m_vScroll.y, iter->vPos.z);

			matWorld = matScale *  matTrans;

			//Set_Ratio(&matWorld, fX, fY, 0.f);
			const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tree", iter->byDrawID);

			if (nullptr == pTexInfo)
				return;
			float fX = STREECX / 2.f;
			float fY = STREECY / 2.f;
			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fX, fY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CMyTree::Release(void)
{
	for_each(m_vecTree.begin(), m_vecTree.end(), CDeleteObj());
	m_vecTree.clear();
	m_vecTree.shrink_to_fit();
}

HRESULT CMyTree::Load_Tree(const TCHAR * pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	DWORD dwByte = 0;

	TILE* pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete<TILE*>(pTile);
			break;
		}

		m_vecTree.push_back(pTile);
	}

	CloseHandle(hFile);


	return S_OK;
}
