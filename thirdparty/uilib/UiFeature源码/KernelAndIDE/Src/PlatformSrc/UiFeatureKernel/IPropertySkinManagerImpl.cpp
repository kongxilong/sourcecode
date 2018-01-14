
#include "StdAfx.h"
#include <assert.h>
#include "IPropertySkinManagerImpl.h"
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
#include "..\..\Inc\IZipFile.h"
#include "CriSec.h"

// 一个窗口或者一个控件的所有属性节点的根节点：属性组的object type name
#define PROP_ROOT_TYPE_NAME									"WndOrCtrlPropHead"

IPropertySkinManagerImpl::IPropertySkinManagerImpl()
{
	m_LocalImgMap.clear();
}

IPropertySkinManagerImpl::~IPropertySkinManagerImpl()
{
	ReleaseSkinManagerPropetry();
}

// 清空属性队列
void IPropertySkinManagerImpl::ReleaseSkinManagerPropetry()
{
	m_SkinFile.ReleaseAllSkinFile();
}

void IPropertySkinManagerImpl::ReleaseBaseProp(IPropertyBase *pCtrlProp)
{
	m_SkinFile.ReleaseBaseProp(pCtrlProp);
}

// 创建一个属性，并将此属性放入队列
IPropertyBase* IPropertySkinManagerImpl::CreateEmptyBaseProp(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD *pdwObjectId)
{
	return m_SkinFile.CreateEmptyBaseProp(pSkinFileItem, propType, pdwObjectId);
}

// 查找指定的属性
IPropertyBase* IPropertySkinManagerImpl::FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD dwObjId)
{
	return m_SkinFile.FindBasePropertyByPropId(pSkinFileItem, propType, dwObjId);
}

IPropertyBase* IPropertySkinManagerImpl::FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, DWORD dwObjId)
{
	return m_SkinFile.FindBasePropertyByPropId(pSkinFileItem, pszPropType, dwObjId);
}

// 初始化皮肤
IPropertyWindow* IPropertySkinManagerImpl::PG_InitWindowSkin(LPCWSTR pszSkinPath, LPCWSTR pszWndName)
{
	if (pszSkinPath == NULL || wcslen(pszSkinPath) <= 0 || pszWndName == NULL || wcslen(pszWndName) <= 0)
		return NULL;

	SKIN_FILE_ITEM* pSkinFileItem = m_SkinFile.ReadSkinFile(pszSkinPath);
	if (pSkinFileItem == NULL)
		return NULL;

	IPropertyWindow *pFindPropWnd = NULL;
	for (IPropertyBaseMap::iterator pWndPropItem = pSkinFileItem->LayoutWindowMap.begin(); pWndPropItem != pSkinFileItem->LayoutWindowMap.end(); pWndPropItem++)
	{
		IPropertyWindow *pPropWnd = dynamic_cast<IPropertyWindow*>(pWndPropItem->second);
		if (pPropWnd == NULL)
			continue;

		CStringW strObjName = pPropWnd->GetObjectName();
		if (strObjName.CompareNoCase(pszWndName) == 0)
		{
			pFindPropWnd = pPropWnd;
			break;
		}
	}

	return pFindPropWnd;
}

// 设置显示的语言种类
void IPropertySkinManagerImpl::SetArea(AREA_TYPE areaType)
{
	return m_SkinFile.SetArea(areaType);
}

// 保存皮肤包
bool IPropertySkinManagerImpl::BD_SaveSkinFile(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSkinDir, LPCWSTR pszSkinName, char *pszPassword)
{
	return m_SkinFile.SaveSkinFileItem(pSkinFileItem, pszSkinDir, pszSkinName, pszPassword);
}

// 向控件队列添加一个控件
bool IPropertySkinManagerImpl::BD_AppendControlToVec(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszCtrlType, IPropertyGroup* pCtrlPropGroup)
{
	if (pSkinFileItem == NULL || pszCtrlType == NULL || pCtrlPropGroup == NULL)
		return false;

	DWORD dwObjId = pCtrlPropGroup->GetObjectId();
	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pCtrlPropGroup);
	if (pPropBase == NULL)
		return false;

	IPropertyBaseMap* pCtrlType = NULL;
	CStringW strCtrlType = pszCtrlType;
	IPropertyBaseMapMap::iterator pCtrlTypeItem = pSkinFileItem->AllCtrlPropMap.find(strCtrlType);
	if (pCtrlTypeItem == pSkinFileItem->AllCtrlPropMap.end())
	{
		pCtrlType = new IPropertyBaseMap;
		if (pCtrlType == NULL)
			return false;

		pSkinFileItem->AllCtrlPropMap.insert(pair<CStringW, IPropertyBaseMap*>(strCtrlType, pCtrlType));
	}
	else
	{
		pCtrlType = pCtrlTypeItem->second;
	}

	if (pCtrlType == NULL)
		return false;

	pCtrlType->insert(pair<DWORD, IPropertyBase*>(dwObjId, pPropBase));
	return true;
}

// 设置可以保存的有效属性
void IPropertySkinManagerImpl::BD_SetWindowPropetryActiveProp(IWindowBase *pWndBase, bool bActive)
{
	if (pWndBase == NULL)
		return;

	pWndBase->SetActivePropetry(bActive);
	IPropertyWindow* pPropWnd = pWndBase->PP_GetWindowPropetry();
	if (pPropWnd != NULL)
		pPropWnd->SetActivePropetry(bActive);

	if (pWndBase->PP_GetWindowPropetryGroup() != NULL)
		pWndBase->PP_GetWindowPropetryGroup()->SetActivePropetry(bActive);

	// 不再需要设置自身的子属性的Active标志，在创建并加载属性的时候已经设置了此标志。
//	// 设置窗口自身的属性
//	BD_SetGroupPropActiveMark(pWndBase->PP_GetWindowPropetryGroup(), bActive);

	// 设置子窗口
	BD_SetChildVecActiveMark(pWndBase->GetChildControlsVec(), bActive);
}

void IPropertySkinManagerImpl::BD_SetChildVecActiveMark(CHILD_CTRLS_VEC* pChildCtrlVec, bool bActive)
{
	if (pChildCtrlVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (pCtrl->PP_GetControlPropetry() != NULL)
			pCtrl->PP_GetControlPropetry()->SetActivePropetry(bActive);

		if (pCtrl->PP_GetControlPropetryGroup() != NULL)
			pCtrl->PP_GetControlPropetryGroup()->SetActivePropetry(bActive);

		pCtrl->SetActivePropetry(bActive);
		// 不再需要设置自身的子属性的Active标志，在创建并加载属性的时候已经设置了此标志。
//		BD_SetGroupPropActiveMark(pCtrl->PP_GetControlPropetryGroup(), bActive);
		BD_SetChildVecActiveMark(pCtrl->GetChildControlsVec(), bActive);
	}
}

