
#include "stdafx.h"
#include "..\..\Inc\CTList.h"

ListObj::ListObj()
{
	pPrior = pNext = NULL;
}

ListObj::~ListObj()
{
}

CTList::CTList()
{
	Init();
}

CTList::~CTList()
{
	Init();
}

void CTList::Init()
{
	m_nCtns = 0;
	ListHeader.pPrior = ListHeader.pNext = &ListHeader;
}

int CTList::Size()
{
	int nCtns = 0;
	ListObj* pNext = NULL;
	for (ListObj* pItem = TopObj(); pItem != NULL; pItem = pNext)
	{
		nCtns++;
		pNext = NextObj(pItem);
	}

	return nCtns;
}

// 清空列表
void CTList::Clear()
{
	Init();
}

BOOL CTList::IsEmpty()
{
	return (ListHeader.pNext == &ListHeader);
}

// 添加到队列的末尾，环形列表
void CTList::PushBackObj(ListObj *pListObj)
{
	if (pListObj != NULL)
	{
		pListObj->pPrior = ListHeader.pPrior;
		pListObj->pNext = &ListHeader;
		ListHeader.pPrior->pNext = pListObj;
		ListHeader.pPrior = pListObj;
		m_nCtns++;
	}
}

// 添加到队列的头，环形列表
void CTList::PushFrontObj(ListObj *pListObj)
{
	if (pListObj != NULL)
	{
		pListObj->pNext = ListHeader.pNext;
		pListObj->pPrior = &ListHeader;
		ListHeader.pNext->pPrior = pListObj;
		ListHeader.pNext = pListObj;
		m_nCtns++;
	}
}

void CTList::DelObj(ListObj *pListObj)
{
	if (pListObj != NULL)
	{
		if (pListObj->pNext != NULL)
		{
			pListObj->pNext->pPrior = pListObj->pPrior;
		}

		if (pListObj->pPrior != NULL)
		{
			pListObj->pPrior->pNext = pListObj->pNext;
		}

		pListObj->pNext = pListObj->pPrior = NULL;
		m_nCtns--;
	}
}

ListObj* CTList::TopObj()
{
	return ((ListHeader.pNext == &ListHeader) ? NULL : ListHeader.pNext);
}

ListObj* CTList::LastObj()
{
	return ((ListHeader.pPrior == &ListHeader) ? NULL : ListHeader.pPrior);
}

ListObj* CTList::NextObj(ListObj *pListObj)
{
	ListObj* pRetObj = NULL;

	if (pListObj != NULL)
		pRetObj = ((pListObj->pNext == &ListHeader) ? NULL : pListObj->pNext);

	return pRetObj;
}

ListObj* CTList::PriorObj(ListObj *pListObj)
{
	ListObj* pRetObj = NULL;

	if (pListObj != NULL)
		pRetObj = ((pListObj->pPrior == &ListHeader) ? NULL : pListObj->pPrior);

	return pRetObj;
}

void CTList::MoveList(CTList *pFromList)
{
	if (pFromList != NULL)
	{
		if (pFromList->ListHeader.pNext != &pFromList->ListHeader)
		{
			if (ListHeader.pNext == &ListHeader)
			{
				ListHeader = pFromList->ListHeader;
				ListHeader.pNext->pPrior = ListHeader.pPrior->pNext = &ListHeader;
			}
			else
			{
				ListHeader.pPrior->pNext = pFromList->ListHeader.pNext;
				pFromList->ListHeader.pNext->pPrior = ListHeader.pPrior;
				pFromList->ListHeader.pPrior->pNext = &ListHeader;
				ListHeader.pPrior = pFromList->ListHeader.pPrior;
			}
			m_nCtns += pFromList->m_nCtns;
			pFromList->Init();
		}
	}
}

// 交换Object位置
bool CTList::ChangeObjPos(ListObj *pFromObj, ListObj *pToObj)
{
	if (pFromObj == NULL || pToObj == NULL)
		return false;

	if (pFromObj->pNext == pToObj)
	{
		ListObj *pPrior = pFromObj->pPrior;
		ListObj *pNext= pToObj->pNext;
		if (pPrior == NULL || pNext == NULL)
			return false;

		pPrior->pNext = pToObj;
		pToObj->pPrior = pPrior;
		pToObj->pNext = pFromObj;
		pFromObj->pPrior = pToObj;
		pFromObj->pNext = pNext;
		pNext->pPrior = pFromObj;
	}
	else if (pFromObj->pPrior == pToObj)
	{
		ListObj *pPrior = pToObj->pPrior;
		ListObj *pNext= pFromObj->pNext;
		if (pPrior == NULL || pNext == NULL)
			return false;

		pPrior->pNext = pFromObj;
		pFromObj->pPrior = pPrior;
		pFromObj->pNext = pToObj;
		pToObj->pPrior = pFromObj;
		pToObj->pNext = pNext;
		pNext->pPrior = pToObj;
	}
	else
	{
		ListObj *pFromPrior = pFromObj->pPrior;
		ListObj *pFromNext= pFromObj->pNext;
		ListObj *pToPrior = pToObj->pPrior;
		ListObj *pToNext= pToObj->pNext;
		if (pFromPrior == NULL || pFromNext == NULL || pToPrior == NULL || pToNext == NULL)
			return false;

		pFromPrior->pNext = pToObj;
		pToObj->pPrior = pFromPrior;
		pToObj->pNext = pFromNext;
		pFromNext->pPrior = pToObj;

		pToPrior->pNext = pFromObj;
		pFromObj->pPrior = pToPrior;
		pFromObj->pNext = pToNext;
		pToNext->pPrior = pFromObj;
	}

	return true;
}
