
#include "StdAfx.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IFeatureObject.h"
#include "..\..\Inc\ICommonFun.h"
#include "IUiFeatureKernelImpl.h"
#include "IWindowBaseImpl.h"
#include "WindowSubclass.h"
#include "ControlImpl.h"
#include "IPropertySkinManagerImpl.h"
#include "..\..\Inc\ICtrlInterface.h"
#include "IPropertyControlImpl.h"
#include "IUiEffectManagerImpl.h"

#define _RUN_PASSWORD_WORD_				"(0xFEFDFCFB)-*-(0xFEFDFCFB)!@#fangshunbao@163.com"

static IUiFeatureKernelImpl* g_pKernelWindowInstance = NULL;
ULONG_PTR IUiFeatureKernelImpl::m_gdiplusToken = NULL;

// 内核对【对话框】的接口
IUiFeatureKernel *GetUiKernelInterface()
{
	return IUiFeatureKernelImpl::GetInstance();
}

IUiFeatureKernelImpl::IUiFeatureKernelImpl()
{
	GdiplusStartupInput gdiplusStartupInput;
	if (m_gdiplusToken == NULL)
		GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_bIsDesignMode = false;
	m_nTimerId = UM_DFT_ANIMATION_TIMER + 1;
	m_nBuilderHwnd = 1;
	m_pSkinMgr = new IPropertySkinManagerImpl;
	m_pUiEngine = new IUiEngineImpl;
	m_pCtrlDllMgr = new CControlImpl(this);

	m_dwKernelId = 1;
	m_bBuilderReadingSkin = false;
}

IUiFeatureKernelImpl::~IUiFeatureKernelImpl()
{
	ReleaseKernelWindow();

	SAFE_DELETE(m_pSkinMgr);
	SAFE_DELETE(m_pUiEngine);
	SAFE_DELETE(m_pCtrlDllMgr);
}

// 退出程序前，调用，卸载kernel的资源
void IUiFeatureKernelImpl::UnInitialized()
{
	ReleaseKernelWindow();
	// 此函数如果调用不当，GdiplusShutdown函数会死锁
	if (m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}
	
	g_pKernelWindowInstance = NULL;
	delete this;
}

void IUiFeatureKernelImpl::ReleaseKernelWindow()
{
	for (WINDOW_IMPL_MAP::iterator pWndItem = m_WndImplMap.begin(); pWndItem != m_WndImplMap.end(); pWndItem++)
	{
		IWindowBaseImpl* pDelWnd = pWndItem->second;
		SAFE_DELETE(pDelWnd);
	}
	m_WndImplMap.clear();

	ClearCtrlRegList();
}

void IUiFeatureKernelImpl::ClearCtrlRegList()
{
	ListObj* pItem = m_CtrlRegMap.TopObj();
	while (pItem != NULL)
	{
		ListObj* pNext = m_CtrlRegMap.NextObj(pItem);

		int nSize = sizeof(CONTROL_REG);

		CONTROL_REG* pCtrlReg = (CONTROL_REG*)pItem;
		if (pCtrlReg != NULL && pCtrlReg->pCtrlMgr != NULL)
		{
			pCtrlReg->pCtrlMgr->DeleteRegControlInfo(pCtrlReg);
		}
		else
		{
			SAFE_DELETE(pCtrlReg);
		}

		pItem = pNext;
	}
	m_CtrlRegMap.Clear();
}

// 设置当前为设计模式
void IUiFeatureKernelImpl::BD_SetDesignMode(bool bIsDesignMode)
{
	m_bIsDesignMode = bIsDesignMode;
}

// 是否为设计模式
bool IUiFeatureKernelImpl::IsDesignMode()
{
	return m_bIsDesignMode;
}

IUiFeatureKernel* IUiFeatureKernelImpl::GetInstance()
{
	if (g_pKernelWindowInstance == NULL)
		g_pKernelWindowInstance = new IUiFeatureKernelImpl;

	return g_pKernelWindowInstance;
}

// 取得所有支持的控件
CONTROL_REG_TLIST* IUiFeatureKernelImpl::BD_GetRegisterControl()
{
	if (m_pCtrlDllMgr == NULL)
		return NULL;

	m_pCtrlDllMgr->SetRegControlMap(&m_CtrlRegMap);
	return &m_CtrlRegMap;
}

