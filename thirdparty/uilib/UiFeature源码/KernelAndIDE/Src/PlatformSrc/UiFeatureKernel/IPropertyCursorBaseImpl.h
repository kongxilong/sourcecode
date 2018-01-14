//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyCursorBase.h"

class IWindowBase;
class IPropertyCursorBaseImpl : public IPropertyCursorBase
{
public:
	IPropertyCursorBaseImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyCursorBaseImpl();

	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	// 设置系统光标
	virtual void SetSystemCursor(bool bIsSysCursor);
	// 是否为系统光标
	virtual bool IsSystemCursor();

	// 设置光标ID
	virtual void SetCursorId(int nId);
	// 得到光标id
	virtual int GetCursorId();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	// 是否是系统默认光标
	bool m_bIsSysCursor;
	// 光标ID
	int m_nCursorId;
};
