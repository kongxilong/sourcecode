
#include "StdAfx.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "..\..\Inc\IPropertyFont.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include <stdarg.h>
#include <shellapi.h>
#include <shlwapi.h>


const char* PathHelper(char *pszFileName)
{
	static char _sszDir[MAX_PATH + 1];
	memset(_sszDir, 0, sizeof(_sszDir));
	::GetModuleFileNameA(NULL, _sszDir, MAX_PATH);
	::PathRemoveFileSpecA(_sszDir);

	strcat_s(_sszDir, MAX_PATH, "\\");
	if (pszFileName != NULL && strlen(pszFileName) > 0)
		strcat_s(_sszDir, MAX_PATH, pszFileName);

	return _sszDir;
}

const WCHAR* PathHelperW(LPCWSTR pszFileName)
{
	static WCHAR _sszDir[MAX_PATH + 1];
	memset(_sszDir, 0, sizeof(_sszDir));
	::GetModuleFileNameW(NULL, _sszDir, MAX_PATH);
	::PathRemoveFileSpecW(_sszDir);

	wcscat_s(_sszDir, MAX_PATH, L"\\");
	if (pszFileName != NULL && wcslen(pszFileName) > 0)
		wcscat_s(_sszDir, MAX_PATH, pszFileName);

	return _sszDir;
}

OBJECT_TYPE_ID PropStringToType(const WCHAR* strPropType)
{
	if (strPropType == NULL || wcslen(strPropType) <= 0)
		return OTID_NONE;

	if (lstrcmpiW(strPropType, _T(PROP_TYPE_FONT_NAME)) == 0)
	{
		return OTID_FONT;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_STRING_NAME)) == 0)
	{
		return OTID_STRING;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_COMBOBOX_NAME)) == 0)
	{
		return OTID_COMBOBOX;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_IMAGE_NAME)) == 0)
	{
		return OTID_IMAGE;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_CURSOR_NAME)) == 0)
	{
		return OTID_CURSOR;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_COLOR_NAME)) == 0)
	{
		return OTID_COLOR;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_BOOL_NAME)) == 0)
	{
		return OTID_BOOL;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_INT_NAME)) == 0)
	{
		return OTID_INT;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_GROUP_NAME)) == 0)
	{
		return OTID_GROUP;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_CONTROL_NAME)) == 0)
	{
		return OTID_CONTROL;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_WINDOW_NAME)) == 0)
	{
		return OTID_WINDOW;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_COLOR_BASE_NAME)) == 0)
	{
		return OTID_COLOR_BASE;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_CURSOR_BASE_NAME)) == 0)
	{
		return OTID_CURSOR_BASE;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_FONT_BASE_NAME)) == 0)
	{
		return OTID_FONT_BASE;
	}
	else if (lstrcmpiW(strPropType, _T(PROP_TYPE_IMAGE_BASE_NAME)) == 0)
	{
		return OTID_IMAGE_BASE;
	}

	return OTID_NONE;
}

const WCHAR* PropTypeToString(OBJECT_TYPE_ID propType)
{
	CStringW strType(L"");
	switch (propType)
	{
	case OTID_BOOL:
		strType = _T(PROP_TYPE_BOOL_NAME);
		break;

	case OTID_COLOR:
		strType = _T(PROP_TYPE_COLOR_NAME);
		break;

	case OTID_COMBOBOX:
		strType = _T(PROP_TYPE_COMBOBOX_NAME);
		break;

	case OTID_CURSOR:
		strType = _T(PROP_TYPE_CURSOR_NAME);
		break;

	case OTID_FONT:
		strType = _T(PROP_TYPE_FONT_NAME);
		break;

	case OTID_IMAGE:
		strType = _T(PROP_TYPE_IMAGE_NAME);
		break;

	case OTID_INT:
		strType = _T(PROP_TYPE_INT_NAME);
		break;

	case OTID_STRING:
		strType = _T(PROP_TYPE_STRING_NAME);
		break;

	case OTID_GROUP:
		strType = _T(PROP_TYPE_GROUP_NAME);
		break;

	case OTID_CONTROL:
		strType = _T(PROP_TYPE_CONTROL_NAME);
		break;

	case OTID_WINDOW:
		strType = _T(PROP_TYPE_WINDOW_NAME);
		break;

	case OTID_COLOR_BASE:
		strType = _T(PROP_TYPE_COLOR_BASE_NAME);
		break;

	case OTID_CURSOR_BASE:
		strType = _T(PROP_TYPE_CURSOR_BASE_NAME);
		break;

	case OTID_FONT_BASE:
		strType = _T(PROP_TYPE_FONT_BASE_NAME);
		break;

	case OTID_IMAGE_BASE:
		strType = _T(PROP_TYPE_IMAGE_BASE_NAME);
		break;

	default:
		break;
	}

	static WCHAR _sType[33];
	memset(_sType, 0, sizeof(_sType));
	swprintf_s(_sType, 32, strType);
	return _sType;
}

