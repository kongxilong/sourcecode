
#include "StdAfx.h"
#include "IControlManagerImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IControlManager.h"

// 创建一个控件的宏
#define CREATE_CONTROL(ctrl_name, ctrl_class_type)				{if (lstrcmpiW(pCtrlType, ctrl_name) == 0)\
																{\
																	ctrl_class_type *pCtrl = new ctrl_class_type(m_pUiKernel);\
																	if (pCtrl != NULL)\
																	{\
																		pRetCtrl = dynamic_cast<ICtrlInterface*>(pCtrl);\
																		if (pRetCtrl == NULL)\
																		{\
																			SAFE_DELETE(pCtrl);\
																		}\
																		else\
																		{\
																			pRetCtrl->SetControlDllManager(this);\
																		}\
																	}\
																}}


// 销毁一个控件的宏
#define DELETE_CONTROL(ctrl_name, ctrl_class_type)				{if (lstrcmpiW(pszObjType, ctrl_name) == 0)\
																{\
																	ctrl_class_type *pPanel = dynamic_cast<ctrl_class_type*>(*ppCtrl);\
																	if (pPanel != NULL)\
																	{\
																		SAFE_DELETE(pPanel);\
																		*ppCtrl = NULL;\
																		return true;\
																	}\
																}}

IControlManager *GetControlManager()
{
	return IControlManagerImpl::GetInstance();
}

IControlManagerImpl::IControlManagerImpl()
{
	m_pUiKernel = NULL;
}

IControlManagerImpl::~IControlManagerImpl()
{
}

IControlManager* IControlManagerImpl::GetInstance()
{
	static IControlManagerImpl _ControlManagerInstance;
	return &_ControlManagerInstance;
}

void IControlManagerImpl::SetUiKernel(IUiFeatureKernel* pUiKernel)
{
	m_pUiKernel = pUiKernel;
}

// 设置一个支持的控件的节点
void IControlManagerImpl::SetRegControl(CONTROL_REG_TLIST *pCtrlMap, CStringW strCtrlGroupName, CStringW strCtrlName, CStringW strControlInfo)
{
	if (pCtrlMap == NULL)
		return;

	CONTROL_REG* pRegInfo = new CONTROL_REG;
	if (pRegInfo == NULL)
		return;

	memset(pRegInfo, 0, sizeof(CONTROL_REG));

	pRegInfo->pCtrlMgr = this;
	swprintf_s(pRegInfo->strCtrlGroupName, MAX_PATH, L"%s", strCtrlGroupName);
	swprintf_s(pRegInfo->strCtrlName, MAX_PATH, L"%s", strCtrlName);
	swprintf_s(pRegInfo->strDllName, MAX_PATH, L"%s", L"_XXXX_Ctrl_Dll_.dll");
	swprintf_s(pRegInfo->strControlInfo, MAX_PATH, L"%s", strControlInfo);

	pCtrlMap->PushBackObj(pRegInfo);
}

// 删除一个控件的注册信息
void IControlManagerImpl::DeleteRegControlInfo(CONTROL_REG* pReg)
{
	SAFE_DELETE(pReg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 需要修改和扩展的函数
// 取得所有支持的控件
void IControlManagerImpl::SetRegControlMap(CONTROL_REG_TLIST *pCtrlMap)
{
	if (pCtrlMap == NULL)
		return;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 添加控件，步骤2：向Builder工具注册控件
//	SetRegControl(pCtrlMap, L"动画控件组", CTRL_NAME_ANIMATION_EXPAND, L"上下展开动画控件");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// 创建一个控件，参数为步骤1的宏
ICtrlInterface* IControlManagerImpl::CreateCtrl(LPCWSTR pCtrlType)
{
	if (pCtrlType == NULL)
		return NULL;

	ICtrlInterface *pRetCtrl = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 添加控件，步骤4：创建一个控件
//	CREATE_CONTROL(CTRL_NAME_ANIMATION_EXPAND, ICtrlAnimationExpandImpl);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return pRetCtrl;
}

// 销毁一个控件
bool IControlManagerImpl::ReleaseCtrl(ICtrlInterface **ppCtrl)
{
	if (ppCtrl == NULL || *ppCtrl == NULL)
		return false;

	const WCHAR * pszObjType = (*ppCtrl)->GetObjectType();
	if (pszObjType == NULL || wcslen(pszObjType) <= 0)
	{
		SAFE_DELETE(*ppCtrl);
		return true;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 添加控件，步骤5：销毁一个控件
//	DELETE_CONTROL(CTRL_NAME_ANIMATION_EXPAND, ICtrlAnimationExpandImpl);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return false;
}