IPropertySkinManager* IUiFeatureKernelImpl::GetSkinManager()
{
	if (m_pSkinMgr == NULL)
		return NULL;

	return dynamic_cast<IPropertySkinManager*>(m_pSkinMgr);
}

// 一个对话框释放皮肤资源
void IUiFeatureKernelImpl::PG_ReleaseFeatureSkin(HWND hWnd)
{
	WINDOW_IMPL_MAP::iterator pWndImplItem = m_WndImplMap.find(hWnd);
	if (pWndImplItem != m_WndImplMap.end())
	{
		IWindowBaseImpl* pWndImpl = pWndImplItem->second;
		SAFE_DELETE(pWndImpl);
		m_WndImplMap.erase(pWndImplItem);
	}
}

// 关闭一个工程
bool IUiFeatureKernelImpl::BD_CloseProject()
{
	ReleaseKernelWindow();
	if (m_pSkinMgr != NULL)
		m_pSkinMgr->ReleaseSkinManagerPropetry();

	return true;
}

// 一个对话框从一个皮肤包里使用指定的对话框皮肤资源初始化自己
IWindowBase* IUiFeatureKernelImpl::PG_InitFeatureSkin(HWND hWnd, LPCWSTR pszSkinPath, LPCWSTR pszWndName, char *pszPassword)
{
	// 防止多线程同时初始化
	CSimpleLock simpleLock(m_KernelSec);

	if (m_pSkinMgr == NULL)
		return NULL;

	if (!::IsWindow(hWnd) || pszSkinPath == NULL || pszWndName == NULL || wcslen(pszSkinPath) <= 0 || wcslen(pszWndName) <= 0)
		return NULL;

	// 开始子类化
	if (CWindowSubclass::GetInstance()->FindSubWindow(hWnd) != NULL)
		return NULL;

	// 如果是Builder中的预览程序需要创建窗口，则不需要密码认证
	bool bNeedPassword = true;
	if (pszPassword != NULL && strlen(pszPassword) > 0)
		bNeedPassword = (lstrcmpA(_RUN_PASSWORD_WORD_, pszPassword) != 0);

	// 密码判断
	if (bNeedPassword && !m_pSkinMgr->ChekUserPassword(pszPassword, pszSkinPath))
	{
		// 向窗口发送通知：密码验证失败
		::PostMessage(hWnd, UI_FEATURE_KERNEL_MSG, WID_INIT_PASSWORD_ERROR, NULL);
		return NULL;
	}

	IWindowBaseImpl *pWndBaseImpl = new IWindowBaseImpl(dynamic_cast<IUiFeatureKernel*>(this));
	if (pWndBaseImpl == NULL)
		return NULL;

	IWindowBase* pWndBase = (dynamic_cast<IWindowBase*>(pWndBaseImpl));
	if (!CWindowSubclass::GetInstance()->SubWindow(hWnd, pWndBase))
	{
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}

	// 初始化窗口
	if (!pWndBase->PG_InitWindowBase(hWnd, pszSkinPath, pszWndName))
	{
		CWindowSubclass::GetInstance()->EraseSubWindow(hWnd);
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}

	// 记录到窗口队列中
	m_WndImplMap.insert(pair<HWND, IWindowBaseImpl*>(hWnd, pWndBaseImpl));
	return pWndBase;
}

// 创建一个Builder使用的空的窗口
IWindowBase* IUiFeatureKernelImpl::BD_CreateWindowEmptyPropetry(SKIN_FILE_ITEM *pSkinFileItem)
{
	if (m_pSkinMgr == NULL || pSkinFileItem == NULL)
		return NULL;

	IPropertyGroup *pWindowPropGroup = dynamic_cast<IPropertyGroup*>(m_pSkinMgr->CreateEmptyBaseProp(pSkinFileItem, OTID_GROUP));
	if (pWindowPropGroup == NULL)
		return NULL;

	IPropertyWindow *pPropWindow = dynamic_cast<IPropertyWindow*>(m_pSkinMgr->CreateEmptyBaseProp(pSkinFileItem, OTID_WINDOW));
	if (pPropWindow == NULL)
		return NULL;

	pPropWindow->SetWindowPropGroup(pWindowPropGroup);

	IWindowBaseImpl *pWndBaseImpl = new IWindowBaseImpl(dynamic_cast<IUiFeatureKernel*>(this));
	if (pWndBaseImpl == NULL)
		return NULL;

	IWindowBase* pWndBase = (dynamic_cast<IWindowBase*>(pWndBaseImpl));
	if (pWndBase == NULL)
	{
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}
	// 初始化在builder中的属性
	pWndBase->BD_InitWindowBase(pSkinFileItem->strSkinFilePath, pPropWindow, true);

	// 记录到窗口队列中
	m_WndImplMap.insert(pair<HWND, IWindowBaseImpl*>((HWND)m_nBuilderHwnd++, pWndBaseImpl));
	pSkinFileItem->AllWindowPropMap.insert(pair<DWORD, IPropertyBase*>(pWndBase->GetObjectId(), pWindowPropGroup));
	return pWndBase;
}

