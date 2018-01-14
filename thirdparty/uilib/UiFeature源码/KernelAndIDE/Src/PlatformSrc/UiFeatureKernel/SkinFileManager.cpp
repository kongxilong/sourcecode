
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

// 一个窗口或者一个控件的所有属性节点的根节点：属性组的object type name
#define PROP_ROOT_TYPE_NAME									"WndOrCtrlPropHead"

CSkinFileManager::CSkinFileManager()
{
	m_AreaType = AT_CN;
	m_SkinFileMap.clear();

	IZipFileImpl *pZipImpl = new IZipFileImpl;
	if (pZipImpl != NULL)
	{
		m_LocalImageSkinFileItem.pZipFileMgr = dynamic_cast<IZipFile*>(pZipImpl);
		if (m_LocalImageSkinFileItem.pZipFileMgr == NULL)
			SAFE_DELETE(pZipImpl);
	}
}

CSkinFileManager::~CSkinFileManager()
{
	ReleaseAllSkinFile();
	ReleaseSkinFileItem(&m_LocalImageSkinFileItem);
}

//////////////////////////////////////////////////////////////////////////
// 删除皮肤包操作
void CSkinFileManager::ReleaseAllSkinFile()
{
	while (true)
	{
		 SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin();
		 if (pSkinFileItem == m_SkinFileMap.end())
			 break;

		 SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		 if (pSkinFile == NULL)
		 {
			 m_SkinFileMap.erase(pSkinFileItem);
			 continue;
		 }

		 CStringW strPath = pSkinFileItem->first;
		 ReleaseSkinFile(strPath);
	}
	m_SkinFileMap.clear();
}

void CSkinFileManager::ReleaseSkinFile(LPCWSTR pszSkinPath)
{
	if (pszSkinPath == NULL || wcslen(pszSkinPath) <= 0)
		return;

	for (SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin(); pSkinFileItem != m_SkinFileMap.end(); pSkinFileItem++)
	{
		SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		if (pSkinFile == NULL)
			continue;

		CStringW strPath = pSkinFileItem->first;
		if (strPath.CompareNoCase(pszSkinPath) == 0)
		{
			ReleaseSkinFileItem(pSkinFile);

			// 从皮肤文件的队列中删除
			m_SkinFileMap.erase(pSkinFileItem);

			// 删除内存
			SAFE_DELETE(pSkinFile);
			break;
		}
	}
}

void CSkinFileManager::ReleaseSkinFileItem(SKIN_FILE_ITEM* pSkinFile)
{
	if (pSkinFile == NULL)
		return;

	ReleaseLayoutMap(pSkinFile->LayoutWindowMap);
	ReleasePropMap(pSkinFile->AllCtrlPropMap, false);
	ReleasePropMap(pSkinFile->AllPropMap, true);

	IZipFileImpl* pZipImpl = dynamic_cast<IZipFileImpl*>(pSkinFile->pZipFileMgr);
	SAFE_DELETE(pZipImpl);
}

// 创建一个空的皮肤文件
SKIN_FILE_ITEM* CSkinFileManager::CreateEmptySkinFileItem(LPCWSTR pszSkinPath)
{
	if (pszSkinPath == NULL || wcslen(pszSkinPath) <= 0)
		return NULL;

	for (SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin(); pSkinFileItem != m_SkinFileMap.end(); pSkinFileItem++)
	{
		SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		if (pSkinFile == NULL)
			continue;

		CStringW strPath = pSkinFileItem->first;
		if (strPath.CompareNoCase(pszSkinPath) == 0)
			return NULL;
	}

	SKIN_FILE_ITEM* pSkinFile = new SKIN_FILE_ITEM;
	if (pSkinFile == NULL)
		return NULL;

	pSkinFile->strSkinFilePath = pszSkinPath;

	IZipFileImpl *pZipImpl = new IZipFileImpl;
	if (pZipImpl == NULL)
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}

	pSkinFile->pZipFileMgr = dynamic_cast<IZipFile*>(pZipImpl);
	if (pSkinFile->pZipFileMgr == NULL)
	{
		SAFE_DELETE(pZipImpl);
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}

	m_SkinFileMap.insert(pair<CStringW, SKIN_FILE_ITEM*>(pszSkinPath, pSkinFile));
	return pSkinFile;
}

SKIN_FILE_ITEM* CSkinFileManager::GetSkinFileItem(LPCWSTR pszSkinPath, bool bNew)
{
	if (pszSkinPath == NULL || wcslen(pszSkinPath) <= 0)
		return NULL;

	for (SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin(); pSkinFileItem != m_SkinFileMap.end(); pSkinFileItem++)
	{
		SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		if (pSkinFile == NULL)
			continue;

		CStringW strPath = pSkinFileItem->first;
		if (strPath.CompareNoCase(pszSkinPath) == 0)
			return pSkinFile;
	}

	if (bNew)
		return ReadSkinFile(pszSkinPath);

	return NULL;
}

void CSkinFileManager::ReleaseLayoutMap(IPropertyBaseMap &LayoutWindowMap)
{
	for (IPropertyBaseMap::iterator pWndItem = LayoutWindowMap.begin(); pWndItem != LayoutWindowMap.end(); pWndItem++)
	{
		IPropertyWindow* pWnd = dynamic_cast<IPropertyWindow*>(pWndItem->second);
		if (pWnd != NULL)
		{
			ReleaseLayoutMapPropControlVec(pWnd->GetChildPropControlVec());
			SAFE_DELETE(pWnd);
		}
	}
	LayoutWindowMap.clear();
}

