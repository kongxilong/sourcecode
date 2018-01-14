//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyInt.h"

// int 在Builder中的值范围：-(0x7FFFFFFF), 0x7FFFFFFF
class IPropertyIntImpl : public IPropertyInt
{
public:
	IPropertyIntImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyIntImpl();

	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	virtual void SetValue(int nValue);
	virtual int GetValue();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	int m_nIntData;
};