// 创建一个属性，如果这个属性在 group 中已经有了，就返回这个属性，如果没有就新建一个属性
IPropertyBase* CreateResourcePropetry(SKIN_FILE_ITEM* pSkinFileItem, IFeatureObject *pOwnerObj, IPropertySkinManager* pSkinPropMgr,
									  IPropertyGroup* pGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo, bool bNotExistCreate)
{
	if (pSkinFileItem == NULL || pSkinPropMgr == NULL || pGroup == NULL || pszPropName == NULL || propType <= OTID_NONE || propType >= OTID_LAST || strlen(pszPropName) <= 0)
		return NULL;

	GROUP_PROP_VEC *pChildGroup = pGroup->GetPropVec();
	if (pChildGroup == NULL)
		return NULL;

	CStringW strPropNameW = pszPropName;
	IPropertyBase* pPropBase = NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildGroup->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildGroup->NextObj(pItem);
		pPropBase = (IPropertyBase*)pItem;
		if (pPropBase == NULL || pPropBase->GetObjectName() == NULL)
			continue;

		if (strPropNameW.CompareNoCase(pPropBase->GetObjectName()) == 0)
		{
			if (propType == pPropBase->GetObjectTypeId())
			{
				// 找到了已经设置的属性
				pPropBase->SetObjectInfo(pszPropInfo);
				pPropBase->SetOwnerObject(pOwnerObj);
				// SetActivePropetry(true) 的属性才是有效的属性，会被显示，会被保存
				pPropBase->SetActivePropetry(true);
				return pPropBase;
			}
			else
			{
				// 属性名字一样，但属性类型不一样，非法
				pChildGroup->DelObj(pItem);
			}
			break;
		}
	}

	if (pPropBase == NULL && !bNotExistCreate)
		return NULL;

	pPropBase = pSkinPropMgr->CreateEmptyBaseProp(pSkinFileItem, propType);
	if (pPropBase == NULL || pPropBase->GetUiKernel() == NULL)
		return NULL;

	// SetActivePropetry(true) 的属性才是有效的属性，会被显示，会被保存
	pPropBase->SetActivePropetry(true);
	pPropBase->SetOwnerObject(pOwnerObj);
	pPropBase->SetObjectName(strPropNameW.LockBuffer());
	strPropNameW.UnlockBuffer();
	pPropBase->SetObjectInfo(pszPropInfo);

	if (propType == OTID_FONT && !pPropBase->GetUiKernel()->BD_IsBuilderReadingSkin())
	{
		IPropertyFont* pFont = dynamic_cast<IPropertyFont*>(pPropBase);
		if (pFont != NULL)
			pFont->SetFontBaseProp(pSkinPropMgr->GetDefaultFontBase(pSkinFileItem));
	}

	pGroup->AppendProperty(pPropBase);
	return pPropBase;
}

bool FileExistsW(LPCWSTR pszFilePath)
{
	if (pszFilePath == NULL)
		return false;

	DWORD dwGet = ::GetFileAttributesW(pszFilePath);
	if (dwGet == 0xFFFFFFFF)
		return false;

	return true;
}

bool FileExists(const char *pszFilePath)
{
	if (pszFilePath == NULL)
		return false;

	DWORD dwGet = ::GetFileAttributesA(pszFilePath);
	if (dwGet == 0xFFFFFFFF)
		return false;

	return true;
}

void AddIntAttrToNode(xml_node* pNode, const WCHAR* pszAttrName, int nInt)
{
	if (pNode == NULL || pszAttrName == NULL || wcslen(pszAttrName) <= 0)
		return;

	WCHAR szInt[MAX_PATH];
	memset(szInt, 0, sizeof(szInt));
	swprintf_s(szInt, MAX_PATH-1, L"%d", nInt);
	AddStringAttrToNode(pNode, pszAttrName, (const WCHAR *)szInt);
}

void AddStringAttrToNode(xml_node* pNode, const WCHAR * pszAttrName, const WCHAR * pszData)
{
	if (pNode == NULL || pszAttrName == NULL || wcslen(pszAttrName) <= 0)
		return;

	xml_attribute xmlAttr = pNode->append_attribute(pszAttrName);
	if (!xmlAttr)
		return;

	if (pszData == NULL || wcslen(pszData) <= 0)
		return;

	xmlAttr = pszData;
}

