//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "..\..\Inc\IPropertyComboBox.h"


class IPropertyComboBoxImpl : public IPropertyComboBox
{
public:
	IPropertyComboBoxImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyComboBoxImpl();

	virtual void Clear();
	// 是否为合法数据
	virtual bool IsRightData();
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode);
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode);

	// 设置选中项
	virtual void SetSelect(int nSel);
	virtual void SetSelectString(LPCWSTR pszSelString);
	virtual int GetSelect();
	virtual int GetDataCounts();
	// 向列表里添加一个数据
	virtual bool AppendString(LPCWSTR pszString);
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

protected:
	virtual COMBOBOX_PROP* GetComboBoxData();

private:
	COMBOBOX_PROP m_ComboBoxPro;
};
