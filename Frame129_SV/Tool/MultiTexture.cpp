#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto		iter = find_if(m_MapMultiTex.begin(), m_MapMultiTex.end(), [&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_MapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	// ����� ���̴� ���� 520byte�� �Ѿ �� �ȴ�.
	TCHAR	szFullPath[MAX_PATH] = L"";


	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� ����ϴ� �Լ�
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			AfxMessageBox(szFullPath);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// ����̽� �� ��ü
			szFullPath,								// �ؽ�ó ������ �ִ� ���
			pTexInfo->tImgInfo.Width,				// �ؽ�ó ���� ������
			pTexInfo->tImgInfo.Height,			// �ؽ�ó ���� ������
			pTexInfo->tImgInfo.MipLevels,			// �ؽ�ó �� ����
			0,										// 0�� ��� �Ϲ����� �ؽ�ó, D3DUSAGE_RENDERTARGET �ɼ��� ���� ����Ÿ�� �� �ؽ�ó�� ������ ���� �ִ�
			pTexInfo->tImgInfo.Format,			// �ؽ�ó ����
			D3DPOOL_MANAGED,						// �ؽ�ó�� � �޸𸮿� ������ ���ΰ�
			D3DX_DEFAULT,							// �̹��� ���͸� ���, �ȼ��� ���ڶ�ų� ������ ��� �� ���ΰ�
			D3DX_DEFAULT,							// �� ���͸� ���, �Ӹ��� �̿��� Ȯ��, ��� �� ���͸��� ��� �� ���ΰ�
			0,										// ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ, D2D���ʹ� ������ ������ ���ĺ����� ����ϱ� ������ Colorkey ��� ����
			nullptr,								// ����ü �̹��� ���� ���(������ �̹� ���� ä����)
			nullptr,								// �ȷ�Ʈ ������ �����ϴ� ����ü(�� ����)
			&(pTexInfo->pTexture))))
		{
			AfxMessageBox(L"Multi Texture Create Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		m_MapMultiTex[pStateKey].push_back(pTexInfo);
	}	

	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_MapMultiTex.begin(), m_MapMultiTex.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
		MyPair.second.clear();	
	});

	m_MapMultiTex.clear();
}
