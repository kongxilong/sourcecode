
#include "stdafx.h"
#include "IPropertyIntImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyIntImpl::IPropertyIntImpl(IUiFeatureKernel* pUiKernel) : IPropertyInt(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_INT_NAME));
	m_nIntData = 0;
}

IPropertyIntImpl::~IPropertyIntImpl()
{

}

bool IPropertyIntImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	const WCHAR * psz_data = XmlGetAttrValue(pXmlNode, L"data");
	if (psz_id == NULL || psz_name == NULL || psz_data == NULL)
		return false;

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);

	m_nIntData = _wtoi(psz_data);
	return true;
}

void IPropertyIntImpl::SetValue(int nValue)
{
	m_nIntData = nValue;
}

int IPropertyIntImpl::GetValue()
{
	return m_nIntData;
}

// 写入xml
bool IPropertyIntImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	if (pParentXmlNode == NULL)
		return false;

	xml_node PropNode = pParentXmlNode->append_child(L"item");
	if (!PropNode)
		return false;

	AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)GetObjectId());
	AddStringAttrToNode(&PropNode, L"name", GetObjectName());

	AddIntAttrToNode(&PropNode, L"data", m_nIntData);

	return true;
}

// 从另一个属性克隆
bool IPropertyIntImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyIntImpl* pFromProp = dynamic_cast<IPropertyIntImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

	m_nIntData = pFromProp->m_nIntData;
	return true;
}
