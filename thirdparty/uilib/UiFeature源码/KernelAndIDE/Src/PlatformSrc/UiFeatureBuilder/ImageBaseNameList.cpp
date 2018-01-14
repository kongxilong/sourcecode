#include "StdAfx.h"
#include "ImageBaseNameList.h"
#include "ImageBasePropEditDlg.h"
#include "LocalImageList.h"
#include "MainFrm.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "Function.h"

typedef list<IPropertyImageBase*>		IMG_BASE_ORDER_VEC;

CImageBaseNameList::CImageBaseNameList(void)
{
	m_pImgBaseDlg = NULL;
	m_pLocalImgList = NULL;
	m_pParentImgProp = NULL;
	m_pUiKernel = NULL;
	m_pSelectImgBaseProp = NULL;
}

CImageBaseNameList::~CImageBaseNameList(void)
{
}

COLORREF CImageBaseNameList::OnGetCellBkColor(int nRow, int nColum)
{
	return(nRow % 2) == 0 ? RGB(255, 255, 255) : RGB(255, 255, 255);
}

void CImageBaseNameList::Init(IUiFeatureKernel* pUiKernel, CImageBasePropEditDlg *pImgBaseDlg, CLocalImageList* pLocalImg, IPropertyImage* pParentImgProp)
{
	if (pImgBaseDlg == NULL || pLocalImg == NULL || pUiKernel == NULL || pUiKernel->GetSkinManager() == NULL)
		return;

	m_pUiKernel = pUiKernel;
	m_pImgBaseDlg = pImgBaseDlg;
	m_pLocalImgList = pLocalImg;
	m_pParentImgProp = pParentImgProp;

	this->DeleteAllItems();

	this->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	this->ModifyStyle(0, LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	this->InsertColumn(0, _T("#"), LVCFMT_LEFT, 50);
	this->InsertColumn(1, _T("图片属性名称"), LVCFMT_LEFT, 160);

	this->InsertItem(0, _T(""));
	this->SetItemText(0, 1, _T("点我属性设置为空"));

	LoadImageBaseProp();
}

void CImageBaseNameList::LoadImageBaseProp()
{
	if (m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL)
		return;
	
	IPropertyBaseMap* pMap = m_pUiKernel->GetSkinManager()->GetIPropertyBaseMap(GetSkinFileItem(), _T(PROP_TYPE_IMAGE_BASE_NAME));
	if (pMap == NULL)
		return;

	IMG_BASE_ORDER_VEC ImgBaseVec;
	ImgBaseVec.clear();

	// 先排序，按照名称排序
	for (IPropertyBaseMap::iterator pImageTemp = pMap->begin(); pImageTemp != pMap->end(); pImageTemp++)
	{
		IPropertyBase* pPropBase = pImageTemp->second;
		IPropertyImageBase* pImageProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
		if (pImageProp == NULL)
			continue;

		if (ImgBaseVec.size() <= 0)
		{
			ImgBaseVec.push_back(pImageProp);
			continue;
		}

		WCHAR* pszObjName = (WCHAR*)(pImageProp->GetObjectName());
		if (pszObjName == NULL || wcslen(pszObjName) <= 0)
		{
			ImgBaseVec.push_front(pImageProp);
			continue;
		}

		bool bInsert = false;
		for (IMG_BASE_ORDER_VEC::iterator pImgBaseItemTemp = ImgBaseVec.begin(); pImgBaseItemTemp != ImgBaseVec.end(); pImgBaseItemTemp++)
		{
			IPropertyImageBase* pImageBaseTemp = *pImgBaseItemTemp;
			if (pImageBaseTemp == NULL || pImageBaseTemp == pImageProp)
				continue;

			WCHAR* pszComObjName = (WCHAR*)(pImageBaseTemp->GetObjectName());
			if (pszComObjName == NULL || wcslen(pszComObjName) <= 0)
				continue;

			if (lstrcmpiW(pszObjName, pszComObjName) <= 0)
			{
				bInsert = true;
				ImgBaseVec.insert(pImgBaseItemTemp, pImageProp);
				break;
			}
		}

		if (!bInsert)
			ImgBaseVec.push_back(pImageProp);
	}

	int nNo = 1;
	bool bFind = false;
	for (IMG_BASE_ORDER_VEC::iterator pImgBaseItem = ImgBaseVec.begin(); pImgBaseItem != ImgBaseVec.end(); pImgBaseItem++, nNo++)
	{
		IPropertyBase* pPropBase = *pImgBaseItem;
		IPropertyImageBase* pImageProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
		if (pImageProp == NULL)
			continue;

		CString strNo(_T(""));
		strNo.Format(_T("%d"), nNo);
		this->InsertItem(nNo, strNo);
		this->SetItemText(nNo, 1, pImageProp->GetObjectName());
		this->SetItemData(nNo, (DWORD_PTR)pImageProp);

		if (m_pParentImgProp != NULL && m_pParentImgProp->GetRelevancyPropName() != NULL && !bFind)
		{
			if (lstrcmpiW(m_pParentImgProp->GetRelevancyPropName(), pImageProp->GetObjectName()) == 0)
			{
				bFind = true;
				this->SetItemState(nNo, LVIS_SELECTED, LVIS_SELECTED);

				m_nSelectItem = nNo;
				OnSelectItem();
			}
		}
	}
}

IPropertyImageBase* CImageBaseNameList::GetSelectImageBase(int &nSeleItem)
{
	nSeleItem = m_nSelectItem;
	return m_pSelectImgBaseProp;
}

bool CImageBaseNameList::OnModifyImageBaseProp(CString strName)
{
	if (m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL || strName.GetLength() <= 0 || m_pSelectImgBaseProp == NULL)
		return false;

	if (!m_pUiKernel->GetSkinManager()->BD_ModifyPropetryName(GetSkinFileItem(), dynamic_cast<IPropertyBase*>(m_pSelectImgBaseProp), strName))
		return false;

	this->SetItemText(m_nSelectItem, 1, strName);
	return true;
}

bool CImageBaseNameList::OnCreateImageBaseProp(CString strName)
{
	if (m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL || strName.GetLength() <= 0)
		return false;

	IPropertyBase* pPropBase = m_pUiKernel->GetSkinManager()->CreateEmptyBaseProp(GetSkinFileItem(), OTID_IMAGE_BASE);
	if (pPropBase == NULL)
		return false;

	IPropertyImageBase * pNewImgBase = dynamic_cast<IPropertyImageBase*>(pPropBase);
	if (pNewImgBase == NULL)
		return false;
	pNewImgBase->SetObjectName(strName);

	int nCtns = this->GetItemCount();
	CString strNo(_T(""));
	strNo.Format(_T("%d"), nCtns);
	this->InsertItem(nCtns, strNo);
	this->SetItemText(nCtns, 1, strName);
	this->SetItemData(nCtns, (DWORD_PTR)pNewImgBase);

	return true;
}

void CImageBaseNameList::OnDeleteImageBaseProp()
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
			IPropertyImageBase *pImgBaseProp = (IPropertyImageBase *)this->GetItemData(i);
			m_pUiKernel->GetSkinManager()->DeletePropetryBaseProp(GetSkinFileItem(), dynamic_cast<IPropertyBase*>(pImgBaseProp));

			if (m_pLocalImgList != NULL)
				m_pLocalImgList->SetSelectImagePropBase(NULL);

			m_nSelectItem = -1;
			m_pSelectImgBaseProp = NULL;
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

//////////////////////////////////////////////////////////////////////////
	// 删除之后需要不显示图片，一个暂时的处理，TBD
	if (m_pImgBaseDlg != NULL && m_pImgBaseDlg->GetImageBaseView() != NULL)
		m_pImgBaseDlg->GetImageBaseView()->SetCurrentShowImage(m_pImgBaseDlg, m_pUiKernel, NULL, NULL);

	if (m_pLocalImgList != NULL)
		m_pLocalImgList->SetSelectImagePropBase(NULL);
//////////////////////////////////////////////////////////////////////////

	if (!bDelete)
		AfxMessageBox(_T("请先勾选属性列表中复选框，选择需要删除的图片属性，再进行删除操作！"), MB_OK | MB_ICONWARNING);
}

