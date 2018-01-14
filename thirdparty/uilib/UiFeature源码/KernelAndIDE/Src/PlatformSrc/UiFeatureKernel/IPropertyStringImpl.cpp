
#include "stdafx.h"
#include "IPropertyStringImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyStringImpl::IPropertyStringImpl(IUiFeatureKernel* pUiKernel) : IPropertyString(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_STRING_NAME));
	m_StringProp.StringMap.clear();
	Clear();
}

IPropertyStringImpl::~IPropertyStringImpl()
{
	Clear();
}

void IPropertyStringImpl::Clear()
{
	if (m_pUiKernel != NULL)
	{
		for (STRING_MAP::iterator pStringItem = m_StringProp.StringMap.begin(); pStringItem != m_StringProp.StringMap.end(); pStringItem++)
		{
			WCHAR* pstrString = pStringItem->second;
			m_pUiKernel->KernelFree((BYTE**)&pstrString);
		}
	}

	m_StringProp.pstrDefaultString = NULL;
	m_StringProp.nDefaultArea = AT_CN;
	m_StringProp.StringMap.clear();
}

bool IPropertyStringImpl::IsRightData()
{
	return (m_StringProp.pstrDefaultString != NULL && wcslen(m_StringProp.pstrDefaultString) > 0);
}

void IPropertyStringImpl::SetArea(AREA_TYPE areaType)
{
	if (areaType > AT_NONE && areaType < AT_LAST)
	{
		m_StringProp.nDefaultArea = areaType;
		ResetDefaultString();
	}
}

bool IPropertyStringImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	Clear();
	if (pXmlNode == NULL || m_pUiKernel == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	const WCHAR * psz_counts = XmlGetAttrValue(pXmlNode, L"counts");
	const WCHAR * psz_default = XmlGetAttrValue(pXmlNode, L"defaultarea");
	if (psz_id == NULL || psz_name == NULL || psz_counts == NULL || psz_default == NULL)
		return false;

	int nCtns = _wtoi(psz_counts);

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);

	m_StringProp.nDefaultArea = (AREA_TYPE)_wtoi(psz_default);
	if (m_StringProp.nDefaultArea <= AT_NONE || m_StringProp.nDefaultArea >= AT_LAST)
		m_StringProp.nDefaultArea = AT_CN;

	for (int i = 0; i < nCtns; i++)
	{
		WCHAR pszDataName[MAX_PATH + 1], pszAreaName[MAX_PATH + 1];
		memset(pszDataName, 0, MAX_PATH + 1);
		memset(pszAreaName, 0, MAX_PATH + 1);

		swprintf_s(pszDataName, MAX_PATH, L"data%d", i);
		swprintf_s(pszAreaName, MAX_PATH, L"area%d", i);

		const WCHAR * psz_data = XmlGetAttrValue(pXmlNode, pszDataName);
		const WCHAR * psz_area = XmlGetAttrValue(pXmlNode, pszAreaName);
		if (psz_data == NULL || psz_area == NULL)
		{
			Clear();
			return false;
		}

		int nMemLen = (wcslen(psz_data) + 1) * sizeof(WCHAR);
		WCHAR * pwszInsertString = (WCHAR*)m_pUiKernel->KernelMalloc(nMemLen);
		if (pwszInsertString == NULL)
			return false;

		memset(pwszInsertString, 0, nMemLen);
		wcscpy_s(pwszInsertString, wcslen(psz_data) + 1, psz_data);

		AREA_TYPE nArea = (AREA_TYPE)_wtoi(psz_area);
		m_StringProp.StringMap.insert(pair<AREA_TYPE, WCHAR*>(nArea, pwszInsertString));
	}

	ResetDefaultString();
	return true;
}

WCHAR* IPropertyStringImpl::GetString(AREA_TYPE areaType)
{
	return m_StringProp.pstrDefaultString;
}

int IPropertyStringImpl::GetLength()
{
	if (m_StringProp.pstrDefaultString == NULL)
		return 0;

	return wcslen(m_StringProp.pstrDefaultString);
}