void IPropertySkinManagerImpl::BD_SetGroupPropActiveMark(IPropertyGroup *pPropGroup, bool bActive)
{
	if (pPropGroup == NULL || pPropGroup->GetPropVec() == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pPropGroup->GetPropVec()->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pPropGroup->GetPropVec()->NextObj(pItem);
		IPropertyBase* pProp = (IPropertyBase*)pItem;
		if (pProp == NULL)
			continue;

		pProp->SetActivePropetry(bActive);
		if (pProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pNextPropGroup = dynamic_cast<IPropertyGroup*>(pProp);
			BD_SetGroupPropActiveMark(pNextPropGroup, bActive);
		}
	}
}

// 删除本地图片
bool IPropertySkinManagerImpl::DeleteLocalImageBase(IPropertyImageBase *pPropImgBase)
{
	return DeleteImageBase(GetLocalImageSkinFileItem(), pPropImgBase);
}

// 删除图片
bool IPropertySkinManagerImpl::DeleteImageBase(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pPropImgBase)
{
	if (pSkinFileItem == NULL || pPropImgBase == NULL)
		return false;

	// 先减去引用计数
	pPropImgBase->SubtractUseCounts();
	if (pPropImgBase->GetUseCounts() > 0)
		return true;

	IPropertyBase *pPropBase = dynamic_cast<IPropertyBase*>(pPropImgBase);
	if (pPropBase == NULL)
		return false;

	IMAGE_BASE_PROP* pImgProp = pPropImgBase->GetImageProp();
	if (pImgProp != NULL)
	{
		LOCAL_IMAGE_MAP::iterator pImgItem = m_LocalImgMap.find(pImgProp->strFileName);
		if (pImgItem != m_LocalImgMap.end())
			m_LocalImgMap.erase(pImgItem);
	}

	return m_SkinFile.DeletePropetryBaseProp(pSkinFileItem, pPropBase);
}

bool IPropertySkinManagerImpl::DeleteImageBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pImgBaseProp)
{
	return m_SkinFile.DeleteImageBaseProp(pSkinFileItem, pImgBaseProp);
}

// 删除一个控件的所有属性
bool IPropertySkinManagerImpl::DeleteControlPropetry(IControlBase* pCtrl)
{
	return m_SkinFile.DeleteControlPropetry(pCtrl);
}

// 删除属性
bool IPropertySkinManagerImpl::DeletePropetryBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase)
{
	return m_SkinFile.DeletePropetryBaseProp(pSkinFileItem, pPropBase);
}

// 取得指定组的属性
IPropertyBaseMap* IPropertySkinManagerImpl::GetIPropertyBaseMap(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pPropGroupName, bool bIsCreate)
{
	return m_SkinFile.GetIPropertyBaseMap(pSkinFileItem, pPropGroupName, bIsCreate);
}

// 修改属性名称
bool IPropertySkinManagerImpl::BD_ModifyPropetryName(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase, LPCWSTR pszNewPropName)
{
	if (pSkinFileItem == NULL || pPropBase == NULL || pszNewPropName == NULL)
		return false;

	if (pPropBase->GetObjectTypeId() == OTID_IMAGE_BASE)
	{
		IPropertyImageBase *pImgBaseProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
		if (pImgBaseProp == NULL)
			return false;

		return m_SkinFile.ModifyImageBaseProp(pSkinFileItem, pImgBaseProp, pszNewPropName);
	}

	if (pPropBase->GetObjectTypeId() == OTID_FONT_BASE)
	{
		IPropertyFontBase *pFontBaseProp = dynamic_cast<IPropertyFontBase*>(pPropBase);
		if (pFontBaseProp == NULL)
			return false;

		// 不允许修改默认字体名称
		if (pFontBaseProp == pSkinFileItem->pDefaultFontBase || lstrcmpW(pFontBaseProp->GetObjectName(), _T(SYS_DEFAULT_FONT_NAME)) == 0)
			return false;

		return m_SkinFile.ModifyFontBaseProp(pSkinFileItem, pFontBaseProp, pszNewPropName);
	}

	return false;
}

// 初始化鼠标手势属性
bool IPropertySkinManagerImpl::InitCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem)
{
	if (pSkinFileItem == NULL)
		return false;

	IPropertyBaseMap* pCursorBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_CURSOR_BASE_NAME));

	if (pCursorBasePropMap == NULL)
	{
		// 创建属性组
		pCursorBasePropMap = new IPropertyBaseMap;
		if (pCursorBasePropMap == NULL)
			return false;

		pCursorBasePropMap->clear();
		pSkinFileItem->AllPropMap.insert(pair<CStringW, IPropertyBaseMap*>(PROP_TYPE_CURSOR_BASE_NAME, pCursorBasePropMap));
	}

	if (pCursorBasePropMap == NULL)
		return false;

	// 正常光标 标准的箭头
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_ARROW) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_ARROW);

	// 十字架光标
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_CROSS) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_CROSS);

	// 双箭头指向西北和东南
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_SIZENWSE) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_SIZENWSE);

	// 双箭头指向东北和西南
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_SIZENESW) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_SIZENESW);

	// 双箭头指向东西
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_SIZEWE) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_SIZEWE);

	// 双箭头指向南北
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_SIZENS) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_SIZENS);

	// 四向箭头指向东、西、南、北
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_SIZEALL) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_SIZEALL);

	// 标准的箭头和小沙漏
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_APPSTARTING) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_APPSTARTING);

	// 标准的箭头和问号
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_HELP) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_HELP);

	// 工字光标
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_IBEAM) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_IBEAM);

	// 禁止圈
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_NO) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_NO);

	// 垂直箭头
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_UPARROW) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_UPARROW);

	// 沙漏
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_WAIT) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_WAIT);

	// 手势点击
	if (FindCursorBasePropetry(pSkinFileItem, UF_IDC_HAND) == NULL)
		AddCursorBasePropetry(pSkinFileItem, true, UF_IDC_HAND);

	return true;
}

IPropertyCursorBase* IPropertySkinManagerImpl::FindCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem, int nCursorId)
{
	IPropertyBaseMap* pCursorBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_CURSOR_BASE_NAME));

	if (pCursorBasePropMap == NULL || pSkinFileItem == NULL)
		return NULL;

	IPropertyCursorBase* pFind = NULL;
	for (IPropertyBaseMap::iterator pCurItem = pCursorBasePropMap->begin(); pCurItem != pCursorBasePropMap->end(); pCurItem++)
	{
		IPropertyBase* pPropBase = pCurItem->second;
		if (pPropBase == NULL)
			continue;

		IPropertyCursorBase* pFindCom = dynamic_cast<IPropertyCursorBase*>(pPropBase);
		if (pFindCom == NULL)
			continue;

		SetCursorObjectName(pFindCom);
		if (pFindCom->GetCursorId() == nCursorId)
		{
			pFind = pFindCom;
			break;
		}
	}

	return pFind;
}

