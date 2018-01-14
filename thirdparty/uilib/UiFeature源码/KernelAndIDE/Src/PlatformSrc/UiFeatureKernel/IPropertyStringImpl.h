//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyString.h"
#include <atlstr.h>
using namespace ATL;

typedef map<AREA_TYPE, WCHAR*>		STRING_MAP;
// String属性
struct STRING_PROP
{
	// 属性名称
	WCHAR* pstrDefaultString;
	AREA_TYPE nDefaultArea;
	STRING_MAP StringMap;
};

class IPropertyStringImpl : public IPropertyString
{
public:
	IPropertyStringImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyStringImpl();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	// 默认显示文字
	virtual WCHAR* GetString(AREA_TYPE areaType = AT_CN);
	virtual void SetString(const WCHAR* pszString, AREA_TYPE areaType = AT_CN);
	virtual int GetLength();

	virtual void SetArea(AREA_TYPE areaType);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	void Clear();
	void ResetDefaultString();

private:
	STRING_PROP m_StringProp;
};
