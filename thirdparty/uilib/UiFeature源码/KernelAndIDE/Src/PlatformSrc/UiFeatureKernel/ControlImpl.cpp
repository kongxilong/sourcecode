
#include "StdAfx.h"
#include "ControlImpl.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "IUiFeatureKernelImpl.h"

CControlImpl::CControlImpl(IUiFeatureKernel* pUiKernel)
{
	m_pUiKernel = pUiKernel;
	m_CtrlDllVec.clear();
	LoadControlDll();
}

CControlImpl::~CControlImpl()
{
	for (CONTROL_DLL_VEC::iterator pCtrlItem = m_CtrlDllVec.begin(); pCtrlItem != m_CtrlDllVec.end(); pCtrlItem++)
	{
		CONTRL_DLL_INFO &CtrlDllInfo = *pCtrlItem;
		SAFE_FREE_LIBRARY(CtrlDllInfo.hDll);
	}
	m_CtrlDllVec.clear();
}

bool CControlImpl::LoadControlDll()
{
	if (m_CtrlDllVec.size() > 0)
		return true;

	CStringW strPath = PathHelperW(_T(NAME_UIFEATURE_XML));
	if (strPath.GetLength() <= 0)
		return false;

	xml_document xmlDoc;
	if(xmlDoc.load_file(strPath, parse_default, encoding_utf8).status != status_ok)
		return false;

	xml_node UiFeatureRoot = xmlDoc.first_child();
	if (UiFeatureRoot.empty())
		return false;

	xml_node CtrlDllNode = UiFeatureRoot.child(L"controldll");
	if (CtrlDllNode.empty())
		return false;

	for(xml_node dllNode = CtrlDllNode.first_child(); !dllNode.empty(); dllNode = dllNode.next_sibling())
	{
		const WCHAR * psz_path = XmlGetAttrValue(&dllNode, L"path");
		if (psz_path != NULL)
		{
			CONTRL_DLL_INFO DllInfo;
			DllInfo.strPath = PathHelperW(psz_path);

			DllInfo.hDll = ::LoadLibraryW(DllInfo.strPath);
			if (DllInfo.hDll == NULL)
				continue;

			GETCONTROLMANAGER GetControl = (GETCONTROLMANAGER)::GetProcAddress(DllInfo.hDll, "GetControlManager");
			if (GetControl == NULL)
				continue;

			DllInfo.pCtrlMgr = GetControl();
			if (DllInfo.pCtrlMgr == NULL)
				continue;

			DllInfo.pCtrlMgr->SetUiKernel(m_pUiKernel);
			m_CtrlDllVec.push_back(DllInfo);
		}
	}

	return true;
}

void CControlImpl::SetRegControlMap(CONTROL_REG_TLIST *pCtrlMap)
{
	if (pCtrlMap == NULL)
		return;
	pCtrlMap->Clear();

	for (CONTROL_DLL_VEC::iterator pCtrlItem = m_CtrlDllVec.begin(); pCtrlItem != m_CtrlDllVec.end(); pCtrlItem++)
	{
		CONTRL_DLL_INFO &CtrlDllInfo = *pCtrlItem;
		if (CtrlDllInfo.pCtrlMgr != NULL)
			CtrlDllInfo.pCtrlMgr->SetRegControlMap(pCtrlMap);
	}
}

// 创建一个控件，参数为步骤1的宏
ICtrlInterface* CControlImpl::CreateCtrl(LPCWSTR pCtrlType)
{
	ICtrlInterface* pNewCtrl = NULL;
	for (CONTROL_DLL_VEC::iterator pCtrlItem = m_CtrlDllVec.begin(); pCtrlItem != m_CtrlDllVec.end(); pCtrlItem++)
	{
		CONTRL_DLL_INFO &CtrlDllInfo = *pCtrlItem;
		if (CtrlDllInfo.pCtrlMgr != NULL)
		{
			pNewCtrl = CtrlDllInfo.pCtrlMgr->CreateCtrl(pCtrlType);
			if (pNewCtrl != NULL)
				return pNewCtrl;
		}
	}
	return pNewCtrl;
}

// 销毁一个控件
bool CControlImpl::ReleaseCtrl(ICtrlInterface **ppCtrl)
{
	if (ppCtrl == NULL || *ppCtrl == NULL)
		return false;

	for (CONTROL_DLL_VEC::iterator pCtrlItem = m_CtrlDllVec.begin(); pCtrlItem != m_CtrlDllVec.end(); pCtrlItem++)
	{
		CONTRL_DLL_INFO &CtrlDllInfo = *pCtrlItem;
		if (CtrlDllInfo.pCtrlMgr != NULL && CtrlDllInfo.pCtrlMgr->ReleaseCtrl(ppCtrl))
			return true;
	}

	SAFE_DELETE(*ppCtrl);
	return true;
}
