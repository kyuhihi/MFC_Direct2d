// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(), [](auto& Pair)
	{
		Pair.second->Destroy();
		Safe_Delete<CImage*>(Pair.second);
	});

	m_mapPngImg.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnLoadTile)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTool::OnButtonLoad)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strSelectName;

	int iSelect = m_ListBox.GetCurSel();

	Set_Tile(iSelect);

	m_ListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR szFilePath[MAX_PATH] = L"";
	TCHAR szFileName[MAX_STR] = L"";

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo::ConvertRelativePath(szFilePath);

		CString strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());

		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*		pPngImg = new CImage;

			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(szFileName);
		}
	}
	Horizontal_Scroll();

	UpdateData(FALSE);
}

void CMapTool::Horizontal_Scroll(void)
{
	CString	strName;
	CSize size;

	int iWidth = 0;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);
}

void CMapTool::Set_Tile(int _iID)
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

	pView->Set_ID(_iID);
}

void CMapTool::OnBnLoadTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(TRUE, L"png", L"*.png", OFN_OVERWRITEPROMPT, L"Data File(*.png) | *.png||", this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Texture\\Stage\\Terrain\\Tile");
	Dlg.m_ofn.lpstrInitialDir = szPath;


	if (Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();

		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CloseHandle(hFile);
	}
}


void CMapTool::OnButtonSave()
{
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		//
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		//CMiniView* pMini = dynamic_cast<CMiniView*>(pMainFrm->Get_SecondSplitter().GetPane(0, 0));

		CToolView* pView = dynamic_cast<CToolView*>(pMain->Get_MainSplitter().GetPane(0, 1));
		vector<TILE*> vecTile = pView->Get_Terrain()->Get_TileVector();

		//타일 벡터를 가져옵니다. 타일벡터는 포인터나 가상함수를 가지고있지않기때문에 WriteFile로 처리해줄수있습니다,

		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);


		CloseHandle(hFile);
	}
}


void CMapTool::OnButtonLoad()
{
	CFileDialog Dlg(TRUE, L"dat", L"*.dat");

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		//
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;


		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->Get_MainSplitter().GetPane(0, 1));
		pView->Get_Terrain()->Relese_Terrain();

		DWORD dwByte = 0;
		TILE* pTile = nullptr;

		// 이렇게 하지 마셈 이왕이면 Set이라던가 아니면 Add라던가 하는 함수를 만들어서 쓰셈. 
		// 이러면 퍼블릭이랑 다를게 일절 없음 
		//그래서 갯함수는 대부분const 붙일수 있는데 다 붙인다. 
		//vector<TILE*>& vectile = pView->m_pTerrain->Get_TileVector(); 


		vector<TILE*> vecTile;
		vecTile.reserve(TILEX * TILEY);
		while (true)
		{
			pTile = new TILE;
			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
			if (0 == dwByte)
			{
				SAFE_DELETE(pTile);
				break;
			}
			vecTile.emplace_back(pTile);
		}
		pView->Get_Terrain()->Set_Vector(vecTile);
		pView->Invalidate(FALSE);

		CloseHandle(hFile);
	}
}
