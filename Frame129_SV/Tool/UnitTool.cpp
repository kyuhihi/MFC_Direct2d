// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

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


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnPushString()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_strCopy = m_strTemp;

	UpdateData(FALSE);
}


void CUnitTool::OnAdd_Charactor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
			ERR_MSG(L"파일 개방 실패!");
			return;
		}

		DWORD dwByte = 0;

		DWORD dwStringSize = 0;

		for (auto& rPair : m_mapUnitData)
		{
			// 유니코드 텍스트 길이만큼 기록
			dwStringSize = sizeof(wchar_t) * (rPair.second->strName.GetLength() + 1);
			// 유니코드임을 알리는 BOM
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			// 내용을 저장
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
			ERR_MSG(L"파일 개방 실패!");
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
			// dwByte의 내용을 dwStringSize 크기만큼 pTemp로 옮겨라
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Radio[0].SetCheck(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}