void CSkinFileManager::ReleaseLayoutMapPropControlVec(PROP_CONTROL_VEC* pCtrlVec)
{
	if (pCtrlVec == NULL)
		return;

	for (PROP_CONTROL_VEC::iterator pVecItem = pCtrlVec->begin(); pVecItem != pCtrlVec->end(); pVecItem++)
	{
		IPropertyControl* pCtrl = *pVecItem;
		if (pCtrl != NULL)
			ReleaseLayoutMapPropControlVec(pCtrl->GetChildPropControlVec());
		SAFE_DELETE(pCtrl);
	}
	pCtrlVec->clear();
}

void CSkinFileManager::ReleasePropMap(IPropertyBaseMapMap &PropMap, bool bReleaseChild)
{
	for (IPropertyBaseMapMap::iterator pGroupItem = PropMap.begin(); pGroupItem != PropMap.end(); pGroupItem++)
	{
		IPropertyBaseMap* pGroup = pGroupItem->second;
		if (pGroup != NULL && bReleaseChild)
			ReleasePropMapItem(pGroup);
		SAFE_DELETE(pGroup);
	}
	PropMap.clear();
}

void CSkinFileManager::ReleasePropMapItem(IPropertyBaseMap* pPropMapItem)
{
	if (pPropMapItem == NULL)
		return;

	for (IPropertyBaseMap::iterator pPropItem = pPropMapItem->begin(); pPropItem != pPropMapItem->end(); pPropItem++)
	{
		IPropertyBase* pProp = pPropItem->second;
		ReleaseIPropertyBase(pProp);
	}
	pPropMapItem->clear();
}

