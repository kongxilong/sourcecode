//////////////////////////////////////////////////////////////////////////
// ObjectId命名规则
// Window

#include "StdAfx.h"
#include "IWindowBaseImpl.h"
#include "IPropertySkinManagerImpl.h"
#include "IUiEffectManagerImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\ICtrlInterface.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <complex>
#include <time.h>
#include <mmsystem.h>

IPropertyGroup* IWindowBaseImpl::PP_GetWindowPropetryGroup()
{
	if (m_pXmlPropWindow == NULL)
		return NULL;

	return m_pXmlPropWindow->GetWindowPropGroup();
}

IPropertyWindow* IWindowBaseImpl::PP_GetWindowPropetry()
{
	return m_pXmlPropWindow;
}

// 将xml中的属性设置到manager中
void IWindowBaseImpl::PP_SetWindowPropetry(IPropertyWindow *pWndPropInXml, bool bSetDftProp)
{
	if (pWndPropInXml == NULL)
		return;

	m_pXmlPropWindow = pWndPropInXml;
	CreateWindowPropetry(bSetDftProp);
}

// 窗口Object名称
void IWindowBaseImpl::PP_SetWindowObjectName(LPCWSTR pszWndName)
{
	if (pszWndName == NULL || m_pPropBase_Name == NULL)
		return;

	m_pPropBase_Name->SetString(pszWndName);
	this->SetObjectName(pszWndName);
}

LPCWSTR IWindowBaseImpl::PP_GetWindowObjectName()
{
	if (m_pPropBase_Name == NULL)
		return NULL;

	return m_pPropBase_Name->GetString();
}

// 窗口名称
void IWindowBaseImpl::PP_SetWindowText(LPCWSTR pszWndText)
{
	if (pszWndText == NULL || m_pPropBase_WindowText == NULL)
		return;

	m_pPropBase_WindowText->SetString(pszWndText);
}

LPCWSTR IWindowBaseImpl::PP_GetWindowText()
{
	if (m_pPropBase_WindowText == NULL)
		return NULL;

	return m_pPropBase_WindowText->GetString();
}

// 是否支持全窗口点击移动
void IWindowBaseImpl::PP_SetDragWindow(bool bDrag)
{
	if (m_pPropDrag_Enable == NULL)
		return;

	m_pPropDrag_Enable->SetValue(bDrag);
}

bool IWindowBaseImpl::PP_GetDragWindow()
{
	if (m_pPropDrag_Enable == NULL)
		return false;

	return m_pPropDrag_Enable->GetValue();
}

void IWindowBaseImpl::SetFullScreen(bool bFull)
{
	m_bIsFullScreen = bFull;
}

bool IWindowBaseImpl::IsFullScreen()
{
	DWORD dwStyle = (DWORD)::GetWindowLong(m_hWnd, GWL_STYLE);
	bool bSysFullScreen = ((dwStyle & WS_MAXIMIZE) != 0);

	return (m_bIsFullScreen || bSysFullScreen);
}

IPropertyBase* IWindowBaseImpl::CreateWindowPublicPropetry(SKIN_FILE_ITEM* pSkinFileItem, IPropertyGroup* pGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo)
{
	if (pSkinFileItem == NULL || m_pSkinPropMgr == NULL || m_pXmlPropWindow == NULL || pszPropName == NULL || strlen(pszPropName) <= 0 || propType <= OTID_NONE || propType >= OTID_LAST)
		return NULL;

	if (pGroup == NULL)
		pGroup = m_pXmlPropWindow->GetWindowPropGroup();

	return CreateResourcePropetry(pSkinFileItem, this, m_pSkinPropMgr, pGroup, propType, pszPropName, pszPropInfo, true);
}