// 创建一个Builder使用的空的窗口
IWindowBase* IUiFeatureKernelImpl::BD_CreateWindowByPropetry(SKIN_FILE_ITEM *pSkinFileItem, IPropertyWindow *pPropWnd)
{
	if (pPropWnd == NULL || pSkinFileItem == NULL)
		return NULL;

	IWindowBaseImpl *pWndBaseImpl = new IWindowBaseImpl(dynamic_cast<IUiFeatureKernel*>(this));
	if (pWndBaseImpl == NULL)
		return NULL;

	IWindowBase* pWndBase = (dynamic_cast<IWindowBase*>(pWndBaseImpl));
	if (pWndBase == NULL)
	{
		SAFE_DELETE(pWndBaseImpl);
		return NULL;
	}
	// 初始化在builder中的属性
	pWndBase->BD_InitWindowBase(pSkinFileItem->strSkinFilePath, pPropWnd, false);

	// 记录到窗口队列中
	m_WndImplMap.insert(pair<HWND, IWindowBaseImpl*>((HWND)m_nBuilderHwnd++, pWndBaseImpl));
	pSkinFileItem->AllWindowPropMap.insert(pair<DWORD, IPropertyBase*>(pWndBase->GetObjectId(), pWndBase->PP_GetWindowPropetryGroup()));
	return pWndBase;
}

// 创建一个Builder使用的空的控件
IControlBase* IUiFeatureKernelImpl::CreateControlEmptyPropetry(SKIN_FILE_ITEM* pSkinFileItem, IWindowBase *pParentWnd, IControlBase *pParentCtrl,
															   LPCWSTR pszNewCtrlTypeName, LPCWSTR pszNewCtrlName, LPCWSTR pszDftSkinDir, bool bIsCloneCtrl)
{
	if (pSkinFileItem == NULL || pParentWnd == NULL || pParentWnd->PP_GetWindowPropetry() == NULL || m_pCtrlDllMgr == NULL || m_pSkinMgr == NULL || pszNewCtrlTypeName == NULL || wcslen(pszNewCtrlTypeName) <= 0)
		return NULL;

	// 创建新控件
	ICtrlInterface* pCtrlIfc = m_pCtrlDllMgr->CreateCtrl(pszNewCtrlTypeName);
	if (pCtrlIfc == NULL)
		return NULL;

	// 设置是否为克隆出来的控件
	pCtrlIfc->SetCloneControl(bIsCloneCtrl);

	DWORD dwObjId = pCtrlIfc->GetObjectId();
	IControlBase *pCtrlBase = dynamic_cast<IControlBase*>(pCtrlIfc);
	if (pCtrlBase == NULL)
	{
		m_pCtrlDllMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}
	pCtrlBase->SetPropertySkinManager(m_pSkinMgr);

	IPropertyControl *pPropCtrl = dynamic_cast<IPropertyControl*>(m_pSkinMgr->CreateEmptyBaseProp(pSkinFileItem, OTID_CONTROL, &dwObjId));
	if (pPropCtrl == NULL)
	{
		m_pCtrlDllMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}

	// 设置新控件的属性
	IPropertyGroup *pCtrlPropGroup = dynamic_cast<IPropertyGroup*>(m_pSkinMgr->CreateEmptyBaseProp(pSkinFileItem, OTID_GROUP, &dwObjId));
	if (pCtrlPropGroup == NULL)
	{
		m_pSkinMgr->ReleaseBaseProp((IPropertyBase*)pPropCtrl);
		m_pCtrlDllMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}

	pPropCtrl->SetCtrlGroupProp(pCtrlPropGroup);
	pCtrlBase->SetUiEngine(GetUiEngine());
	pCtrlBase->SetPropertySkinManager(m_pSkinMgr);
	pCtrlBase->SetOwnerWindow(pParentWnd);
	pCtrlBase->SetParentControl(pParentCtrl);

	CStringW strNewCtrlName = L"新建控件";
	if (pszNewCtrlName != NULL && wcslen(pszNewCtrlName) > 0)
		strNewCtrlName = pszNewCtrlName;

	pCtrlBase->SetObjectName(strNewCtrlName);
	pPropCtrl->SetObjectName(strNewCtrlName);
	pCtrlPropGroup->SetObjectName(L"");

	m_pSkinMgr->BD_AppendControlToVec(pSkinFileItem, pszNewCtrlTypeName, pCtrlPropGroup);
	// 插入控件队列
	if (pParentCtrl != NULL)
	{
		pParentCtrl->AppendChildContrl(pCtrlBase);
		pParentCtrl->PP_GetControlPropetry()->AppendChildCtrlProp(pPropCtrl);
	}
	else
	{
		pParentWnd->AppendChildContrl(pCtrlBase);
		pParentWnd->PP_GetWindowPropetry()->AppendChildCtrlProp(pPropCtrl);
	}

	pCtrlBase->InitControlPropetry(pPropCtrl, true);
	// new child 由builder通过鼠标拖入一个控件或者Clone一个控件，需要创建子控件
	pCtrlBase->CreateSubControl(true);

//////////////////////////////////////////////////////////////////////////
	// 设置控件默认皮肤，如果是编辑模式的话
	if (this->IsDesignMode() && pSkinFileItem != NULL && pszDftSkinDir != NULL && wcslen(pszDftSkinDir) > 0)
	{
		CStringW strSkinPath = pszDftSkinDir;
		strSkinPath += pCtrlBase->GetObjectType();
		strSkinPath += L"\\";

		m_pSkinMgr->SetControlDefaultSkin(pSkinFileItem, pCtrlBase, strSkinPath);
		SetSubControlDftSkin(pSkinFileItem, pCtrlBase, strSkinPath);
	}
//////////////////////////////////////////////////////////////////////////

	pCtrlBase->OnFinalCreate();
	return pCtrlBase;
}

