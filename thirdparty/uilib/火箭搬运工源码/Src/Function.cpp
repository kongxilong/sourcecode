
#include "stdafx.h"
#include "Function.h"
#include "define.h"

// 设置指定特权
BOOL SetPrivilege(LPSTR pszPrivilege, BOOL bEnable)
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp;

	if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
		return FALSE;

	if (!::LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
		return FALSE;

	tp.PrivilegeCount = 1;

	if (bEnable)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!::AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		return FALSE;

	if (!::CloseHandle(hToken))
		return FALSE;

	return TRUE;
}

// 组合路径
int MakePath(WCHAR *dest, const WCHAR *dir, const WCHAR *file)
{
	size_t	len;

	if ((len = wcslen(dir)) == 0)
		return wsprintfW(dest, L"%s", file);

	return wsprintfW(dest, L"%s%s%s", dir, dir[len -1] == L'\\' ? L"" : L"\\" , file);
}

// 判断路径是否为目录
BOOL PathIsDir(WCHAR *pFile, BOOL &bIsDir)
{
	BOOL bRet = FALSE;

	if (pFile != NULL)
	{
		DWORD dwAttr = ::GetFileAttributesW(pFile);
		if (dwAttr != 0xFFFFFFFF)
		{
			bRet = TRUE;
			bIsDir = IsDir(dwAttr);
		}
	}

	return bRet;
}

// 取得当前路径的盘符
BOOL GetRootDir(WCHAR *pPath, WCHAR *pRetRoot)
{
	BOOL bRet = FALSE;

	if (pPath != NULL && pRetRoot != NULL)
	{
		if (GetChar(pPath, 0) == '\\')
		{	// "\\server\volname\"
			DWORD dwCh;
			int nBackslashCnt = 0, nOffset;

			for (nOffset = 0; (dwCh = GetChar(pPath, nOffset)) != 0 && nBackslashCnt < 4; nOffset++)
			{
				if (dwCh == '\\')
					nBackslashCnt++;
			}

			memcpy(pRetRoot, pPath, nOffset * WCHAR_SIZE);
			if (nBackslashCnt < 4)
				SetChar(pRetRoot, nOffset++, '\\');

			SetChar(pRetRoot, nOffset, 0);
		}
		else
		{	// "C:\"
			memcpy(pRetRoot, pPath, 3 * WCHAR_SIZE);
			SetChar(pRetRoot, 3, 0);
		}

		bRet = TRUE;
	}

	return bRet;
}

// 计算时间差
UINT GetTickDiff(UINT nOldTickCount, UINT nNewTickCount)
{
	if (nNewTickCount >= nOldTickCount)
		return (nNewTickCount - nOldTickCount);
	else
		return (UINT(-1) - nOldTickCount + nNewTickCount);
}

// 取得单个文件的文件长度
BOOL FileSize(WCHAR *pFile, _int64 &nFileSize)
{
	BOOL bRet = FALSE;

	if (pFile != NULL)
	{
		LARGE_INTEGER lnSize;
		lnSize.QuadPart = 0;

		WIN32_FILE_ATTRIBUTE_DATA FileData;
		memset(&FileData, 0, sizeof(WIN32_FILE_ATTRIBUTE_DATA));

		bRet = ::GetFileAttributesExW(pFile, GetFileExInfoStandard, &FileData);
		lnSize.HighPart = FileData.nFileSizeHigh;
		lnSize.LowPart = FileData.nFileSizeLow;

		nFileSize = lnSize.QuadPart;
	}

	return	bRet;
}

// 取得目录下的所有文件的长度总和
BOOL GetDirSize(WCHAR *pPath, _int64 &nSize, int &nFileCnts, BOOL &bIsAbort)
{
	BOOL bRet = FALSE;
	WIN32_FIND_DATAW FindDat;
	LARGE_INTEGER lnSize;
	WCHAR *pFindPath = new WCHAR[NTFS_MAX_PATH];

	if (pFindPath != NULL)
	{
		MakePath(pFindPath, pPath, L"*.*");

		HANDLE hFile = ::FindFirstFileW(pFindPath, &FindDat);
		if (IS_SAVE_HANDLE(hFile))
		{
			bRet = TRUE;
			do
			{
				if (FindDat.cFileName[0] == '.')
					continue;

				if (IsDir(FindDat.dwFileAttributes))
				{
					MakePath(pFindPath, pPath, FindDat.cFileName);
					bRet = GetDirSize(pFindPath, nSize, nFileCnts, bIsAbort);

					if (bRet)
						continue;
					else
						break;
				}

				lnSize.HighPart = FindDat.nFileSizeHigh;
				lnSize.LowPart = FindDat.nFileSizeLow;

				nSize += lnSize.QuadPart;
				nFileCnts++;
			}while (::FindNextFileW(hFile, &FindDat) && !bIsAbort);
			::FindClose(hFile);
		}
		SAFE_DELETE_MEM_LIST(pFindPath);
	}

	return bRet;
}