// 创建空的属性队列
void IWindowBaseImpl::CreateWindowPropetry(bool bSetDftProp)
{
	if (m_pXmlPropWindow == NULL)
		return;

	SKIN_FILE_ITEM* pSkinFileItem = this->GetSkinFileItem();
	if (pSkinFileItem == NULL)
		return;

	// Group:base
	m_pPropGroupBase = (IPropertyGroup*)CreateWindowPublicPropetry(pSkinFileItem, NULL, OTID_GROUP, "WindowBase", "窗口/面板基本属性");
	if (m_pPropGroupBase == NULL)
		return;

	// base-objectid
	m_pPropBase_ObjectId = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_INT, NAME_SKIN_PROP_NAME_OBJ_ID, "当前 Object Id");
	if (m_pPropBase_ObjectId == NULL)
		return;
	m_pPropBase_ObjectId->SetValue((int)m_pXmlPropWindow->GetObjectId());

	// base-类型名称
	m_pPropBase_TypeName = (IPropertyString*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_STRING, NAME_SKIN_PROP_NAME_TYPE, "当前 Object 类型");
	if (m_pPropBase_TypeName == NULL)
		return;
	m_pPropBase_TypeName->SetString(_T(PROP_TYPE_WINDOW_NAME));

	// base-name
	m_pPropBase_Name = (IPropertyString*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_STRING, NAME_SKIN_PROP_NAME, "当前 Object 名称");
	if (m_pPropBase_Name == NULL)
		return;
	if (m_pPropBase_Name->GetString() != NULL && m_pPropBase_Name->GetLength() <= 0)
		m_pPropBase_Name->SetString(L"新建窗口/面板");

	// base-windowtitle
	m_pPropBase_WindowText = (IPropertyString*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_STRING, "WindowText", "当前窗口标题");
	if (m_pPropBase_WindowText == NULL)
		return;
	if (m_pPropBase_WindowText->GetString() != NULL && m_pPropBase_WindowText->GetLength() <= 0)
		m_pPropBase_WindowText->SetString(L"无窗口标题");

	// base-在任务栏显示按钮
	m_pPropBase_ShowInTaskbar = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "ShowInTaskbar", "是否在任务栏显示按钮");
	if (m_pPropBase_ShowInTaskbar == NULL)
		return;

	// base-支持分层窗口
	m_pPropBase_Layered = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "LayeredWindow", "是否支持分层窗口");
	if (m_pPropBase_Layered == NULL)
		return;

	// base-topmost
	m_pPropBase_TopMost = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "TopMost", "窗口是否在最上层");
	if (m_pPropBase_TopMost == NULL)
		return;

	// base-TemporaryDrawMem
	m_pPropBase_TempDrawMem = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "TempDrawMem", "窗口是否使用临时内存绘制（注意：不包括控件的绘制内存）。是：在WM_PAINT消息执行完毕之后就释放内存（可降低内存使用）；否：在WM_PAINT消息执行完毕之后还保有内存下次使用。");
	if (m_pPropBase_TempDrawMem == NULL)
		return;

	// base-BigTabOrder 对话框中最大TabOrder值
	m_pPropBase_BigTabOrder = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupBase, OTID_INT, "BigTabOrder", "窗口所有控件中最大TabOrder值。");
	if (m_pPropBase_BigTabOrder == NULL)
		return;

	// Group-WindowSize
	m_pPropGroupWindowSize = (IPropertyGroup*)CreateWindowPublicPropetry(pSkinFileItem, NULL, OTID_GROUP, "WindowSize", "窗口/面板大小");
	if (m_pPropGroupWindowSize == NULL)
		return;

	// size-width
	m_pPropSize_WindowWidth = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupWindowSize, OTID_INT, NAME_WINDOW_WIDTH, "窗口宽度");
	if (m_pPropSize_WindowWidth == NULL)
		return;

	// size-height
	m_pPropSize_WindowHeight = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupWindowSize, OTID_INT, NAME_WINDOW_HEIGHT, "窗口高度");
	if (m_pPropSize_WindowHeight == NULL)
		return;

	// Group-drag(拖拽窗口)
	m_pPropGroupDrag = (IPropertyGroup*)CreateWindowPublicPropetry(pSkinFileItem, NULL, OTID_GROUP, "Drag", "拖拽窗口");
	if (m_pPropGroupDrag == NULL)
		return;

	// drag-enable
	m_pPropDrag_Enable = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupDrag, OTID_BOOL, "Enable", "是否可以拖动窗口，随处移动");
	if (m_pPropDrag_Enable == NULL)
		return;

	// Group-Size(窗口的变化大小)
	m_pPropGroupSize = (IPropertyGroup*)CreateWindowPublicPropetry(pSkinFileItem, NULL, OTID_GROUP, "WindowMaxMinSize", "窗口变化的尺寸");
	if (m_pPropGroupSize == NULL)
		return;

	// Size-enable
	m_pPropSize_Enable = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupSize, OTID_BOOL, "Enable", "是否设置窗口的最大尺寸和最小尺寸");
	if (m_pPropSize_Enable == NULL)
		return;

	// Size-MaxWidth
	m_pPropSize_MaxWidth = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupSize, OTID_INT, "MaxWidth", "窗口最大宽度，单位：像素。如果设置为 -1，那系统无视此属性。");
	if (m_pPropSize_MaxWidth == NULL)
		return;

	// Size-MaxHeight
	m_pPropSize_MaxHeight = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupSize, OTID_INT, "MaxHeight", "窗口最大高度，单位：像素。如果设置为 -1，那系统无视此属性。");
	if (m_pPropSize_MaxHeight == NULL)
		return;

	// Size-MinWidth
	m_pPropSize_MinWidth = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupSize, OTID_INT, "MinWidth", "窗口最小宽度，单位：像素。如果设置为 -1，那系统无视此属性。");
	if (m_pPropSize_MinWidth == NULL)
		return;

	// Size-MinHeight
	m_pPropSize_MinHeight = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupSize, OTID_INT, "MinHeight", "窗口最小高度，单位：像素。如果设置为 -1，那系统无视此属性。");
	if (m_pPropSize_MinHeight == NULL)
		return;

	// Group-stretching(拉伸窗口)
	m_pPropGroupStretching = (IPropertyGroup*)CreateWindowPublicPropetry(pSkinFileItem, NULL, OTID_GROUP, "Stretching", "拉伸窗口");
	if (m_pPropGroupStretching == NULL)
		return;

	// stretching-enable
	m_pPropStretching_Enable = (IPropertyBool*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupStretching, OTID_BOOL, "Enable", "是否可以拉伸窗口，使窗口可以变大变小");
	if (m_pPropStretching_Enable == NULL)
		return;

	// stretching-leftspace
	m_pPropStretching_LeftSpace = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupStretching, OTID_INT, "LeftSpace", "拉伸窗口，窗口左侧鼠标探测范围");
	if (m_pPropStretching_LeftSpace == NULL)
		return;

	// stretching-rightspace
	m_pPropStretching_RightSpace = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupStretching, OTID_INT, "RightSpace", "拉伸窗口，窗口右侧鼠标探测范围");
	if (m_pPropStretching_RightSpace == NULL)
		return;

	// stretching-topspace
	m_pPropStretching_TopSpace = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupStretching, OTID_INT, "TopSpace", "拉伸窗口，窗口上方鼠标探测范围");
	if (m_pPropStretching_TopSpace == NULL)
		return;

	// stretching-bottomspace
	m_pPropStretching_BottomSpace = (IPropertyInt*)CreateWindowPublicPropetry(pSkinFileItem, m_pPropGroupStretching, OTID_INT, "BottomSpace", "拉伸窗口，窗口下方鼠标探测范围");
	if (m_pPropStretching_BottomSpace == NULL)
		return;

	if (bSetDftProp)
	{
		// 设置默认值
		if (m_pPropBase_WindowText != NULL)
			m_pPropBase_WindowText->SetString(L"窗口标题");
		if (m_pPropBase_ShowInTaskbar != NULL)
			m_pPropBase_ShowInTaskbar->SetValue(true);
		if (m_pPropBase_Layered != NULL)
			m_pPropBase_Layered->SetValue(false);
		if (m_pPropBase_TopMost != NULL)
			m_pPropBase_TopMost->SetValue(false);
		if (m_pPropSize_WindowWidth != NULL)
			m_pPropSize_WindowWidth->SetValue(500);
		if (m_pPropSize_WindowHeight != NULL)
			m_pPropSize_WindowHeight->SetValue(500);
		if (m_pPropDrag_Enable != NULL)
			m_pPropDrag_Enable->SetValue(true);
		if (m_pPropStretching_Enable != NULL)
			m_pPropStretching_Enable->SetValue(false);
		if (m_pPropStretching_LeftSpace != NULL)
			m_pPropStretching_LeftSpace->SetValue(0);
		if (m_pPropStretching_RightSpace != NULL)
			m_pPropStretching_RightSpace->SetValue(0);
		if (m_pPropStretching_TopSpace != NULL)
			m_pPropStretching_TopSpace->SetValue(0);
		if (m_pPropStretching_BottomSpace != NULL)
			m_pPropStretching_BottomSpace->SetValue(0);
		if (m_pPropSize_Enable != NULL)
			m_pPropSize_Enable->SetValue(false);
		if (m_pPropSize_MaxWidth != NULL)
			m_pPropSize_MaxWidth->SetValue(0);
		if (m_pPropSize_MaxHeight != NULL)
			m_pPropSize_MaxHeight->SetValue(0);
		if (m_pPropSize_MinWidth != NULL)
			m_pPropSize_MinWidth->SetValue(0);
		if (m_pPropSize_MinHeight != NULL)
			m_pPropSize_MinHeight->SetValue(0);
		if (m_pPropBase_TempDrawMem != NULL)
			m_pPropBase_TempDrawMem->SetValue(true);
	}
}

