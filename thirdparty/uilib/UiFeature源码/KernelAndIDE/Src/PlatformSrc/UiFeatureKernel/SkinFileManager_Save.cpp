
#include "StdAfx.h"
#include "SkinFileManager.h"
#include "IUiFeatureKernelImpl.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "IPropertyFontImpl.h"
#include "IPropertyFontBaseImpl.h"
#include "IPropertyBoolImpl.h"
#include "IPropertyColorImpl.h"
#include "IPropertyComboBoxImpl.h"
#include "IPropertyCursorImpl.h"
#include "IPropertyCursorBaseImpl.h"
#include "IPropertyImageImpl.h"
#include "IPropertyImageBaseImpl.h"
#include "IPropertyIntImpl.h"
#include "IPropertyStringImpl.h"
#include "IPropertyGroupImpl.h"
#include "IPropertyControlImpl.h"
#include "IWindowBaseImpl.h"
#include "CriSec.h"


// 保存皮肤包
bool CSkinFileManager::SaveSkinFileItem(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSkinDir, LPCWSTR pszSkinName, char *pszPassword)
{
	if (pszSkinDir == NULL || pszSkinName == NULL || pSkinFileItem == NULL || pSkinFileItem->pZipFileMgr == NULL)
		return false;

	IZipFileImpl WriteZip;

	CStringW strDir = pszSkinDir;
	if (pszSkinDir[wcslen(pszSkinDir) - 1] != '\\')
		strDir += L"\\";

	// 保存 Resource.xml
	CStringW strResourceXmlPath = strDir;
	strResourceXmlPath += _T(RESOURCE_XML_NAME);
	if (!SaveResourceXml(pSkinFileItem, strResourceXmlPath))
		return false;

	// 保存 Controls.xml
	CStringW strControlsXmlPath = strDir;
	strControlsXmlPath += _T(CONTROLS_XML_NAME);
	if (!SaveControlsXml(pSkinFileItem, strControlsXmlPath))
		return false;

	// 保存 Windows.xml
	CStringW strWindowsXmlPath = strDir;
	strWindowsXmlPath += _T(WINDOWS_XML_NAME);
	if (!SaveWindowsXml(pSkinFileItem, strWindowsXmlPath))
		return false;

	// 保存 Layout.xml
	CStringW strLayoutXmlPath = strDir;
	strLayoutXmlPath += _T(LAYOUT_XML_NAME);
	if (!SaveLayoutXml(pSkinFileItem, strLayoutXmlPath))
		return false;

	CStringW strZipFile(strDir);
	strZipFile += pszSkinName;
	strZipFile += _T(NAME_SKIN_FILE_EX_NAME);

	// 写入zip文件
	if (!WriteZip.WriteZipInit(pszSkinDir, strZipFile, SV_2, pszPassword))
		return false;

	if (!WriteZip.WriteZipAppendFile(_T(RESOURCE_XML_NAME), ZFT_NORMAL))
		return false;

	if (!WriteZip.WriteZipAppendFile(_T(CONTROLS_XML_NAME), ZFT_NORMAL))
		return false;

	if (!WriteZip.WriteZipAppendFile(_T(WINDOWS_XML_NAME), ZFT_NORMAL))
		return false;

	if (!WriteZip.WriteZipAppendFile(_T(LAYOUT_XML_NAME), ZFT_NORMAL))
		return false;

	::DeleteFileW(strResourceXmlPath);
	::DeleteFileW(strControlsXmlPath);
	::DeleteFileW(strWindowsXmlPath);
	::DeleteFileW(strLayoutXmlPath);

	// 写入图片文件
	for (ZIP_FILE_MAP::iterator pZipItem = pSkinFileItem->pZipFileMgr->GetUnZipFileMap()->begin();
		pZipItem != pSkinFileItem->pZipFileMgr->GetUnZipFileMap()->end(); pZipItem++)
	{
		ZIP_FILE* pZip = pZipItem->second;
		if (pZip == NULL)
			continue;

		if (!WriteZip.WriteZipAppendStruct(pZip))
			return false;
	}

	return WriteZip.WriteZipEnd(true);
}