BOOL IUiFeatureKernelImpl::SetSubControlDftSkin(SKIN_FILE_ITEM* pSkinFileItem, IControlBase *pParentCtrl, LPCWSTR pszDftSkinDir)
{
	if (pSkinFileItem == NULL || pParentCtrl == NULL || pszDftSkinDir == NULL || wcslen(pszDftSkinDir) <= 0)
		return FALSE;

	CHILD_CTRLS_VEC* pChildVec = pParentCtrl->GetChildControlsVec();
	if (pChildVec == NULL)
		return FALSE;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		CStringW strDir = pszDftSkinDir;
		strDir += pCtrl->GetObjectName();
		strDir += L"\\";

		if (!m_pSkinMgr->SetControlDefaultSkin(pSkinFileItem, pCtrl, strDir))
			return FALSE;

		if (!SetSubControlDftSkin(pSkinFileItem, pCtrl, strDir))
			return FALSE;
	}

	return TRUE;
}

// 创建一个Builder使用的控件，并配置上属性
IControlBase* IUiFeatureKernelImpl::CreateControlByPropetry(IWindowBase *pParentWnd, IControlBase *pParentCtrl, IPropertyControl *pPropCtrl)
{
	if (pParentWnd == NULL || pPropCtrl == NULL || m_pCtrlDllMgr == NULL || m_pSkinMgr == NULL)
		return NULL;

	// 创建新控件
	ICtrlInterface* pCtrlIfc = m_pCtrlDllMgr->CreateCtrl(pPropCtrl->GetControlType());
	if (pCtrlIfc == NULL)
		return NULL;

	pCtrlIfc->SetObjectId(pPropCtrl->GetObjectId());
	IControlBase *pCtrlBase = dynamic_cast<IControlBase*>(pCtrlIfc);
	if (pCtrlBase == NULL)
	{
		m_pCtrlDllMgr->ReleaseCtrl(&pCtrlIfc);
		return NULL;
	}
	pCtrlBase->SetPropertySkinManager(m_pSkinMgr);
	pCtrlBase->SetUiEngine(GetUiEngine());
	pCtrlBase->SetPropertySkinManager(m_pSkinMgr);
	pCtrlBase->SetOwnerWindow(pParentWnd);
	pCtrlBase->SetParentControl(pParentCtrl);

	// 插入控件队列
	if (pParentCtrl != NULL)
		pParentCtrl->AppendChildContrl(pCtrlBase);
	else
		pParentWnd->AppendChildContrl(pCtrlBase);

	// 初始化属性
	pCtrlBase->InitControlPropetry(pPropCtrl, false);

	// 设置ObjName
	pCtrlBase->SetObjectName(pCtrlBase->PP_GetControlObjectName());
	pPropCtrl->SetObjectName(pCtrlBase->PP_GetControlObjectName());

	// 创建控件完毕
	pCtrlBase->OnFinalCreate();
	pCtrlBase->ClearDesignModePropetry();
	return pCtrlBase;
}

