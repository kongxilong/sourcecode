
#include "StdAfx.h"
#include "FontBaseNameList.h"
#include "resource.h"
#include "UiFeatureBuilder.h"
#include "ModifyFontBaseDlg.h"
#include "MainFrm.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "Function.h"

typedef list<IPropertyFontBase*>		FONT_BASE_ORDER_VEC;

CFontBaseNameList::CFontBaseNameList()
{
	m_pFontBaseMap = NULL;
	m_pParentFontProp = NULL;
	m_pOwnerDlg = NULL;
	m_pSkinMgr = NULL;
	m_pUiKernel = NULL;
}

CFontBaseNameList::~CFontBaseNameList()
{
}

COLORREF CFontBaseNameList::OnGetCellBkColor(int nRow, int nColum)
{
	return(nRow % 2) == 0 ? RGB(255, 255, 255) : RGB(255, 255, 255);
}

void CFontBaseNameList::OnSelectItem()
{
	if (m_pParentFontProp == NULL || m_pOwnerDlg == NULL)
		return;

	if (m_nSelectItem <= 0)
	{
		// 没有选择
		m_pParentFontProp->SetFontBaseProp(NULL);
		m_pParentFontProp->SetRelevancyPropName(NULL);
		m_pOwnerDlg->UpdateCurrentFontBaseProp(NULL);
	}
	else
	{
		IPropertyFontBase* pFontBaseProp = (IPropertyFontBase*)(this->GetItemData(m_nSelectItem));
		m_pParentFontProp->SetFontBaseProp(pFontBaseProp);
		m_pParentFontProp->SetRelevancyPropName((WCHAR*)pFontBaseProp->GetObjectName());
		m_pOwnerDlg->UpdateCurrentFontBaseProp(pFontBaseProp);
	}
}