bool CSkinFileManager::SaveLayoutXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath)
{
	if (pSkinFileItem == NULL || pszSavePath == NULL)
		return false;

	xml_document XmlStrObj;
	xml_node RootNode = XmlStrObj.append_child(L"layout");
	if (!RootNode)
		return false;

	AddIntAttrToNode(&RootNode, L"area", m_AreaType);
	AddIntAttrToNode(&RootNode, L"lastobjid", IUiFeatureKernelImpl::GetInstance()->KernelGetId());

	for (IPropertyBaseMap::iterator pWndPropItem = pSkinFileItem->LayoutWindowMap.begin(); pWndPropItem != pSkinFileItem->LayoutWindowMap.end(); pWndPropItem++)
	{
		IPropertyWindow *pPropWnd = dynamic_cast<IPropertyWindow*>(pWndPropItem->second);
		if (pPropWnd == NULL)
			continue;

		if (!pPropWnd->GetActivePropetry())
			continue;

		xml_node WndNode = RootNode.append_child(L"window");
		if (!WndNode)
			return false;

		AddIntAttrToNode(&WndNode, _T(SKIN_OBJECT_ID), (int)pPropWnd->GetObjectId());
		AddStringAttrToNode(&WndNode, L"name", pPropWnd->GetObjectName());

		PROP_CONTROL_VEC* pWndChildVec = pPropWnd->GetChildPropControlVec();
		if (!SaveLayoutXml_ChildCtrl(XmlStrObj, &WndNode, pWndChildVec))
			return false;
	}

	return XmlStrObj.save_file(pszSavePath, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

bool CSkinFileManager::SaveLayoutXml_ChildCtrl(xml_document &XmlStrObj, xml_node* pNode, PROP_CONTROL_VEC* pWndChildVec)
{
	if (pWndChildVec == NULL || pNode == NULL)
		return false;

	for (int i = 0; i < (int)pWndChildVec->size(); i++)
	{
		IPropertyControl* pPropCtrl = (*pWndChildVec)[i];
		if (pPropCtrl == NULL)
			continue;

		if (!pPropCtrl->GetActivePropetry())
			continue;

		xml_node CtrlNode = pNode->append_child(L"ctrl");
		if (!CtrlNode)
			return false;

		AddIntAttrToNode(&CtrlNode, _T(SKIN_OBJECT_ID), (int)pPropCtrl->GetObjectId());
		AddStringAttrToNode(&CtrlNode, L"name", pPropCtrl->GetObjectName());

		PROP_CONTROL_VEC* pCtrlChildVec = pPropCtrl->GetChildPropControlVec();
		if (!SaveLayoutXml_ChildCtrl(XmlStrObj, &CtrlNode, pCtrlChildVec))
			return false;
	}
	return true;
}

bool CSkinFileManager::SaveResourceXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath)
{
	if (pSkinFileItem == NULL || pszSavePath == NULL)
		return false;

	xml_document XmlStrObj;
	xml_node RootNode = XmlStrObj.append_child(L"resource");
	if (!RootNode)
		return false;

	for (IPropertyBaseMapMap::iterator pPropTypeItem = pSkinFileItem->AllPropMap.begin(); pPropTypeItem != pSkinFileItem->AllPropMap.end(); pPropTypeItem++)
	{
		IPropertyBaseMap *pPropType = pPropTypeItem->second;
		if (pPropType == NULL)
			continue;

		CStringW strPropType = pPropTypeItem->first;		
		xml_node PropTypeNode = RootNode.append_child(strPropType);
		if (!PropTypeNode)
			return false;

		for (IPropertyBaseMap::iterator pPropItem = pPropType->begin(); pPropItem != pPropType->end(); pPropItem++)
		{
			IPropertyBase* pPropBase = dynamic_cast<IPropertyBase*>(pPropItem->second);
			if (pPropBase == NULL)
				continue;

			if (!pPropBase->GetActivePropetry())
				continue;

			if (!pPropBase->AppendToXmlNode(XmlStrObj, &PropTypeNode))
				return false;
		}
	}

	return XmlStrObj.save_file(pszSavePath, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

bool CSkinFileManager::SaveControlsXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath)
{
	if (pSkinFileItem == NULL || pszSavePath == NULL)
		return false;

	xml_document XmlStrObj;
	xml_node RootNode = XmlStrObj.append_child(L"controls");
	if (!RootNode)
		return false;

	for (IPropertyBaseMapMap::iterator pCtrlTypeItem = pSkinFileItem->AllCtrlPropMap.begin(); pCtrlTypeItem != pSkinFileItem->AllCtrlPropMap.end(); pCtrlTypeItem++)
	{
		IPropertyBaseMap *pCtrlType = pCtrlTypeItem->second;
		if (pCtrlType == NULL)
			continue;

		for (IPropertyBaseMap::iterator pCtrlItem = pCtrlType->begin(); pCtrlItem != pCtrlType->end(); pCtrlItem++)
		{
			IPropertyGroup* pCtrlPropGroup = dynamic_cast<IPropertyGroup*>(pCtrlItem->second);
			if (pCtrlPropGroup == NULL)
				continue;

			if (!pCtrlPropGroup->GetActivePropetry())
				continue;

			CStringW strCtrlType = pCtrlTypeItem->first;
			xml_node CtrlTypeNode = RootNode.append_child(strCtrlType);
			if (!CtrlTypeNode)
				return false;

			AddIntAttrToNode(&CtrlTypeNode, _T(SKIN_OBJECT_ID), (int)pCtrlPropGroup->GetObjectId());

			// 写入group中的数据
			if (!SaveXml_GroupProp(XmlStrObj, &CtrlTypeNode, pCtrlPropGroup))
				return false;
		}
	}

	return XmlStrObj.save_file(pszSavePath, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

bool CSkinFileManager::SaveXml_GroupProp(xml_document &XmlStrObj, xml_node* pParentNode, IPropertyGroup *pPropGroup)
{
	if (pParentNode == NULL || pPropGroup == NULL)
		return false;

	GROUP_PROP_VEC* pPropVec = pPropGroup->GetPropVec();
	if (pPropVec == NULL)
		return false;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pPropVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pPropVec->NextObj(pItem);

		IPropertyBase* pProp = (IPropertyBase*)pItem;
		if (pProp == NULL)
			continue;

		if (!pProp->GetActivePropetry())
			continue;

		xml_node PropNode = pParentNode->append_child(pProp->GetObjectType());
		if (!PropNode)
			return false;

		AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)pProp->GetObjectId());
		// 写上属性名称，便于查看
		AddStringAttrToNode(&PropNode, L"name", pProp->GetObjectName());

		if (pProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pNextPropGroup = dynamic_cast<IPropertyGroup*>(pProp);
			if (pNextPropGroup == NULL)
				continue;

			if (!SaveXml_GroupProp(XmlStrObj, &PropNode, pNextPropGroup))
				return false;
		}
	}

	return true;
}

bool CSkinFileManager::SaveWindowsXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath)
{
	if (pSkinFileItem == NULL || pszSavePath == NULL)
		return false;

	xml_document XmlStrObj;
	xml_node RootNode = XmlStrObj.append_child(L"windows");
	if (!RootNode)
		return false;

	for (IPropertyBaseMap::iterator pWndItem = pSkinFileItem->AllWindowPropMap.begin(); pWndItem != pSkinFileItem->AllWindowPropMap.end(); pWndItem++)
	{
		IPropertyGroup *pWndPropGroup = dynamic_cast<IPropertyGroup*>(pWndItem->second);
		if (pWndPropGroup == NULL)
			continue;

		if (!pWndPropGroup->GetActivePropetry())
			continue;

		xml_node WindowNode = RootNode.append_child(_T(PROP_TYPE_WINDOW_NAME));
		if (!WindowNode)
			return false;

		AddIntAttrToNode(&WindowNode, _T(SKIN_OBJECT_ID), (int)pWndPropGroup->GetObjectId());

		SaveXml_GroupProp(XmlStrObj, &WindowNode, pWndPropGroup);
	}

	return XmlStrObj.save_file(pszSavePath, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

bool CSkinFileManager::DeleteImageBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pImgBaseProp)
{
	if (pImgBaseProp == false || pSkinFileItem == NULL)
		return false;

	// ImageBase 属性队列
	IPropertyBaseMap* pImageBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), false);
	IPropertyBaseMap* pImagePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_NAME), false);

	if (pImagePropMap != NULL)
	{
		for (IPropertyBaseMap::iterator pImage = pImagePropMap->begin(); pImage != pImagePropMap->end(); pImage++)
		{
			IPropertyBase* pPropBase = pImage->second;
			IPropertyImage* pImageProp = dynamic_cast<IPropertyImage*>(pPropBase);
			if (pImageProp == NULL)
				continue;

			IPropertyImageBase *pComImgBase = dynamic_cast<IPropertyImageBase*>(pImageProp->GetRelevancyProp());
			if (lstrcmpiW(pImageProp->GetRelevancyPropName(), pImgBaseProp->GetObjectName()) == 0 || pComImgBase == pImgBaseProp)
			{
				pImageProp->SetRelevancyProp(NULL);
				pImageProp->SetRelevancyPropName(NULL);
			}
		}
	}

	if (pImageBasePropMap != NULL)
	{
		DWORD dwObjId(pImgBaseProp->GetObjectId());
		IPropertyBaseMap::iterator pFindImageBase = pImageBasePropMap->find(dwObjId);
		if (pFindImageBase != pImageBasePropMap->end())
			pImageBasePropMap->erase(pFindImageBase);
	}

	ReleaseBaseProp(dynamic_cast<IPropertyBase*>(pImgBaseProp));
	return true;
}

