#pragma once
#include "Include.h"

#include "Obj.h"
#include "Device.h"
class CMyTree :
	public CObj
{
public:
	CMyTree();
	virtual ~CMyTree();

	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
private:
	HRESULT			Load_Tree(const TCHAR* pFilePath);
	vector<TILE*>		m_vecTree;


};

