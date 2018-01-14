//////////////////////////////////////////////////////////////////////////
// 当前皮肤所有窗口中所有控件的属性管理类

#pragma once
#include "IPropertyBase.h"
#include "IPropertyWindow.h"
#include "IZipFile.h"
#include "IPropertyFontBase.h"
#include "IPropertyImageBase.h"
#include "IPropertyString.h"

class IControlBase;
class _declspec(novtable) IPropertySkinManager
{
public:
//////////////////////////////////////////////////////////////////////////
	// 皮肤包相关
	// 取得皮肤包版本号
	virtual SKIN_VERSION GetSkinFileVersion(LPCWSTR pZipFilePath, int &nPasswordLen) = 0;
	// 验证用户皮肤包密码
	virtual bool ChekUserPassword(const char *pUserPassword, LPCWSTR pZipFilePath) = 0;

//////////////////////////////////////////////////////////////////////////
	// 清空4个xml文件的属性队列
	virtual void ReleaseSkinManagerPropetry() = 0;
	// 从控件属性队列中删除一个控件，仅仅是删除控件的节点，控件的具体属性并没有删除
	virtual bool DeleteControlPropetryFromCtrlPropMap(IControlBase* pCtrl) = 0;
	// 删除一个控件的所有属性
	virtual bool DeleteControlPropetry(IControlBase* pCtrl) = 0;
	// 删除属性中的Base属性，如ImageBase、FontBase
	virtual bool DeletePropetryBaseProp(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase) = 0;
	// 初始化鼠标手势属性
	virtual bool InitCursorBasePropetry(SKIN_FILE_ITEM* pSkinFileItem) = 0;

	// 加载本地图片，如果这个路径的图片已经加载过了，返回原有的，使用 DeleteLocalImageBase 删除图片
	virtual IPropertyImageBase* LoadLocalImage(LPCWSTR pszLocalImgPath, bool bReLoad = false) = 0;
	// 根据 HBITAMP 创建图片，用 DeleteLocalImageBase 删除图片
	virtual IPropertyImageBase* LoadImageFromHBITMAP(HBITMAP hBmp) = 0;
	// 根据 HICON 创建图片，用 DeleteLocalImageBase 删除图片
	virtual IPropertyImageBase* LoadImageFromHICON(HICON hIcon) = 0;
	// 输入的内存格式为 DIB 32 位的数据。
	virtual IPropertyImageBase* LoadImage(int nWidth, int nHeight, BYTE* pScan0) = 0;

	// 删除本地图片
	virtual bool DeleteLocalImageBase(IPropertyImageBase *pPropImgBase) = 0;
	// 删除图片
	virtual bool DeleteImageBase(SKIN_FILE_ITEM* pSkinFileItem, IPropertyImageBase *pPropImgBase) = 0;

	// 从指定皮肤包文件中找到指定文件
	virtual bool FindUnZipFile(LPCWSTR pszSkinFilePath, LPCWSTR pFileName, BYTE **ppOutBuffer, int *pnFileLen, ZIP_FILE **ppFindZipFile = NULL) = 0;

//////////////////////////////////////////////////////////////////////////
	// 初始化Window皮肤
	virtual IPropertyWindow* PG_InitWindowSkin(LPCWSTR pszSkinPath, LPCWSTR pszWndName) = 0;
	// 设置显示的语言种类
	virtual void SetArea(AREA_TYPE areaType) = 0;
	// 创建一个属性，并将次属性放入队列
	virtual IPropertyBase* CreateEmptyBaseProp(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD *pdwObjectId = NULL) = 0;
	// 取得整个程序默认的字体信息
	virtual IPropertyFontBase* GetDefaultFontBase(SKIN_FILE_ITEM* pSkinFileItem) = 0;

	// 查找指定的属性
	virtual IPropertyBase* FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, DWORD dwObjId) = 0;
	virtual IPropertyBase* FindBasePropertyByPropId(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, DWORD dwObjId) = 0;
	// 查找指定的属性
	virtual IPropertyBase* FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, OBJECT_TYPE_ID propType, LPCWSTR pszPropName) = 0;
	virtual IPropertyBase* FindBasePropertyByPropName(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszPropType, LPCWSTR pszPropName) = 0;

	// 内存优化需要，只在需要用到zip中的图片的时候才加载图片
	virtual bool ReadSkinFileItem(ZIP_FILE* pZipItem) = 0;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	// 取得皮肤文件，参数 bNew：如果当前皮肤包列表中不存在此皮肤文件，是否从本地读取此皮肤文件
	virtual SKIN_FILE_ITEM* GetSkinFileItem(LPCWSTR pszFilePath, bool bNew = false) = 0;
	// 创建一个空的皮肤文件
	virtual SKIN_FILE_ITEM* CreateEmptySkinFileItem(LPCWSTR pszFilePath) = 0;

//////////////////////////////////////////////////////////////////////////
	// 保存控件皮肤到默认文件夹
	virtual bool SaveControlDefaultSkin(LPCWSTR pszSaveDir, LPCWSTR pszCtrlName, IPropertyGroup* pPublicGroup, IPropertyGroup* pPrivateGroup) = 0;
	// 加载默认皮肤
	virtual bool SetControlDefaultSkin(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pControl, LPCWSTR pszDefSkinDir) = 0;
//////////////////////////////////////////////////////////////////////////

	// 保存皮肤包
	virtual bool BD_SaveSkinFile(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pszSkinDir, LPCWSTR pszSkinName, char *pszPassword) = 0;

	// 修改属性名称
	virtual bool BD_ModifyPropetryName(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pPropBase, LPCWSTR pszNewPropName) = 0;
	// 删除一张图片
	virtual bool BD_DeleteZipImage(SKIN_FILE_ITEM* pSkinFileItem, ZIP_FILE* pZipImage) = 0;

	// 取得指定组的属性
	virtual IPropertyBaseMap* GetIPropertyBaseMap(SKIN_FILE_ITEM *pSkinFileItem, LPCWSTR pPropGroupName, bool bIsCreate = false) = 0;
	
	// 本地图片使用的zip文件
	virtual SKIN_FILE_ITEM * GetLocalImageSkinFileItem() = 0;
	// 释放一个属性的内存空间
	virtual void ReleaseIPropertyBase(SKIN_FILE_ITEM* pSkinFileItem, IPropertyBase *pCtrlProp) = 0;

//////////////////////////////////////////////////////////////////////////
	// 内存管理
	virtual ZIP_FILE* MEM_CreateZipFile(SKIN_FILE_ITEM* pSkinFileItem, int nFileBufLen, BYTE byFileType, LPCWSTR pszFileName) = 0;
	virtual void MEM_DeleteZipFileByCom(SKIN_FILE_ITEM* pSkinFileItem, ZIP_FILE *pComZipFile) = 0;
};