void CSkinFileManager::ReleaseIPropertyBase(IPropertyBase *pCtrlProp)
{
	if (pCtrlProp == NULL)
		return;

	switch (pCtrlProp->GetObjectTypeId())
	{
	case OTID_BOOL:
		{
			IPropertyBoolImpl* pProp = dynamic_cast<IPropertyBoolImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_COLOR:
		{
			IPropertyColorImpl* pProp = dynamic_cast<IPropertyColorImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_COMBOBOX:
		{
			IPropertyComboBoxImpl* pProp = dynamic_cast<IPropertyComboBoxImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CURSOR:
		{
			IPropertyCursorImpl* pProp = dynamic_cast<IPropertyCursorImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CURSOR_BASE:
		{
			IPropertyCursorBaseImpl* pProp = dynamic_cast<IPropertyCursorBaseImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_FONT:
		{
			IPropertyFontImpl* pProp = dynamic_cast<IPropertyFontImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_FONT_BASE:
		{
			IPropertyFontBaseImpl* pProp = dynamic_cast<IPropertyFontBaseImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_GROUP:
		{
			IPropertyGroupImpl* pProp = dynamic_cast<IPropertyGroupImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_IMAGE:
		{
			IPropertyImageImpl* pProp = dynamic_cast<IPropertyImageImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_IMAGE_BASE:
		{
			IPropertyImageBaseImpl* pProp = dynamic_cast<IPropertyImageBaseImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_INT:
		{
			IPropertyIntImpl* pProp = dynamic_cast<IPropertyIntImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_STRING:
		{
			IPropertyStringImpl* pProp = dynamic_cast<IPropertyStringImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_WINDOW:
		{
			IPropertyWindowImpl* pProp = dynamic_cast<IPropertyWindowImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CONTROL:
		{
			IPropertyControlImpl* pProp = dynamic_cast<IPropertyControlImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	default:
		break;
	}

	assert(false);
	SAFE_DELETE(pCtrlProp);
}

//////////////////////////////////////////////////////////////////////////
// 读取皮肤包操作

// 读取一个皮肤包信息
SKIN_FILE_ITEM* CSkinFileManager::ReadSkinFile(LPCWSTR pszSkinPath)
{
	if (pszSkinPath == NULL || wcslen(pszSkinPath) <= 0)
		return NULL;

	for (SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin(); pSkinFileItem != m_SkinFileMap.end(); pSkinFileItem++)
	{
		SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		if (pSkinFile == NULL)
			continue;

		CStringW strPath = pSkinFileItem->first;
		if (strPath.CompareNoCase(pszSkinPath) == 0)
			return pSkinFile;
	}

	SKIN_FILE_ITEM* pSkinFile = new SKIN_FILE_ITEM;
	if (pSkinFile == NULL)
		return NULL;

	IZipFileImpl *pZipImpl = new IZipFileImpl;
	if (pZipImpl == NULL)
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}

	pSkinFile->pZipFileMgr = dynamic_cast<IZipFile*>(pZipImpl);
	if (pSkinFile->pZipFileMgr == NULL)
	{
		SAFE_DELETE(pZipImpl);
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}

	m_SkinFileMap.insert(pair<CStringW, SKIN_FILE_ITEM*>(pszSkinPath, pSkinFile));

	// 从皮肤文件中初始化皮肤队列
	if (!pSkinFile->pZipFileMgr->ReadZipFile(pszSkinPath))
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}

	ZIP_FILE * pResurceXml = pSkinFile->pZipFileMgr->FindUnZipFile(_T(RESOURCE_XML_NAME));
	ZIP_FILE * pControlsXml = pSkinFile->pZipFileMgr->FindUnZipFile(_T(CONTROLS_XML_NAME));
	ZIP_FILE * pWindowsXml = pSkinFile->pZipFileMgr->FindUnZipFile(_T(WINDOWS_XML_NAME));
	ZIP_FILE * pLayoutXml = pSkinFile->pZipFileMgr->FindUnZipFile(_T(LAYOUT_XML_NAME));
	if (pWindowsXml == NULL || pResurceXml == NULL)
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}
	pSkinFile->strSkinFilePath = pszSkinPath;

	// 解读 Resource.xml
	if (!TranslateResourceXml(pSkinFile, pResurceXml))
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}
	pSkinFile->pZipFileMgr->RemoveFile(pResurceXml);

	// 解读 Controls.xml
	if (!TranslateControlsXml(pSkinFile, pControlsXml))
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}
	pSkinFile->pZipFileMgr->RemoveFile(pControlsXml);

	// 解读 Windows.xml
	if (!TranslateWindowsXml(pSkinFile, pWindowsXml))
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}
	pSkinFile->pZipFileMgr->RemoveFile(pWindowsXml);

	// 解析 Layout.xml
	if (!TranslateLayoutXml(pSkinFile, pLayoutXml))
	{
		ReleaseSkinFile(pszSkinPath);
		return NULL;
	}
	pSkinFile->pZipFileMgr->RemoveFile(pLayoutXml);

	return pSkinFile;
}

// 解析Windows.xml
bool CSkinFileManager::TranslateLayoutXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pLayoutXml)
{
	if (pSkinFileItem == NULL || pLayoutXml == NULL || pLayoutXml->pFileData == NULL)
		return false;

	xml_document xmlDoc;
	if(xmlDoc.load_buffer(pLayoutXml->pFileData, pLayoutXml->dwSrcFileLen, parse_default, encoding_utf8).status != status_ok)
		return false;

	xml_node LayoutRoot = xmlDoc.first_child();
	if (LayoutRoot.empty())
		return false;

	const WCHAR* psz_area = LayoutRoot.attribute(L"area").as_string();
	if (psz_area == NULL)
	{
		SetArea(AT_CN);
	}
	else
	{
		AREA_TYPE areaType = (AREA_TYPE)_wtoi(psz_area);
		SetArea(areaType);
	}

	IUiFeatureKernelImpl::GetInstance()->KernelSetIdBegin(LayoutRoot.attribute(L"lastobjid").as_int());

	for(xml_node WindowNode = LayoutRoot.first_child(); !WindowNode.empty(); WindowNode = WindowNode.next_sibling())
	{
		const WCHAR* psz_id = WindowNode.attribute(_T(SKIN_OBJECT_ID)).as_string();
		if (psz_id == NULL)
			return false;

		DWORD dwObjId = _wtoi(psz_id);
		IPropertyBaseMap::iterator pWndPropGroupItem = pSkinFileItem->AllWindowPropMap.find(dwObjId);
		if (pWndPropGroupItem == pSkinFileItem->AllWindowPropMap.end())
			return false;

		IPropertyGroup *pWndPropGroup = dynamic_cast<IPropertyGroup*>(pWndPropGroupItem->second);
		if (pWndPropGroup == NULL)
			return false;

		IPropertyBaseMap::iterator pWndItem = pSkinFileItem->LayoutWindowMap.find(dwObjId);
		if (pWndItem != pSkinFileItem->LayoutWindowMap.end())
			return false;

		IPropertyBase* pPropBase = CreateEmptyBaseProp(pSkinFileItem, OTID_WINDOW, &dwObjId);
		IPropertyWindow* pOneWndLayoutProp = dynamic_cast<IPropertyWindow*>(pPropBase);
		if (pOneWndLayoutProp == NULL)
			return false;

		pOneWndLayoutProp->SetWindowPropGroup(pWndPropGroup);
		if (!GeneralCreateWindowLayoutProp(pSkinFileItem, &WindowNode, pOneWndLayoutProp->GetChildPropControlVec(), NULL))
			return false;
	}

	return true;
}

bool CSkinFileManager::GeneralCreateWindowLayoutProp(SKIN_FILE_ITEM *pSkinFileItem, xml_node* pXmlNode, PROP_CONTROL_VEC* pChildCtrlVec, IPropertyControl* pParentProp)
{
	if (pSkinFileItem == NULL || pXmlNode == NULL || pChildCtrlVec == NULL)
		return false;

	for(xml_node CtrlNode = pXmlNode->first_child(); !CtrlNode.empty(); CtrlNode = CtrlNode.next_sibling())
	{
		const WCHAR * psz_id = XmlGetAttrValue(&CtrlNode, _T(SKIN_OBJECT_ID));
		if (psz_id == NULL)
			return false;

		CStringW strCtrlType = L"";
		DWORD dwObjId = _wtoi(psz_id);
		IPropertyGroup* pCtrlPropGroup = FindControlPropGroup(pSkinFileItem, dwObjId, strCtrlType);
		if (pCtrlPropGroup == NULL)
			return false;

		IPropertyControl* pCtrlProp = dynamic_cast<IPropertyControl*>(CreateEmptyBaseProp(pSkinFileItem, OTID_CONTROL, &dwObjId));
		if (pCtrlProp == NULL)
			return false;

		pCtrlProp->SetControlType(strCtrlType.LockBuffer());
		strCtrlType.UnlockBuffer();	
		pCtrlProp->SetCtrlGroupProp(pCtrlPropGroup);
		pCtrlProp->SetParentCtrlProp(pParentProp);
		pChildCtrlVec->push_back(pCtrlProp);

		if (!GeneralCreateWindowLayoutProp(pSkinFileItem, &CtrlNode, pCtrlProp->GetChildPropControlVec(), pCtrlProp))
			return false;
	}

	return true;
}

IPropertyGroup* CSkinFileManager::FindControlPropGroup(SKIN_FILE_ITEM *pSkinFileItem, DWORD dwObjId, CStringW &strCtrlType)
{
	strCtrlType = L"";
	if (pSkinFileItem == NULL || dwObjId == ERROR_OBJECT_ID)
		return NULL;

	for (IPropertyBaseMapMap::iterator pGroupItem = pSkinFileItem->AllCtrlPropMap.begin(); pGroupItem != pSkinFileItem->AllCtrlPropMap.end(); pGroupItem++)
	{
		IPropertyBaseMap* pGroup = pGroupItem->second;
		if (pGroup != NULL)
		{
			IPropertyBaseMap::iterator pPropItem = pGroup->find(dwObjId);
			if (pPropItem == pGroup->end())
				continue;

			IPropertyBase* pPropBase = pPropItem->second;
			if (pPropBase == NULL)
				continue;

			strCtrlType = pGroupItem->first;
			IPropertyGroup* pFindGroup = dynamic_cast<IPropertyGroup*>(pPropBase);
			return pFindGroup;
		}
	}

	return NULL;
}

// 解析Windows.xml
bool CSkinFileManager::TranslateWindowsXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pWindowsXml)
{
	if (pSkinFileItem == NULL || pWindowsXml == NULL || pWindowsXml->pFileData == NULL)
		return false;

	xml_document xmlDoc;
	if(xmlDoc.load_buffer(pWindowsXml->pFileData, pWindowsXml->dwSrcFileLen, parse_default, encoding_utf8).status != status_ok)
		return false;

	xml_node WindowsRoot = xmlDoc.first_child();
	if (WindowsRoot.empty())
		return false;

	for(xml_node WindowNode = WindowsRoot.first_child(); !WindowNode.empty(); WindowNode = WindowNode.next_sibling())
	{
		if (!GeneralCreateSubProp(pSkinFileItem, &WindowNode, &(pSkinFileItem->AllWindowPropMap)))
			return false;
	}

	return true;
}

// Controls.xml 相关
bool CSkinFileManager::TranslateControlsXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pControlsXml)
{
	if (pSkinFileItem == NULL || pControlsXml == NULL || pControlsXml->pFileData == NULL)
		return false;

	xml_document xmlDoc;
	if(xmlDoc.load_buffer(pControlsXml->pFileData, pControlsXml->dwSrcFileLen, parse_default, encoding_utf8).status != status_ok)
		return false;

	xml_node ControlsRoot = xmlDoc.first_child();
	if (ControlsRoot.empty())
		return false;

	for(xml_node CtrlTypeXml = ControlsRoot.first_child(); !CtrlTypeXml.empty(); CtrlTypeXml = CtrlTypeXml.next_sibling())
	{
		IPropertyBaseMap* pOneCtrlMap = NULL;
		CStringW strTypeName = CtrlTypeXml.name();
		IPropertyBaseMapMap::iterator pTypeItem = pSkinFileItem->AllCtrlPropMap.find(strTypeName);
		if (pTypeItem != pSkinFileItem->AllCtrlPropMap.end())
		{
			// 找到属性组
			pOneCtrlMap = pTypeItem->second;
		}
		else
		{
			// 创建属性组
			pOneCtrlMap = new IPropertyBaseMap;
			if (pOneCtrlMap == NULL)
				return false;

			pOneCtrlMap->clear();
			pSkinFileItem->AllCtrlPropMap.insert(pair<CStringW, IPropertyBaseMap*>(strTypeName, pOneCtrlMap));
		}

		if (pOneCtrlMap == NULL)
			return false;

		if (!GeneralCreateSubProp(pSkinFileItem, &CtrlTypeXml, pOneCtrlMap))
			return false;
	}

	return true;
}

bool CSkinFileManager::GeneralCreateSubProp(SKIN_FILE_ITEM *pSkinFileItem, xml_node* pXmlNode, IPropertyBaseMap* pCtrlPropMap)
{
	if (pSkinFileItem == NULL || pXmlNode == NULL || pCtrlPropMap == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	if (psz_id == NULL)
		return false;

	DWORD dwObjId = _wtoi(psz_id);
	IPropertyBaseMap::iterator pFindGroupItem = pCtrlPropMap->find(dwObjId);
	if (pFindGroupItem != pCtrlPropMap->end())
		return false;

	// 创建一个窗口或者一个控件的所有属性节点的根节点：属性组
	IPropertyGroup* pCtrlProp = dynamic_cast<IPropertyGroup*>(CreateEmptyBaseProp(pSkinFileItem, OTID_GROUP));
	if (pCtrlProp == NULL)
		return false;

	pCtrlProp->SetObjectId(dwObjId);
	pCtrlProp->SetObjectName(_T(PROP_ROOT_TYPE_NAME));

	IPropertyBase* pBaseProp = dynamic_cast<IPropertyBase*>(pCtrlProp);
	if (pBaseProp == NULL)
	{
		SAFE_DELETE(pCtrlProp);
		return false;
	}
	pCtrlPropMap->insert(pair<DWORD, IPropertyBase*>(dwObjId, pBaseProp));

	for(xml_node PropNode = pXmlNode->first_child(); !PropNode.empty(); PropNode = PropNode.next_sibling())
	{
		psz_id = XmlGetAttrValue(&PropNode, _T(SKIN_OBJECT_ID));
		if (psz_id == NULL)
			return false;

		IPropertyBase* pFindBaseProp = FindBasePropertyByPropId(pSkinFileItem, PropNode.name(), _wtoi(psz_id));
		if (pFindBaseProp == NULL)
			return false;

		pCtrlProp->AppendProperty(pFindBaseProp);
		if (pFindBaseProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pNewGroup = dynamic_cast<IPropertyGroup*>(pFindBaseProp);
			if (pNewGroup == NULL)
				return false;

			if (!AppendBasePropToGroup(pSkinFileItem, pNewGroup, &PropNode))
				return false;
		}
	}

	return true;
}

// 查找指定的属性
IPropertyBase* CSkinFileManager::FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD dwObjId)
{
	CStringW strType = PropTypeToString(propType);
	return FindBasePropertyByPropId(pSkinFileItem, strType, dwObjId);
}

IPropertyBase* CSkinFileManager::FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, DWORD dwObjId)
{
	if (pSkinFileItem == NULL || pszPropType == NULL || wcslen(pszPropType) <= 0 || dwObjId == ERROR_OBJECT_ID || pSkinFileItem->AllPropMap.size() <= 0)
		return NULL;

	IPropertyBaseMapMap::iterator pPropTypeItem = pSkinFileItem->AllPropMap.find(pszPropType);
	if (pPropTypeItem == pSkinFileItem->AllPropMap.end())
		return NULL;

	IPropertyBaseMap* pPropMap = pPropTypeItem->second;
	if (pPropMap == NULL)
		return NULL;

	IPropertyBaseMap::iterator pPropItem = pPropMap->find(dwObjId);
	if (pPropItem == pPropMap->end())
		return NULL;

	return (pPropItem->second);
}

// 查找指定的属性
IPropertyBase* CSkinFileManager::FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, LPCWSTR pszPropName)
{
	CStringW strType = PropTypeToString(propType);
	return FindBasePropertyByPropName(pSkinFileItem, strType, pszPropName);
}

IPropertyBase* CSkinFileManager::FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, LPCWSTR pszPropName)
{
	if (pSkinFileItem == NULL || pszPropType == NULL || wcslen(pszPropType) <= 0 || pszPropName == NULL || wcslen(pszPropName) <= 0 || pSkinFileItem->AllPropMap.size() <= 0)
		return NULL;

	CStringW strType = pszPropType;
	IPropertyBaseMapMap::iterator pPropTypeItem = pSkinFileItem->AllPropMap.find(strType);
	if (pPropTypeItem == pSkinFileItem->AllPropMap.end())
		return NULL;

	IPropertyBaseMap* pPropMap = pPropTypeItem->second;
	if (pPropMap == NULL)
		return NULL;

	for (IPropertyBaseMap::iterator pPropItem = pPropMap->begin(); pPropItem != pPropMap->end(); pPropItem++)
	{
		IPropertyBase* pFindProp = pPropItem->second;
		if (pFindProp == NULL)
			continue;

		if (lstrcmpW(pFindProp->GetObjectName(), pszPropName) == 0)
			return pFindProp;
	}

	return NULL;
}

bool CSkinFileManager::AppendBasePropToGroup(SKIN_FILE_ITEM *pSkinFileItem, IPropertyGroup *pGroup, xml_node* pXmlNode)
{
	if (pSkinFileItem == NULL || pGroup == NULL || pXmlNode == NULL)
		return false;

	for(xml_node PropNode = pXmlNode->first_child(); !PropNode.empty(); PropNode = PropNode.next_sibling())
	{
		const WCHAR * psz_id = XmlGetAttrValue(&PropNode, _T(SKIN_OBJECT_ID));
		if (psz_id == NULL)
			return false;

		IPropertyBase* pFindBaseProp = FindBasePropertyByPropId(pSkinFileItem, PropNode.name(), _wtoi(psz_id));
		if (pFindBaseProp == NULL)
			continue;

		pGroup->AppendProperty(pFindBaseProp);
		if (pFindBaseProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pNewGroup = dynamic_cast<IPropertyGroup*>(pFindBaseProp);
			if (pNewGroup == NULL)
				return false;

			if (!AppendBasePropToGroup(pSkinFileItem, pNewGroup, &PropNode))
				return false;
		}
	}

	return true;
}

// 解析Resource.xml
bool CSkinFileManager::TranslateResourceXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pResurceXml)
{
	if (pSkinFileItem == NULL || pResurceXml == NULL || pResurceXml->pFileData == NULL)
		return false;

	IPropertyBaseMapMap &AllPropMap = (pSkinFileItem->AllPropMap);

	xml_document xmlDoc;
	if(xmlDoc.load_buffer(pResurceXml->pFileData, pResurceXml->dwSrcFileLen, parse_default, encoding_utf8).status != status_ok)
		return false;

	xml_node ResurceRoot = xmlDoc.first_child();
	if (ResurceRoot.empty())
		return false;

	for(xml_node PropTypeXml = ResurceRoot.first_child(); !PropTypeXml.empty(); PropTypeXml = PropTypeXml.next_sibling())
	{
		IPropertyBaseMap* pOnePropMap = NULL;
		CStringW strTypeName = PropTypeXml.name();
		IPropertyBaseMapMap::iterator pTypeItem = AllPropMap.find(strTypeName);
		if (pTypeItem != AllPropMap.end())
		{
			// 找到属性组
			pOnePropMap = pTypeItem->second;
		}
		else
		{
			// 创建属性组
			pOnePropMap = new IPropertyBaseMap;
			if (pOnePropMap == NULL)
				return false;

			pOnePropMap->clear();
			AllPropMap.insert(pair<CStringW, IPropertyBaseMap*>(strTypeName, pOnePropMap));
		}

		if (pOnePropMap == NULL)
			return false;

		if (!GeneralCreateBaseProp(pSkinFileItem, strTypeName, &PropTypeXml, pOnePropMap))
			return false;
	}

	for (SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin(); pSkinFileItem != m_SkinFileMap.end(); pSkinFileItem++)
	{
		SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		if (pSkinFile == NULL)
			continue;

		// 设置属性的关联性，如：image和imagebase
		SetPropetryBaseRelevancy(pSkinFile);
	}

	return true;
}

bool CSkinFileManager::GeneralCreateBaseProp(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pPropType, xml_node* pXmlNode, IPropertyBaseMap* pPropMap)
{
	if (pSkinFileItem == NULL || pPropType == NULL || pXmlNode == NULL || pPropMap == NULL)
		return false;

	for(xml_node XmlNode = pXmlNode->first_child(); !XmlNode.empty(); XmlNode = XmlNode.next_sibling())
	{
		const WCHAR * psz_id = XmlGetAttrValue(&XmlNode, _T(SKIN_OBJECT_ID));
		if (psz_id == NULL)
			return false;

		DWORD dwObjId = _wtoi(psz_id);
		IPropertyBaseMap::iterator pPropItem = pPropMap->find(dwObjId);
		if (pPropItem != pPropMap->end())
		{
			// 有重复就直接略过
			continue;
		}

		CStringW strProp = pPropType;
		OBJECT_TYPE_ID ObjTypeId = PropStringToType(strProp.LockBuffer());
		strProp.UnlockBuffer();
		IPropertyBase* pBaseProp = CreateEmptyBaseProp(pSkinFileItem, ObjTypeId, &dwObjId);
		if (pBaseProp == NULL)
			return false;

		if (!pBaseProp->ReadPropertyFromXmlNode(&XmlNode))
		{
			ReleaseBaseProp(pBaseProp);
			return false;
		}
	}

	return true;
}

// 取得指定组的属性
IPropertyBaseMap* CSkinFileManager::GetIPropertyBaseMap(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropGroupName, bool bIsCreate)
{
	if (pSkinFileItem == NULL || pszPropGroupName == NULL)
		return NULL;

	IPropertyBaseMap* pPropGroupItem = NULL;
	IPropertyBaseMapMap::iterator pPropGroup = pSkinFileItem->AllPropMap.find(pszPropGroupName);
	if (pPropGroup != pSkinFileItem->AllPropMap.end())
		pPropGroupItem = pPropGroup->second;

	if (pPropGroupItem == NULL && bIsCreate)
	{
		pPropGroupItem = new IPropertyBaseMap;
		if (pPropGroupItem != NULL)
		{
			pSkinFileItem->AllPropMap.insert(pair<CStringW, IPropertyBaseMap*>(pszPropGroupName, pPropGroupItem));
		}
	}

	return pPropGroupItem;
}

// 设置属性与base属性的关联
void CSkinFileManager::SetPropetryBaseRelevancy(SKIN_FILE_ITEM *pSkinFileItem)
{
	if (pSkinFileItem == NULL)
		return;

	SetPropRelevancy(GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), false), GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_NAME), false));
	SetImageFileRelevancy(pSkinFileItem);
	SetPropRelevancy(GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_BASE_NAME), false), GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_FONT_NAME), false));
	SetPropRelevancy(GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_CURSOR_BASE_NAME), false), GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_CURSOR_NAME), false));
	SetPropRelevancy(GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_COLOR_BASE_NAME), false), GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_COLOR_NAME), false));

	SetImageBaseSkinPath(pSkinFileItem);
}