bool IWindowBaseImpl::PP_GetLayeredWindow()
{
	if (m_pPropBase_Layered == NULL)
		return false;

	return m_pPropBase_Layered->GetValue();
}

// 设置窗体的透明特性
void IWindowBaseImpl::PP_SetLayeredWindow(bool bIsLayered)
{
	if (m_pPropBase_Layered == NULL)
		return;
	m_pPropBase_Layered->SetValue(bIsLayered);

	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (bIsLayered)
	{
		// 透明
		if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
		{
			dwExStyle |= WS_EX_LAYERED;
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
		}
	}
	else
	{
		// 不透明
		if ((dwExStyle & WS_EX_LAYERED) == WS_EX_LAYERED)
		{
			dwExStyle &= (~WS_EX_LAYERED);
			::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
		}
	}
}

// 取得子控件的属性列表：Layout.xml 中的布局
PROP_CONTROL_VEC* IWindowBaseImpl::GetChildPropControlVec()
{
	if (m_pXmlPropWindow == NULL)
		return NULL;

	return m_pXmlPropWindow->GetChildPropControlVec();
}

// Builder有属性修改，重新刷新界面
void IWindowBaseImpl::BD_RefreshWindowPropetry()
{
	// 重新计算所有控件的位置
	if (m_pPropSize_WindowWidth == NULL || m_pPropSize_WindowHeight == NULL)
		return;
	OnSize(-1, m_pPropSize_WindowWidth->GetValue(), m_pPropSize_WindowHeight->GetValue());
}