bool CSkinFileManager::ModifyImageBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pImgBaseProp, LPCWSTR pszNewPropName)
{
	if (pSkinFileItem == NULL || pImgBaseProp == NULL || pszNewPropName == NULL)
		return false;

	IPropertyBaseMap* pImageBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), false);
	IPropertyBaseMap* pImagePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_NAME), false);

	CStringW strOldName = "";
	if (pImageBasePropMap != NULL)
	{
		DWORD dwObjId = pImgBaseProp->GetObjectId();
		IPropertyBaseMap::iterator pFindImageBase = pImageBasePropMap->find(dwObjId);
		if (pFindImageBase != pImageBasePropMap->end())
		{
			IPropertyBase* pFind = pFindImageBase->second;
			if (pFind == NULL)
				return false;

			IPropertyImageBase *pFindImgBaseProp = dynamic_cast<IPropertyImageBase*>(pFind);
			if (pFindImgBaseProp == NULL)
				return false;

			strOldName = pFindImgBaseProp->GetObjectName();
			pFindImgBaseProp->SetObjectName(pszNewPropName);
		}
	}

	if (pImagePropMap != NULL)
	{
		for (IPropertyBaseMap::iterator pImage = pImagePropMap->begin(); pImage != pImagePropMap->end(); pImage++)
		{
			IPropertyBase* pPropBase = pImage->second;
			IPropertyImage* pImageProp = dynamic_cast<IPropertyImage*>(pPropBase);
			if (pImageProp == NULL)
				continue;

			IPropertyImageBase *pComImgBase = dynamic_cast<IPropertyImageBase*>(pImageProp->GetRelevancyProp());
			if (pComImgBase == NULL)
				continue;

			if (strOldName.CompareNoCase(pImageProp->GetRelevancyPropName()) == 0 || pComImgBase == pImgBaseProp)
			{
				CStringW strDataW = pszNewPropName;
				pImageProp->SetRelevancyPropName(strDataW.LockBuffer());
				strDataW.UnlockBuffer();
				pImageProp->SetRelevancyProp(pImgBaseProp);
			}
		}
	}

	return true;
}