IUiEngine* IUiFeatureKernelImpl::GetUiEngine()
{
	if (m_pUiEngine == NULL)
		return m_pUiEngine;

	return dynamic_cast<IUiEngine*>(m_pUiEngine);
}

// 设置可以保存的有效属性
void IUiFeatureKernelImpl::BD_SetWindowPropetryActiveProp(IWindowBase *pWndBase, bool bActive)
{
	m_pSkinMgr->BD_SetWindowPropetryActiveProp(pWndBase, bActive);
}

// 删除一个windows
bool IUiFeatureKernelImpl::BD_DeleteWindow(IWindowBase *pWndBase)
{
	if (pWndBase == NULL)
		return false;

	for (WINDOW_IMPL_MAP::iterator pWndItem = m_WndImplMap.begin(); pWndItem != m_WndImplMap.end(); pWndItem++)
	{
		IWindowBaseImpl* pWndBaseImpl = pWndItem->second;
		if (pWndBaseImpl == NULL)
			continue;

		IWindowBase* pComWndBase = dynamic_cast<IWindowBase*>(pWndBaseImpl);
		if (pComWndBase == NULL)
			continue;

		if (pComWndBase == pWndBase || (pWndBase->GetObjectId() == pComWndBase->GetObjectId()))
		{
			m_pSkinMgr->BD_SetWindowPropetryActiveProp(pWndBase, false);
			m_WndImplMap.erase(pWndItem);
			SAFE_DELETE(pWndBaseImpl);
			return true;
		}
	}

	return false;
}

// 删除一个control
bool IUiFeatureKernelImpl::BD_DeleteControl(IControlBase *pCtrlBase)
{
	if (pCtrlBase == NULL)
		return false;

	for (WINDOW_IMPL_MAP::iterator pWndItem = m_WndImplMap.begin(); pWndItem != m_WndImplMap.end(); pWndItem++)
	{
		IWindowBaseImpl* pWndBaseImpl = pWndItem->second;
		if (pWndBaseImpl == NULL)
			continue;

		if (BD_DeleteControl_FromCtrlVec(pWndBaseImpl->GetChildControlsVec(), pCtrlBase))
			return true;
	}

	return false;
}

bool IUiFeatureKernelImpl::BD_DeleteControl_FromCtrlVec(CHILD_CTRLS_VEC* pCtrlVec, IControlBase *pCtrlBase)
{
	if (pCtrlVec == NULL || pCtrlBase == NULL || m_pCtrlDllMgr == NULL)
		return false;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pCtrlVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pCtrlVec->NextObj(pItem);
		IControlBase* pComCtrl = (IControlBase *)pItem;
		if (pComCtrl == NULL)
			continue;

		if (pComCtrl == pCtrlBase || (pComCtrl->GetObjectId() == pCtrlBase->GetObjectId()))
		{
			if (pComCtrl->PP_GetControlPropetry() != NULL)
				pComCtrl->PP_GetControlPropetry()->SetActivePropetry(false);

			if (pComCtrl->PP_GetControlPropetryGroup() != NULL)
				pComCtrl->PP_GetControlPropetryGroup()->SetActivePropetry(false);

			pComCtrl->SetActivePropetry(false);
			m_pSkinMgr->BD_SetGroupPropActiveMark(pComCtrl->PP_GetControlPropetryGroup(), false);
			m_pSkinMgr->BD_SetChildVecActiveMark(pComCtrl->GetChildControlsVec(), false);

			pComCtrl->OnDestroy();
			pCtrlVec->DelObj(pComCtrl);
			ICtrlInterface* pDelCtrl = dynamic_cast<ICtrlInterface*>(pComCtrl);
			m_pCtrlDllMgr->ReleaseCtrl(&pDelCtrl);
			return true;
		}

		if (BD_DeleteControl_FromCtrlVec(pComCtrl->GetChildControlsVec(), pCtrlBase))
			return true;
	}

	return false;
}

