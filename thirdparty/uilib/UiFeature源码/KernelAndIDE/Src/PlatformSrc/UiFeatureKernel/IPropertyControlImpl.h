//////////////////////////////////////////////////////////////////////////
// 一个控件在xml中的读取的属性
// 存储 Layout.xml 中 ctrl 节点下所有属性的队列

#pragma once
#include "..\..\Inc\IPropertyControl.h"


class IPropertyControlImpl : public IPropertyControl
{
public:
	IPropertyControlImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyControlImpl();

	virtual IPropertyControl* GetParentCtrlProp();
	virtual void SetParentCtrlProp(IPropertyControl* pCtrlProp);
	
	virtual void AppendChildCtrlProp(IPropertyControl* pCtrlProp);

	virtual void SetCtrlGroupProp(IPropertyGroup *pBasePropGroup);
	virtual IPropertyGroup *GetControlPropGroup();

	virtual const WCHAR * GetControlType();
	virtual void SetControlType(WCHAR *pszCtrlType);

	virtual PROP_CONTROL_VEC* GetChildPropControlVec();

	// 清空属性的隶属于哪个控件的信息
	virtual void ClearPropertyOwner();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	// 子控件属性节点 的列表
	PROP_CONTROL_VEC m_ChildCtrlPropVec;
	// 父控件属性节点
	IPropertyControl* m_pPrentCtrlProp;
	// 自身控件属性列表
	IPropertyGroup *m_pCtrlPropGroup;
	// 控件类型名称
	WCHAR m_strControlType[MAX_PATH + 1];
};