// 删除属性
bool CSkinFileManager::DeletePropetryBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase)
{
	if (pSkinFileItem == NULL || pPropBase == NULL)
		return false;

	if (pPropBase->GetObjectTypeId() == OTID_IMAGE_BASE)
	{
		IPropertyImageBase *pImgBaseProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
		if (pImgBaseProp == NULL)
			return false;

		return DeleteImageBaseProp(pSkinFileItem, pImgBaseProp);
	}

	if (pPropBase->GetObjectTypeId() == OTID_FONT_BASE)
	{
		IPropertyFontBase *pFontBaseProp = dynamic_cast<IPropertyFontBase*>(pPropBase);
		if (pFontBaseProp == NULL)
			return false;

		// 不允许删除默认字体
		if (pFontBaseProp == pSkinFileItem->pDefaultFontBase)
			return false;

		return DeleteFontBaseProp(pSkinFileItem, pFontBaseProp);
	}

	return false;
}

bool CSkinFileManager::DeleteFontBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyFontBase *pFontBaseProp)
{
	if (pSkinFileItem == NULL || pFontBaseProp == false)
		return false;

	// FontBase 属性队列
	IPropertyBaseMap* pFontBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_BASE_NAME), false);
	IPropertyBaseMap* pFontPropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_NAME), false);

	if (pFontPropMap != NULL)
	{
		for (IPropertyBaseMap::iterator pFont = pFontPropMap->begin(); pFont != pFontPropMap->end(); pFont++)
		{
			IPropertyBase* pPropBase = pFont->second;
			IPropertyFont* pFontProp = dynamic_cast<IPropertyFont*>(pPropBase);
			if (pFontProp == NULL)
				continue;

			IPropertyFontBase *pComFontBase = dynamic_cast<IPropertyFontBase*>(pFontProp->GetRelevancyProp());
			if (lstrcmpiW(pFontProp->GetRelevancyPropName(), pFontBaseProp->GetObjectName()) == 0 || pComFontBase == pFontBaseProp)
			{
				pFontProp->SetRelevancyProp(NULL);
				pFontProp->SetRelevancyPropName(NULL);
			}
		}
	}

	if (pFontBasePropMap != NULL)
	{
		DWORD dwObjId = pFontBaseProp->GetObjectId();
		IPropertyBaseMap::iterator pFindFontBase = pFontBasePropMap->find(dwObjId);
		if (pFindFontBase != pFontBasePropMap->end())
			pFontBasePropMap->erase(pFindFontBase);
	}

	ReleaseBaseProp(dynamic_cast<IPropertyBase*>(pFontBaseProp));
	return true;
}

