#pragma once
#include "afxwin.h"


// CPathFind ��ȭ �����Դϴ�.

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();

private:
	CListBox m_ListBox;

	list<IMGPATH*> m_PathList;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};