// 取得全局唯一定时器号
int IUiFeatureKernelImpl::GetTimerId()
{
	return m_nTimerId++;
}

// 上下方向交换控件位置
bool IUiFeatureKernelImpl::BD_UpDownChangeControlPostion(IControlBase *pToUpCtrl, IControlBase *pToDownCtrl)
{
	if (pToUpCtrl == NULL || pToDownCtrl == NULL)
		return false;

	CHILD_CTRLS_VEC* pChildVec = NULL;
	PROP_CONTROL_VEC* pCtrlPropVec = NULL;
	IControlBase* pParentCtrl = pToUpCtrl->GetParentControl();
	if (pParentCtrl == NULL)
	{
		IWindowBase* pWndBase = pToUpCtrl->GetOwnerWindow();
		pChildVec = pWndBase->GetChildControlsVec();
		pCtrlPropVec = pWndBase->GetChildPropControlVec();
	}
	else
	{
		pChildVec = pParentCtrl->GetChildControlsVec();
		pCtrlPropVec = pParentCtrl->GetChildPropControlVec();
	}

	if (pChildVec == NULL || pCtrlPropVec == NULL)
		return false;

	// 交换控件位置
	bool bChange = false;
	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (pCtrl == pToDownCtrl)
		{
			bChange = true;
			break;
		}
	}

	if (!bChange)
		return false;

	// 交换属性位置
	bChange = false;
	PROP_CONTROL_VEC::iterator pPropCtrlItem;
	for (pPropCtrlItem = pCtrlPropVec->begin(); pPropCtrlItem != pCtrlPropVec->end(); pPropCtrlItem++)
	{
		IPropertyControl* pPropCtrl = *pPropCtrlItem;
		if (pPropCtrl == NULL)
			continue;

		IFeatureObject* pOwner = pPropCtrl->GetOwnerObject();
		if (pOwner == NULL)
			continue;

		IControlBase* pCtrl = dynamic_cast<IControlBase*>(pOwner);
		if (pCtrl == NULL)
			continue;

		if (pCtrl == pToDownCtrl)
		{
			bChange = true;
			break;
		}
	}

	if (!bChange)
		return false;

	// 交换控件位置
	if (!pChildVec->ChangeObjPos(pToUpCtrl, pToDownCtrl))
		return false;

	// 交换属性位置
	*pPropCtrlItem = pToUpCtrl->PP_GetControlPropetry();
	pPropCtrlItem++;
	*pPropCtrlItem = pToDownCtrl->PP_GetControlPropetry();

	return true;
}

