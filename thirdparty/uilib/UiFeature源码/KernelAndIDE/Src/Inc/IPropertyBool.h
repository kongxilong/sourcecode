//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"

class _declspec(novtable) IPropertyBool : public IPropertyBase
{
public:
	IPropertyBool(IUiFeatureKernel* pUiKernel);
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	virtual void SetValue(bool bBoolData) = 0;
	virtual bool GetValue() = 0;
};