// 设置 IPropertyImageBase 属性中的 IMAGE_BASE_PROP 的 pszSkinFilePath
void CSkinFileManager::SetImageBaseSkinPath(SKIN_FILE_ITEM *pSkinFileItem)
{
	if (pSkinFileItem == NULL)
		return;

	// 设置 IPropertyImageBase 属性中的 IMAGE_BASE_PROP 的 pszSkinFilePath
	IPropertyBaseMap* pImgBaseMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), false);
	if (pImgBaseMap != NULL)
	{
		for (IPropertyBaseMap::iterator pImgBaseItem = pImgBaseMap->begin(); pImgBaseItem != pImgBaseMap->end(); pImgBaseItem++)
		{
			IPropertyBase* pPropBase = pImgBaseItem->second;
			if (pPropBase == NULL)
				continue;

			IPropertyImageBase* pImgBaseProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
			if (pImgBaseProp == NULL)
				continue;

			if (pImgBaseProp->GetImageProp() == NULL)
				continue;

			pImgBaseProp->GetImageProp()->pszSkinFilePath = pSkinFileItem->strSkinFilePath;
		}
	}
}

// 设置imagebase属性和图片的关联
bool CSkinFileManager::SetImageFileRelevancy(SKIN_FILE_ITEM *pSkinFileItem)
{
	if (pSkinFileItem == NULL || pSkinFileItem->pZipFileMgr == NULL)
		return false;

	IPropertyBaseMap* pImageBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), false);
	if (pImageBasePropMap == NULL)
		return false;

	for (IPropertyBaseMap::iterator pImage = pImageBasePropMap->begin(); pImage != pImageBasePropMap->end(); pImage++)
	{
		IPropertyBase* pPropBase = pImage->second;
		IPropertyImageBase* pImageBaseProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
		if (pImageBaseProp == NULL || pImageBaseProp->GetImageProp() == NULL)
			continue;

		CStringW strFileName = pImageBaseProp->GetImageProp()->strFileName;
		if (strFileName.GetLength() <= 0)
			continue;

		ZIP_FILE *pZipFile = pSkinFileItem->pZipFileMgr->FindUnZipFile(strFileName);
		if (pZipFile == NULL)
			return false;

		pImageBaseProp->SetZipFile(pZipFile, false);
	}

	return true;
}

