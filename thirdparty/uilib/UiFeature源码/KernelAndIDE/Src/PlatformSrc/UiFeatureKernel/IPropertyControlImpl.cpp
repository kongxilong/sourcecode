
#include "StdAfx.h"
#include "IPropertyControlImpl.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyControlImpl::IPropertyControlImpl(IUiFeatureKernel* pUiKernel) : IPropertyControl(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_CONTROL_NAME));
	m_ChildCtrlPropVec.clear();
	m_pPrentCtrlProp = NULL;
	m_pCtrlPropGroup = NULL;
	memset(m_strControlType, 0, sizeof(m_strControlType));
}

IPropertyControlImpl::~IPropertyControlImpl()
{
}

const WCHAR * IPropertyControlImpl::GetControlType()
{
	return (const WCHAR *)m_strControlType;
}

void IPropertyControlImpl::SetControlType(WCHAR *pszCtrlType)
{
	if (pszCtrlType == NULL)
		return;

	swprintf_s(m_strControlType, MAX_PATH, L"%s", pszCtrlType);
}

IPropertyControl* IPropertyControlImpl::GetParentCtrlProp()
{
	return m_pPrentCtrlProp;
}

void IPropertyControlImpl::SetParentCtrlProp(IPropertyControl* pCtrlProp)
{
	if (pCtrlProp != NULL)
		m_pPrentCtrlProp = pCtrlProp;
}

void IPropertyControlImpl::AppendChildCtrlProp(IPropertyControl* pCtrlProp)
{
	if (pCtrlProp != NULL)
		m_ChildCtrlPropVec.push_back(pCtrlProp);
}

void IPropertyControlImpl::SetCtrlGroupProp(IPropertyGroup *pBasePropGroup)
{
	if (pBasePropGroup != NULL)
		m_pCtrlPropGroup = pBasePropGroup;
}

IPropertyGroup* IPropertyControlImpl::GetControlPropGroup()
{
	return m_pCtrlPropGroup;
}

PROP_CONTROL_VEC* IPropertyControlImpl::GetChildPropControlVec()
{
	return &m_ChildCtrlPropVec;
}

// 清空属性的隶属于哪个控件的信息
void IPropertyControlImpl::ClearPropertyOwner()
{
	ClearGroupPropertyOwner(m_pCtrlPropGroup);
}

// 从另一个属性克隆
bool IPropertyControlImpl::Clone(IPropertyBase* pFromPropBase)
{
	return false;
}