bool CSkinFileManager::ModifyFontBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyFontBase *pFontBaseProp, LPCWSTR pszNewPropName)
{
	if (pSkinFileItem == NULL || pFontBaseProp == NULL || pszNewPropName == NULL)
		return false;

	// FontBase 属性队列
	IPropertyBaseMap* pFontBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_NAME), false);
	IPropertyBaseMap* pFontPropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_BASE_NAME), false);

	CStringW strOldName = "";
	if (pFontBasePropMap != NULL)
	{
		DWORD dwObjId = pFontBaseProp->GetObjectId();
		IPropertyBaseMap::iterator pFindFontBase = pFontBasePropMap->find(dwObjId);
		if (pFindFontBase != pFontBasePropMap->end())
		{
			IPropertyBase* pFind = pFindFontBase->second;
			if (pFind == NULL)
				return false;

			IPropertyFontBase *pFindFontBaseProp = dynamic_cast<IPropertyFontBase*>(pFind);
			if (pFindFontBaseProp == NULL)
				return false;

			strOldName = pFindFontBaseProp->GetObjectName();
			pFindFontBaseProp->SetObjectName(pszNewPropName);
		}
	}

	if (pFontPropMap != NULL)
	{
		for (IPropertyBaseMap::iterator pFont = pFontPropMap->begin(); pFont != pFontPropMap->end(); pFont++)
		{
			IPropertyBase* pPropBase = pFont->second;
			IPropertyFont* pFontProp = dynamic_cast<IPropertyFont*>(pPropBase);
			if (pFontProp == NULL)
				continue;

			IPropertyFontBase *pComFontBase = dynamic_cast<IPropertyFontBase*>(pFontProp->GetRelevancyProp());
			if (pComFontBase == NULL)
				continue;

			if (strOldName.CompareNoCase(pFontProp->GetRelevancyPropName()) == 0 || pComFontBase == pFontBaseProp)
			{
				CStringW strDataW = pszNewPropName;
				pFontProp->SetRelevancyPropName(strDataW.LockBuffer());
				strDataW.UnlockBuffer();
				pFontProp->SetRelevancyProp(pFontBaseProp);
			}
		}
	}

	return true;
}