void CImageBaseNameList::OnSelectItem()
{
	m_pSelectImgBaseProp = NULL;
	if (m_pImgBaseDlg == NULL)
		return;

	if (m_nSelectItem >= 1)
		m_pSelectImgBaseProp = (IPropertyImageBase*)this->GetItemData(m_nSelectItem);

	if (m_pSelectImgBaseProp == NULL)
		return;

	m_pImgBaseDlg->SetSelectImageBaseFromListSelect(m_pSelectImgBaseProp);

	if (m_pLocalImgList != NULL)
		m_pLocalImgList->SetSelectImagePropBase(m_pSelectImgBaseProp);

	RefreshImageBaseShow();
}

void CImageBaseNameList::RefreshImageBaseShow()
{
	if (m_pSelectImgBaseProp == NULL)
	{
		if (m_pImgBaseDlg != NULL)
		{
			m_pImgBaseDlg->m_nShowAreaLeft = 0;
			m_pImgBaseDlg->m_nShowAreaTop = 0;
			m_pImgBaseDlg->m_nShowAreaBottom = 0;
			m_pImgBaseDlg->m_nShowAreaRight = 0;
			m_pImgBaseDlg->m_nJggLeft = 0;
			m_pImgBaseDlg->m_nJggTop = 0;
			m_pImgBaseDlg->m_nJggRight = 0;
			m_pImgBaseDlg->m_nJggBottom = 0;
			m_pImgBaseDlg->m_nSelelShowImgType = 0;
			m_pImgBaseDlg->m_Jgg_MidDrawCheckBox.SetCheck(0);
		}
	}
	else
	{
		IMAGE_BASE_PROP* pImgProp = m_pSelectImgBaseProp->GetImageProp();
		if (pImgProp == NULL)
		{
			if (m_pImgBaseDlg != NULL)
				m_pImgBaseDlg->SetImageEditEnableStyle((m_nSelectItem >= 1));
			return;
		}

		if (m_pImgBaseDlg != NULL)
		{
			m_pImgBaseDlg->m_nShowAreaLeft = pImgProp->RectInImage.left;
			m_pImgBaseDlg->m_nShowAreaTop = pImgProp->RectInImage.top;
			m_pImgBaseDlg->m_nShowAreaBottom = pImgProp->RectInImage.bottom;
			m_pImgBaseDlg->m_nShowAreaRight = pImgProp->RectInImage.right;
			m_pImgBaseDlg->m_nJggLeft = pImgProp->jggInfo.left;
			m_pImgBaseDlg->m_nJggTop = pImgProp->jggInfo.top;
			m_pImgBaseDlg->m_nJggRight = pImgProp->jggInfo.right;
			m_pImgBaseDlg->m_nJggBottom = pImgProp->jggInfo.bottom;
			m_pImgBaseDlg->m_nSelelShowImgType = pImgProp->ImgShowType;
			m_pImgBaseDlg->m_Jgg_MidDrawCheckBox.SetCheck(pImgProp->bIsDrawJggMid ? 1 : 0);
		}
	}

	if (m_pImgBaseDlg != NULL)
	{
		m_pImgBaseDlg->UpdateData(FALSE);
		m_pImgBaseDlg->SetImageEditEnableStyle((m_nSelectItem >= 1));
	}
}
