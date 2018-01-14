
#pragma once
#include "IFeatureObject.h"
#include "IPropertySkinManager.h"
#include "IPropertyImageBase.h"
#include "IPropertyBase.h"
#include "IWindowBase.h"

// 属性、对象名称最大长度
#ifndef STATIC_LEN
#define STATIC_LEN							(33)
#endif

// 在output窗口输出debug信息，请使用宏 DEBUG_INFO 来输出
void DebugInfoOutput(char *pszFormat, ...);
#ifndef _DEBUG
#define DEBUG_INFO
#else
#define DEBUG_INFO				DebugInfoOutput
#endif

// 根据当前运行的exe的绝对目录，加上参数目录，得到的完整路径
const char* PathHelper(char *pszFileName);
const WCHAR* PathHelperW(LPCWSTR pszFileName);
// 根据属性类型字符串得到属性类型的ID值
OBJECT_TYPE_ID PropStringToType(const WCHAR* strPropType);
// 根据属性类型ID值得到属性类型字符串
const WCHAR* PropTypeToString(OBJECT_TYPE_ID propType);
// 创建一个属性，如果这个属性在 group 中已经有了，就返回这个属性，如果没有就新建一个属性
IPropertyBase* CreateResourcePropetry(SKIN_FILE_ITEM* pSkinFileItem, IFeatureObject *pOwnerObj, IPropertySkinManager* pSkinPropMgr,
									  IPropertyGroup* pGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo, bool bNotExistCreate);
// 判断指定文件是否存在
bool FileExists(const char *pszFilePath);
bool FileExistsW(LPCWSTR pszFilePath);
// 向一个xml节点插入一个int属性
void AddIntAttrToNode(xml_node* pNode, const WCHAR* pszAttrName, int nInt);
void AddStringAttrToNode(xml_node* pNode, const WCHAR* pszAttrName, const WCHAR* pszData);
// 从一个属性组中查找名为 "@Object_Name" 的属性
IPropertyString* FindObjectNameProperty(IPropertyGroup *pGroupProp);

void RectScreenToClient(HWND hWnd, RECT& rct);
void RectClientToScreen(HWND hWnd, RECT& rct);
void InitPropImageBase(IMAGE_BASE_PROP *pImgBaseProp);

// 清空属性的隶属于哪个控件/窗口的信息
void ClearGroupPropertyOwner(IPropertyGroup *pPropGroup);

// 图片抠边，MarkDrawBoard 图片中黑色部分将会从 DstDrawBoard 中抠出
bool ClipImageWithBlack(CDrawingBoard &MarkDrawBoard, CDrawingBoard &DstDrawBoard);
// 将原图中的 Alpha 值复制到目标位图中的 Alpha 值
bool ClipImageWithAlpha(CDrawingBoard &MarkDrawBoard, CDrawingBoard &DstDrawBoard);
// 参数：nPixelCtns 总共的像素值
bool ClipImageWithAlpha(BYTE* pBitsSrc, BYTE* pBitsDes, int nPixelCtns);
// 读取普通的Text的小文件
bool ReadTextFile(LPCWSTR pszFilePath, string &strOutData);
// 是否为同一类型的属性
bool IsSameTypePropetry(IPropertyBase* pPropBase, IPropertyBase* pComPropBase);
// 从xml中取得指定node的指定attr
const WCHAR * XmlGetAttrValue(xml_node* pXmlNode, const WCHAR* pwszAttrName);
// 保存数据到文件
bool SaveDataToFile(LPCWSTR pszFilePath, BYTE *pData, int nDataLen);
// 取得 GUID
const WCHAR * GetGuidString();
// 取得控件指针
IControlBase* GetSubControlByName(char *pszCtrlName, CHILD_CTRLS_VEC &ChildCtrlsVec);
IControlBase* GetSubControlByName(LPCWSTR pszCtrlName, CHILD_CTRLS_VEC &ChildCtrlsVec);
IControlBase* GetSubControlByName(WCHAR *pszCtrlName, CHILD_CTRLS_VEC &ChildCtrlsVec);
bool GetControlByNameFromVec(CHILD_CTRLS_VEC *pCtrlVec, LPCWSTR pszCtrlName, IControlBase **ppCtrl);
// 取得控件指针
IControlBase* GetSubControlByObjectId(DWORD dwObjId, CHILD_CTRLS_VEC &ChildCtrlsVec);
bool GetControlByObjectIdFromVec(CHILD_CTRLS_VEC *pCtrlVec, DWORD dwObjId, IControlBase **ppCtrl);
