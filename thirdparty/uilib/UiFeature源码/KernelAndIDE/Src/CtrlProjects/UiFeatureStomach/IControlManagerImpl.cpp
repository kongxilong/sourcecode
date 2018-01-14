
#include "StdAfx.h"
#include "IControlManagerImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "ICtrlImageBoxImpl.h"
#include "ICtrlShadowButtonImpl.h"
#include "ICtrlStaticImpl.h"
#include "ICtrlLinkImpl.h"
#include "ICtrlEditImpl.h"
#include "ICtrlIEWebBrowserImpl.h"
#include "ICtrlShadowProgressBarImpl.h"
#include "ICtrlCheckBoxImpl.h"
#include "ICtrlWindowTitleBarImpl.h"
#include "ICtrlMacDockBarImpl.h"
#include "ICtrlFocusImpl.h"
#include "ICtrlPanelImpl.h"
#include "ICtrlStraightLineFlyImageImpl.h"
#include "ICtrlGradualLightImpl.h"
#include "ICtrlRollAdvertisementImpl.h"
#include "ICtrlRichEditImpl.h"
#include "ICtrlRadioBoxImpl.h"
#include "ICtrlContextMenuImpl.h"
#include "ICtrlContextMenuItemImpl.h"
#include "ICtrlScrollBarImpl.h"
#include "ICtrlAndroidListImpl.h"
#include "ICtrlHwndObjImpl.h"
#include "ICtrlLEDCtrlImpl.h"
#include "ICtrlSplitBarImpl.h"

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
	swprintf_s(pRegInfo->strDllName, MAX_PATH, L"%s", L"UiFeatureStomach.dll");
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
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_BASE_PANEL, L"容器控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_IMAGE_BOX, L"图片框控件，一般作为窗口背景");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_SHADOW_BUTTON, L"按钮控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_STATIC, L"静态文本控件，此控件可添加文本图片背景");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_LINK, L"类似网页的超链接控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_EDIT, L"普通Edit控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_IE_WEB_BROWSER, L"Windows标准Ie控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_SHADOW_PROGRESS_BAR, L"普通进度条控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_CHECK_BOX, L"多选框控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_RADIO_BOX, L"单选框控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_WINDOW_TITLE_BAR, L"一个窗口的标题栏控件，包括最小化、关闭、最大化、还原按钮");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_MAC_DOCK_BAR, L"仿Mac的DockBar的控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_FOCUS_CTRL, L"焦点控件，当前鼠标点击那个控件，那个控件外边就会有高亮外框");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_STRAIGHTLINE_FLY_IMAGE, L"图片飞翔控件，类似MAC的图片下载图片飞入效果，不同的是飞行路线是直线缓冲");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_GRADUAL_LIGHT, L"渐隐渐现的呼吸效果的控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_ROLL_ADVERTISEMENT, L"滚动广告栏控件");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_SPLITBAR, L"SplitBar分隔条控件对以对界面上的两块区域进行分隔，并且通过鼠标拖拽分隔条控件可以改变分隔的两块区域的大小");
	SetRegControl(pCtrlMap, L"UiFeature基本控件", CTRL_NAME_RICH_EDIT, L"Windows标准RichEdit控件");
	SetRegControl(pCtrlMap,	L"UiFeature基本控件", CTRL_NAME_CONTEXTMENU, L"鼠标右键环境菜单控件");
	SetRegControl(pCtrlMap,	L"UiFeature基本控件", CTRL_NAME_LEDCTRL, L"LEDCtrl可以用图片来代替字符，进而达到更加美观的效果，如播放器的时间等");
	SetRegControl(pCtrlMap,	L"UiFeature基本控件", CTRL_NAME_CONTEXTMENUITEM, L"鼠标右键环境菜单项");
	SetRegControl(pCtrlMap,	L"UiFeature基本控件", CTRL_NAME_SCROLLBAR, L"滚动条控件");
	SetRegControl(pCtrlMap,	L"UiFeature基本控件", CTRL_NAME_HWNDOBJ, L"让UIFeature的控件可以和其它标准的窗口混合使用的控件");
	SetRegControl(pCtrlMap,	L"UiFeature基本控件", CTRL_NAME_ANDROID_LIST, L"和安卓运行原理一样的List，采用可视化编辑界面，UI和数据分离的模式工作");
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
	CREATE_CONTROL(CTRL_NAME_BASE_PANEL, ICtrlPanelImpl);
	CREATE_CONTROL(CTRL_NAME_IMAGE_BOX, ICtrlImageBoxImpl);
	CREATE_CONTROL(CTRL_NAME_SHADOW_BUTTON, ICtrlShadowButtonImpl);
	CREATE_CONTROL(CTRL_NAME_STATIC, ICtrlStaticImpl);
	CREATE_CONTROL(CTRL_NAME_LINK, ICtrlLinkImpl);
	CREATE_CONTROL(CTRL_NAME_EDIT, ICtrlEditImpl);
	CREATE_CONTROL(CTRL_NAME_IE_WEB_BROWSER, ICtrlIEWebBrowserImpl);
	CREATE_CONTROL(CTRL_NAME_SHADOW_PROGRESS_BAR, ICtrlShadowProgressBarImpl);
	CREATE_CONTROL(CTRL_NAME_CHECK_BOX, ICtrlCheckBoxImpl);
	CREATE_CONTROL(CTRL_NAME_RADIO_BOX, ICtrlRadioBoxImpl);
	CREATE_CONTROL(CTRL_NAME_WINDOW_TITLE_BAR, ICtrlWindowTitleBarImpl);
	CREATE_CONTROL(CTRL_NAME_MAC_DOCK_BAR, ICtrlMacDockBarImpl);
	CREATE_CONTROL(CTRL_NAME_FOCUS_CTRL, ICtrlFocusImpl);
	CREATE_CONTROL(CTRL_NAME_STRAIGHTLINE_FLY_IMAGE, ICtrlStraightLineFlyImageImpl);
	CREATE_CONTROL(CTRL_NAME_GRADUAL_LIGHT, ICtrlGradualLightImpl);
	CREATE_CONTROL(CTRL_NAME_ROLL_ADVERTISEMENT, ICtrlRollAdvertisementImpl);
	CREATE_CONTROL(CTRL_NAME_RICH_EDIT, ICtrlRichEditImpl);
	CREATE_CONTROL(CTRL_NAME_CONTEXTMENU, ICtrlContextMenuImpl);
	CREATE_CONTROL(CTRL_NAME_CONTEXTMENUITEM, ICtrlContextMenuItemImpl);
	CREATE_CONTROL(CTRL_NAME_SCROLLBAR, ICtrlScrollBarImpl);
	CREATE_CONTROL(CTRL_NAME_ANDROID_LIST, ICtrlAndroidListImpl);
	CREATE_CONTROL(CTRL_NAME_HWNDOBJ, ICtrlHwndObjImpl);
	CREATE_CONTROL(CTRL_NAME_LEDCTRL, ICtrlLEDCtrlImpl);
	CREATE_CONTROL(CTRL_NAME_SPLITBAR, ICtrlSplitBarImpl);

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
	DELETE_CONTROL(CTRL_NAME_BASE_PANEL, ICtrlPanelImpl);
	DELETE_CONTROL(CTRL_NAME_IMAGE_BOX, ICtrlImageBoxImpl);
	DELETE_CONTROL(CTRL_NAME_SHADOW_BUTTON, ICtrlShadowButtonImpl);
	DELETE_CONTROL(CTRL_NAME_STATIC, ICtrlStaticImpl);
	DELETE_CONTROL(CTRL_NAME_LINK, ICtrlLinkImpl);
	DELETE_CONTROL(CTRL_NAME_EDIT, ICtrlEditImpl);
	DELETE_CONTROL(CTRL_NAME_IE_WEB_BROWSER, ICtrlIEWebBrowserImpl);
	DELETE_CONTROL(CTRL_NAME_SHADOW_PROGRESS_BAR, ICtrlShadowProgressBarImpl);
	DELETE_CONTROL(CTRL_NAME_CHECK_BOX, ICtrlCheckBoxImpl);
	DELETE_CONTROL(CTRL_NAME_RADIO_BOX, ICtrlRadioBoxImpl);
	DELETE_CONTROL(CTRL_NAME_WINDOW_TITLE_BAR, ICtrlWindowTitleBarImpl);
	DELETE_CONTROL(CTRL_NAME_MAC_DOCK_BAR, ICtrlMacDockBarImpl);
	DELETE_CONTROL(CTRL_NAME_FOCUS_CTRL, ICtrlFocusImpl);
	DELETE_CONTROL(CTRL_NAME_STRAIGHTLINE_FLY_IMAGE, ICtrlStraightLineFlyImageImpl);
	DELETE_CONTROL(CTRL_NAME_GRADUAL_LIGHT, ICtrlGradualLightImpl);
	DELETE_CONTROL(CTRL_NAME_ROLL_ADVERTISEMENT, ICtrlRollAdvertisementImpl);
	DELETE_CONTROL(CTRL_NAME_RICH_EDIT, ICtrlRichEditImpl);
	DELETE_CONTROL(CTRL_NAME_CONTEXTMENU, ICtrlContextMenuImpl);
	DELETE_CONTROL(CTRL_NAME_CONTEXTMENUITEM, ICtrlContextMenuItemImpl);
	DELETE_CONTROL(CTRL_NAME_SCROLLBAR, ICtrlScrollBarImpl);
	DELETE_CONTROL(CTRL_NAME_ANDROID_LIST, ICtrlAndroidListImpl);
	DELETE_CONTROL(CTRL_NAME_HWNDOBJ, ICtrlHwndObjImpl);
	DELETE_CONTROL(CTRL_NAME_LEDCTRL, ICtrlLEDCtrlImpl);
	DELETE_CONTROL(CTRL_NAME_SPLITBAR, ICtrlSplitBarImpl);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return false;
}
