//////////////////////////////////////////////////////////////////////////
// 一个窗口在xml中的读取的属性
// 存储 Layout.xml 中 window 节点下所有属性的队列

#pragma once
#include "..\..\Inc\IPropertyWindow.h"
#include "..\..\Inc\IPropertyControl.h"


class IPropertyWindowImpl : public IPropertyWindow
{
public:
	IPropertyWindowImpl(IUiFeatureKernel* pUiKernel);
	virtual ~IPropertyWindowImpl();

	virtual PROP_CONTROL_VEC* GetChildPropControlVec();
	virtual void SetWindowPropGroup(IPropertyGroup *pWindowProp);
	virtual IPropertyGroup* GetWindowPropGroup();
	virtual void AppendChildCtrlProp(IPropertyControl *pPropCtrl);

	// 清空属性的隶属于哪个窗口的信息
	virtual void ClearPropertyOwner();
	// 从另一个属性克隆
	virtual bool Clone(IPropertyBase* pFromPropBase);

private:
	IPropertyGroup *m_pWindowProp;
	// 子控件属性节点 的列表
	PROP_CONTROL_VEC m_ChildCtrlPropVec;
};