void CSkinFileManager::SetPropRelevancy(IPropertyBaseMap* pFromMap, IPropertyBaseMap* pToMap)
{
	if (pFromMap == NULL || pToMap == NULL)
		return;

	for (IPropertyBaseMap::iterator pToItem = pToMap->begin(); pToItem != pToMap->end(); pToItem++)
	{
		IPropertyBase* pToProp = pToItem->second;
		if (pToProp == NULL)
			continue;

		if (pToProp->GetRelevancyPropName() == NULL || wcslen(pToProp->GetRelevancyPropName()) <= 0)
			continue;

		for (IPropertyBaseMap::iterator pFromItem = pFromMap->begin(); pFromItem != pFromMap->end(); pFromItem++)
		{
			IPropertyBase* pFromProp = pFromItem->second;
			if (pFromProp == NULL)
				continue;

			if (pFromProp->GetObjectName() == NULL || wcslen(pFromProp->GetObjectName()) <= 0)
				continue;

			if (lstrcmpiW(pToProp->GetRelevancyPropName(), pFromProp->GetObjectName()) == 0)
			{
				pToProp->SetRelevancyProp(pFromProp);
				break;
			}
		}
	}
}

// 创建一个属性，并将此属性放入队列
IPropertyBase* CSkinFileManager::CreateEmptyBaseProp(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD *pdwObjectId)
{
	if (pSkinFileItem == NULL)
		return NULL;

	IUiFeatureKernel* pUiKernel = IUiFeatureKernelImpl::GetInstance();

	IPropertyBase* pBaseProp = NULL;
	switch (propType)
	{
	case OTID_BOOL:
		{
			IPropertyBoolImpl* pProp = new IPropertyBoolImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_COLOR:
		{
			IPropertyColorImpl* pProp = new IPropertyColorImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_COMBOBOX:
		{
			IPropertyComboBoxImpl* pProp = new IPropertyComboBoxImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_CURSOR:
		{
			IPropertyCursorImpl* pProp = new IPropertyCursorImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_CURSOR_BASE:
		{
			IPropertyCursorBaseImpl* pProp = new IPropertyCursorBaseImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_FONT:
		{
			IPropertyFontImpl* pProp = new IPropertyFontImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_FONT_BASE:
		{
			IPropertyFontBaseImpl* pProp = new IPropertyFontBaseImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_GROUP:
		{
			IPropertyGroupImpl* pProp = new IPropertyGroupImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_IMAGE:
		{
			IPropertyImageImpl* pProp = new IPropertyImageImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_IMAGE_BASE:
		{
			IPropertyImageBaseImpl* pProp = new IPropertyImageBaseImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_INT:
		{
			IPropertyIntImpl* pProp = new IPropertyIntImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_STRING:
		{
			IPropertyStringImpl* pProp = new IPropertyStringImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_WINDOW:
		{
			IPropertyWindowImpl* pProp = new IPropertyWindowImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	case OTID_CONTROL:
		{
			IPropertyControlImpl* pProp = new IPropertyControlImpl(pUiKernel);
			if (pProp != NULL)
			{
				pBaseProp = dynamic_cast<IPropertyBase*>(pProp);
				if (pBaseProp == NULL)
					SAFE_DELETE(pProp);
			}
		}
		break;

	default:
		break;
	}

	if (pBaseProp != NULL)
	{
		if (pdwObjectId != NULL)
			pBaseProp->SetObjectId(*pdwObjectId);

		if (propType == OTID_WINDOW)
		{
			// 创建窗口属性
			IPropertyWindow* pWndProp = dynamic_cast<IPropertyWindow*>(pBaseProp);
			if (pWndProp == NULL)
			{
				ReleaseBaseProp(pBaseProp);
				return NULL;
			}

			pSkinFileItem->LayoutWindowMap.insert(pair<DWORD, IPropertyWindow*>(pBaseProp->GetObjectId(), pWndProp));
		}
		else if (propType == OTID_CONTROL)
		{
			// 创建控件属性
		}
		else
		{
			// 创建普通属性
			IPropertyBaseMap* pOnePropMap = NULL;
			CStringW strTypeName = PropTypeToString(propType);
			IPropertyBaseMapMap::iterator pTypeItem = pSkinFileItem->AllPropMap.find(strTypeName);
			if (pTypeItem != pSkinFileItem->AllPropMap.end())
			{
				// 找到属性组
				pOnePropMap = pTypeItem->second;
			}
			else
			{
				// 创建属性组
				pOnePropMap = new IPropertyBaseMap;
				if (pOnePropMap == NULL)
				{
					ReleaseBaseProp(pBaseProp);
					return NULL;
				}

				pOnePropMap->clear();
				pSkinFileItem->AllPropMap.insert(pair<CStringW, IPropertyBaseMap*>(strTypeName, pOnePropMap));
			}

			if (pOnePropMap != NULL)
			{
				pOnePropMap->insert(pair<DWORD, IPropertyBase*>(pBaseProp->GetObjectId(), pBaseProp));
			}
		}
	}

	return pBaseProp;
}

void CSkinFileManager::ReleaseBaseProp(IPropertyBase *pCtrlProp)
{
	if (pCtrlProp == NULL)
		return;

	switch (pCtrlProp->GetObjectTypeId())
	{
	case OTID_BOOL:
		{
			IPropertyBoolImpl* pProp = dynamic_cast<IPropertyBoolImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_COLOR:
		{
			IPropertyColorImpl* pProp = dynamic_cast<IPropertyColorImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_COMBOBOX:
		{
			IPropertyComboBoxImpl* pProp = dynamic_cast<IPropertyComboBoxImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CURSOR:
		{
			IPropertyCursorImpl* pProp = dynamic_cast<IPropertyCursorImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CURSOR_BASE:
		{
			IPropertyCursorBaseImpl* pProp = dynamic_cast<IPropertyCursorBaseImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_FONT:
		{
			IPropertyFontImpl* pProp = dynamic_cast<IPropertyFontImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_FONT_BASE:
		{
			IPropertyFontBaseImpl* pProp = dynamic_cast<IPropertyFontBaseImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_GROUP:
		{
			IPropertyGroupImpl* pProp = dynamic_cast<IPropertyGroupImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_IMAGE:
		{
			IPropertyImageImpl* pProp = dynamic_cast<IPropertyImageImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_IMAGE_BASE:
		{
			IPropertyImageBaseImpl* pProp = dynamic_cast<IPropertyImageBaseImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_INT:
		{
			IPropertyIntImpl* pProp = dynamic_cast<IPropertyIntImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_STRING:
		{
			IPropertyStringImpl* pProp = dynamic_cast<IPropertyStringImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_WINDOW:
		{
			IPropertyWindowImpl* pProp = dynamic_cast<IPropertyWindowImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	case OTID_CONTROL:
		{
			IPropertyControlImpl* pProp = dynamic_cast<IPropertyControlImpl*>(pCtrlProp);
			if (pProp != NULL)
			{
				SAFE_DELETE(pProp);
				return;
			}
		}
		break;

	default:
		break;
	}

	SAFE_DELETE(pCtrlProp);
}

// 设置显示的语言种类
void CSkinFileManager::SetArea(AREA_TYPE areaType)
{
	for (SKIN_FILE_MAP::iterator pSkinFileItem = m_SkinFileMap.begin(); pSkinFileItem != m_SkinFileMap.end(); pSkinFileItem++)
	{
		SKIN_FILE_ITEM* pSkinFile = pSkinFileItem->second;
		if (pSkinFile == NULL)
			continue;

		IPropertyBaseMapMap::iterator pStringItem = pSkinFile->AllPropMap.find(PROP_TYPE_STRING_NAME);
		if (pStringItem == pSkinFile->AllPropMap.end())
			return;

		IPropertyBaseMap* pPropMap = pStringItem->second;
		if (pPropMap == NULL)
			return;

		for (IPropertyBaseMap::iterator pPropItem = pPropMap->begin(); pPropItem != pPropMap->end(); pPropItem++)
		{
			IPropertyBase* pProp = pPropItem->second;
			if (pProp == NULL)
				continue;

			IPropertyString *pStringProp = dynamic_cast<IPropertyString*>(pProp);
			if (pStringProp == NULL)
				continue;

			pStringProp->SetArea(areaType);
		}
	}
}

// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
bool CSkinFileManager::ReadSkinFileItem(IZipFile *pZipFileMgr, ZIP_FILE* pZipItem)
{
	if (pZipFileMgr == NULL || pZipItem == NULL || pZipItem->pFileData != NULL)
		return false;

	return pZipFileMgr->ReadZipFileItem(pZipItem);
}

// 本地图片使用的zip文件
SKIN_FILE_ITEM * CSkinFileManager::GetLocalImageSkinFileItem()
{
	return &m_LocalImageSkinFileItem;
}