// 取得窗口控件指针
IControlBase* IWindowBaseImpl::GetControlByObjectId(DWORD dwObjId)
{
	return GetSubControlByObjectId(dwObjId, m_ChildCtrlsVec);
}

// 删除一个子控件
bool IWindowBaseImpl::DeleteSubControl(IControlBase* pDelCtrl)
{
	if (m_pRegFocusCtrl != NULL && m_pRegFocusCtrl->CompareControl(pDelCtrl))
		m_pRegFocusCtrl = NULL;

	bool bRet = DeleteSubControlFromVec(&m_ChildCtrlsVec, pDelCtrl);

	// 删除控件后需要清空相关的控件指针
	ResetActiveControl();

	return bRet;
}

bool IWindowBaseImpl::DeleteSubControlFromVec(CHILD_CTRLS_VEC *pCtrlVec, IControlBase* pDelCtrl)
{
	if (pDelCtrl == NULL || pCtrlVec == NULL || m_pSkinPropMgr == NULL)
		return false;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pCtrlVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pCtrlVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (pCtrl == pDelCtrl)
		{
			// 从队列中删除
			pCtrlVec->DelObj(pItem);

			// 删除子控件下的子控件
			ReleaseChildVec(pCtrl->GetChildControlsVec());

			pCtrl->OnDestroy();
			ICtrlInterface* pCtrlInf = dynamic_cast<ICtrlInterface*>(pCtrl);
			if (pCtrlInf != NULL && pCtrlInf->GetControlDllManager() != NULL)
			{
				pCtrlInf->GetControlDllManager()->ReleaseCtrl(&pCtrlInf);
			}
			else
			{
				SAFE_DELETE(pCtrl);
			}

			return true;
		}

		if (DeleteSubControlFromVec(pCtrl->GetChildControlsVec(), pDelCtrl))
			return true;
	}

	return false;
}