// 转移控件到另外的控件队列中
bool IUiFeatureKernelImpl::BD_MoveControlToOtherParentControl(IControlBase *pFromCtrl, IFeatureObject *pToParentCtrl)
{
	if (pFromCtrl == NULL || pToParentCtrl == NULL)
		return false;

	CHILD_CTRLS_VEC* pFromChildVec = NULL;
	PROP_CONTROL_VEC* pFromCtrlPropVec = NULL;
	IControlBase* pParentCtrl = pFromCtrl->GetParentControl();
	if (pParentCtrl == NULL)
	{
		IWindowBase* pWndBase = pFromCtrl->GetOwnerWindow();
		if (pWndBase == NULL)
			return false;

		pFromChildVec = pWndBase->GetChildControlsVec();
		pFromCtrlPropVec = pWndBase->GetChildPropControlVec();
	}
	else
	{
		pFromChildVec = pParentCtrl->GetChildControlsVec();
		pFromCtrlPropVec = pParentCtrl->GetChildPropControlVec();
	}

	if (pFromChildVec == NULL || pFromCtrlPropVec == NULL)
		return false;

	// 从控件队列中删除
	bool bDelete = false;
	ListObj* pNext = NULL;
	for (ListObj* pItem = pFromChildVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pFromChildVec->NextObj(pItem);
		IControlBase* pComCtrl = (IControlBase *)pItem;
		if (pComCtrl == pFromCtrl)
		{
			bDelete = true;
			break;
		}
	}

	if (!bDelete)
		return false;

	// 从属性队列中删除
	bDelete = false;
	PROP_CONTROL_VEC::iterator pFromPropCtrlItem;
	for (pFromPropCtrlItem = pFromCtrlPropVec->begin(); pFromPropCtrlItem != pFromCtrlPropVec->end(); pFromPropCtrlItem++)
	{
		IPropertyControl* pPropCtrl = *pFromPropCtrlItem;
		if (pPropCtrl == NULL)
			continue;

		IFeatureObject* pOwner = pPropCtrl->GetOwnerObject();
		if (pOwner == NULL)
			continue;

		IControlBase* pComCtrl = dynamic_cast<IControlBase*>(pOwner);
		if (pComCtrl == pFromCtrl)
		{
			bDelete = true;
			break;
		}
	}

	if (!bDelete)
		return false;

	CHILD_CTRLS_VEC* pToChildVec = NULL;
	PROP_CONTROL_VEC* pToCtrlPropVec = NULL;
	if (pToParentCtrl->GetObjectTypeId() == OTID_WINDOW)
	{
		IWindowBase* pWndBase = dynamic_cast<IWindowBase*>(pToParentCtrl);
		if (pWndBase == NULL)
			return false;
	
		pToChildVec = pWndBase->GetChildControlsVec();
		pToCtrlPropVec = pWndBase->GetChildPropControlVec();
	}
	else
	{
		IControlBase* pCtrlBase = dynamic_cast<IControlBase*>(pToParentCtrl);
		if (pCtrlBase == NULL)
			return false;
	
		pToChildVec = pCtrlBase->GetChildControlsVec();
		pToCtrlPropVec = pCtrlBase->GetChildPropControlVec();
	}
	
	if (pToChildVec == NULL || pToCtrlPropVec == NULL)
		return false;

	IPropertyControl* pFromPropCtrl = *pFromPropCtrlItem;
	if (pFromCtrl == NULL || pFromPropCtrl == NULL)
		return false;

	// 从控件队列中删除
	pFromChildVec->DelObj(pFromCtrl);
	// 插入到控件队列
	pToChildVec->PushBackObj(pFromCtrl);
	// 从属性队列中删除
	pFromCtrlPropVec->erase(pFromPropCtrlItem);
	// 插入到属性队列
	pToCtrlPropVec->push_back(pFromPropCtrl);

	if (pToParentCtrl->GetObjectTypeId() == OTID_WINDOW)
	{
		pFromCtrl->SetParentControl(NULL);
	}
	else
	{
		IControlBase* pCtrlBase = dynamic_cast<IControlBase*>(pToParentCtrl);
		pFromCtrl->SetParentControl(pCtrlBase);
	}

	return true;
}

// 取得所有支持的动画列表
int IUiFeatureKernelImpl::GetAnimationList(const char* &strType)
{
	return (int)(IUiEffectManagerImpl::GetInstance()->GetSurportAnimationType(strType));
}

// 内存函数
BYTE* IUiFeatureKernelImpl::KernelMalloc(int nLen)
{
	if (nLen <= 0)
		return NULL;

	if (nLen == 1)
		nLen += 1;

	BYTE* pOut = new BYTE[nLen];
	//BYTE* pOut = (BYTE*)malloc(nLen);
	return pOut;
}

void IUiFeatureKernelImpl::KernelFree(BYTE** ppbyFreeBuf)
{
	if (ppbyFreeBuf == NULL || *ppbyFreeBuf == NULL)
		return;

	delete[] *ppbyFreeBuf;
	//free(*ppbyFreeBuf);

	*ppbyFreeBuf = NULL;
}

// 取得一个递增的序号
DWORD IUiFeatureKernelImpl::KernelGetId()
{
	return m_dwKernelId++;
}

void IUiFeatureKernelImpl::KernelSetIdBegin(DWORD dwIdBegin)
{
	m_dwKernelId = dwIdBegin;
}

// 设置builder正在读取皮肤包的状态
void IUiFeatureKernelImpl::BD_SetBuilderReadingSkin(bool bReading)
{
	m_bBuilderReadingSkin = bReading;
}

bool IUiFeatureKernelImpl::BD_IsBuilderReadingSkin()
{
	return m_bBuilderReadingSkin;
}
