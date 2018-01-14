
#pragma once
#include "featurebaselist.h"
#include "..\..\Inc\IPropertyBase.h"
#include "..\..\Inc\IPropertyFont.h"
#include "..\..\Inc\IPropertyFontBase.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "ModifyPropBaseNameDlg.h"

class CModifyFontBaseDlg;
class CFontBaseNameList : public CFeatureBaseList
{
public:
	CFontBaseNameList();
	virtual ~CFontBaseNameList();

	void InitFontBaseNameList(CModifyFontBaseDlg* pDlg, IUiFeatureKernel* pUiKernel, IPropertyFont* pParentCursorProp, IPropertyBaseMap* pFontBaseMap);
	void NewFontBase(CString strName);
	void DeleteCurrentFontBase();
	void ModifyCurrentFontBase();

protected:
	virtual COLORREF OnGetCellBkColor(int nRow, int nColum);
	virtual void OnSelectItem();
	bool FindNameInFontBaseNameList(CString &strName);
	void DeleteFontBase(IPropertyFontBase* pFontBaseProp);

private:
	CModifyPropBaseNameDlg m_ModifyFontDlg;
	IPropertySkinManager* m_pSkinMgr;
	IUiFeatureKernel* m_pUiKernel;
	CModifyFontBaseDlg* m_pOwnerDlg;
	IPropertyFont* m_pParentFontProp;
	IPropertyBaseMap* m_pFontBaseMap;
};