// new child 创建子控件
IControlBase* IWindowBaseImpl::CreateSubControl(IControlMessage* pCtrlMsg, IControlBase* pParentCtrl, LPCWSTR pszNewCtrlTypeName, LPCWSTR pszNewCtrlObjectName, bool bIsCreate, int nSubCtrlId)
{
	if (pCtrlMsg == NULL || pParentCtrl == NULL || pszNewCtrlTypeName == NULL || wcslen(pszNewCtrlTypeName) <= 0
	|| pszNewCtrlObjectName == NULL || wcslen(pszNewCtrlObjectName) <= 0
	|| m_pUiKernel == NULL || nSubCtrlId <= NOT_BY_CREATE_SUB_CONTROL)
		return NULL;

	// 取得已经创建的控件
	IControlBase* pNewCtrl = NULL;
	CStringW strDataW = pszNewCtrlObjectName;
	GetControlByNameFromVec(pParentCtrl->GetChildControlsVec(), strDataW, &pNewCtrl);
	if (pNewCtrl == NULL && bIsCreate)
	{
		// 创建新控件
		pNewCtrl = m_pUiKernel->CreateControlEmptyPropetry(GetSkinFileItem(), dynamic_cast<IWindowBase*>(this), pParentCtrl,
			pszNewCtrlTypeName, pszNewCtrlObjectName, NULL, pParentCtrl->IsCloneControl());
		if (pNewCtrl == NULL)
			return NULL;
	}

	if (pNewCtrl != NULL)
	{
		pNewCtrl->SetControlMessage(pCtrlMsg);
		if (pNewCtrl->m_pPropBase_Name != NULL)
			pNewCtrl->m_pPropBase_Name->EnableInBuilder(false);

		pNewCtrl->m_nSubCtrlId = nSubCtrlId;
	}

	return pNewCtrl;
}

// 是否窗口置顶
bool IWindowBaseImpl::IsTopmost()
{
	if (m_pPropBase_TopMost == NULL)
		return false;

	return m_pPropBase_TopMost->GetValue();
}

// 滚动条消息
void IWindowBaseImpl::OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar)
{

}

void IWindowBaseImpl::OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar)
{

}

// 停止绘制窗口
void IWindowBaseImpl::StopDrawWindow(bool bStop)
{
	m_bStopDrawWindow = bStop;
}

// 使用发消息的方式重绘窗口
void IWindowBaseImpl::RedrawWindowInNextMessage()
{
	this->PostMessage(UI_FEATURE_KERNEL_MSG, WID_REDRAWWINDOW, NULL);
}
//////////////////////////////////////////////////////////////////////////
// 动画
// 清空动画资源
void IWindowBaseImpl::ClearAnimation()
{
	if (m_hCtrlAnimateThread != NULL)
		return;

	m_pUserDefAnimationCtrl = NULL;
	if (m_pUiEffect != NULL)
	{
		// 清空原有设置的系统内置动画
		m_pUiEffect->ClearAllAnimation();
		IUiEffectManagerImpl::GetInstance()->ReleaseAnimation(m_pUiEffect);
		m_pUiEffect = NULL;
	}
}

// 添加一个控件动画
bool IWindowBaseImpl::AppendAnimationControl(IControlBase* pCtrl)
{
	// 已经启动动画了，不能再增加动画控件
	if (pCtrl == NULL || m_hCtrlAnimateThread != NULL)
		return false;

	// 如果设置了用户自定义强动画，同时只允许启动一个用户自定义强动画
	if (pCtrl->PP_GetCtrlAnimationType() <= 0 || m_pUserDefAnimationCtrl != NULL)
		return false;

	if (pCtrl->PP_GetCtrlAnimationType() == 1)
	{
		// 自定义强动画
		m_pUserDefAnimationCtrl = pCtrl;
		if (m_pUiEffect != NULL)
		{
			// 清空原有设置的系统内置动画
			m_pUiEffect->ClearAllAnimation();
			IUiEffectManagerImpl::GetInstance()->ReleaseAnimation(m_pUiEffect);
			m_pUiEffect = NULL;
		}
		return true;
	}
	else
	{
		// 系统内置动画
		return pCtrl->ControlAnimationReady();
	}
}

