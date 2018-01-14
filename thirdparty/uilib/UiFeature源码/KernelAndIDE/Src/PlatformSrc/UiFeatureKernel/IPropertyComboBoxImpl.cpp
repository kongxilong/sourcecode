
#include "stdafx.h"
#include "IPropertyComboBoxImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\ICommonFun.h"

IPropertyComboBoxImpl::IPropertyComboBoxImpl(IUiFeatureKernel* pUiKernel) : IPropertyComboBox(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_COMBOBOX_NAME));
	Clear();
}

IPropertyComboBoxImpl::~IPropertyComboBoxImpl()
{
	Clear();
}

void IPropertyComboBoxImpl::Clear()
{
	m_ComboBoxPro.nSelect = -1;
	m_ComboBoxPro.DataVec.clear();
}

bool IPropertyComboBoxImpl::IsRightData()
{
	return (m_ComboBoxPro.DataVec.size() > 0 && m_ComboBoxPro.nSelect >= 0 && m_ComboBoxPro.nSelect < (int)m_ComboBoxPro.DataVec.size());
}

COMBOBOX_PROP* IPropertyComboBoxImpl::GetComboBoxData()
{
	return &m_ComboBoxPro;
}

bool IPropertyComboBoxImpl::ReadPropertyFromXmlNode(xml_node* pXmlNode)
{
	if (pXmlNode == NULL)
		return false;

	const WCHAR * psz_id = XmlGetAttrValue(pXmlNode, _T(SKIN_OBJECT_ID));
	const WCHAR * psz_name = XmlGetAttrValue(pXmlNode, L"name");
	const WCHAR * psz_counts = XmlGetAttrValue(pXmlNode, L"counts");
	const WCHAR * psz_select = XmlGetAttrValue(pXmlNode, L"select");
	if (psz_id == NULL || psz_name == NULL || psz_counts == NULL || psz_select == NULL)
		return false;

	SetObjectId(_wtoi(psz_id));
	SetObjectName(psz_name);

	int nCtns = _wtoi(psz_counts);
	m_ComboBoxPro.nSelect = _wtoi(psz_select);
	if (m_ComboBoxPro.nSelect < 0 || m_ComboBoxPro.nSelect >= nCtns)
		m_ComboBoxPro.nSelect = 0;

	return true;
}

// 写入xml
bool IPropertyComboBoxImpl::AppendToXmlNode(xml_document &XmlStrObj, xml_node* pParentXmlNode)
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

	AddIntAttrToNode(&PropNode, L"counts", m_ComboBoxPro.DataVec.size());
	AddIntAttrToNode(&PropNode, L"select", m_ComboBoxPro.nSelect);

	return true;
}

// 设置选中项
void IPropertyComboBoxImpl::SetSelect(int nSel)
{
	// 分开处理是为了加快运行时态的速度，运行时态加载各个选项的数据，只记录最终的选择序号
	if (m_pUiKernel != NULL && m_pUiKernel->IsDesignMode())
	{
		if (nSel < 0 || nSel >= (int)m_ComboBoxPro.DataVec.size() || m_ComboBoxPro.nSelect == nSel)
			return;
	}

	m_ComboBoxPro.nSelect = nSel;
}

// 向列表里添加一个数据
bool IPropertyComboBoxImpl::AppendString(LPCWSTR pszString)
{
	if (pszString == NULL)
		return false;

	m_ComboBoxPro.DataVec.push_back(pszString);
	return true;
}

int IPropertyComboBoxImpl::GetSelect()
{
	return m_ComboBoxPro.nSelect;
}

int IPropertyComboBoxImpl::GetDataCounts()
{
	return (int)m_ComboBoxPro.DataVec.size();
}

void IPropertyComboBoxImpl::SetSelectString(LPCWSTR pszSelString)
{
	if (pszSelString == NULL || wcslen(pszSelString) <= 0)
		return;

	int nNo = 0;
	for (STRING_VEC::iterator pStringItem = m_ComboBoxPro.DataVec.begin(); pStringItem != m_ComboBoxPro.DataVec.end(); pStringItem++, nNo++)
	{
		CStringW strString = *pStringItem;
		if (strString.GetLength() <= 0)
			continue;

		if (strString.CompareNoCase(pszSelString) == 0)
		{
			m_ComboBoxPro.nSelect = nNo;
			return;
		}
	}
}

// 从另一个属性克隆
bool IPropertyComboBoxImpl::Clone(IPropertyBase* pFromPropBase)
{
	if (!IsSameTypePropetry(dynamic_cast<IPropertyBase*>(this), pFromPropBase))
		return false;

	IPropertyComboBoxImpl* pFromProp = dynamic_cast<IPropertyComboBoxImpl*>(pFromPropBase);
	if (pFromProp == NULL)
		return false;

	m_ComboBoxPro.nSelect = pFromProp->m_ComboBoxPro.nSelect;

	for (STRING_VEC::iterator pStrItem = pFromProp->m_ComboBoxPro.DataVec.begin();
		pStrItem != pFromProp->m_ComboBoxPro.DataVec.end(); pStrItem++)
	{
		CStringW &strData = *pStrItem;
		m_ComboBoxPro.DataVec.push_back(strData);
	}

	return true;
}
