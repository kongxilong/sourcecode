
#pragma once

#include "define.h"
#include "TList.h"

enum PathActionType
{
	PAT_COPY,
	PAT_MOVE,
	PAT_DELETE
};

struct PathObj : public ListObj
{
	PathActionType ActionType;
	WCHAR *pScrPath;
	WCHAR *pDstPath;
	BOOL bSrcIsDir;
	BOOL bDstIsDir;
	BOOL bIsSameDrv;
	int nSrcSectorSize;
	int nDstSectorSize;
	DiskType SrcFsType;
	DiskType DstFsType;
	DWORD dwLastErr;
	_int64 nAllSrcFileSize;
	BOOL bIsActionEnd;
};

class CPathList : public CTList
{
public:
	CPathList(void) {};
	~CPathList(void) { ClearAll(); };

	PathObj *TopObj(void)
	{
		return (PathObj *)CTList::TopObj();
	};

	PathObj *NextObj(PathObj *pReqObj)
	{
		return (PathObj *)CTList::NextObj(pReqObj);
	};

	PathObj *NewPathObj()
	{
		PathObj *pNewObj = new PathObj;
		if (pNewObj != NULL)
		{
			memset(pNewObj, 0, sizeof(PathObj));
			pNewObj->pScrPath = new WCHAR[NTFS_MAX_PATH];
			if (pNewObj->pScrPath == NULL)
			{
				SAFE_DELETE(pNewObj);
			}
			else
			{
				pNewObj->pDstPath = new WCHAR[NTFS_MAX_PATH];
				if (pNewObj->pDstPath == NULL)
				{
					SAFE_DELETE_MEM_LIST(pNewObj->pScrPath);
					SAFE_DELETE(pNewObj);
				}
				else
				{
					this->AddObj(pNewObj);
				}
			}
		}

		return pNewObj;
	};

	void DelPathObj(PathObj *pReqObj)
	{
		if (pReqObj != NULL)
		{
			this->DelObj(pReqObj);
			SAFE_DELETE_MEM_LIST(pReqObj->pScrPath);
			SAFE_DELETE_MEM_LIST(pReqObj->pDstPath);
			SAFE_DELETE(pReqObj);
		}
	};

	void ClearAll()
	{
		while (!this->IsEmpty())
		{
			PathObj *pPathObj = this->TopObj();
			DelPathObj(pPathObj);
		}
	};
};
