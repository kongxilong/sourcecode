//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"
#include "IPropertyCursorBase.h"

class IWindowBase;
class _declspec(novtable) IPropertyCursor : public IPropertyBase
{
public:
	IPropertyCursor(IUiFeatureKernel* pUiKernel);
	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	virtual const WCHAR * GetCursorBaseName() = 0;

	// 设置鼠标信息
	virtual void SetCursorBaseProp(IPropertyCursorBase *pCursorProp) = 0;
	virtual IPropertyCursorBase* GetCursorBaseProp() = 0;
};
