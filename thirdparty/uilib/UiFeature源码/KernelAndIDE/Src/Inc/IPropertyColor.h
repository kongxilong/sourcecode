//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"

class _declspec(novtable) IPropertyColor : public IPropertyBase
{
public:
	IPropertyColor(IUiFeatureKernel* pUiKernel);
	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
};
