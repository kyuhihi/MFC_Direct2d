#pragma once

#include "afxwin.h"

// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnLoadTile();
	afx_msg void OnBnLoadTree();
	afx_msg void OnTreeListBox();
	afx_msg void OnSaveData();
	afx_msg void OnSheepListBox();
	afx_msg void OnBnLoadSheep();
public:
	void Horizontal_Scroll(void);
	void Set_Tile(int _iID);
	void Set_Tree(int _iID);
	void Set_Sheep(int _iID);

	int& Get_iDrawID(void) { return m_iDrawID; }

private:
//control
	CListBox m_ListBox;
	CStatic m_Picture;
	CListBox m_TreeListBox;
	CStatic m_TreePicture;
	CListBox m_SheepListBox;
	CStatic m_SheepPicture;
	int		m_iDrawID;
	map<CString, CImage*> m_mapPngImg;


};