void IPropertyStringImpl::SetString(const WCHAR* pszString, AREA_TYPE areaType)
{
	if (pszString == NULL || wcslen(pszString) <= 0 || m_pUiKernel == NULL)
		return;

	int nMemLen = (wcslen(pszString) + 1) * sizeof(WCHAR);
	WCHAR * pwszInsertString = (WCHAR*)m_pUiKernel->KernelMalloc(nMemLen);
	if (pwszInsertString == NULL)
		return;

	memset(pwszInsertString, 0, nMemLen);
	wcscpy_s(pwszInsertString, wcslen(pszString) + 1, pszString);

	STRING_MAP::iterator pStringItem = m_StringProp.StringMap.find(m_StringProp.nDefaultArea);
	if (pStringItem == m_StringProp.StringMap.end())
	{
		m_StringProp.StringMap.insert(pair<AREA_TYPE, WCHAR*>(m_StringProp.nDefaultArea, pwszInsertString));
	}
	else
	{
		WCHAR* pDelString = pStringItem->second;
		m_pUiKernel->KernelFree((BYTE**)&pDelString);
		m_StringProp.StringMap.erase(pStringItem);

		m_StringProp.StringMap.insert(pair<AREA_TYPE, WCHAR*>(m_StringProp.nDefaultArea, pwszInsertString));
	}

	ResetDefaultString();
}

void IPropertyStringImpl::ResetDefaultString()
{
	m_StringProp.pstrDefaultString = NULL;
	STRING_MAP::iterator pStringItem = m_StringProp.StringMap.find(m_StringProp.nDefaultArea);
	if (pStringItem == m_StringProp.StringMap.end())
		return;

	WCHAR *pstrString = pStringItem->second;
	m_StringProp.pstrDefaultString = pstrString;
}

// 写入xml
bool IPropertyStringImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
{
	// 如果是无效属性，不写入XML
	if (!GetActivePropetry())
		return true;

	if (pParentXmlNode == NULL)
		return false;

	// 如果没有数据，就不存储到xml中了
	if (((int)(m_StringProp.StringMap.size())) <= 0)
		return true;

	xml_node PropNode = pParentXmlNode->append_child(L"item");
	if (!PropNode)
		return false;

	AddIntAttrToNode(&PropNode, _T(SKIN_OBJECT_ID), (int)GetObjectId());
	AddStringAttrToNode(&PropNode, L"name", GetObjectName());

	AddIntAttrToNode(&PropNode, L"counts", m_StringProp.StringMap.size());
	AddIntAttrToNode(&PropNode, L"defaultarea", m_StringProp.nDefaultArea);

	int nNo = 0;
	for (STRING_MAP::iterator pStringItem = m_StringProp.StringMap.begin(); pStringItem != m_StringProp.StringMap.end(); pStringItem++)
	{
		AREA_TYPE areaType = pStringItem->first;
		WCHAR *pstrString = pStringItem->second;
		if (pstrString == NULL)
			continue;

		WCHAR pszDataName[MAX_PATH + 1], pszAreaName[MAX_PATH + 1];
		memset(pszDataName, 0, sizeof(pszDataName));
		memset(pszAreaName, 0, sizeof(pszAreaName));
		swprintf_s(pszDataName, MAX_PATH, L"data%d", nNo);
		swprintf_s(pszAreaName, MAX_PATH, L"area%d", nNo);

		AddIntAttrToNode(&PropNode, pszAreaName, areaType);
		AddStringAttrToNode(&PropNode, pszDataName, (const WCHAR*)pstrString);
		nNo++;
	}

	return true;
}

// 从另一个属性克隆
bool IPropertyStringImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyStringImpl* pFromProp = dynamic_cast<IPropertyStringImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

	Clear();

	m_StringProp.nDefaultArea = pFromProp->m_StringProp.nDefaultArea;

	for (STRING_MAP::iterator pStringItem = pFromProp->m_StringProp.StringMap.begin();
		pStringItem != pFromProp->m_StringProp.StringMap.end(); pStringItem++)
	{
		AREA_TYPE areaType = pStringItem->first;
		WCHAR* psz_data = pStringItem->second;
		if (psz_data == NULL || wcslen(psz_data) <= 0)
			continue;

		int nMemLen = (wcslen(psz_data) + 1) * sizeof(WCHAR);
		WCHAR * pwszInsertString = (WCHAR*)m_pUiKernel->KernelMalloc(nMemLen);
		if (pwszInsertString == NULL)
			return false;

		memset(pwszInsertString, 0, nMemLen);
		wcscpy_s(pwszInsertString, wcslen(psz_data) + 1, psz_data);

		m_StringProp.StringMap.insert(pair<AREA_TYPE, WCHAR*>(areaType, pwszInsertString));
	}

	ResetDefaultString();
	return true;
}
