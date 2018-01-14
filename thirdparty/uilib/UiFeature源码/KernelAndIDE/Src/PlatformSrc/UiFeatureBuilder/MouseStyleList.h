
#pragma once
#include "featurebaselist.h"
#include "..\..\Inc\IPropertyBase.h"
#include "..\..\Inc\IPropertyCursor.h"
#include "..\..\Inc\IPropertyCursorBase.h"


class CMouseStyleList : public CFeatureBaseList
{
public:
	CMouseStyleList();
	virtual ~CMouseStyleList();

	void InitMouseStyleList(IPropertyCursor* pParentCursorProp, IPropertyBaseMap* pCursorBaseMap);

protected:
	virtual COLORREF OnGetCellBkColor(int nRow, int nColum);
	virtual void OnSelectItem();
	void AddCursorInfo(IPropertyCursorBase* pCursorBaseProp, int nNo);

private:
	IPropertyCursor* m_pParentCursorProp;
	CImageList m_CursorImageList;
	IPropertyBaseMap* m_pCursorBaseMap;
};
