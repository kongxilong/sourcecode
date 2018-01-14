//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"

// 语言种类
enum AREA_TYPE
{
	AT_NONE	= 0,
	// 中文简体
	AT_CN		= 1,
	// 中文繁体
	AT_CN_TW	= 2,
	// 英文
	AT_EN		= 3,
	// 日文
	AT_JP		= 4,
	// 德文
	AT_GE		= 5,
	AT_LAST		= 6
};

class _declspec(novtable) IPropertyString : public IPropertyBase
{
public:
	IPropertyString(IUiFeatureKernel* pUiKernel);
	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	// 默认显示文字
	virtual WCHAR* GetString(AREA_TYPE areaType = AT_CN) = 0;
	virtual void SetString(const WCHAR* pszString, AREA_TYPE areaType = AT_CN) = 0;
	virtual int GetLength() = 0;

	virtual void SetArea(AREA_TYPE areaType) = 0;
};