void DebugInfoOutput(char *pszFormat, ...)
{
	if (pszFormat == NULL || strlen(pszFormat) <= 0)
		return;

	char *pszOut = NULL;
	pszOut = new char[4096];
	if (pszOut == NULL)
		return;
	memset(pszOut, 0, 4096);

	va_list argPtr;
	va_start(argPtr, pszFormat);
	int nPrint = vsprintf_s(pszOut, 4095, pszFormat, argPtr);
	va_end(argPtr);

	::OutputDebugStringA(pszOut);
	::OutputDebugStringA("\n");

	SAFE_DELETE_LIST(pszOut);
}

IPropertyString* FindObjectNameProperty(IPropertyGroup *pGroupProp)
{
	if (pGroupProp == NULL)
		return NULL;

	GROUP_PROP_VEC* pPropVec = pGroupProp->GetPropVec();
	if (pPropVec == NULL)
		return NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pPropVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pPropVec->NextObj(pItem);
		IPropertyBase* pProp = (IPropertyBase*)pItem;
		if (pProp == NULL)
			continue;

		if (pProp->GetObjectTypeId() == OTID_STRING)
		{
			if (lstrcmpiW(pProp->GetObjectName(), _T(NAME_SKIN_PROP_NAME)) == 0)
				return (dynamic_cast<IPropertyString*>(pProp));
		}

		if (pProp->GetObjectTypeId() == OTID_GROUP)
		{
			IPropertyGroup *pGroup = dynamic_cast<IPropertyGroup*>(pProp);
			if (pGroup != NULL)
			{
				IPropertyString *pFind = FindObjectNameProperty(pGroup);
				if (pFind != NULL)
					return pFind;
			}
		}
	}

	return NULL;
}

void RectScreenToClient(HWND hWnd, RECT& rct)
{
	if (!::IsWindow(hWnd))
		return;

	POINT pt = {rct.left, rct.top};
	::ScreenToClient(hWnd, &pt);

	int nW = RECT_WIDTH(rct);
	int nH = RECT_HEIGHT(rct);

	rct.left = pt.x;
	rct.right = rct.left + nW;
	rct.top = pt.y;
	rct.bottom = rct.top + nH;
}

void RectClientToScreen(HWND hWnd, RECT& rct)
{
	if (!::IsWindow(hWnd))
		return;

	POINT pt = {rct.left, rct.top};
	::ClientToScreen(hWnd, &pt);

	int nW = RECT_WIDTH(rct);
	int nH = RECT_HEIGHT(rct);

	rct.left = pt.x;
	rct.right = rct.left + nW;
	rct.top = pt.y;
	rct.bottom = rct.top + nH;
}

void InitPropImageBase(IMAGE_BASE_PROP *pImgBaseProp)
{
	if (pImgBaseProp == NULL)
		return;

	pImgBaseProp->bIsZipFile = true;
	pImgBaseProp->pszSkinFilePath = NULL;
	memset(pImgBaseProp->strFileName, 0, MAX_PATH + 1);
	pImgBaseProp->ImgPlayType = IPT_STATIC_IMG;
	pImgBaseProp->ImgShowType = IST_PINGPU;
	pImgBaseProp->ImgLoopType = ILT_LOOP_1;
	pImgBaseProp->ImgBoFangType = IBFT_ZHENGXIANG;
	pImgBaseProp->bIsDrawJggMid = false;
	INIT_RECT(pImgBaseProp->RectInImage);
	INIT_RECT(pImgBaseProp->jggInfo);
}

// 清空属性的隶属于哪个控件/窗口的信息
void ClearGroupPropertyOwner(IPropertyGroup *pPropGroup)
{
	if (pPropGroup == NULL)
		return;

	GROUP_PROP_VEC *pPropVec = pPropGroup->GetPropVec();
	if (pPropVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pPropVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pPropVec->NextObj(pItem);
		IPropertyBase* pProp = (IPropertyBase*)pItem;
		if (pProp == NULL)
			continue;

		pProp->SetOwnerObject(NULL);

		OBJECT_TYPE_ID ObjTypeId = pProp->GetObjectTypeId();
		if (ObjTypeId == OTID_GROUP)
			ClearGroupPropertyOwner(dynamic_cast<IPropertyGroup*>(pProp));
	}
}

