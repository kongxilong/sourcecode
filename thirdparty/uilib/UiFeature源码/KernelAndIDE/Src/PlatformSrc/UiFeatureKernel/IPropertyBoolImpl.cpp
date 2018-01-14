
#include "stdafx.h"
#include "IPropertyBoolImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyBoolImpl::IPropertyBoolImpl(IUiFeatureKernel* pUiKernel) : IPropertyBool(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_BOOL_NAME));
	m_bBoolData = false;
}

IPropertyBoolImpl::~IPropertyBoolImpl()
{

}

bool IPropertyBoolImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
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

	m_bBoolData = (_wtoi(psz_data) != 0);
	return true;
}

void IPropertyBoolImpl::SetValue(bool bBoolData)
{
	m_bBoolData = bBoolData;
}

bool IPropertyBoolImpl::GetValue()
{
	return m_bBoolData;
}

// 写入xml
bool IPropertyBoolImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
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
	AddStringAttrToNode(&PropNode, L"data", (m_bBoolData ? L"1" : L"0"));
	return true;
}

// 从另一个属性克隆
bool IPropertyBoolImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyBoolImpl* pFromProp = dynamic_cast<IPropertyBoolImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

	m_bBoolData = pFromProp->m_bBoolData;
	return true;
}
