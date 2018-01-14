//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyBool.h"

class IPropertyBoolImpl : public IPropertyBool
{
public:
	IPropertyBoolImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyBoolImpl();

	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	virtual void SetValue(bool bBoolData);
	virtual bool GetValue();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	bool m_bBoolData;
};
