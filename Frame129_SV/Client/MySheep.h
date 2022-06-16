#pragma once
#include "Obj.h"
class CMySheep :
	public CObj
{
public:
	CMySheep();
	virtual ~CMySheep();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT			Load_Sheep(const TCHAR* pFilePath);
	vector<TILE*>		m_vecSheep;

};