// 设置光标属性的名称
void IPropertySkinManagerImpl::SetCursorObjectName(IPropertyCursorBase *pCursorBase)
{
	if (pCursorBase == NULL || (pCursorBase->GetObjectName() != NULL && wcslen(pCursorBase->GetObjectName()) > 0))
		return;

	switch (pCursorBase->GetCursorId())
	{
		// 正常光标 标准的箭头
	case UF_IDC_ARROW:
		pCursorBase->SetObjectName(L"ARROW");
		break;

		// 双箭头指向东北和西南
	case UF_IDC_SIZENESW:
		pCursorBase->SetObjectName(L"SIZENESW");
		break;

		// 工字光标
	case UF_IDC_IBEAM:
		pCursorBase->SetObjectName(L"IBEAM");
		break;

		// 十字架光标
	case UF_IDC_CROSS:
		pCursorBase->SetObjectName(L"CROSS");
		break;

		// 四向箭头指向东、西、南、北
	case UF_IDC_SIZEALL:
		pCursorBase->SetObjectName(L"SIZEALL");
		break;

		// 垂直箭头
	case UF_IDC_UPARROW:
		pCursorBase->SetObjectName(L"UPARROW");
		break;

		// 双箭头指向东西
	case UF_IDC_SIZEWE:
		pCursorBase->SetObjectName(L"SIZEWE");
		break;

		// 双箭头指向南北
	case UF_IDC_SIZENS:
		pCursorBase->SetObjectName(L"SIZENS");
		break;

		// 双箭头指向西北和东南
	case UF_IDC_SIZENWSE:
		pCursorBase->SetObjectName(L"SIZENWSE");
		break;

		// 手势
	case UF_IDC_HAND:
		pCursorBase->SetObjectName(L"HAND");
		break;

		// 标准的箭头和小沙漏
	case UF_IDC_APPSTARTING:
		pCursorBase->SetObjectName(L"APPSTARTING");
		break;

		// 沙漏
	case UF_IDC_WAIT:
		pCursorBase->SetObjectName(L"WAIT");
		break;

		// 标准的箭头和问号
	case UF_IDC_HELP:
		pCursorBase->SetObjectName(L"HELP");
		break;

		// 禁止圈
	case UF_IDC_NO:
		pCursorBase->SetObjectName(L"NO");
		break;

	default:
		break;
	}
}

// 增加一个鼠标手势
bool IPropertySkinManagerImpl::AddCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem, bool bSysCursor, int nCursorId)
{
	IPropertyBaseMap* pCursorBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_CURSOR_BASE_NAME));

	if (pCursorBasePropMap == NULL || nCursorId <= 0 || pSkinFileItem == NULL)
		return false;

	IPropertyCursorBase *pCursorBase = dynamic_cast<IPropertyCursorBase*>(CreateEmptyBaseProp(pSkinFileItem, OTID_CURSOR_BASE));
	if (pCursorBase == NULL)
		return false;

	pCursorBase->SetCursorId(nCursorId);
	pCursorBase->SetSystemCursor(bSysCursor);
	SetCursorObjectName(pCursorBase);

	pCursorBasePropMap->insert(pair<DWORD, IPropertyBase*>(pCursorBase->GetObjectId(), pCursorBase));
	return true;
}

// 加载本地图片
IPropertyImageBase* IPropertySkinManagerImpl::LoadLocalImage(LPCWSTR pszLocalImgPath, bool bReLoad)
{
	if (!FileExistsW(pszLocalImgPath))
		return NULL;

	LOCAL_IMAGE_MAP::iterator pImgItem = m_LocalImgMap.find(pszLocalImgPath);

	if (pImgItem != m_LocalImgMap.end())
	{
		IPropertyImageBase* pImgBase = pImgItem->second;
		if (bReLoad)
		{
			if (pImgBase != NULL)
				this->DeleteImageBase(m_SkinFile.GetLocalImageSkinFileItem(), pImgBase);
			else
				m_LocalImgMap.erase(pImgItem);
		}
		else
		{
			// 计数器加1
			if (pImgBase != NULL)
				pImgBase->AddUseCounts();

			return pImgBase;
		}
	}

	IPropertyImageBase* pNewImgBase = (IPropertyImageBase*)this->CreateEmptyBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), OTID_IMAGE_BASE);
	if (pNewImgBase == NULL)
		return NULL;

	WCHAR szObjId[MAX_PATH] = {0};
	swprintf_s(szObjId, MAX_PATH, L"%d", pNewImgBase->GetObjectId());
	pNewImgBase->SetObjectName(szObjId);

	CDrawingImage* pDrawingImg = pNewImgBase->CreateDrawingImage();
	if (pDrawingImg == NULL)
	{
		this->DeleteImageBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	pDrawingImg->CreateByFile(pszLocalImgPath);
	if (pDrawingImg->GetSafeHdc() == NULL)
	{
		this->DeleteImageBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	IMAGE_BASE_PROP LocalImgProp;
	InitPropImageBase(&LocalImgProp);
	LocalImgProp.bIsZipFile = false;
	wcscpy_s(LocalImgProp.strFileName, MAX_PATH, pszLocalImgPath);
	LocalImgProp.ImgShowType = IST_ALL_LASHEN;
	LocalImgProp.RectInImage.left = LocalImgProp.RectInImage.top = 0;
	LocalImgProp.RectInImage.right = pDrawingImg->GetDcSize().cx;
	LocalImgProp.RectInImage.bottom = pDrawingImg->GetDcSize().cy;
	pNewImgBase->SetImageProp(&LocalImgProp);

	m_LocalImgMap.insert(pair<CStringW, IPropertyImageBase*>(pszLocalImgPath, pNewImgBase));
	return pNewImgBase;
}

// 输入的内存格式为 DIB 32 位的数据。
IPropertyImageBase* IPropertySkinManagerImpl::LoadImage(int nWidth, int nHeight, BYTE* pScan0)
{
	if (nWidth <= 0 || nHeight <= 0 || pScan0 == NULL)
		return NULL;

	IPropertyImageBase* pNewImgBase = (IPropertyImageBase*)this->CreateEmptyBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), OTID_IMAGE_BASE);
	if (pNewImgBase == NULL)
		return NULL;

	WCHAR szObjId[MAX_PATH] = {0};
	swprintf_s(szObjId, MAX_PATH, L"%d", pNewImgBase->GetObjectId());
	pNewImgBase->SetObjectName(szObjId);

	CDrawingImage* pDrawingImg = pNewImgBase->CreateDrawingImage();
	if (pDrawingImg == NULL)
	{
		this->DeleteImageBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	pDrawingImg->CreateByScanMem(nWidth, nHeight, pScan0);
	if (pDrawingImg->GetSafeHdc() == NULL)
	{
		this->DeleteImageBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	IMAGE_BASE_PROP LocalImgProp;
	InitPropImageBase(&LocalImgProp);
	LocalImgProp.bIsZipFile = false;
	LocalImgProp.ImgShowType = IST_ALL_LASHEN;
	LocalImgProp.RectInImage.left = LocalImgProp.RectInImage.top = 0;
	LocalImgProp.RectInImage.right = pDrawingImg->GetDcSize().cx;
	LocalImgProp.RectInImage.bottom = pDrawingImg->GetDcSize().cy;
	pNewImgBase->SetImageProp(&LocalImgProp);

	m_LocalImgMap.insert(pair<CStringW, IPropertyImageBase*>(szObjId, pNewImgBase));
	return pNewImgBase;
}

// 删除一张图片
bool IPropertySkinManagerImpl::BD_DeleteZipImage(SKIN_FILE_ITEM* pSkinFileItem, ZIP_FILE* pZipImage)
{
	if (pSkinFileItem == NULL || pZipImage == NULL)
		return false;

	IPropertyBaseMap* pImageBasePropMap = GetIPropertyBaseMap(pSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME));
	if (pImageBasePropMap == NULL)
		return true;

	for (IPropertyBaseMap::iterator pImageBaseItem = pImageBasePropMap->begin(); pImageBaseItem != pImageBasePropMap->end(); pImageBaseItem++)
	{
		IPropertyBase* pPropBase = pImageBaseItem->second;
		IPropertyImageBase* pImageBaseProp = dynamic_cast<IPropertyImageBase*>(pPropBase);
		if (pImageBaseProp == NULL)
			continue;

		if (pImageBaseProp->GetZipFile() == pZipImage)
			pImageBaseProp->SetZipFile(NULL, false);
	}
	return true;
}

