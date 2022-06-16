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
	const	wstring& Get_String(void) { return m_wstrFullPath; }
	void	Set_String(wstring wstrFullPath) { m_wstrFullPath = wstrFullPath; }

public:
	HRESULT	ReadImgPath(const wstring& wstrPath);

public:
	const TEXINFO*	Get_Texture(const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

public:
	HRESULT		InsertTexture(const TCHAR* pFilePath,	// 텍스처 파일 경로
								TEXTYPE	eType,
								const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	void		Release(void);

private:
	map<wstring, CTexture*>		m_MapTex;
	wstring						m_wstrFullPath;
};

