//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"

// Group 中的属性列表
typedef CTList				GROUP_PROP_VEC;

class _declspec(novtable) IPropertyGroup : public IPropertyBase
{
public:
	IPropertyGroup(IUiFeatureKernel* pUiKernel);
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	// 追加一个属性
	virtual void AppendProperty(IPropertyBase* pProperty) = 0;

	// 取得Group下的属性列表
	virtual GROUP_PROP_VEC* GetPropVec() = 0;
};
