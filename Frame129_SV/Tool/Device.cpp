#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Initialize(void)
{
	// ��ġ �ʱ�ȭ ����

	// 1. ������ ��ġ�� �����ϰ� ������ ��ü ����

	// Direct3DCreate9 : SDK ������ �°� �����ϴ� �Լ�,D3D_SDK_VERSION ������ �츮�� ������ DX����� ���ǵǾ� �֤���
	// m_pSDK��ü�� ��ġ ������ LPDIRECT3DDEVICE9 ��ü ���� �� ���� �뵵�� �̿�
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. ��ġ ����(���� ������ �ľ�)

	// HAL : �ϵ���� �߻�ȭ ����, Ư�� �÷������� ������ �ϵ��� ���� ������ �������� �ʿ��ѵ� �ϵ���� ������� �پ��ϴ�.
	// �����簡 �ٸ����� Ư�� �÷������� �����ϰ� �����ϱ� ���� ������ ������ �߻�ȭ���� ������ �صд�.
	
	// HAL�� ���Ͽ� ���� ��ġ�� ���� ������ ���� �� D3DCAPS9 ����ü�� ���
	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// SUCCEEDED() : ���� ����� �� TRUE ����
	// FAILED : ���� ������ �� TRUE ����

	// GetDeviceCaps : ��ġ�� ���� ���� ������ ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"Get Device Caps Failed");
		return E_FAIL;
	}

	DWORD		vp = 0;	

	// HAL�� ���� ������ ��ġ�� �ϵ���� ���ؽ� ���μ����� �����Ѵٸ�
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// �ϵ��� ����

	else	// �׷��� �ʴٸ� ����ڰ� ���� ����
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3. ������ ��ġ ���� ��ü ����

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));


	Set_Parameters(d3dpp);

	// CreateDevice : �׷��� ��ġ�� ������ ��ü�� �����ϴ� �Լ�

	// 1. � �׷��� ī�带 ������ ���ΰ�
	// 2. � ������ ��ġ�� ������ ���ΰ�
	// 3. ��ġ�� ����� ������ �ڵ�
	// 4. ���� ���
	// 5. ��� ȯ��
	// 6. ������ ��ü�� �ּҰ��� �����ϱ� ���� ������

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"Create Device Failed");
		return E_FAIL;	}

	// ��������Ʈ ��ü ����

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"Create Sprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 8;
	tFontInfo.Weight = FW_HEAVY; // ��Ʈ ����
	tFontInfo.CharSet = HANGEUL_CHARSET; // ��� ����
	lstrcpy(tFontInfo.FaceName, L"�ü�"); // ��ü

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"Create Font Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Release(void)
{
	if (nullptr != m_pFont)
		m_pFont->Release();

	if (nullptr != m_pSprite)
		m_pSprite->Release();

	if (nullptr != m_pDevice)
		m_pDevice->Release();

	if (nullptr != m_pSDK)
		m_pSDK->Release();
}

void CDevice::Render_Begin(void)
{
	// ������ ���� : �ĸ� ���۸� ���� -> �ĸ� ���ۿ� �׸��� -> �ĸ� ���ۿ� ���� ���۸� ��ü -> ȭ�� ���

	/*RECT	rc[10] { }*/

	m_pDevice->Clear(0,			// ��Ʈ�� ����
					nullptr,	// ��Ʈ �迭�� ù ��° �ּ�
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // �������ϴ� ����
					D3DCOLOR_ARGB(255, 125, 125, 125), // �ĸ���� ����
					1.f, // z���� �ʱ�ȭ ��
					0);  // ���ٽ� ���� �ʱ�ȭ ��
		
	// ���⼭���� �ĸ� ���ۿ� �׸���.
	m_pDevice->BeginScene();

	// 2D �̹����� �׸� �� �ֵ��� ��ġ���� �˸�(���� ������ �ɼ�)
	// ���� ������ �ɼ��� ���� �׽�Ʈ�� ��ȿ�� ���¿��� ���� ������ ����ϰڴٴ� �ɼ�
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}


void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();

	// ��������� �ĸ� ���ۿ� �� �׸� ����
	m_pDevice->EndScene();

	// �ĸ� ���ۿ� ���� ���۸� ��ȯ�Ͽ� ȭ�鿡 ���

	// 1,2�� ���� : ����ü�� �ɼ��� D3DSWAPEFFECT_COPY�� �ۼ����� ���� �� NULL
	// 3�� ���� : ��� ��� ������ �ڵ�, NULL�� ��� Set_Parameters���� ������ �ڵ��� �⺻ ������ ������
	// 4�� ���� : ����ü�� �ɼ��� D3DSWAPEFFECT_COPY�� �ۼ����� ���� �� NULL

	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth = WINCX; //NULL;
	d3dpp.BackBufferHeight = WINCY; //NULL;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
/*
	D3DSWAPEFFECT_DISCARD : ���� ü��
	D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ����ϴ� ���
	D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���� ���
*/
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// ��ġ�� ����� ������ �ڵ� ����
	d3dpp.hDeviceWindow = g_hWnd;

	// â ���� ������ ������ ���θ� ����
	d3dpp.Windowed = TRUE;		// true �� ��� â ���
	
	// �׸��� ���� ���� : Ÿ��, ���ٽ�, Z(����)

	// DX�� �ڵ������� ���� / ���ٽ� ���۸� ����� �����ϱ� ���ϴ°�
	d3dpp.EnableAutoDepthStencil = TRUE;

	// ���� ���ۿ� ���׽� ������ ���� ����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ȭ���� ��üȭ�� �ÿ��� ���Ǵ� �Ű�������
	// â ��忡���� OS�� ����� ������� ����

	// ��ü ȭ�� �ÿ��� �׷��� ī�尡 ���� ����� ������� ���� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// ����� ������� �ÿ�(���α׷� ������)�� ���� ����

	// D3DPRESENT_INTERVAL_IMMEDIATE : ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ DX�� �˾Ƽ� ����, ���� ����� ������� ����

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
