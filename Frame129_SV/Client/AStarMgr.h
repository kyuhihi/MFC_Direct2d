#pragma once

#include "Include.h"

class CAStarMgr
{
private:
	CAStarMgr();
	~CAStarMgr();

private:

private:
	list<int> m_OpenList;
	list<int> m_CloseList;
	list<TILE*> m_BestList;

};

