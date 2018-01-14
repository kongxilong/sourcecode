
#pragma once

#define PUGIXML_WCHAR_MODE
#include "..\..\Inc\PugiXml\pugixml.hpp"
using namespace pugi;

#include "..\..\Inc\IPropertySkinManager.h"
#include "IPropertyWindowImpl.h"
#include "IZipFileImpl.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "..\..\Inc\IWindowBase.h"


class CSkinFileManager
{
public:
	CSkinFileManager();
	~CSkinFileManager();

	// 设置显示的语言种类
	virtual void SetArea(AREA_TYPE areaType);

	// 读取一个皮肤包信息
	SKIN_FILE_ITEM* ReadSkinFile(LPCWSTR pszSkinPath);

	// 删除所有的皮肤包文件
	void ReleaseAllSkinFile();
	// 删除其中一个皮肤包文件
	void ReleaseSkinFile(LPCWSTR pszSkinPath);
	// 删除一个属性
	void ReleaseBaseProp(IPropertyBase *pCtrlProp);
	// 取得皮肤文件
	SKIN_FILE_ITEM* GetSkinFileItem(LPCWSTR pszSkinPath, bool bNew = false);
	// 创建一个空的皮肤文件
	SKIN_FILE_ITEM* CreateEmptySkinFileItem(LPCWSTR pszSkinPath);

	// 取得指定组的属性
	IPropertyBaseMap* GetIPropertyBaseMap(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropGroupName, bool bIsCreate);

	// 查找指定的属性
	IPropertyBase* FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD dwObjId);
	IPropertyBase* FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, DWORD dwObjId);
	// 查找指定的属性
	IPropertyBase* FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, LPCWSTR pszPropName);
	IPropertyBase* FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, LPCWSTR pszPropName);

	// 从控件属性队列中删除一个控件，仅仅是删除控件的节点，控件的具体属性并没有删除
	bool DeleteControlPropetryFromCtrlPropMap(IControlBase* pCtrl);
	// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
	bool ReadSkinFileItem(IZipFile *pZipFileMgr, ZIP_FILE* pZipItem);

	// 保存皮肤包
	bool SaveSkinFileItem(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSkinDir, LPCWSTR pszSkinName, char *pszPassword);

	// 创建一个属性，并将此属性放入队列
	IPropertyBase* CreateEmptyBaseProp(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD *pdwObjectId = NULL);

	// 本地图片使用的zip文件
	SKIN_FILE_ITEM * GetLocalImageSkinFileItem();
	// 取得整个皮肤包默认的字体信息
	IPropertyFontBase* GetDefaultFontBase(SKIN_FILE_ITEM* pSkinFileItem);

	// 删除一个控件的所有属性
	bool DeleteControlPropetry(IControlBase* pCtrl);

	// 删除属性中的Base属性，如ImageBase、FontBase
	bool DeletePropetryBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase);
	bool DeleteFontBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyFontBase *pFontBaseProp);
	bool ModifyFontBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyFontBase *pFontBaseProp, LPCWSTR pszNewPropName);

	bool DeleteImageBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pImgBaseProp);
	bool ModifyImageBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pImgBaseProp, LPCWSTR pszNewPropName);

//////////////////////////////////////////////////////////////////////////
	// 解析Resource.xml
	bool TranslateResourceXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pResurceXml);
	// Controls.xml 相关
	bool TranslateControlsXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pControlsXml);
	void ReleaseSkinFileItem(SKIN_FILE_ITEM *pSkinFile);
	void SetPropRelevancy(IPropertyBaseMap* pFromMap, IPropertyBaseMap* pToMap);
//////////////////////////////////////////////////////////////////////////

	void ReleaseIPropertyBase(IPropertyBase *pCtrlProp);

private:

	bool DeleteControlPropetryFromCtrlPropMapByVec(PROP_CONTROL_VEC* pVec, IPropertyControl* pCtrlProp);
	bool DeleteControlPropetryByGroup(IPropertyGroup *pGroup, SKIN_FILE_ITEM* pSkinItem);
	void ReleaseLayoutMap(IPropertyBaseMap &LayoutWindowMap);
	void ReleaseLayoutMapPropControlVec(PROP_CONTROL_VEC* pCtrlVec);
	void ReleasePropMap(IPropertyBaseMapMap &PropMap, bool bReleaseChild);
	void ReleasePropMapItem(IPropertyBaseMap* pPropMapItem);

	bool GeneralCreateSubProp(SKIN_FILE_ITEM *pSkinFileItem, xml_node* pXmlNode, IPropertyBaseMap* pCtrlPropMap);
	bool AppendBasePropToGroup(SKIN_FILE_ITEM *pSkinFileItem, IPropertyGroup *pGroup, xml_node* pXmlNode);

	// 解析Windows.xml
	bool TranslateWindowsXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pWindowsXml);

	// Layout.xml 相关
	bool TranslateLayoutXml(SKIN_FILE_ITEM *pSkinFileItem, ZIP_FILE *pLayoutXml);
	bool GeneralCreateWindowLayoutProp(SKIN_FILE_ITEM *pSkinFileItem, xml_node* pXmlNode, PROP_CONTROL_VEC* pChildCtrlVec, IPropertyControl* pParentProp);

	// 通用的从资源xml中创建属性
	bool GeneralCreateBaseProp(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pPropType, xml_node* pXmlNode, IPropertyBaseMap* pPropMap);

	// 设置属性与base属性的关联
	void SetPropetryBaseRelevancy(SKIN_FILE_ITEM *pSkinFileItem);
	// 设置imagebase属性和图片的关联
	bool SetImageFileRelevancy(SKIN_FILE_ITEM *pSkinFileItem);

	IPropertyGroup* FindControlPropGroup(SKIN_FILE_ITEM *pSkinFileItem, DWORD dwObjId, CStringW &strCtrlType);

	// 设置 IPropertyImageBase 属性中的 IMAGE_BASE_PROP 的 pszSkinFilePath
	void SetImageBaseSkinPath(SKIN_FILE_ITEM *pSkinFileItem);

//////////////////////////////////////////////////////////////////////////
	// 保存皮肤包
	bool SaveLayoutXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath);
	bool SaveLayoutXml_ChildCtrl(xml_document &XmlStrObj, xml_node* pNode, PROP_CONTROL_VEC* pWndChildVec);
	bool SaveResourceXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath);
	bool SaveControlsXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath);
	bool SaveXml_GroupProp(xml_document &XmlStrObj, xml_node* pParentNode, IPropertyGroup *pPropGroup);
	bool SaveWindowsXml(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSavePath);

private:
	AREA_TYPE m_AreaType;
	// 所有皮肤文件的map
	SKIN_FILE_MAP m_SkinFileMap;
	// 本地图片使用的zip文件
	SKIN_FILE_ITEM m_LocalImageSkinFileItem;
};
