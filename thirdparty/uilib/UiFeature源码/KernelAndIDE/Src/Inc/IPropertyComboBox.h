//////////////////////////////////////////////////////////////////////////
// 属性

#pragma once
#include "IPropertyBase.h"

class CPropertyViewCtrl;

// string 队列
typedef vector<CStringW>									STRING_VEC;
// ComboBox属性
struct COMBOBOX_PROP
{
	int nSelect;
	STRING_VEC DataVec;
};

class _declspec(novtable) IPropertyComboBox : public IPropertyBase
{
	friend class CPropertyViewCtrl;

public:
	IPropertyComboBox(IUiFeatureKernel* pUiKernel);
	virtual void Clear() = 0;
	// 是否为合法数据
	virtual bool IsRightData() = 0;
	// 从XML节点读取属性值，并放入属性队列
	virtual bool ReadPropertyFromXmlNode(xml_node* pXmlNode) = 0;
	// 写入xml
	virtual bool AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode) = 0;

	// 设置选中项
	virtual void SetSelectString(LPCWSTR pszSelString) = 0;
	virtual void SetSelect(int nSel) = 0;
	virtual int GetSelect() = 0;
	virtual int GetDataCounts() = 0;
	// 向列表里添加一个数据
	virtual bool AppendString(LPCWSTR pszString) = 0;

protected:
	virtual COMBOBOX_PROP* GetComboBoxData() = 0;
};
