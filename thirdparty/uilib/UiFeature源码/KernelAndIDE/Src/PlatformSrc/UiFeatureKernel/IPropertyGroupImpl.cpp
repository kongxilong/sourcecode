
#include "stdafx.h"
#include "IPropertyGroupImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyGroupImpl::IPropertyGroupImpl(IUiFeatureKernel* pUiKernel) : IPropertyGroup(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_GROUP_NAME));
}

IPropertyGroupImpl::~IPropertyGroupImpl()
{

}

// 取得Group下的属性列表
GROUP_PROP_VEC* IPropertyGroupImpl::GetPropVec()
{
	return &m_PropVec;
}

bool IPropertyGroupImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	if (psz_id == NULL || psz_name == NULL)
		return false;

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);

	return true;
}

// 追加一个属性
void IPropertyGroupImpl::AppendProperty(IPropertyBase* pProperty)
{
	if (pProperty == NULL)
		return;

	m_PropVec.PushBackObj(pProperty);
}

// 写入xml
bool IPropertyGroupImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	// name 字段为空的group不写入属性表
	if (GetObjectName() == NULL || wcslen(GetObjectName()) <= 0)
		return true;

	if (pParentXmlNode == NULL)
		return false;

	xml_node PropNode = pParentXmlNode->append_child(L"item");
	if (!PropNode)
		return false;

	AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)GetObjectId());
	AddStringAttrToNode(&PropNode, L"name", GetObjectName());

	return true;
}

// 从另一个属性克隆
bool IPropertyGroupImpl::Clone(IPropertyBase* pFromPropBase)
{
	return false;
}