// 取得整个程序默认的字体信息
IPropertyFontBase* IPropertySkinManagerImpl::GetDefaultFontBase(SKIN_FILE_ITEM* pSkinFileItem)
{
	return m_SkinFile.GetDefaultFontBase(pSkinFileItem);
}

ZIP_FILE* IPropertySkinManagerImpl::MEM_CreateZipFile(SKIN_FILE_ITEM* pSkinFileItem, int nFileBufLen, BYTE byFileType, LPCWSTR pszFileName)
{
	if (nFileBufLen <= 0 || pszFileName == NULL || wcslen(pszFileName) <= 0 || pSkinFileItem == NULL || pSkinFileItem->pZipFileMgr == NULL)
		return NULL;

	ZIP_FILE* pNewZipFile = new ZIP_FILE;
	if (pNewZipFile == NULL)
		return NULL;

	pNewZipFile->byFileType = byFileType;

	memset(pNewZipFile->strFileName, 0, sizeof(pNewZipFile->strFileName));
	wcscpy_s(pNewZipFile->strFileName, MAX_PATH, pszFileName);

	pNewZipFile->dwSrcFileLen = nFileBufLen;
	pNewZipFile->pFileData = new BYTE[nFileBufLen];
	if (pNewZipFile->pFileData == NULL)
	{
		SAFE_DELETE(pNewZipFile);
		return NULL;
	}

	pSkinFileItem->pZipFileMgr->GetUnZipFileMap()->insert(pair<CStringW, ZIP_FILE*>(pNewZipFile->strFileName, pNewZipFile));
	return pNewZipFile;
}

void IPropertySkinManagerImpl::MEM_DeleteZipFileByCom(SKIN_FILE_ITEM* pSkinFileItem, ZIP_FILE *pComZipFile)
{
	if (pComZipFile == NULL || pSkinFileItem == NULL || pSkinFileItem->pZipFileMgr == NULL)
		return;

	for (ZIP_FILE_MAP::iterator pZipItem = pSkinFileItem->pZipFileMgr->GetUnZipFileMap()->begin();
		pZipItem != pSkinFileItem->pZipFileMgr->GetUnZipFileMap()->end(); pZipItem++)
	{
		ZIP_FILE* pZip = pZipItem->second;
		if (pZip == pComZipFile)
		{
			SAFE_DELETE(pZip->pFileData);
			SAFE_DELETE(pZip);
			pSkinFileItem->pZipFileMgr->GetUnZipFileMap()->erase(pZipItem);
			break;
		}
	}
}

// 查找指定的属性
IPropertyBase* IPropertySkinManagerImpl::FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, LPCWSTR pszPropName)
{
	return m_SkinFile.FindBasePropertyByPropName(pSkinFileItem, propType, pszPropName);
}

IPropertyBase* IPropertySkinManagerImpl::FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, LPCWSTR pszPropName)
{
	return m_SkinFile.FindBasePropertyByPropName(pSkinFileItem, pszPropType, pszPropName);
}

// 取得皮肤文件
SKIN_FILE_ITEM* IPropertySkinManagerImpl::GetSkinFileItem(LPCWSTR pszFilePath, bool bNew)
{
	return m_SkinFile.GetSkinFileItem(pszFilePath, bNew);
}

// 创建一个空的皮肤文件
SKIN_FILE_ITEM* IPropertySkinManagerImpl::CreateEmptySkinFileItem(LPCWSTR pszFilePath)
{
	return m_SkinFile.CreateEmptySkinFileItem(pszFilePath);
}

// 从指定皮肤包文件中找到指定文件
bool IPropertySkinManagerImpl::FindUnZipFile(LPCWSTR pszSkinFilePath, LPCWSTR pFileName, BYTE **ppOutBuffer, int *pnFileLen, ZIP_FILE **ppFindZipFile)
{
	if (pszSkinFilePath == NULL || wcslen(pszSkinFilePath) <= 0 || pFileName == NULL || wcslen(pFileName) <= 0 || ppOutBuffer == NULL || pnFileLen == NULL)
		return false;

	if (ppFindZipFile != NULL)
		*ppFindZipFile = NULL;

	SKIN_FILE_ITEM* pSkinFileItem = m_SkinFile.GetSkinFileItem(pszSkinFilePath);
	if (pSkinFileItem == NULL || pSkinFileItem->pZipFileMgr == NULL)
		return false;

	*ppOutBuffer = NULL;
	*pnFileLen = 0;

	ZIP_FILE *pZipFile = pSkinFileItem->pZipFileMgr->FindUnZipFile(pFileName);
	if (pZipFile == NULL)
		return false;

	// 内存优化，只有在使用的时候从磁盘读取
	if (pZipFile->pFileData == NULL)
		m_SkinFile.ReadSkinFileItem(pSkinFileItem->pZipFileMgr, pZipFile);

	if (ppFindZipFile != NULL)
		*ppFindZipFile = pZipFile;

	*ppOutBuffer = pZipFile->pFileData;
	*pnFileLen = pZipFile->dwSrcFileLen;

	return true;
}

// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
bool IPropertySkinManagerImpl::ReadSkinFileItem(ZIP_FILE* pZipItem)
{
	if (pZipItem == NULL)
		return false;

	if (pZipItem->pFileData != NULL)
		return true;

	SKIN_FILE_ITEM* pSkinFileItem = m_SkinFile.GetSkinFileItem(pZipItem->pstrSkinPath);
	if (pSkinFileItem == NULL || pSkinFileItem->pZipFileMgr == NULL)
		return false;

	return m_SkinFile.ReadSkinFileItem(pSkinFileItem->pZipFileMgr, pZipItem);
}

// 本地图片使用的zip文件
SKIN_FILE_ITEM * IPropertySkinManagerImpl::GetLocalImageSkinFileItem()
{
	return m_SkinFile.GetLocalImageSkinFileItem();
}

IPropertyImageBase* IPropertySkinManagerImpl::LoadImageFromHICON(HICON hIcon)
{
	return LoadImageFromHANDLE(NULL, hIcon);
}

IPropertyImageBase* IPropertySkinManagerImpl::LoadImageFromHBITMAP(HBITMAP hBmp)
{
	return LoadImageFromHANDLE(hBmp, NULL);
}

IPropertyImageBase* IPropertySkinManagerImpl::LoadImageFromHANDLE(HBITMAP hBmp, HICON hIcon)
{
	if (hBmp == NULL && hIcon == NULL)
		return NULL;

	IPropertyImageBase* pNewImgBase = (IPropertyImageBase*)this->CreateEmptyBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), OTID_IMAGE_BASE);
	if (pNewImgBase == NULL)
		return NULL;

	WCHAR szObjId[MAX_PATH] = {0};
	swprintf_s(szObjId, MAX_PATH, L"%d", pNewImgBase->GetObjectId());
	pNewImgBase->SetObjectName(szObjId);

	CDrawingImage* pDrawingImg = pNewImgBase->CreateDrawingImage();
	if (pDrawingImg == NULL)
	{
		this->DeleteImageBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	if (hBmp != NULL)
		pDrawingImg->CreateByHBITMAP(hBmp);
	else
		pDrawingImg->CreateByHICON(hIcon);

	if (pDrawingImg->GetSafeHdc() == NULL)
	{
		this->DeleteImageBaseProp(m_SkinFile.GetLocalImageSkinFileItem(), pNewImgBase);
		return NULL;
	}

	IMAGE_BASE_PROP LocalImgProp;
	InitPropImageBase(&LocalImgProp);
	LocalImgProp.bIsZipFile = false;
	LocalImgProp.ImgShowType = IST_ALL_LASHEN;
	LocalImgProp.RectInImage.left = LocalImgProp.RectInImage.top = 0;
	LocalImgProp.RectInImage.right = pDrawingImg->GetDcSize().cx;
	LocalImgProp.RectInImage.bottom = pDrawingImg->GetDcSize().cy;
	pNewImgBase->SetImageProp(&LocalImgProp);

	m_LocalImgMap.insert(pair<CStringW, IPropertyImageBase*>(szObjId, pNewImgBase));

	return pNewImgBase;
}

// 取得皮肤包版本号
SKIN_VERSION IPropertySkinManagerImpl::GetSkinFileVersion(LPCWSTR pZipFilePath, int &nPasswordLen)
{
	IZipFileImpl zipTemp;
	return zipTemp.GetSkinFileVersion(pZipFilePath, nPasswordLen);
}

