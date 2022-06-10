#pragma once


// CA_Star 대화 상자입니다.

class CA_Star : public CDialog
{
	DECLARE_DYNAMIC(CA_Star)

public:
	CA_Star(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CA_Star();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_A_STAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void A_Star_Cord(void);
};
