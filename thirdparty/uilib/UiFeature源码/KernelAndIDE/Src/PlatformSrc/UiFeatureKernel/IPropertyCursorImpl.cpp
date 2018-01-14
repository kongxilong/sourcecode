
#include "stdafx.h"
#include "IPropertyCursorImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IWindowBase.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyCursorImpl::IPropertyCursorImpl(IUiFeatureKernel *pUiKernel) : IPropertyCursor(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_CURSOR_NAME));
}

IPropertyCursorImpl::~IPropertyCursorImpl()
{

}

const WCHAR * IPropertyCursorImpl::GetCursorBaseName()
{
	return GetRelevancyPropName();
}

bool IPropertyCursorImpl::IsRightData()
{
	return (GetRelevancyProp() != NULL);
}

bool IPropertyCursorImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	const WCHAR * psz_cursorbasename = XmlGetAttrValue(pXmlNode, L"cursorbasename");
	if (psz_id == NULL || psz_name == NULL || psz_cursorbasename == NULL)
		return false;

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);
	SetRelevancyPropName((WCHAR*)psz_cursorbasename);
	return true;
}

// 写入xml
bool IPropertyCursorImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	if (pParentXmlNode == NULL)
		return false;

	// 如果没有数据，就不存入xml
	if (GetRelevancyPropName() == NULL || wcslen(GetRelevancyPropName()) <= 0)
		return true;

	xml_node PropNode = pParentXmlNode->append_child(L"item");
	if (!PropNode)
		return false;

	AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)GetObjectId());
	AddStringAttrToNode(&PropNode, L"name", GetObjectName());
	AddStringAttrToNode(&PropNode, L"cursorbasename", GetRelevancyPropName());
	return true;
}

// 设置鼠标信息
void IPropertyCursorImpl::SetCursorBaseProp(IPropertyCursorBase *pCursorProp)
{
	if (pCursorProp == NULL)
	{
		SetRelevancyProp(NULL);
		return;
	}

	IPropertyBase* pPropBase = dynamic_cast<IPropertyBase*>(pCursorProp);
	SetRelevancyProp(pPropBase);
}

IPropertyCursorBase* IPropertyCursorImpl::GetCursorBaseProp()
{
	return dynamic_cast<IPropertyCursorBase*>(GetRelevancyProp());
}

// 从另一个属性克隆
bool IPropertyCursorImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyCursorImpl* pFromProp = dynamic_cast<IPropertyCursorImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

	SetRelevancyProp(pFromPropBase->GetRelevancyProp());
	return true;
}