// 验证用户皮肤包密码
bool IPropertySkinManagerImpl::ChekUserPassword(const char *pUserPassword, LPCWSTR pZipFilePath)
{
	IZipFileImpl zipTemp;
	return zipTemp.ChekUserPassword_SV_1(pUserPassword, pZipFilePath);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 保存控件皮肤到默认文件夹
bool IPropertySkinManagerImpl::SaveControlDefaultSkin(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup)
{
	if (!SaveControlDefaultSkin_ControlXml(pszSaveDir, pszCtrlName, pPublicGroup, pPrivateGroup))
		return false;

	if (!SaveControlDefaultSkin_ResourceXml(pszSaveDir, pszCtrlName, pPublicGroup, pPrivateGroup))
		return false;

	return true;
}

bool IPropertySkinManagerImpl::SaveControlDefaultSkin_ControlXml(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup)
{
	if (pPublicGroup == NULL || pPrivateGroup == NULL || pszSaveDir == NULL || wcslen(pszSaveDir) <= 0 || pszCtrlName == NULL || wcslen(pszCtrlName) <= 0)
		return false;

	xml_document XmlStrObj;
	xml_node RootNode = XmlStrObj.append_child(L"controls");
	if (!RootNode)
		return false;

	xml_node ControlNode = RootNode.append_child(pszCtrlName);
	if (!ControlNode)
		return false;

	AddIntAttrToNode(&ControlNode, _T(SKIN_OBJECT_ID), (int)pPrivateGroup->GetObjectId());

	xml_node GroupNode = ControlNode.append_child(pPrivateGroup->GetObjectType());
	if (!GroupNode)
		return false;

	AddIntAttrToNode(&GroupNode, _T(SKIN_OBJECT_ID), (int)pPrivateGroup->GetObjectId());
	AddStringAttrToNode(&GroupNode, L"name", pPrivateGroup->GetObjectName());


	if (!SaveCtrlGroupProp_CtrlXml(XmlStrObj, &GroupNode, pPublicGroup))
		return false;

	if (!SaveCtrlGroupProp_CtrlXml(XmlStrObj, &GroupNode, pPrivateGroup))
		return false;

	WCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	swprintf_s(szPath, MAX_PATH, L"%s\\%s", pszSaveDir, _T(CONTROLS_XML_NAME));
	return XmlStrObj.save_file(szPath, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

bool IPropertySkinManagerImpl::SaveCtrlGroupProp_CtrlXml(xml_document &XmlStrObj, xml_node* pParentNode, IPropertyGroup *pPropGroup)
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

		xml_node GroupChildNode = pParentNode->append_child(pProp->GetObjectType());
		if (!GroupChildNode)
			return false;

		AddIntAttrToNode(&GroupChildNode, _T(SKIN_OBJECT_ID), (int)pProp->GetObjectId());
		AddStringAttrToNode(&GroupChildNode, L"name", pProp->GetObjectName());

		if (pProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pNextPropGroup = dynamic_cast<IPropertyGroup*>(pProp);
			if (pNextPropGroup == NULL)
				continue;

			if (!SaveCtrlGroupProp_CtrlXml(XmlStrObj, &GroupChildNode, pNextPropGroup))
				return false;
		}
	}

	return true;
}

bool IPropertySkinManagerImpl::SaveControlDefaultSkin_ResourceXml(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup)
{
	if (pPublicGroup == NULL || pPrivateGroup == NULL || pszSaveDir == NULL || wcslen(pszSaveDir) <= 0 || pszCtrlName == NULL || wcslen(pszCtrlName) <= 0)
		return false;

	xml_document XmlStrObj;
	xml_node RootNode = XmlStrObj.append_child(L"resource");
	if (!RootNode)
		return false;

	if (!SaveCtrlGroupProp_ResXml(XmlStrObj, &RootNode, pPublicGroup, pszSaveDir))
		return false;

	if (!SaveCtrlGroupProp_ResXml(XmlStrObj, &RootNode, pPrivateGroup, pszSaveDir))
		return false;

	WCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	swprintf_s(szPath, MAX_PATH, L"%s\\%s", pszSaveDir, _T(RESOURCE_XML_NAME));
	return XmlStrObj.save_file(szPath, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

bool IPropertySkinManagerImpl::SaveCtrlGroupProp_ResXml(xml_document &XmlStrObj, xml_node* pParentNode, IPropertyGroup *pPropGroup, LPCWSTR pszSaveDir)
{
	if (pParentNode == NULL || pPropGroup == NULL)
		return false;

	GROUP_PROP_VEC* pPropVec = pPropGroup->GetPropVec();
	if (pPropVec == NULL)
		return false;

	xml_node GroupPropTypeNode = pParentNode->append_child(pPropGroup->GetObjectType());
	if (!GroupPropTypeNode)
		return false;

	if (!pPropGroup->AppendToXmlNode(XmlStrObj, &GroupPropTypeNode))
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

		xml_node PropTypeNode = pParentNode->append_child(pProp->GetObjectType());
		if (!PropTypeNode)
			return false;

		if (!pProp->AppendToXmlNode(XmlStrObj, &PropTypeNode))
			continue;

		if (pProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pNextPropGroup = dynamic_cast<IPropertyGroup*>(pProp);
			if (pNextPropGroup == NULL)
				continue;

			if (!SaveCtrlGroupProp_ResXml(XmlStrObj, pParentNode, pNextPropGroup, pszSaveDir))
				return false;
		}
		else if (pProp->GetObjectTypeId() == OTID_IMAGE)
		{
			IPropertyImage* pImage = dynamic_cast<IPropertyImage*>(pProp);
			if (pImage != NULL)
			{
				IPropertyImageBase* pImgBase = pImage->GetImageBaseProp();
				if (pImgBase != NULL)
				{
					xml_node ImgPropTypeNode = pParentNode->append_child(pImgBase->GetObjectType());
					if (!ImgPropTypeNode)
						return false;

					if (!pImgBase->AppendToXmlNode(XmlStrObj, &ImgPropTypeNode))
						continue;

					ZIP_FILE * pZipFile = pImgBase->GetZipFile();
					if (pZipFile != NULL && pZipFile->pFileData != NULL)
					{
						WCHAR szPath[MAX_PATH];
						memset(szPath, 0, sizeof(szPath));
						swprintf_s(szPath, MAX_PATH, L"%s\\%s", pszSaveDir, pZipFile->strFileName);
						SaveDataToFile(szPath, pZipFile->pFileData, pZipFile->dwSrcFileLen);						
					}
				}
			}
		}
		else if (pProp->GetObjectTypeId() == OTID_FONT)
		{
			IPropertyFont* pFont = dynamic_cast<IPropertyFont*>(pProp);
			if (pFont != NULL)
			{
				IPropertyFontBase* pFontBase = pFont->GetFontBaseProp();
				if (pFontBase != NULL)
				{
					xml_node FontPropTypeNode = pParentNode->append_child(pFontBase->GetObjectType());
					if (!FontPropTypeNode)
						return false;

					pFontBase->AppendToXmlNode(XmlStrObj, &FontPropTypeNode);
				}
			}
		}
		else if (pProp->GetObjectTypeId() == OTID_CURSOR)
		{
			IPropertyCursor* pCursor = dynamic_cast<IPropertyCursor*>(pProp);
			if (pCursor != NULL)
			{
				IPropertyCursorBase* pCursorBase = pCursor->GetCursorBaseProp();
				if (pCursorBase != NULL)
				{
					xml_node CursorPropTypeNode = pParentNode->append_child(pCursorBase->GetObjectType());
					if (!CursorPropTypeNode)
						return false;

					pCursorBase->AppendToXmlNode(XmlStrObj, &CursorPropTypeNode);
				}
			}
		}
		else if (pProp->GetObjectTypeId() == OTID_COLOR)
		{
			IPropertyColor* pColor = dynamic_cast<IPropertyColor*>(pProp);
			if (pColor != NULL)
			{
			//	IPropertyColorBase* pColorBase = pColor->GetColorBaseProp();
			//	if (pColorBase != NULL)
			//		pColorBase->AppendToXmlNode(XmlStrObj, pParentNode);
			}
		}
	}

	return true;
}

// 解析控件自定义皮肤的Resource.xml
bool IPropertySkinManagerImpl::ReadCtrlDftSkinXml(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pControl, LPCWSTR pszDefSkinDir, SKIN_FILE_ITEM &DftSkinFileItem)
{
	if (pszDefSkinDir == NULL || wcslen(pszDefSkinDir) <= 0)
		return false;

	CStringW strSkinPath = pszDefSkinDir;
	CStringW strResXml = strSkinPath + _T(RESOURCE_XML_NAME);
	CStringW strCtrlXml = strSkinPath + _T(CONTROLS_XML_NAME);

	string strResXmlData = "";
	if (!ReadTextFile(strResXml, strResXmlData))
		return false;

	string strCtrlXmlData = "";
	if (!ReadTextFile(strCtrlXml, strCtrlXmlData))
		return false;

	ZIP_FILE ResurceXml;
	ResurceXml.pFileData = (BYTE*)(strResXmlData.c_str());
	ResurceXml.dwSrcFileLen = (int)(strResXmlData.size());

	CSkinFileManager skinFile;
	if (!skinFile.TranslateResourceXml(&DftSkinFileItem, &ResurceXml))
	{
		skinFile.ReleaseSkinFileItem(&DftSkinFileItem);
		return false;
	}

	ResurceXml.pFileData = (BYTE*)(strCtrlXmlData.c_str());
	ResurceXml.dwSrcFileLen = (int)(strCtrlXmlData.size());
	if (!skinFile.TranslateControlsXml(&DftSkinFileItem, &ResurceXml))
	{
		skinFile.ReleaseSkinFileItem(&DftSkinFileItem);
		return false;
	}

	skinFile.SetPropRelevancy(skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), false), skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_IMAGE_NAME), false));
	skinFile.SetPropRelevancy(skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_FONT_BASE_NAME), false), skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_FONT_NAME), false));
	skinFile.SetPropRelevancy(skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_CURSOR_BASE_NAME), false), skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_CURSOR_NAME), false));
	skinFile.SetPropRelevancy(skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_COLOR_BASE_NAME), false), skinFile.GetIPropertyBaseMap(&DftSkinFileItem, _T(PROP_TYPE_COLOR_NAME), false));

	return true;
}

