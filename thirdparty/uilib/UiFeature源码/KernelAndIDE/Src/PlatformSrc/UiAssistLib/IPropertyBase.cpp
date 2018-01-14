
#include "stdafx.h"
#include "..\..\Inc\IPropertyBase.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IUiFeatureKernel.h"

IPropertyBase::IPropertyBase(IUiFeatureKernel *pUiKernel) : IFeatureObject(pUiKernel)
{
	SetObjectType(L"PropertyBase");

	// 关联属性名称
	m_pszRelevancyPropName = NULL;
	// 关联属性
	m_pRelevancyProp = NULL;
	m_pBuilderCtrl = NULL;
	m_bIsEnableInBuilder = true;

	m_pszObjInfo = NULL;
}

IPropertyBase::~IPropertyBase()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->KernelFree((BYTE**)&m_pszObjInfo);

	m_pszObjInfo = NULL;
}

bool IPropertyBase::IsRightData()
{
	return (wcslen(this->GetObjectName()) > 0);
}

// 从XML节点读取属性值，并放入属性队列
bool IPropertyBase::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	return true;
}

const WCHAR* IPropertyBase::GetRelevancyPropName()
{
	return (const WCHAR*)m_pszRelevancyPropName;
}

void IPropertyBase::SetRelevancyPropName(WCHAR *pszPropName)
{
	if (m_pUiKernel == NULL)
		return;

	m_pUiKernel->KernelFree((BYTE**)&m_pszRelevancyPropName);

	if (pszPropName == NULL || wcslen(pszPropName) <= 0)
	{
		m_pRelevancyProp = NULL;
		return;
	}

	int nLen = (wcslen(pszPropName) + 1) * sizeof(WCHAR);

	m_pszRelevancyPropName = (WCHAR*)m_pUiKernel->KernelMalloc(nLen);
	if (m_pszRelevancyPropName == NULL)
		return;

	memset(m_pszRelevancyPropName, 0, nLen);
	swprintf_s(m_pszRelevancyPropName, wcslen(pszPropName) + 1, L"%s", pszPropName);

}

IPropertyBase* IPropertyBase::GetRelevancyProp()
{
	return m_pRelevancyProp;
}

void IPropertyBase::SetRelevancyProp(IPropertyBase *pszProp)
{
	m_pRelevancyProp = NULL;
	if (m_pUiKernel == NULL)
		return;

	m_pUiKernel->KernelFree((BYTE**)&m_pszRelevancyPropName);

	if (pszProp != NULL)
	{
		SetRelevancyPropName((WCHAR*)pszProp->GetObjectName());
		m_pRelevancyProp = pszProp;
	}
}

bool IPropertyBase::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	return true;
}

// 在Builder中禁用
void IPropertyBase::EnableInBuilder(bool bEnable)
{
	m_bIsEnableInBuilder = bEnable;
	if (m_pBuilderCtrl != NULL)
		m_pBuilderCtrl->EnableBuilderCtrl(bEnable);
}

void IPropertyBase::SetPropBuilderCtrl(IPropertyBuilderCtrl* pBuilderCtrl)
{
	m_pBuilderCtrl = pBuilderCtrl;
}

bool IPropertyBase::IsEnableInBuilder()
{
	return m_bIsEnableInBuilder;
}

const char* IPropertyBase::GetObjectInfo()
{
	return (const char*)m_pszObjInfo;
}

void IPropertyBase::SetObjectInfo(const char* pszObjInfo)
{
	if (m_pUiKernel == NULL || !m_pUiKernel->IsDesignMode())
		return;

	m_pUiKernel->KernelFree((BYTE**)&m_pszObjInfo);

	if (pszObjInfo == NULL || strlen(pszObjInfo) <= 0)
		return;

	int nLen = strlen(pszObjInfo) + 1;

	m_pszObjInfo = (char*)m_pUiKernel->KernelMalloc(nLen);
	if (m_pszObjInfo == NULL)
		return;

	memset(m_pszObjInfo, 0, nLen);
	sprintf_s(m_pszObjInfo, nLen, "%s", pszObjInfo);
}
