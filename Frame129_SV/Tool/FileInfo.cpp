#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";

	TCHAR szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	PathRelativePathTo(szRelativePath, szCurDirPath, FILE_ATTRIBUTE_DIRECTORY, strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{
	CFileFind Find;

	// ��� ���� ��� ����(*.*)�� ã�� ���� ��θ� ������
	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : �־��� ��ο� ������ ������ �Ǵ��ϴ� �Լ�
	// �������� ������ false, ������ ������ true�� ����

	// ���� ������ ������ Ȯ���ϰ� ���� �� ���Ͽ� �����ϰ� ���� ���� FindNextFile �Լ��� ������ ȣ���ؾ� �Ѵ�.

	// �Ű������� ��η� *.*�� ���� ��� �ش� ����� ���� ��� ������ ã�� ������ �ؼ��ϸ�
	// �� ��, FindNextFile�Լ��� ȣ���ϸ� ���� ���ϵ� �� ���� ù ��° ���Ͽ� ������ �� �ִ�.

	BOOL bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : ���� ��� �ȿ��� ���� ������ ã�´�. ���̻� ã���� ���ų� ������ ����� ã���� 0�� ��ȯ�Ѵ�.

		bContinue = Find.FindNextFile();

		// ã�� ���� �̸��� .�̳� ..�̸� ã���� �ϴ� ������ �ƴϱ� ������ ���� Ž���� �����Ѵ�.
		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath :  Find��ü�� ���� ���� ã�� ������ ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}

		else
		{
			// ã�� ������ ���� ������ �ý��� ������ ��� �ǳʶ�
			if (Find.IsSystem())
				continue;

			IMGPATH* pImgPath = new IMGPATH;
			TCHAR szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			// ���� ��ο��� ���� ������ ��θ� �߶�
			PathRemoveFileSpec(szPathBuf);

			// ���� ���� ���� ������ ������ ����Ѵ�
			pImgPath->iCount = CFileInfo::DirFileCount(szPathBuf);

			//GetFileTitle : ���� Find�� ����Ű�� ������ ���ϸ� ������ �Լ�
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(����, ��) : ���ۿ������� ������ �ش��ϴ� ���ڿ��� ����
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 2) + L"%02d.png";

			TCHAR	szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);

			// PathCombine(1, 2, 3)
			// 2�� 3�� �̾�پ� �ϼ��� ��θ� 1�� ����, 2�� 3���̿� \\�� �ڵ����� ���Ե�ä 1�� ����

			// D:\����ȯ\129��\Frame129\Texture\Stage\Effect\BossMultiAttack\\BossMultiAttack%d.png
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());

			// ����� ��ȯ���Ѽ� ��� ������ �������ش�.
			//..\Frame129\Texture\Stage\Effect\BossMultiAttack\\BossMultiAttack%d.png
			pImgPath->wstrPath = CFileInfo::ConvertRelativePath(szBuf);

			//..\Frame129\Texture\Stage\Effect\BossMultiAttack
			PathRemoveFileSpec(szBuf);

			// PathFindFileName : ���ϸ��� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			//..\Frame129\Texture\Stage\Effect
			PathRemoveFileSpec(szBuf);

			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathList.push_back(pImgPath);

			bContinue = 0;
		}
	}
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{

	CFileFind		Find;

	wstring			wstrFilePath = wstrPath + L"\\*.*";

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCount = 0;

	while (bContinue)
	{

		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCount;
	}

	return iFileCount;
}