// 加载默认皮肤
bool IPropertySkinManagerImpl::SetControlDefaultSkin(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pControl, LPCWSTR pszDefSkinDir)
{
	if (pSkinFileItem == NULL || pControl == NULL || pszDefSkinDir == NULL || wcslen(pszDefSkinDir) <= 0)
		return false;

	IPropertyGroup* pCtrlPrivateGroup = pControl->GetCtrlPrivatePropGroup();
	IPropertyGroup* pCtrlPublicGroup = pControl->GetCtrlPublicPropGroup();
	if (pCtrlPrivateGroup == NULL || pCtrlPublicGroup == NULL)
		return false;

	SKIN_FILE_ITEM DftSkinFileItem;

	// 读取默认皮肤的xml
	if (!ReadCtrlDftSkinXml(pSkinFileItem, pControl, pszDefSkinDir, DftSkinFileItem))
		return false;

	bool bPrivateRet = SetControlDefaultSkinToCtrl(DftSkinFileItem, pSkinFileItem, pCtrlPrivateGroup, pControl, pszDefSkinDir);
	bool bPublicRet = SetControlDefaultSkinToCtrl(DftSkinFileItem, pSkinFileItem, pCtrlPublicGroup, pControl, pszDefSkinDir);

	CSkinFileManager skinFile;
	skinFile.ReleaseSkinFileItem(&DftSkinFileItem);
	return (bPrivateRet && bPublicRet);
}

bool IPropertySkinManagerImpl::SetControlDefaultSkinToCtrl(SKIN_FILE_ITEM &DftSkinFileItem, SKIN_FILE_ITEM* pCtrlSkinFileItem, IPropertyGroup* pCtrlPrivateGroup, IControlBase *pControl, LPCWSTR pszDefSkinDir)
{
	if (pCtrlPrivateGroup == NULL || pCtrlSkinFileItem == NULL || pControl == NULL || pszDefSkinDir == NULL || wcslen(pszDefSkinDir) <= 0)
		return false;

	IPropertyBaseMapMap::iterator pCtrlMapItem = DftSkinFileItem.AllCtrlPropMap.begin();
	if (pCtrlMapItem == DftSkinFileItem.AllCtrlPropMap.end())
		return false;

	IPropertyBaseMap* pCtrlMap = pCtrlMapItem->second;
	if (pCtrlMap == NULL)
		return false;

	IPropertyBaseMap::iterator pCtrlGroupItem = pCtrlMap->begin();
	if (pCtrlGroupItem == pCtrlMap->end())
		return false;

	IPropertyBase* pGroupBase = pCtrlGroupItem->second;
	if (pGroupBase == NULL)
		return false;

	IPropertyGroup* pCtrlPropGroup = dynamic_cast<IPropertyGroup*>(pGroupBase);
	if (pCtrlPropGroup == NULL)
		return false;

	GROUP_PROP_VEC* pPropVec =  pCtrlPropGroup->GetPropVec();
	if (pPropVec == NULL)
		return false;

	pGroupBase = (IPropertyBase*)pPropVec->TopObj();
	if (pGroupBase == NULL)
		return false;

	IPropertyGroup* pComGroup = dynamic_cast<IPropertyGroup*>(pGroupBase);
	if (pComGroup == NULL)
		return false;

	return CopyPropetry(DftSkinFileItem, pCtrlSkinFileItem, pComGroup, pCtrlPrivateGroup, pControl, pszDefSkinDir);
}

