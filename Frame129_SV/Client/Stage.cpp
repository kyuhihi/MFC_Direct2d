#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "MyTerrain.h"
#include "Player.h"
#include "ObjMgr.h"
#include "MyTree.h"
#include "MySheep.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	CObj* pObj = new CMyTerrain;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();
	
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pObj);


	pObj = new CPlayer;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pObj);
	
	pObj = new CMyTree;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::EFFECT, pObj);

	pObj = new CMySheep;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::UI, pObj);

	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
	
}
