#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

	// ��ġ(������ ��� ��ü) �ʱ�ȭ ����
	
	// 1. ������ ��ġ�� �����ϰ� ������ ��ü ����
	// 2. ��ġ ����(���� ������ �ľ�)
	// 3. ������ ��ġ ���� ��ü ����

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT				Get_Font(void)	 { return m_pFont; }

public:
	HRESULT				Initialize(void);
	void				Release(void);

	void				Render_Begin(void);
	void				Render_End(HWND hWnd = nullptr);
	
private:
	void				Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);


private:
	LPDIRECT3D9			m_pSDK;	// ����ϰ��� �ϴ� ��ġ�� �����ϰ� �ش� ��ġ�� �����ϴ� ��ü�� �����ϴ� ���� ��ü
	LPDIRECT3DDEVICE9	m_pDevice;	// �׷��� ��ġ�� ���� �������� �����ϴ� ��ü
	LPD3DXSPRITE		m_pSprite;	// dx���� 2d �̹��� ����� �����ϴ� ��ü
	LPD3DXFONT			m_pFont;
};