// 取得整个皮肤包默认的字体信息
IPropertyFontBase* CSkinFileManager::GetDefaultFontBase(SKIN_FILE_ITEM* pSkinFileItem)
{
	if (pSkinFileItem == NULL || IUiFeatureKernelImpl::GetInstance() == NULL)
		return NULL;

	if (pSkinFileItem->pDefaultFontBase == NULL)
	{
		IPropertyBaseMap* pFontBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_BASE_NAME), false);
		if (pFontBasePropMap != NULL)
		{
			for (IPropertyBaseMap::iterator pFontBaseItem = pFontBasePropMap->begin(); pFontBaseItem != pFontBasePropMap->end(); pFontBaseItem++)
			{
				IPropertyFontBase* pFontBaseProp = dynamic_cast<IPropertyFontBase*>(pFontBaseItem->second);
				if (pFontBaseProp == NULL)
					continue;

				if (lstrcmpW(pFontBaseProp->GetObjectName(), _T(SYS_DEFAULT_FONT_NAME)) == 0)
				{
					pSkinFileItem->pDefaultFontBase = pFontBaseProp;
					break;
				}
			}
		}
	}

	if (pSkinFileItem->pDefaultFontBase == NULL && IUiFeatureKernelImpl::GetInstance()->IsDesignMode())
	{
		pSkinFileItem->pDefaultFontBase = dynamic_cast<IPropertyFontBase*>(CreateEmptyBaseProp(pSkinFileItem, OTID_FONT_BASE));
		if (pSkinFileItem->pDefaultFontBase != NULL)
			pSkinFileItem->pDefaultFontBase->SetObjectName(_T(SYS_DEFAULT_FONT_NAME));
	}

	return pSkinFileItem->pDefaultFontBase;
}

// 删除一个控件的所有属性
bool CSkinFileManager::DeleteControlPropetry(IControlBase* pCtrl)
{
	if (pCtrl == NULL || pCtrl->PP_GetControlPropetry() == NULL || pCtrl->GetOwnerWindow() == NULL)
		return false;

	IWindowBase* pWnd = pCtrl->GetOwnerWindow();
	if (pWnd->GetSkinFileItem() == NULL)
		return false;

	IPropertyControl *pCtrlProp = pCtrl->PP_GetControlPropetry();
	IPropertyGroup *pGroup = pCtrlProp->GetControlPropGroup();
	if (pGroup == NULL)
		return false;

	return DeleteControlPropetryByGroup(pGroup, pWnd->GetSkinFileItem());
}

