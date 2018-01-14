#include "StdAfx.h"
#include "..\..\Inc\CStringObjList.h"
#include "..\..\Inc\UiFeatureDefs.h"


STRING_OBJ::STRING_OBJ()
{
	pString = NULL;
	nFlag = -1;
}

STRING_OBJ::~STRING_OBJ()
{

}

CStringObjList::CStringObjList()
{
}

CStringObjList::~CStringObjList()
{
	clear();
}

void CStringObjList::clear()
{
	Clear();
}

void CStringObjList::Clear()
{
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		STRING_OBJ *pDelObj = (STRING_OBJ *)pItem;
		if (pDelObj == NULL)
			continue;

		SAFE_DELETE(pDelObj->pString);
		SAFE_DELETE(pDelObj);
	}
	CTList::Clear();
}

const char* CStringObjList::find(int nFlag)
{
	if (nFlag <= -1)
		return NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		STRING_OBJ *pSelObj = (STRING_OBJ *)pItem;
		if (pSelObj != NULL && pSelObj->nFlag == nFlag)
			return pSelObj->pString;
	}
	return NULL;
}

bool CStringObjList::ResetStringByFlag(const char * pszString, int nFlag)
{
	if (pszString == NULL || nFlag <= -1)
		return false;

	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = CTList::NextObj(pItem);
		STRING_OBJ *pSelObj = (STRING_OBJ *)pItem;
		if (pSelObj != NULL && pSelObj->nFlag == nFlag)
		{
			CTList::DelObj(pItem);
			SAFE_DELETE(pSelObj->pString);
			SAFE_DELETE(pSelObj);
			break;
		}
	}

	return push_back(pszString, nFlag);
}

int CStringObjList::size()
{
	return Size();
}

bool CStringObjList::push_back(const char * pszString, int nFlag)
{
	return PushBackString(pszString, nFlag);
}

bool CStringObjList::push_back(char * pszString, int nFlag)
{
	return PushBackString(pszString, nFlag);
}

bool CStringObjList::PushBackString(char * pszString, int nFlag)
{
	return PushBackString((const char *)pszString, nFlag);
}

const char * CStringObjList::operator[](int nNum)
{
	int nCtns = 0;
	ListObj* pNext = NULL;
	for (ListObj* pItem = CTList::TopObj(); pItem != NULL; pItem = pNext, nCtns++)
	{
		pNext = CTList::NextObj(pItem);
		if (nNum == nCtns)
		{
			STRING_OBJ *pSelObj = (STRING_OBJ *)pItem;
			if (pSelObj == NULL)
				return NULL;

			return (const char *)pSelObj->pString;
		}
	}
	return NULL;
}

bool CStringObjList::PushBackString(const char * pszString, int nFlag)
{
	if (pszString == NULL || strlen(pszString) <= 0)
		return false;

	int nLen = strlen(pszString);

	STRING_OBJ *pNewObj = new STRING_OBJ;
	if (pNewObj == NULL)
		return false;

	pNewObj->nFlag = nFlag;
	pNewObj->pString = new char(nLen + 2);
	if (pNewObj->pString == NULL)
	{
		SAFE_DELETE(pNewObj);
		return false;
	}

	memset(pNewObj->pString, 0, nLen + 2);
	sprintf_s(pNewObj->pString, nLen + 1, "%s", pszString);

	CTList::PushBackObj(pNewObj);
	return true;
}
