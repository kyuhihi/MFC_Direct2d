#pragma once
#include "afxwin.h"

#include "Include.h"

// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPushString();
	afx_msg void OnAdd_Charactor();
	afx_msg void OnListBox();

	afx_msg void OnBnSaveData();
	afx_msg void OnBnRoadData();
	afx_msg void OnBnSearchData();
	afx_msg void OnBnDeleteData();

	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();

private:
	// value
	CString m_strTemp;
	CString m_strCopy;
	CString m_strName;
	CString m_strSearchData;

	int m_iHp;
	int m_iAttack;

	// control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;

	// general
	map<CString, UNITDATA*> m_mapUnitData;

public:
};
