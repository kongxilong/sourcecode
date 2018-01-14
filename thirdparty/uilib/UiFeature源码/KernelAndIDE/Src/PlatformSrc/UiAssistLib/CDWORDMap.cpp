
#include "StdAfx.h"
#include "..\..\Inc\CDWORDMap.h"
#include "..\..\Inc\UiFeatureDefs.h"

DWORD_MAP_OBJ::DWORD_MAP_OBJ()
{
	dwKeyDword = 0;
	pData = NULL;
}

DWORD_MAP_OBJ::~DWORD_MAP_OBJ()
{
}

CDWORDMap::CDWORDMap()
{
}

CDWORDMap::~CDWORDMap()
{
}

void CDWORDMap::Clear()
{
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		DWORD_MAP_OBJ *pDelObj = (DWORD_MAP_OBJ *)pItem;
		SAFE_DELETE(pDelObj);
	}
	CTList::Clear();
}

VOID* CDWORDMap::find(const DWORD dwKey, DWORD_MAP_OBJ** ppOutObj)
{
	if (ppOutObj != NULL)
		*ppOutObj = NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		DWORD_MAP_OBJ *pSelObj = (DWORD_MAP_OBJ *)pItem;
		if (pSelObj != NULL && pSelObj->dwKeyDword == dwKey)
		{
			if (ppOutObj != NULL)
				*ppOutObj = pSelObj;
			return pSelObj->pData;
		}
	}
	return NULL;
}

VOID* CDWORDMap::erase(const DWORD dwKey)
{
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		DWORD_MAP_OBJ *pSelObj = (DWORD_MAP_OBJ *)pItem;
		if (pSelObj != NULL && pSelObj->dwKeyDword == dwKey)
		{
			CTList::DelObj(pItem);
			VOID* pOutData = pSelObj->pData;
			SAFE_DELETE(pSelObj);
			return pOutData;
		}
	}

	return NULL;
}

VOID* CDWORDMap::operator[](int nNum)
{
	int nCtns = 0;
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext, nCtns++)
	{
		pNext = CTList::NextObj(pItem);
		if (nNum == nCtns)
		{
			DWORD_MAP_OBJ *pSelObj = (DWORD_MAP_OBJ *)pItem;
			if (pSelObj == NULL)
				return NULL;

			return pSelObj->pData;
		}
	}
	return NULL;
}

bool CDWORDMap::insert(const DWORD dwKey, VOID* pData)
{
	if (pData == NULL)
		return false;

	DWORD_MAP_OBJ *pNewObj = new DWORD_MAP_OBJ;
	if (pNewObj == NULL)
		return false;

	pNewObj->pData = pData;
	pNewObj->dwKeyDword = dwKey;

	CTList::PushBackObj(pNewObj);
	return true;
}
