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
	DDX_Control(pDX, IDC_LIST2, m_TreeListBox);
	DDX_Control(pDX, IDC_PICTURE2, m_TreePicture);
	DDX_Control(pDX, IDC_LIST3, m_SheepListBox);
	DDX_Control(pDX, IDC_PICTURE3, m_SheepPicture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnLoadTile)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnLoadTree)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnTreeListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnSaveData)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMapTool::OnSheepListBox)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTool::OnBnLoadSheep)
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

	int i = 0;

	for (; i < strSelectName.GetLength(); ++i)
	{
		if (0 != isdigit(strSelectName[i]))
			break;
	}

	strSelectName.Delete(0, i);

	m_iDrawID = _tstoi(strSelectName);

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

void CMapTool::Set_Tree(int _iID)
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

	pView->Set_TreeID(_iID);
}

void CMapTool::Set_Sheep(int _iID)
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

	pView->Set_SheepID(_iID);
}


//=======================================================================
void CMapTool::OnBnLoadTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_ListBox.ResetContent();
/*======================================================================================================
형이 만들다가 만것
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
//======================================================================================================*/

	TCHAR FileName;
	CFileFind ff;
	TCHAR szFilePath[MAX_PATH] = L"";
	TCHAR szFileName[MAX_STR] = L"";

	//int ret = ff.FindFile(L"C:\\Users\\reaso\\Documents\\MFC_Direct2d\\Frame129_SV\\Texture\\Stage\\Terrain\\Tile\\*.png");
	int ret = ff.FindFile(L"../Texture/Stage/Terrain/Tile\\*.png");

	if (ret != 0) {
		while (ff.FindNextFile())
		{
			CString CstrFilePath = ff.GetFilePath();
			CString strRelative = CFileInfo::ConvertRelativePath(CstrFilePath);
			CString strFileName = PathFindFileName(strRelative);

			lstrcpy(szFileName, strFileName.GetString());

			PathRemoveExtension(szFileName);

			strFileName = szFileName;

			auto iter = m_mapPngImg.find(strFileName);

			if (iter == m_mapPngImg.end())
			{
				CImage*	pPngImg = new CImage;

				pPngImg->Load(strRelative);

				m_mapPngImg.insert({ strFileName, pPngImg });
				m_ListBox.AddString(szFileName);
			}
		}
	}
	ff.Close();
}

void CMapTool::OnBnLoadTree()
{
	m_TreeListBox.ResetContent();

	TCHAR FileName;
	CFileFind ff;
	TCHAR szFilePath[MAX_PATH] = L"";
	TCHAR szFileName[MAX_STR] = L"";

	//int ret = ff.FindFile(L"C:\\Users\\reaso\\Documents\\MFC_Direct2d\\Frame129_SV\\Texture\\Stage\\Tree\\*.png");
	int ret = ff.FindFile(L"../Texture/Stage/Terrain/Tree\\*.png");

	if (ret != 0) {
		while (ff.FindNextFile())
		{
			CString CstrFilePath = ff.GetFilePath();
			CString strRelative = CFileInfo::ConvertRelativePath(CstrFilePath);
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
				m_TreeListBox.AddString(szFileName);
			}
		}
	}
	ff.Close();
}
//=======================================================================
void CMapTool::OnBnLoadSheep()
{
	m_SheepListBox.ResetContent();

	TCHAR FileName;
	CFileFind ff;
	TCHAR szFilePath[MAX_PATH] = L"";
	TCHAR szFileName[MAX_STR] = L"";

	//int ret = ff.FindFile(L"C:\\Users\\reaso\\Documents\\MFC_Direct2d\\Frame129_SV\\Texture\\Stage\\Tree\\*.png");
	int ret = ff.FindFile(L"../Texture/Stage/Terrain/TSheep\\*.png");

	if (ret != 0) {
		while (ff.FindNextFile())
		{
			CString CstrFilePath = ff.GetFilePath();
			CString strRelative = CFileInfo::ConvertRelativePath(CstrFilePath);
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
				m_SheepListBox.AddString(szFileName);
			}
		}
	}
	ff.Close();
}

void CMapTool::OnTreeListBox()
{
	UpdateData(TRUE);

	CString strSelectName;

	int iSelect = m_TreeListBox.GetCurSel();
	
	Set_Tree(iSelect);

	m_TreeListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	m_TreePicture.SetBitmap(*(iter->second));

	UpdateData(FALSE);
}


void CMapTool::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString	str = Dlg.GetPathName();

		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

		CToolView*		pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

		CTerrain*		pTerrain = pView->Get_Terrain();

		vector<TILE*>& vecTile = pTerrain->Get_VecTile();

		DWORD	dwByte = 0;

		for (auto& iter : vecTile)
		{
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	//=========================================================
	CFileDialog		DlgTree(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPathTree[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPathTree);

	PathRemoveFileSpec(szPathTree);

	lstrcat(szPathTree, L"\\Data");

	DlgTree.m_ofn.lpstrInitialDir = szPathTree;

	if (IDOK == DlgTree.DoModal())
	{
		CString	str = DlgTree.GetPathName();

		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

		CToolView*		pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

		CTree*		pTree = pView->Get_Tree();

		vector<TILE*>& vecTree = pTree->Get_VecTree();

		DWORD	dwByte = 0;

		for (auto& iter : vecTree)
		{
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	//=========================================================

	CFileDialog		DlgSheep(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPathSheep[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPathSheep);

	PathRemoveFileSpec(szPathSheep);

	lstrcat(szPathSheep, L"\\Data");

	DlgSheep.m_ofn.lpstrInitialDir = szPathSheep;

	if (IDOK == DlgSheep.DoModal())
	{
		CString	str = DlgSheep.GetPathName();

		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

		CToolView*		pView = dynamic_cast<CToolView*>(pMainFrm->Get_MainSplitter().GetPane(0, 1));

		CSheep*		pSheep = pView->Get_Sheep();

		vector<TILE*>& vecSheep = pSheep->Get_VecSheep();

		DWORD	dwByte = 0;

		for (auto& iter : vecSheep)
		{
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	//=========================================================

}


void CMapTool::OnSheepListBox()
{
	UpdateData(TRUE);

	CString strSelectName;

	int iSelect = m_SheepListBox.GetCurSel();

	Set_Sheep(iSelect);

	m_SheepListBox.GetText(iSelect, strSelectName);

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	m_SheepPicture.SetBitmap(*(iter->second));

	UpdateData(FALSE);
}

