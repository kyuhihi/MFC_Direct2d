#include "stdafx.h"
#include "AStarMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CAStarMgr)

CAStarMgr::CAStarMgr()
	: m_iStartIdx(0)
{
}

CAStarMgr::~CAStarMgr()
{
	Release();
}

void CAStarMgr::Astar_Start(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	Release();

	CObj* pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();

	// ��� ������ Ÿ�� �ε����� ����
	m_iStartIdx = GetTileIndex(vStart);

	int	iGoalIdx = GetTileIndex(vGoal);

	if (0 > m_iStartIdx || 0 > iGoalIdx || m_iStartIdx >= TILEX * TILEY || iGoalIdx >= TILEX * TILEY)
		return;

	if (m_iStartIdx == iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	if (Make_Route(m_iStartIdx, iGoalIdx))
		Make_BestList(m_iStartIdx, iGoalIdx);
}

void CAStarMgr::Release(void)
{
	m_CloseList.clear();
	m_OpenList.clear();
	m_BestList.clear();
}

bool CAStarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj* pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecAdj();

	// queue �������� �� ���� �����
	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	// ���� ��ġ�� Ÿ�� �ε����� ���������� �ƴ� ���̱� ������ closelist�� ���Եȴ�
	m_CloseList.push_back(iStartIdx);

	// �������ִ� �̿� Ÿ�ϵ��� ���� ��θ� ã�� ���� �ݺ���
	for (auto& pTile : vecAdj[iStartIdx])
	{
		// ã�� ���� �������� �����ߴٸ�
		if (iGoalIdx == pTile->iIndex)
		{
			pTile->iParentIndex = iStartIdx;
			return true;
		}

		// ������ Ÿ�� �߿� close�� open ����Ʈ�� tile �ε����� �ִ��� ����
		if (false == Check_Close(pTile->iIndex) &&
			false == Check_Open(pTile->iIndex))
		{
			// ���� Ÿ���� �ε����� ����
			pTile->iParentIndex = iStartIdx;

			// �� ����Ʈ�� �������� �ʱ� ������ �ϴ� �̵��� �� �ִ� ����Ʈ�� �Ǻ�
			m_OpenList.push_back(pTile->iIndex);
		}
	}

	// ���̻� ã�� ���� ���ٴ� ��
	if (m_OpenList.empty())
		return false;

	// �޸���ƽ = ���� ������������ �Ÿ� ��� + �� �������� �Ÿ����
	int	iOriginIdx = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, &iOriginIdx](int iTemp, int Sour) {

		D3DXVECTOR3	vPCost1 = vecTile[iOriginIdx]->vPos - vecTile[iTemp]->vPos;
		D3DXVECTOR3	vPCost2 = vecTile[iOriginIdx]->vPos - vecTile[Sour]->vPos;

		D3DXVECTOR3	vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[iTemp]->vPos;
		D3DXVECTOR3	vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[Sour]->vPos;

		float		fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float		fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});

	// queue �������� �ʺ� Ž���� ��� ������ ��� �����Ѵ�.
	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAStarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj* pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();


	m_BestList.push_front(vecTile[iGoalIdx]);

	int	iRouteIdx = vecTile[iGoalIdx]->iParentIndex;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		// �θ� Ÿ���� �������ϸ鼭 �տ������� best�� ä���.
		m_BestList.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIndex;
	}
}

int CAStarMgr::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	CObj* pTerrain = CObjMgr::Get_Instance()->Get_Terrain();

	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();

	for (size_t iIndex = 0; iIndex < vecTile.size(); ++iIndex)
	{
		if (Picking(vPos, iIndex))
		{
			return iIndex;
		}
	}
	// ��ŷ ���� �� ���� ��ȯ
	return -1;
}

bool CAStarMgr::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj*		pTerrain = CObjMgr::Get_Instance()->Get_Terrain();

	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetvecTile();

	if ((STILECX / 2) >= abs(vecTile[iIndex]->vPos.x - vPos.x) && (STILECY / 2) >= abs(vecTile[iIndex]->vPos.y - vPos.y))
	{
		return true;
	}
	return false;
}

bool CAStarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iIndex == iCloseIdx)
			return true;
	}

	return false;
}

bool CAStarMgr::Check_Open(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iIndex == iOpenIdx)
			return true;
	}

	return false;
}
