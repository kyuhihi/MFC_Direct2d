#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const TCHAR* pFilePath,
									const TCHAR* pStateKey /*= L""*/, 
									const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� ����ϴ� �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		AfxMessageBox(pFilePath);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),	// ����̽� �� ��ü
											pFilePath,								// �ؽ�ó ������ �ִ� ���
											m_pTexInfo->tImgInfo.Width,				// �ؽ�ó ���� ������
											m_pTexInfo->tImgInfo.Height,			// �ؽ�ó ���� ������
											m_pTexInfo->tImgInfo.MipLevels,			// �ؽ�ó �� ����
											0,										// 0�� ��� �Ϲ����� �ؽ�ó, D3DUSAGE_RENDERTARGET �ɼ��� ���� ����Ÿ�� �� �ؽ�ó�� ������ ���� �ִ�
											m_pTexInfo->tImgInfo.Format,			// �ؽ�ó ����
											D3DPOOL_MANAGED,						// �ؽ�ó�� � �޸𸮿� ������ ���ΰ�
											D3DX_DEFAULT,							// �̹��� ���͸� ���, �ȼ��� ���ڶ�ų� ������ ��� �� ���ΰ�
											D3DX_DEFAULT,							// �� ���͸� ���, �Ӹ��� �̿��� Ȯ��, ��� �� ���͸��� ��� �� ���ΰ�
											0,										// ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ, D2D���ʹ� ������ ������ ���ĺ����� ����ϱ� ������ Colorkey ��� ����
											nullptr,								// ����ü �̹��� ���� ���(������ �̹� ���� ä����)
											nullptr,								// �ȷ�Ʈ ������ �����ϴ� ����ü(�� ����)
											&(m_pTexInfo->pTexture))))
	{
		AfxMessageBox(L"Single Texture Create Failed");
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}


	// D3DPOOL_DEFAULT : ���� ������ �޸𸮿� ����, ���� �׷��� �޸𸮸� �ǹ�, ���� ����
	// D3DPOOL_MANAGED : DX�� �ڿ��� �����ϸ� �׷��� �޸𸮸� ��������� RAM�� ���, ���� ����
	
	// D3DPOOL_SYSTEMMEM : �ý��� �޸𸮿� ����
	// D3DPOOL_SCRATCH : �ý��� �޸𸮸� ��������� DX ��ġ�κ��� ������ �� ����


	return S_OK;
}

void CSingleTexture::Release(void)
{
	m_pTexInfo->pTexture->Release();
	Safe_Delete(m_pTexInfo);
}
