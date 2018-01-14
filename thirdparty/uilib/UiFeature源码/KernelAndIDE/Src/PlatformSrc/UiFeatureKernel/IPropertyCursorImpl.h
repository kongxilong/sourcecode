//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyCursor.h"
#include "..\..\Inc\IPropertyCursorBase.h"

class IWindowBase;
class IPropertyCursorImpl : public IPropertyCursor
{
public:
	IPropertyCursorImpl(IUiFeatureKernel *pUiKernel);
	virtual ~IPropertyCursorImpl();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	virtual const WCHAR * GetCursorBaseName();

	// 设置鼠标信息
	virtual void SetCursorBaseProp(IPropertyCursorBase *pCursorProp);
	virtual IPropertyCursorBase* GetCursorBaseProp();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);
};