bool IPropertySkinManagerImpl::CopyPropetry(SKIN_FILE_ITEM &DftSkinFileItem, SKIN_FILE_ITEM* pCtrlSkinFileItem,
											IPropertyGroup* pFromGroup, IPropertyGroup* pCopyToGroup, IControlBase *pControl, LPCWSTR pszDefSkinDir)
{
	if (pFromGroup == NULL || pCopyToGroup == NULL || pCtrlSkinFileItem == NULL || pControl == NULL || pszDefSkinDir == NULL || wcslen(pszDefSkinDir) <= 0)
		return false;

	GROUP_PROP_VEC* pFromVec = pFromGroup->GetPropVec();
	GROUP_PROP_VEC* pCopyToVec = pCopyToGroup->GetPropVec();
	if (pCopyToVec == NULL || pFromVec == NULL)
		return false;

	ListObj* pCopyToNext = NULL;
	for (ListObj* pCopyToItem = pCopyToVec->TopObj(); pCopyToItem != NULL; pCopyToItem = pCopyToNext)
	{
		pCopyToNext = pCopyToVec->NextObj(pCopyToItem);
		IPropertyBase* pCopyToPropBase = (IPropertyBase*)pCopyToItem;
		if (pCopyToPropBase == NULL || pCopyToPropBase->GetObjectName() == NULL)
			continue;

		ListObj* pFromNext = NULL;
		for (ListObj* pFromItem = pFromVec->TopObj(); pFromItem != NULL; pFromItem = pFromNext)
		{
			pFromNext = pFromVec->NextObj(pFromItem);
			IPropertyBase* pFromPropBase = (IPropertyBase*)pFromItem;
			if (pFromPropBase == NULL || pFromPropBase->GetObjectName() == NULL)
				continue;

			// 比较名称，相同就克隆
			if (lstrcmpiW(pCopyToPropBase->GetObjectName(), pFromPropBase->GetObjectName()) == 0 &&
				pCopyToPropBase->GetObjectTypeId() == pFromPropBase->GetObjectTypeId())
			{
				if (pCopyToPropBase->GetObjectTypeId() == OTID_GROUP)
				{
					if (!CopyPropetry(DftSkinFileItem, pCtrlSkinFileItem, dynamic_cast<IPropertyGroup*>(pFromPropBase), dynamic_cast<IPropertyGroup*>(pCopyToPropBase), pControl, pszDefSkinDir))
						return false;
				}
				else
				{
					// ObjId 不克隆
					if (lstrcmpiW(pCopyToPropBase->GetObjectName(), _T(NAME_SKIN_PROP_NAME_OBJ_ID)) == 0)
						continue;

					pCopyToPropBase->Clone(pFromPropBase);
					if (pCopyToPropBase->GetObjectTypeId() == OTID_IMAGE)
					{
						pCopyToPropBase->SetRelevancyProp(NULL);
						IPropertyImageBase* pFromImgBase = dynamic_cast<IPropertyImageBase*>(pFromPropBase->GetRelevancyProp());
						if (pFromImgBase == NULL)
							continue;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 查找现有的属性队列中是否已经存在了相同名称的属性，如果存在就不再复制，直接利用当下的属性。
						CSkinFileManager skinFile;
						IPropertyBaseMap* pCopyToImgBaseMap = skinFile.GetIPropertyBaseMap(pCtrlSkinFileItem, _T(PROP_TYPE_IMAGE_BASE_NAME), true);
						if (pCopyToImgBaseMap == NULL)
							return false;

						IPropertyImageBase* pFindCopyToImgBase = NULL;
						for (IPropertyBaseMap::iterator pCopytoImgBaseItem = pCopyToImgBaseMap->begin();
							pCopytoImgBaseItem != pCopyToImgBaseMap->end(); pCopytoImgBaseItem++)
						{
							IPropertyImageBase* pCopyToImgBase = dynamic_cast<IPropertyImageBase*>(pCopytoImgBaseItem->second);;
							if (pCopyToImgBase == NULL)
								continue;

							if (lstrcmpiW(pFromImgBase->GetObjectName(), pCopyToImgBase->GetObjectName()) == 0)
							{
								pFindCopyToImgBase = pCopyToImgBase;
								break;
							}
						}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						if (pFindCopyToImgBase == NULL)
						{
							// 如果没有这个属性，需要创建一个新的属性
							pFindCopyToImgBase = dynamic_cast<IPropertyImageBase*>(this->CreateEmptyBaseProp(pCtrlSkinFileItem, OTID_IMAGE_BASE));
							if (pFindCopyToImgBase == NULL)
								continue;

							if (!pFindCopyToImgBase->Clone(pFromImgBase))
								continue;
							pFindCopyToImgBase->SetObjectName(pFromImgBase->GetObjectName());

							//////////////////////////////////////////////////////////////////////////
							// 处理图片信息
							IZipFile * pZipFile = pCtrlSkinFileItem->pZipFileMgr;
							IMAGE_BASE_PROP* pPictureProp = pFindCopyToImgBase->GetImageProp();
							if (pZipFile == NULL || pPictureProp == NULL)
								return false;

							ZIP_FILE *pFindZipFile = pZipFile->FindUnZipFile(pPictureProp->strFileName);
							if (pFindZipFile == NULL)
							{
								CStringW strImgPath = pszDefSkinDir;
								strImgPath += pPictureProp->strFileName;

								pFindZipFile = pZipFile->AppendLocalFileToZipFile(strImgPath, pPictureProp->strFileName);
							}

							pFindCopyToImgBase->SetZipFile(pFindZipFile, false);
						}

						pCopyToPropBase->SetRelevancyProp(pFindCopyToImgBase);
					}
					else if (pCopyToPropBase->GetObjectTypeId() == OTID_FONT)
					{
						pCopyToPropBase->SetRelevancyProp(NULL);
						IPropertyFontBase* pFromFontBase = dynamic_cast<IPropertyFontBase*>(pFromPropBase->GetRelevancyProp());
						if (pFromFontBase == NULL)
							continue;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 查找现有的属性队列中是否已经存在了相同名称的属性，如果存在就不再复制，直接利用当下的属性。
						CSkinFileManager skinFile;
						IPropertyBaseMap* pCopyToFontBaseMap = skinFile.GetIPropertyBaseMap(pCtrlSkinFileItem, _T(PROP_TYPE_FONT_BASE_NAME), true);
						if (pCopyToFontBaseMap == NULL)
							return false;

						IPropertyFontBase* pFindCopyToFontBase = NULL;
						for (IPropertyBaseMap::iterator pCopytoFontBaseItem = pCopyToFontBaseMap->begin();
							pCopytoFontBaseItem != pCopyToFontBaseMap->end(); pCopytoFontBaseItem++)
						{
							IPropertyFontBase* pCopyToFontBase = dynamic_cast<IPropertyFontBase*>(pCopytoFontBaseItem->second);;
							if (pCopyToFontBase == NULL)
								continue;

							if (lstrcmpiW(pFromFontBase->GetObjectName(), pCopyToFontBase->GetObjectName()) == 0)
							{
								pFindCopyToFontBase = pCopyToFontBase;
								break;
							}
						}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						if (pFindCopyToFontBase == NULL)
						{
							// 如果没有这个属性，需要创建一个新的属性
							pFindCopyToFontBase = dynamic_cast<IPropertyFontBase*>(this->CreateEmptyBaseProp(pCtrlSkinFileItem, OTID_FONT_BASE));
							if (pFindCopyToFontBase == NULL)
								continue;

							if (!pFindCopyToFontBase->Clone(pFromFontBase))
								continue;
							pFindCopyToFontBase->SetObjectName(pFromFontBase->GetObjectName());
						}

						pCopyToPropBase->SetRelevancyProp(pFindCopyToFontBase);

					}
					else if (pCopyToPropBase->GetObjectTypeId() == OTID_CURSOR)
					{
						pCopyToPropBase->SetRelevancyProp(NULL);
						IPropertyCursorBase* pFromCursorBase = dynamic_cast<IPropertyCursorBase*>(pFromPropBase->GetRelevancyProp());
						if (pFromCursorBase == NULL)
							continue;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// 查找现有的属性队列中是否已经存在了相同名称的属性，如果存在就不再复制，直接利用当下的属性。
						CSkinFileManager skinFile;
						IPropertyBaseMap* pCopyToCursorBaseMap = skinFile.GetIPropertyBaseMap(pCtrlSkinFileItem, _T(PROP_TYPE_CURSOR_BASE_NAME), true);
						if (pCopyToCursorBaseMap == NULL)
							return false;

						IPropertyCursorBase* pFindCopyToCursorBase = NULL;
						for (IPropertyBaseMap::iterator pCopytoCursorBaseItem = pCopyToCursorBaseMap->begin();
							pCopytoCursorBaseItem != pCopyToCursorBaseMap->end(); pCopytoCursorBaseItem++)
						{
							IPropertyCursorBase* pCopyToCursorBase = dynamic_cast<IPropertyCursorBase*>(pCopytoCursorBaseItem->second);;
							if (pCopyToCursorBase == NULL)
								continue;

							if (lstrcmpiW(pFromCursorBase->GetObjectName(), pCopyToCursorBase->GetObjectName()) == 0)
							{
								pFindCopyToCursorBase = pCopyToCursorBase;
								break;
							}
						}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						pCopyToPropBase->SetRelevancyProp(pFindCopyToCursorBase);
					}
				}
			}
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 释放一个属性的内存空间
void IPropertySkinManagerImpl::ReleaseIPropertyBase(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pCtrlProp)
{
	m_SkinFile.ReleaseIPropertyBase(pCtrlProp);
}

// 从控件属性队列中删除一个控件，仅仅是删除控件的节点，控件的具体属性并没有删除
bool IPropertySkinManagerImpl::DeleteControlPropetryFromCtrlPropMap(IControlBase* pCtrl)
{
	return m_SkinFile.DeleteControlPropetryFromCtrlPropMap(pCtrl);
}
