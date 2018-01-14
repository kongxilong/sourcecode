
#include "StdAfx.h"
#include "IPropertyWindowImpl.h"
#include "..\..\Inc\IPropertyString.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyWindowImpl::IPropertyWindowImpl(IUiFeatureKernel* pUiKernel) : IPropertyWindow(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_WINDOW_NAME));
	m_ChildCtrlPropVec.clear();
	m_pWindowProp = NULL;
}

IPropertyWindowImpl::~IPropertyWindowImpl()
{
}

void IPropertyWindowImpl::AppendChildCtrlProp(IPropertyControl *pPropCtrl)
{
	if (pPropCtrl == NULL)
		return;

	m_ChildCtrlPropVec.push_back(pPropCtrl);
}

PROP_CONTROL_VEC* IPropertyWindowImpl::GetChildPropControlVec()
{
	return &m_ChildCtrlPropVec;
}

void IPropertyWindowImpl::SetWindowPropGroup(IPropertyGroup *pWindowProp)
{
	if (pWindowProp == NULL)
		return;

	SetObjectId(pWindowProp->GetObjectId());

	m_pWindowProp = pWindowProp;
	IPropertyString* pNameProp = FindObjectNameProperty(m_pWindowProp);
	if (pNameProp == NULL)
		return;

	SetObjectName(pNameProp->GetString());
}

IPropertyGroup* IPropertyWindowImpl::GetWindowPropGroup()
{
	return m_pWindowProp;
}

void IPropertyWindowImpl::ClearPropertyOwner()
{
	ClearGroupPropertyOwner(m_pWindowProp);
}

// 从另一个属性克隆
bool IPropertyWindowImpl::Clone(IPropertyBase* pFromPropBase)
{
	return false;
}