void CFontBaseNameList::InitFontBaseNameList(CModifyFontBaseDlg* pDlg, IUiFeatureKernel* pUiKernel, IPropertyFont* pParentCursorProp, IPropertyBaseMap* pFontBaseMap)
{
	if (pFontBaseMap == NULL || pDlg == NULL || pUiKernel == NULL)
		return;

	m_pUiKernel = pUiKernel;
	m_pSkinMgr = m_pUiKernel->GetSkinManager();
	m_pOwnerDlg = pDlg;
	m_pParentFontProp = pParentCursorProp;

	this->DeleteAllItems();
	m_pFontBaseMap = pFontBaseMap;

	this->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	this->ModifyStyle(0, LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	this->InsertColumn(0, _T("#"), LVCFMT_LEFT, 35);
	this->InsertColumn(1, _T("名称"), LVCFMT_LEFT, 100);

	this->InsertItem(0, _T("#"));
	this->SetItemText(0, 1, _T("点我设置FONT属性为空哦"));
	this->SetItemData(0, NULL);

	FONT_BASE_ORDER_VEC FontBaseVec;
	FontBaseVec.clear();

	// 先排序，按照名称排序
	for (IPropertyBaseMap::iterator pFontBaseItem = m_pFontBaseMap->begin();
		pFontBaseItem != m_pFontBaseMap->end(); pFontBaseItem++)
	{
		IPropertyFontBase* pFontBaseProp = dynamic_cast<IPropertyFontBase*>(pFontBaseItem->second);
		if (pFontBaseProp == NULL || pFontBaseProp->GetObjectName() == NULL || wcslen(pFontBaseProp->GetObjectName()) <= 0)
			continue;

		if (FontBaseVec.size() <= 0)
		{
			FontBaseVec.push_back(pFontBaseProp);
			continue;
		}

		WCHAR* pszObjName = (WCHAR*)pFontBaseProp->GetObjectName();
		if (pszObjName == NULL || wcslen(pszObjName) <= 0)
		{
			FontBaseVec.push_front(pFontBaseProp);
			continue;
		}

		bool bInsert = false;
		for (FONT_BASE_ORDER_VEC::iterator pFontBaseItemTemp = FontBaseVec.begin(); pFontBaseItemTemp != FontBaseVec.end(); pFontBaseItemTemp++)
		{
			IPropertyFontBase* pFontBaseTemp = *pFontBaseItemTemp;
			if (pFontBaseTemp == NULL || pFontBaseTemp == pFontBaseProp)
				continue;

			WCHAR* pszComObjName = (WCHAR*)(pFontBaseTemp->GetObjectName());
			if (pszComObjName == NULL || wcslen(pszComObjName) <= 0)
				continue;

			if (lstrcmpiW(pszObjName, pszComObjName) <= 0)
			{
				bInsert = true;
				FontBaseVec.insert(pFontBaseItemTemp, pFontBaseProp);
				break;
			}
		}

		if (!bInsert)
			FontBaseVec.push_back(pFontBaseProp);
	}

	int nNo = 1;
	for (FONT_BASE_ORDER_VEC::iterator pFontBaseItemTemp = FontBaseVec.begin(); pFontBaseItemTemp != FontBaseVec.end(); pFontBaseItemTemp++)
	{
		IPropertyFontBase* pFontBaseProp = *pFontBaseItemTemp;
		if (pFontBaseProp == NULL || pFontBaseProp->GetObjectName() == NULL || wcslen(pFontBaseProp->GetObjectName()) <= 0)
			continue;

		CString strNo(_T(""));
		strNo.Format(_T("%d"), nNo);
		this->InsertItem(nNo, strNo);
		this->SetItemText(nNo, 1, pFontBaseProp->GetObjectName());
		this->SetItemData(nNo, (DWORD_PTR)pFontBaseProp);

		if (pFontBaseProp == m_pParentFontProp->GetFontBaseProp())
		{
			m_nSelectItem = nNo;
			this->SetItemState(nNo, LVIS_SELECTED, LVIS_SELECTED);
		}

		nNo++;
	}
}

void CFontBaseNameList::NewFontBase(CString strName)
{
	if (strName.GetLength() <= 0 || m_pUiKernel == NULL || m_pSkinMgr == NULL)
		return;

	IPropertyFontBase* pNewFontBase = dynamic_cast<IPropertyFontBase*>(m_pSkinMgr->CreateEmptyBaseProp(GetSkinFileItem(), OTID_FONT_BASE));
	if (pNewFontBase == NULL)
		return;

	pNewFontBase->SetObjectName(strName);

	int nCtns = this->GetItemCount();
	CString strNo(_T(""));
	strNo.Format(_T("%d"), nCtns);

	this->InsertItem(nCtns, strNo);
	this->SetItemText(nCtns, 1, strName);
	this->SetItemData(nCtns, (DWORD_PTR)pNewFontBase);
	this->SetItemState(nCtns, LVIS_SELECTED, LVIS_SELECTED);
	m_pOwnerDlg->UpdateCurrentFontBaseProp(pNewFontBase);
	m_nSelectItem = nCtns;
}

void CFontBaseNameList::DeleteCurrentFontBase()
{
	bool bDelete = false;
	int nCtns = this->GetItemCount();
	for (int i = 1; i < nCtns; )
	{
		BOOL bCheck = this->GetCheck(i);
		if (bCheck)
		{
			bDelete = true;
			// 选中，删除
			IPropertyFontBase *pFontBaseProp = (IPropertyFontBase *)this->GetItemData(i);
			DeleteFontBase(pFontBaseProp);

			m_nSelectItem = -1;
			this->DeleteItem(i);
			nCtns = this->GetItemCount();
			if (nCtns <= 1)
				return;

			i = 1;
		}
		else
		{
			i++;
		}
	}

	nCtns = this->GetItemCount();
	if (nCtns >= 1)
		this->SetCheck(0, FALSE);

	if (bDelete)
		m_pOwnerDlg->UpdateCurrentFontBaseProp(NULL);
	else
		AfxMessageBox(_T("请先勾选属性列表中复选框，选择需要删除的字体属性，再进行删除操作！"), MB_OK | MB_ICONWARNING);

}

void CFontBaseNameList::DeleteFontBase(IPropertyFontBase* pFontBaseProp)
{
	if (pFontBaseProp == NULL)
		return;

	if (lstrcmpW(pFontBaseProp->GetObjectName(), _T(SYS_DEFAULT_FONT_NAME)) == 0)
	{
		int nRetId = AfxMessageBox(_T("确定要删除系统默认字体吗，删除此字体可能会造成系统异常？"), MB_YESNO | MB_ICONWARNING);
		if (nRetId != IDYES)
			return;
	}

	m_pUiKernel->GetSkinManager()->DeletePropetryBaseProp(GetSkinFileItem(), pFontBaseProp);
}

void CFontBaseNameList::ModifyCurrentFontBase()
{
	if (m_pSkinMgr == NULL || m_pOwnerDlg == NULL)
		return;

	if (m_nSelectItem <= 0)
	{
		AfxMessageBox(_T("请选择需要改名的FONT属性！"), MB_OK | MB_ICONERROR);
		return;
	}

	IPropertyFontBase* pFontBaseProp = (IPropertyFontBase*)(this->GetItemData(m_nSelectItem));
	if (pFontBaseProp == NULL)
		return;

	m_ModifyFontDlg.SetWindowInfomation(_T("修改字体属性名称"));
	if (m_ModifyFontDlg.DoModal() != IDOK)
		return;

	CString strName;
	m_ModifyFontDlg.GetPropBaseName(strName);
	if (strName.GetLength() <= 0 || FindNameInFontBaseNameList(strName))
		return;

	if (!m_pUiKernel->GetSkinManager()->BD_ModifyPropetryName(GetSkinFileItem(), dynamic_cast<IPropertyBase*>(pFontBaseProp), strName))
		return;

	this->SetItemText(m_nSelectItem, 1, strName);
}

bool CFontBaseNameList::FindNameInFontBaseNameList(CString &strName)
{
	for (int i = 1; i < this->GetItemCount(); i++)
	{
		CString strComName = this->GetItemText(i, 1);
		if (strComName.CompareNoCase(strName) == 0)
		{
			this->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			CString strInfo(_T(""));
			strInfo.Format(_T("已经存在名为【%s】的字体属性！"), strComName);
			AfxMessageBox(strInfo, MB_OK | MB_ICONERROR);
			return true;
		}
	}

	return false;
}
