
#pragma once

#define PUGIXML_WCHAR_MODE
#include "..\..\Inc\PugiXml\pugixml.hpp"
using namespace pugi;

#include "..\..\Inc\IPropertySkinManager.h"
#include "IPropertyWindowImpl.h"
#include "IZipFileImpl.h"
#include "..\..\Inc\IPropertyGroup.h"
#include "..\..\Inc\IWindowBase.h"
#include "SkinFileManager.h"

class CLocalImageList;
//////////////////////////////////////////////////////////////////////////
// 资源属性队列

// 本地图片map
typedef map<CStringW, IPropertyImageBase*>	LOCAL_IMAGE_MAP;

class IPropertySkinManagerImpl : public IPropertySkinManager
{
	friend class CUiFeatureSkinMergerDlg;

public:
	IPropertySkinManagerImpl();
	virtual ~IPropertySkinManagerImpl();

	// 取得皮肤包版本号
	virtual SKIN_VERSION GetSkinFileVersion(LPCWSTR pZipFilePath, int &nPasswordLen);
	// 验证用户皮肤包密码
	virtual bool ChekUserPassword(const char *pUserPassword, LPCWSTR pZipFilePath);

	// 初始化鼠标手势属性
	virtual bool InitCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem);
	// 加载本地图片
	virtual IPropertyImageBase* LoadLocalImage(LPCWSTR pszLocalImgPath, bool bReLoad = false);
	virtual IPropertyImageBase* LoadImageFromHBITMAP(HBITMAP hBmp);
	virtual IPropertyImageBase* LoadImageFromHICON(HICON hIcon);
	// 输入的内存格式为 DIB 32 位的数据。
	virtual IPropertyImageBase* LoadImage(int nWidth, int nHeight, BYTE* pScan0);

	// 删除本地图片
	virtual bool DeleteLocalImageBase(IPropertyImageBase *pPropImgBase);
	// 删除图片
	virtual bool DeleteImageBase(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pPropImgBase);

	// 取得指定组的属性
	virtual IPropertyBaseMap* GetIPropertyBaseMap(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pPropGroupName, bool bIsCreate = false);

	// 清空属性队列
	virtual void ReleaseSkinManagerPropetry();
	// 删除一个控件的所有属性
	virtual bool DeleteControlPropetry(IControlBase* pCtrl);
	// 删除属性
	virtual bool DeletePropetryBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase);
	// 从控件属性队列中删除一个控件，仅仅是删除控件的节点，控件的具体属性并没有删除
	virtual bool DeleteControlPropetryFromCtrlPropMap(IControlBase* pCtrl);

	// 从指定皮肤包文件中找到指定文件
	virtual bool FindUnZipFile(LPCWSTR pszSkinFilePath, LPCWSTR pFileName, BYTE **ppOutBuffer, int *pnFileLen, ZIP_FILE **ppFindZipFile = NULL);

//////////////////////////////////////////////////////////////////////////
	// 初始化Window皮肤
	virtual IPropertyWindow* PG_InitWindowSkin(LPCWSTR pszSkinPath, LPCWSTR pszWndName);
	// 查找指定的属性
	virtual IPropertyBase* FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD dwObjId);
	virtual IPropertyBase* FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, DWORD dwObjId);
	// 查找指定的属性
	virtual IPropertyBase* FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, LPCWSTR pszPropName);
	virtual IPropertyBase* FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, LPCWSTR pszPropName);
	// 设置显示的语言种类
	virtual void SetArea(AREA_TYPE areaType);
	// 创建一个属性，并将次属性放入队列
	virtual IPropertyBase* CreateEmptyBaseProp(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD *pdwObjectId = NULL);

	// 删除一个属性
	void ReleaseBaseProp(IPropertyBase *pCtrlProp);

	// 取得整个程序默认的字体信息
	virtual IPropertyFontBase* GetDefaultFontBase(SKIN_FILE_ITEM* pSkinFileItem);

	// 保存皮肤包
	virtual bool BD_SaveSkinFile(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSkinDir, LPCWSTR pszSkinName, char *pszPassword);
	// 向控件队列添加一个控件
	virtual bool BD_AppendControlToVec(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszCtrlType, IPropertyGroup* pCtrlPropGroup);
	// 设置可以保存的有效属性
	virtual void BD_SetWindowPropetryActiveProp(IWindowBase *pWndBase, bool bActive);
	void BD_SetChildVecActiveMark(CHILD_CTRLS_VEC* pChildCtrlVec, bool bActive);
	void BD_SetGroupPropActiveMark(IPropertyGroup *pPropGroup, bool bActive);
	// 删除一张图片
	virtual bool BD_DeleteZipImage(SKIN_FILE_ITEM* pSkinFileItem, ZIP_FILE* pZipImage);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	// 取得皮肤文件
	virtual SKIN_FILE_ITEM* GetSkinFileItem(LPCWSTR pszFilePath, bool bNew = false);
	// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
	virtual bool ReadSkinFileItem(ZIP_FILE* pZipItem);
	// 创建一个空的皮肤文件
	virtual SKIN_FILE_ITEM* CreateEmptySkinFileItem(LPCWSTR pszFilePath);
	// 释放一个属性的内存空间
	virtual void ReleaseIPropertyBase(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pCtrlProp);