// 开始动画
bool IWindowBaseImpl::AppendAnimation(IControlBase* pCtrl, CDrawingImage& CtrlAnimationMemDc,
											DWORD animType, int nFrameTime, int nAnimationDirection)
{
	if (pCtrl == NULL || !pCtrl->IsVisible() || CtrlAnimationMemDc.GetSafeHdc() == NULL || animType == 0)
		return false;

	if (m_pUiEffect == NULL)
		m_pUiEffect = IUiEffectManagerImpl::GetInstance()->GetAnimation();

	if (m_pUiEffect == NULL)
		return false;

	AnimationParam animParam;
	animParam.effectKey = (WPARAM)pCtrl;
	animParam.animationEffect = animType;
	animParam.animationFrequency = nFrameTime;
	animParam.bShow = (nAnimationDirection == 0);
	animParam.hBitmap = CtrlAnimationMemDc.GetBmpHandle();
	animParam.pBmpData = CtrlAnimationMemDc.GetBits();
	animParam.bmpSize = CtrlAnimationMemDc.GetDcSize();
	animParam.hdc = CtrlAnimationMemDc.GetSafeHdc();

	return (m_pUiEffect->AppendAnimation(animParam) == TRUE);
}

// 开始控件动画
void IWindowBaseImpl::DoControlAnimation()
{
	// 动画开始时，关闭tips
	this->DestroySystemTips();
	if (m_hCtrlAnimateThread == NULL)
	{
		m_hCtrlAnimateThread = ::CreateThread(NULL, 0, ControlAnimationThread, (LPVOID)this, 0, NULL);
		if (m_hCtrlAnimateThread == NULL)
			ClearAnimation();
	}
}

DWORD WINAPI IWindowBaseImpl::ControlAnimationThread(LPVOID lpParam)
{
	IWindowBaseImpl* pThis = (IWindowBaseImpl*)lpParam;
	if (pThis == NULL)
		return -1;

	// 用户自定义强动画开始
	if (pThis->m_pUserDefAnimationCtrl != NULL)
		pThis->m_pUserDefAnimationCtrl->OnUserDefHighAnimationBegin();

	// 动画绘制
	pThis->DrawControlAnimation();

	if (pThis->m_pUiEffect != NULL)
	{
		IUiEffectManagerImpl::GetInstance()->ReleaseAnimation(pThis->m_pUiEffect);
		pThis->m_pUiEffect = NULL;
	}
	SAFE_CLOSE_HANDLE(pThis->m_hCtrlAnimateThread);

	// 用户自定义强动结束
	if (pThis->m_pUserDefAnimationCtrl != NULL)
		pThis->m_pUserDefAnimationCtrl->OnUserDefHighAnimationEnd();

	pThis->m_pUserDefAnimationCtrl = NULL;

	// 窗口动画线程结束通知
	pThis->PostMessage(UI_FEATURE_KERNEL_MSG, WID_ANIMATION_THREAD_END, NULL);
	return 0;
}

