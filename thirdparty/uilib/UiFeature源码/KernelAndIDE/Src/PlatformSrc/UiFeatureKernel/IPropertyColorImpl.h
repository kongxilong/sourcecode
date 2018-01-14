//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyColor.h"

class IPropertyColorImpl : public IPropertyColor
{
public:
	IPropertyColorImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyColorImpl();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

};