//////////////////////////////////////////////////////////////////////////
	// 保存控件皮肤到默认文件夹
	virtual bool SaveControlDefaultSkin(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup);
	// 加载默认皮肤
	virtual bool SetControlDefaultSkin(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pControl, LPCWSTR pszDefSkinDir);
//////////////////////////////////////////////////////////////////////////

protected:
	//////////////////////////////////////////////////////////////////////////
	// 打开一个皮肤包时，需要读取皮肤包的文件
	// 修改属性名称
	virtual bool BD_ModifyPropetryName(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase, LPCWSTR pszNewPropName);

	// 本地图片使用的zip文件
	virtual SKIN_FILE_ITEM * GetLocalImageSkinFileItem();

//////////////////////////////////////////////////////////////////////////
	// 内存管理
	virtual ZIP_FILE* MEM_CreateZipFile(SKIN_FILE_ITEM* pSkinFileItem, int nFileBufLen, BYTE byFileType, LPCWSTR pszFileName);
	virtual void MEM_DeleteZipFileByCom(SKIN_FILE_ITEM* pSkinFileItem, ZIP_FILE *pComZipFile);

private:
//////////////////////////////////////////////////////////////////////////
	// 查找光标
	IPropertyCursorBase* FindCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem, int nCursorId);
	// 增加一个鼠标收拾
	bool AddCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem, bool bSysCursor, int nCursorId);
	// 设置光标属性的名称
	void SetCursorObjectName(IPropertyCursorBase *pCursorBase);

//////////////////////////////////////////////////////////////////////////
	// Layout.xml 相关

	bool DeleteImageBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pImgBaseProp);
	bool DeleteFontBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyFontBase *pFontBaseProp);

	IPropertyImageBase* LoadImageFromHANDLE(HBITMAP hBmp, HICON hIcon);

	bool SaveControlDefaultSkin_ControlXml(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup);
	bool SaveControlDefaultSkin_ResourceXml(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup);
	bool SaveCtrlGroupProp_ResXml(xml_document &XmlStrObj, xml_node* pParentNode, IPropertyGroup *pPropGroup, LPCWSTR pszSaveDir);
	bool SaveCtrlGroupProp_CtrlXml(xml_document &XmlStrObj, xml_node* pParentNode, IPropertyGroup *pPropGroup);


	// 解析控件自定义皮肤的Resource.xml
	bool ReadCtrlDftSkinXml(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pControl, LPCWSTR pszDefSkinDir, SKIN_FILE_ITEM &DftSkinFileItem);
	bool SetControlDefaultSkinToCtrl(SKIN_FILE_ITEM &DftSkinFileItem, SKIN_FILE_ITEM* pCtrlSkinFileItem, IPropertyGroup* pCtrlPrivateGroup, IControlBase *pControl, LPCWSTR pszDefSkinDir);
	bool CopyPropetry(SKIN_FILE_ITEM &DftSkinFileItem, SKIN_FILE_ITEM* pCtrlSkinFileItem, IPropertyGroup* pFromGroup, IPropertyGroup* pCopyToGroup, IControlBase *pControl, LPCWSTR pszDefSkinDir);

private:
	// 本地图片的map
	LOCAL_IMAGE_MAP m_LocalImgMap;

	// 所有皮肤包文件管理器
	CSkinFileManager m_SkinFile;
};
