#pragma once


// CA_Star ��ȭ �����Դϴ�.

class CA_Star : public CDialog
{
	DECLARE_DYNAMIC(CA_Star)

public:
	CA_Star(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CA_Star();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_A_STAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void A_Star_Cord(void);
};