// 图片抠边，MarkDrawBoard 图片中黑色部分将会从 DstDrawBoard 中抠出
bool ClipImageWithBlack(CDrawingBoard &MarkDrawBoard, CDrawingBoard &DstDrawBoard)
{
	if (MarkDrawBoard.GetBits() == NULL || DstDrawBoard.GetBits() == NULL)
		return false;

	if (MarkDrawBoard.GetDcSize().cx != DstDrawBoard.GetDcSize().cx || MarkDrawBoard.GetDcSize().cy != DstDrawBoard.GetDcSize().cy)
		return false;

	BYTE* pDataMask = MarkDrawBoard.GetBits();
	BYTE* pDataImg = DstDrawBoard.GetBits();

	for(int i = 0; i < MarkDrawBoard.GetDcSize().cx * MarkDrawBoard.GetDcSize().cy; i++)
	{
		if((*(pDataMask+1)) == 0 && (*(pDataMask+2)) == 0)
			memset(pDataImg, 0, 4);

		pDataImg += 4;
		pDataMask += 4;
	}

	return true;
}

bool ClipImageWithAlpha(CDrawingBoard &MarkDrawBoard, CDrawingBoard &DstDrawBoard)
{
	if (MarkDrawBoard.GetBits() == NULL || DstDrawBoard.GetBits() == NULL)
		return false;

	if (MarkDrawBoard.GetDcSize().cx != DstDrawBoard.GetDcSize().cx || MarkDrawBoard.GetDcSize().cy != DstDrawBoard.GetDcSize().cy)
		return false;

	return ClipImageWithAlpha(MarkDrawBoard.GetBits(), DstDrawBoard.GetBits(), MarkDrawBoard.GetDcSize().cx * MarkDrawBoard.GetDcSize().cy);
}

bool ClipImageWithAlpha(BYTE* pBitsSrc, BYTE* pBitsDes, int nPixelCtns)
{
	if (pBitsDes == NULL || pBitsSrc == NULL || nPixelCtns <= 0)
		return false;

	__asm
	{
		pxor		xmm6, xmm6
		mov			edi, pBitsDes
		mov			esi, pBitsSrc
		mov			eax, nPixelCtns
loopJudge:
		cmp			eax, 2
		jl			LastOne

		movlps		xmm0,[esi]			
		punpcklbw 	xmm0,xmm6			
		pshuflw		xmm2,xmm0,0ffh		
		pshufhw		xmm1,xmm0,0ffh		
		shufpd		xmm2, xmm1,2		
		movlps		xmm3,[edi]	
		punpcklbw 	xmm3,xmm6			
		pmullw		xmm2, xmm3
		psrlw       xmm2, 8
		packuswb	xmm2,xmm6
		movlps		[edi], xmm2

		add			esi, 8
		add			edi, 8
		sub			eax, 2
		jmp			loopJudge
LastOne:
		test		eax, eax
		jz			End

		movd		xmm0,[esi]		
		movd		xmm1,[edi]		
		punpcklbw 	xmm0,xmm6		
		punpcklbw	xmm1,xmm6
		pshuflw		xmm2,xmm0,0ffh	
		pmullw		xmm2, xmm1
		psrlw       xmm2, 8
		packuswb	xmm2,xmm6
		movd		[edi], xmm2
End:
	}

	return true;
}

bool ReadTextFile(LPCWSTR pszFilePath, string &strOutData)
{
	strOutData = "";
	if (pszFilePath == NULL || wcslen(pszFilePath) <= 0)
		return false;

	WIN32_FILE_ATTRIBUTE_DATA FileAttr;
	if (!::GetFileAttributesExW(pszFilePath, GetFileExInfoStandard, &FileAttr))
		return false;

	FILE *pFile = NULL;
	_wfopen_s(&pFile, pszFilePath, L"rb");
	if (pFile == NULL)
		return false;

	BYTE *pReadBuf = new BYTE[FileAttr.nFileSizeLow];
	if (pReadBuf == NULL)
	{
		fclose(pFile);
		return false;
	}

	int nReadCtns = 0;
	while (nReadCtns < (int)FileAttr.nFileSizeLow)
	{
		BYTE *pRead = pReadBuf + nReadCtns;
		int nNeedRead = FileAttr.nFileSizeLow - nReadCtns;

		int nRead = fread_s(pRead, nNeedRead, 1, nNeedRead, pFile);
		nReadCtns += nRead;
	}

	fclose(pFile);
	if (nReadCtns != (int)FileAttr.nFileSizeLow)
	{
		SAFE_DELETE(pReadBuf);
		return false;
	}

	strOutData = (char*)pReadBuf;
	SAFE_DELETE(pReadBuf);
	return true;
}

