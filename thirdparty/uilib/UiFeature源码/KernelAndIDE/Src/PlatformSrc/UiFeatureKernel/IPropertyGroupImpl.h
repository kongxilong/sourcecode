//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyGroup.h"

class IPropertyGroupImpl : public IPropertyGroup
{
public:
	IPropertyGroupImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyGroupImpl();

	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	// 追加一个属性
	virtual void AppendProperty(IPropertyBase* pProperty);
	// 取得Group下的属性列表
	virtual GROUP_PROP_VEC* GetPropVec();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	GROUP_PROP_VEC m_PropVec;
};
