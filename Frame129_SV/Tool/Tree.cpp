#include "stdafx.h"
#include "Tree.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ToolView.h"

CTree::CTree() 
{
	Set_MouseReset();
}

CTree::~CTree()
{
	Release();

}

void CTree::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tree/STree%02d.png", TEX_MULTI, L"Tree", L"STree", 3)))
	{
		AfxMessageBox(L"Tile Image Insert failed");
		return;
	}
}

void CTree::Update(void)
{
}

void CTree::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	RECT	rc{};

	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	if (!m_vecTree.empty()) {
		for (auto& iter : m_vecTree)
		{
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0), iter->vPos.y - m_pMainView->GetScrollPos(1), iter->vPos.z);

			matWorld = matScale *  matTrans;

			//Set_Ratio(&matWorld, fX, fY, 0.f);
			const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Tree", L"STree", iter->byDrawID);

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
	//==================================================================================
	if (m_pMouseTree.vPos.x != -50.f ) {
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, m_pMouseTree.vPos.x - m_pMainView->GetScrollPos(0), m_pMouseTree.vPos.y - m_pMainView->GetScrollPos(1)-(STREECY/2), m_pMouseTree.vPos.z);
		matWorld = matScale *  matTrans;

		const TEXINFO*	pTexInfo2;
		float TreefX = STREECX / 2.f;
		float TreefY = STREECY / 2.f;
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		pTexInfo2 = CTextureMgr::Get_Instance()->Get_Texture(L"Tree", L"STree", 2);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture,
			nullptr,
			&D3DXVECTOR3(TreefX, TreefY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	//==================================================================================

}

void CTree::Mini_Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;
	for (auto& iter : m_vecTree)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x, iter->vPos.y, iter->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.45f, 0.45f, 0.45f);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Tree", L"STree", iter->byDrawID);

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

void CTree::Release(void)
{
	for_each(m_vecTree.begin(), m_vecTree.end(), CDeleteObj());
	m_vecTree.clear();
	m_vecTree.shrink_to_fit();
}

bool CTree::Set_MouseTree(D3DXVECTOR3 _vPos, int _byDrawID)
{

	m_pMouseTree.vPos = _vPos;

	return true;
}

void CTree::Add_Tree(BYTE _NewTreeType)
{
	TILE* pNewTree = new TILE;

	pNewTree->vPos = { m_pMouseTree.vPos.x, m_pMouseTree.vPos.y-(STREECY/2), 0.f };
	pNewTree->vSize = { (float)STREECX, (float)STREECY, 0.f };
	pNewTree->byDrawID = _NewTreeType;
	pNewTree->byOption = 1;

	m_vecTree.push_back(pNewTree);

}

void CTree::Set_Ratio(D3DXMATRIX * _pOut, const float & _fX, const float & _fY, const float & _fZ)
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