// 播放动画消息
void IWindowBaseImpl::DrawControlAnimation()
{
	m_hAnimationWndDc = ::GetDC(m_hWnd);
	if (m_hAnimationWndDc == NULL)
		return;

	if (m_pUserDefAnimationCtrl == NULL)
	{
		if (m_pUiEffect == NULL)
			return;
	}

	// 提高线程优先级
	HANDLE hThreadMe = ::GetCurrentThread();
	INT priority = ::GetThreadPriority(hThreadMe);
	::SetThreadPriority(hThreadMe, THREAD_PRIORITY_HIGHEST);

	//设置最小时间周期
	TIMECAPS ptc={0,0};
	MMRESULT mr = MMSYSERR_ERROR;
	mr = timeGetDevCaps(&ptc, sizeof(ptc));
	if (mr != MMSYSERR_ERROR)
		mr = timeBeginPeriod(ptc.wPeriodMin);

//////////////////////////////////////////////////////////////////////////
	StopDrawWindow(true);

	// 开始绘制动画
	if (m_pUserDefAnimationCtrl != NULL)
	{
		if (m_pUserDefAnimationCtrl->m_pCtrlAnimationMemDc == NULL)
			m_pUserDefAnimationCtrl->m_pCtrlAnimationMemDc = new CDrawingImage;

		// 用户自定义强动画
		if (m_pUserDefAnimationCtrl->m_pCtrlAnimationMemDc != NULL)
			m_pUserDefAnimationCtrl->OnUserDefHighAnimationDraw(*(m_pUserDefAnimationCtrl->m_pCtrlAnimationMemDc), m_hAnimationWndDc);
	}
	else
	{
		// 系统内置动画
		if (m_pUiEffect != NULL)
			m_pUiEffect->Animation(dynamic_cast<IUIEffectCallBack*>(this), -1);
	}

	if (m_hAnimationWndDc != NULL)
	{
		::ReleaseDC(m_hWnd, m_hAnimationWndDc);
		m_hAnimationWndDc = NULL;
	}

	StopDrawWindow(false);
//////////////////////////////////////////////////////////////////////////

	::SetThreadPriority(hThreadMe, priority);

	//恢复最小时间周期
	if (mr != MMSYSERR_ERROR)
		timeEndPeriod(ptc.wPeriodMin);
}

// 当一个动画开始画第一帧之前，先发送动画开始的消息
void IWindowBaseImpl::OnUiEffectBegin(WPARAM effectKey, DWORD animaType)
{
	IControlBase* pCtrl = (IControlBase*)effectKey;
	if (pCtrl == NULL)
		return;

	pCtrl->OnCtrlAnimation(CMB_ANIMATION_BEGIN, animaType);
}

// 当一个动画的最后一帧绘制完毕，发送动画结束的消息
void IWindowBaseImpl::OnUiEffectEnd(WPARAM effectKey, DWORD animaType)
{
	IControlBase* pCtrl = (IControlBase*)effectKey;
	if (pCtrl == NULL)
		return;

	pCtrl->OnCtrlAnimation(CMB_ANIMATION_END, animaType);
}

// 所有动画资源都计算完毕，开始绘制动画
void IWindowBaseImpl::OnUiEffectDraw()
{
	OnPaint(m_hAnimationWndDc, NULL);
}
//////////////////////////////////////////////////////////////////////////

// 清空绘制缓存
void IWindowBaseImpl::ClearDrawMemoryDc()
{
	// 清空整个窗口的绘制缓存
	this->PostMessage(UI_FEATURE_KERNEL_MSG, WID_CLEAR_DRAW_MEM_DC, NULL);
}

// 清空子控件绘制缓存
void IWindowBaseImpl::ClearCtrlDrawMemoryDc(CHILD_CTRLS_VEC *pCtrlVec)
{
	if (pCtrlVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pCtrlVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pCtrlVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		ClearCtrlDrawMemoryDc(pCtrl->GetChildControlsVec());
		pCtrl->ClearDrawMemoryDc();
	}
}

// 显示窗口
void IWindowBaseImpl::OnShowWindow(bool bIsShow, int nShowStatus)
{
	if (!bIsShow)
	{
		// 窗口隐藏的时候清空绘制内存
		ClearDrawMemoryDc();
	}
}

// 是否使用临时内存绘制
bool IWindowBaseImpl::PP_IsUseTempDrawMem()
{
	if (m_pPropBase_TempDrawMem == NULL)
		return false;

	return m_pPropBase_TempDrawMem->GetValue();
}

bool IWindowBaseImpl::PP_IsSetMinMaxInfo()
{
	if (m_pPropSize_Enable == NULL)
		return false;

	return m_pPropSize_Enable->GetValue();
}

// 窗口是否可以拖拽四边、四角进行拉伸
bool IWindowBaseImpl::PP_IsCanStretchWindow()
{
	if (m_pPropStretching_Enable == NULL)
		return false;

	return m_pPropStretching_Enable->GetValue();
}

void IWindowBaseImpl::SetMaximizeWndDragMove(bool bMove)
{
	m_bMaximizeWndDragMove = bMove;
}

bool IWindowBaseImpl::GetMaximizeWndDragMove()
{
	return m_bMaximizeWndDragMove;
}