bool CSkinFileManager::DeleteControlPropetryByGroup(IPropertyGroup *pGroup, SKIN_FILE_ITEM* pSkinItem)
{
	if (pGroup == NULL || pSkinItem == NULL)
		return false;

	// 从属性队列中删除
	GROUP_PROP_VEC *pChildGroup = pGroup->GetPropVec();
	if (pChildGroup == NULL)
		return false;

	IPropertyBase* pPropBase = NULL;
	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildGroup->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildGroup->NextObj(pItem);
		pPropBase = (IPropertyBase*)pItem;
		if (pPropBase == NULL)
			continue;

		// 先删除子属性
		if (pPropBase->GetObjectTypeId() == OTID_GROUP)
			DeleteControlPropetryByGroup(dynamic_cast<IPropertyGroup*>(pPropBase), pSkinItem);

		IPropertyBaseMapMap::iterator pPropTypeItem = pSkinItem->AllPropMap.find(pPropBase->GetObjectType());
		if (pPropTypeItem == pSkinItem->AllPropMap.end())
		{
			pChildGroup->DelObj(pItem);
			if (!this->DeletePropetryBaseProp(pSkinItem, pPropBase))
				this->ReleaseBaseProp(pPropBase);
			continue;
		}

		IPropertyBaseMap* pPropMap = pPropTypeItem->second;
		if (pPropMap == NULL)
		{
			pChildGroup->DelObj(pItem);
			if (!this->DeletePropetryBaseProp(pSkinItem, pPropBase))
				this->ReleaseBaseProp(pPropBase);
			continue;
		}

		IPropertyBaseMap::iterator pPropItem = pPropMap->find(pPropBase->GetObjectId());
		if (pPropItem == pPropMap->end())
		{
			pChildGroup->DelObj(pItem);
			if (!this->DeletePropetryBaseProp(pSkinItem, pPropBase))
				this->ReleaseBaseProp(pPropBase);
			continue;
		}

		pChildGroup->DelObj(pItem);
		if (!this->DeletePropetryBaseProp(pSkinItem, pPropBase))
			this->ReleaseBaseProp(pPropBase);
		pPropMap->erase(pPropItem);
	}

	return true;
}

// 从控件属性队列中删除一个控件，仅仅是删除控件的节点，控件的具体属性并没有删除
bool CSkinFileManager::DeleteControlPropetryFromCtrlPropMap(IControlBase* pCtrl)
{
	if (pCtrl == NULL || pCtrl->GetOwnerWindow() == NULL || pCtrl->PP_GetControlPropetry() == NULL)
		return false;

	IWindowBase* pWnd = pCtrl->GetOwnerWindow();
	if (pWnd->GetSkinFileItem() == NULL)
		return false;

	IPropertyWindow* pWndProp = pWnd->PP_GetWindowPropetry();
	if(pWndProp == NULL)
		return false;

	return DeleteControlPropetryFromCtrlPropMapByVec(pWndProp->GetChildPropControlVec(), pCtrl->PP_GetControlPropetry());
}

bool CSkinFileManager::DeleteControlPropetryFromCtrlPropMapByVec(PROP_CONTROL_VEC* pVec, IPropertyControl* pCtrlProp)
{
	if (pVec == NULL || pCtrlProp == NULL)
		return false;

	for (PROP_CONTROL_VEC::iterator pCtrlItem = pVec->begin(); pCtrlItem != pVec->end(); pCtrlItem++)
	{
		IPropertyControl* pComPropCtrl = *pCtrlItem;
		if (pComPropCtrl == NULL)
			continue;

		if (pComPropCtrl == pCtrlProp)
		{
			pVec->erase(pCtrlItem);
			this->ReleaseBaseProp(dynamic_cast<IPropertyBase*>(pCtrlProp));
			return true;
		}

		if (DeleteControlPropetryFromCtrlPropMapByVec(pComPropCtrl->GetChildPropControlVec(), pCtrlProp))
			return true;
	}

	return false;
}