// 是否为同一类型的属性
bool IsSameTypePropetry(IPropertyBase* pPropBase, IPropertyBase* pComPropBase)
{
	if (pPropBase == NULL || pComPropBase == NULL)
		return false;
	
	const WCHAR* pCom = pComPropBase->GetObjectType();
	const WCHAR* pTo = pPropBase->GetObjectType();

	if (pCom == NULL || pTo == NULL)
		return false;

	return (lstrcmpiW(pCom, pTo) == 0);
}

// 从xml中取得指定node的指定attr
const WCHAR * XmlGetAttrValue(xml_node* pXmlNode, const WCHAR* pwszAttrName)
{
	if (pXmlNode == NULL || pwszAttrName == NULL || wcslen(pwszAttrName) <= 0)
		return NULL;

	return pXmlNode->attribute(pwszAttrName).as_string();
}

bool SaveDataToFile(LPCWSTR pszFilePath, BYTE *pData, int nDataLen)
{
	if (pszFilePath == NULL || pData == NULL || nDataLen <= 0)
		return false;

	FILE* fp = NULL;
	_wfopen_s(&fp, pszFilePath, L"wb");
	if (fp != NULL)
	{
		fwrite(pData, nDataLen, 1, fp);
		fclose(fp);
		return true;
	}

	return false;
}

const WCHAR * GetGuidString()
{
	static WCHAR szGuid[STATIC_LEN];
	memset(szGuid, 0, sizeof(szGuid));

	GUID guid;
	::CoCreateGuid(&guid);
	swprintf_s(szGuid, STATIC_LEN, L"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return szGuid;
}

// 取得控件指针
IControlBase* GetSubControlByName(char *pszCtrlName, CHILD_CTRLS_VEC &ChildCtrlsVec)
{
	if (pszCtrlName == NULL || strlen(pszCtrlName) <= 0)
		return NULL;

	IControlBase *pCtrl = NULL;
	CStringW strDataW = pszCtrlName;
	GetControlByNameFromVec(&ChildCtrlsVec, strDataW, &pCtrl);
	return pCtrl;
}

// 取得控件指针
IControlBase* GetSubControlByName(LPCWSTR pszCtrlName, CHILD_CTRLS_VEC &ChildCtrlsVec)
{
	if (pszCtrlName == NULL || wcslen(pszCtrlName) <= 0)
		return NULL;

	IControlBase *pCtrl = NULL;
	CStringW strDataW = pszCtrlName;
	GetControlByNameFromVec(&ChildCtrlsVec, strDataW, &pCtrl);
	return pCtrl;
}

// 取得控件指针
IControlBase* GetSubControlByName(WCHAR *pszCtrlName, CHILD_CTRLS_VEC &ChildCtrlsVec)
{
	if (pszCtrlName == NULL || wcslen(pszCtrlName) <= 0)
		return NULL;

	IControlBase *pCtrl = NULL;
	CStringW strDataW = pszCtrlName;
	GetControlByNameFromVec(&ChildCtrlsVec, strDataW, &pCtrl);
	return pCtrl;
}

bool GetControlByNameFromVec(CHILD_CTRLS_VEC *pCtrlVec, LPCWSTR pszCtrlName, IControlBase **ppCtrl)
{
	if (pCtrlVec == NULL || pszCtrlName == NULL || wcslen(pszCtrlName) <= 0 || ppCtrl == NULL)
		return false;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pCtrlVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pCtrlVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (!pCtrl->GetActivePropetry())
			continue;

		if (lstrcmpiW(pCtrl->GetObjectName(), pszCtrlName) == 0)
		{
			*ppCtrl = pCtrl;
			return true;
		}

		if (GetControlByNameFromVec(pCtrl->GetChildControlsVec(), pszCtrlName, ppCtrl))
			return true;
	}

	return false;
}

// 取得控件指针
IControlBase* GetSubControlByObjectId(DWORD dwObjId, CHILD_CTRLS_VEC &ChildCtrlsVec)
{
	IControlBase *pCtrl = NULL;
	GetControlByObjectIdFromVec(&ChildCtrlsVec, dwObjId, &pCtrl);
	return pCtrl;
}

bool GetControlByObjectIdFromVec(CHILD_CTRLS_VEC *pCtrlVec, DWORD dwObjId, IControlBase **ppCtrl)
{
	if (pCtrlVec == NULL || dwObjId == ERROR_OBJECT_ID || ppCtrl == NULL)
		return false;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pCtrlVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pCtrlVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (!pCtrl->GetActivePropetry())
			continue;

		if (pCtrl->GetObjectId() == dwObjId)
		{
			*ppCtrl = pCtrl;
			return true;
		}

		if (GetControlByObjectIdFromVec(pCtrl->GetChildControlsVec(), dwObjId, ppCtrl))
			return true;
	}

	return false;
}
