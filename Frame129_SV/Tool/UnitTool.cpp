// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTemp(_T(""))
	, m_strCopy(_T(""))
	, m_strName(_T(""))
	, m_iHP(0)
	, m_iAttack(0)
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_strTemp);
	DDX_Text(pDX, IDC_EDIT2, m_strCopy);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iHP);
	DDX_Text(pDX, IDC_EDIT5, m_iAttack);

	DDX_Control(pDX, IDC_LIST1, m_ListBox);

	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);

	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
}

BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnPushString)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAdd_Charactor)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)

	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnRoad)

	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CUnitTool::OnPushString()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	m_strCopy = m_strTemp;

	UpdateData(FALSE);
}


void CUnitTool::OnAdd_Charactor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	UNITDATA* pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iHp = m_iHP;
	pUnit->iAttack = m_iAttack;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({pUnit->strName, pUnit});

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString strFindName;

	int iSelect = m_ListBox.GetCurSel();

	m_ListBox.GetText(iSelect, strFindName);

	auto iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
	{
		return;
	}

	m_strName = iter->second->strName;
	m_iHP = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	UpdateData(FALSE);
}

void CUnitTool::OnBnSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;


	if (Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();

		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"���� ���� ����!");
			return;
		}

		DWORD dwByte = 0;

		DWORD dwStringSize = 0;

		for (auto& rPair : m_mapUnitData)
		{
			// �����ڵ� �ؽ�Ʈ ���̸�ŭ ���
			dwStringSize = sizeof(wchar_t) * (rPair.second->strName.GetLength() + 1);
			// �����ڵ����� �˸��� BOM
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			// ������ ����
			WriteFile(hFile, rPair.second->strName.GetString(), dwStringSize, &dwByte, nullptr);

			WriteFile(hFile, &rPair.second->iHp, sizeof(int), &dwByte, nullptr);

			WriteFile(hFile, &rPair.second->iAttack, sizeof(int), &dwByte, nullptr);

			WriteFile(hFile, &rPair.second->byJobIndex, sizeof(BYTE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnBnRoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	for (auto& rPair : m_mapUnitData)
		Safe_Delete(rPair.second);

	m_mapUnitData.clear();

	m_ListBox.ResetContent();


	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;


	if (Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();

		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"���� ���� ����!");
			return;
		}

		DWORD dwByte = 0;

		DWORD dwStringSize = 0;

		UNITDATA* pUnitData = nullptr;

		while (true)
		{
			pUnitData = new UNITDATA;

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pTemp = new TCHAR[dwStringSize];
			// dwByte�� ������ dwStringSize ũ�⸸ŭ pTemp�� �Űܶ�
			ReadFile(hFile, pTemp, dwStringSize, &dwByte, nullptr);

			ReadFile(hFile, &pUnitData->iHp, sizeof(int), &dwByte, nullptr);

			ReadFile(hFile, &pUnitData->iAttack, sizeof(int), &dwByte, nullptr);

			ReadFile(hFile, &pUnitData->byJobIndex, sizeof(BYTE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pUnitData);

				if (pTemp)
				{
					delete[] pTemp;
					pTemp = nullptr;
				}
				break;
			}

			pUnitData->strName = pTemp;

			m_mapUnitData.emplace(pUnitData->strName, pUnitData);

			m_ListBox.AddString(pUnitData->strName);
		}
		CloseHandle(hFile);
	}
}

void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_Radio[0].SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}