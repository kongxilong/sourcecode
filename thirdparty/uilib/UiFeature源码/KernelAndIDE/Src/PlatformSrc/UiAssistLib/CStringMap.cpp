
#include "StdAfx.h"
#include "..\..\Inc\CStringMap.h"
#include "..\..\Inc\UiFeatureDefs.h"

STRING_MAP_OBJ::STRING_MAP_OBJ()
{
	pKeyString = NULL;
	pData = NULL;
}

STRING_MAP_OBJ::~STRING_MAP_OBJ()
{

}

CStringMap::CStringMap()
{
}

CStringMap::~CStringMap()
{
}

void CStringMap::clear()
{
	Clear();
}

void CStringMap::Clear()
{
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		STRING_MAP_OBJ *pDelObj = (STRING_MAP_OBJ *)pItem;
		if (pDelObj == NULL)
			continue;

		SAFE_DELETE_LIST(pDelObj->pKeyString);
		SAFE_DELETE(pDelObj);
	}
	CTList::Clear();
}

VOID* CStringMap::find(const char *pKeyString, STRING_MAP_OBJ** ppOutObj)
{
	if (pKeyString == NULL || strlen(pKeyString) <= 0)
		return NULL;

	if (ppOutObj != NULL)
		*ppOutObj = NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		STRING_MAP_OBJ *pSelObj = (STRING_MAP_OBJ *)pItem;
		if (pSelObj != NULL && pSelObj->pKeyString != NULL)
		{
			if (lstrcmpiA(pKeyString, pSelObj->pKeyString) == 0)
			{
				if (ppOutObj != NULL)
					*ppOutObj = pSelObj;
				return pSelObj->pData;
			}
		}
	}
	return NULL;
}

int CStringMap::size()
{
	return Size();
}

VOID* CStringMap::operator[](int nNum)
{
	int nCtns = 0;
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext, nCtns++)
	{
		pNext = CTList::NextObj(pItem);
		if (nNum == nCtns)
		{
			STRING_MAP_OBJ *pSelObj = (STRING_MAP_OBJ *)pItem;
			if (pSelObj == NULL)
				return NULL;

			return pSelObj->pData;
		}
	}
	return NULL;
}

bool CStringMap::insert(const char * pszKeyString, VOID* pData)
{
	if (pszKeyString == NULL || strlen(pszKeyString) <= 0 || pData == NULL)
		return false;

	int nLen = strlen(pszKeyString);

	STRING_MAP_OBJ *pNewObj = new STRING_MAP_OBJ;
	if (pNewObj == NULL)
		return false;

	pNewObj->pData = pData;
	pNewObj->pKeyString = new char(nLen + 2);
	if (pNewObj->pKeyString == NULL)
	{
		SAFE_DELETE(pNewObj);
		return false;
	}

	memset(pNewObj->pKeyString, 0, nLen + 2);
	sprintf_s(pNewObj->pKeyString, nLen + 1, "%s", pszKeyString);

	CTList::PushBackObj(pNewObj);
	return true;
}
