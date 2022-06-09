#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO*	Get_Texture(const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

public:
	HRESULT		InsertTexture(const TCHAR* pFilePath,	// �ؽ�ó ���� ���
								TEXTYPE	eType,
								const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	void		Release(void);

private:
	map<wstring, CTexture*>		m_MapTex;
};

