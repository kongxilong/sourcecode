
#include "stdafx.h"
#include "IWindowBaseImpl.h"
#include <shobjidl.h>
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IPropertyControl.h"
#include "..\..\Inc\IPropertyCursor.h"
#include "..\..\Inc\ICtrlInterface.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "IPropertySkinManagerImpl.h"
#include "IUiFeatureKernelImpl.h"
#include "CriSec.h"

// 绘制窗口和被选中的控件的边框的8个方块的宽度
#define FANGKUAI_SIZE							(6)


IWindowBase::IWindowBase(IUiFeatureKernel * pUiKernel) : IFeatureObject(pUiKernel)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IWindowBaseImpl::IWindowBaseImpl(IUiFeatureKernel * pUiKernel) : IWindowBase(pUiKernel),
m_BuilderWndFrameImageBase(pUiKernel),
m_BuilderCtrlFrameImageBase(pUiKernel)
{
	SetObjectType(_T(PROP_TYPE_WINDOW_NAME));

	m_pSysTips = NULL;
	m_pUiEffect = NULL;
	m_hCtrlAnimateThread = NULL;
	m_hAnimationWndDc = NULL;
	m_pUserDefAnimationCtrl = NULL;

	m_hPreviousCursor = NULL;
	m_nHitType = HTNOWHERE;
	m_Blend.BlendOp = AC_SRC_OVER;
	m_Blend.BlendFlags = 0;
	m_Blend.AlphaFormat = AC_SRC_ALPHA;
	m_Blend.SourceConstantAlpha = 255;

	m_pCtrlMsgCallBack = NULL;
	m_hParent = NULL;
	m_hWnd = NULL;
	m_pHighTimer = NULL;
	m_ChildCtrlsVec.Clear();

	m_CtrlRegMsgMap.clear();
	m_bIsLButtonDown = false;
	m_pLButtonDownCtrl = NULL;
	m_pMouseHoverCtrl = NULL;
	m_pFocusCtrl = NULL;
	m_pRegFocusCtrl = NULL;
	m_pTabCtrl = NULL;
	m_nCurMouseStyle = UF_IDC_ARROW;
	m_bStopDrawWindow = false;
	m_bMouseDragging = false;
	m_bMaximizeWndDragMove = false;

	memset(&m_BD_FangKuai8, 0, sizeof(FANGKUAI_8));

	m_strSkinPath = L"";
	m_strWindowObjectName = L"";
	m_pCurrentCursorBase = NULL;
	m_pWindowMessageCtrl = NULL;

	if (m_pUiKernel != NULL)
	{
		m_pSkinPropMgr = m_pUiKernel->GetSkinManager();
		m_pUiEngine = m_pUiKernel->GetUiEngine();
	}

	m_bIsFullScreen = false;
	m_bMouseLeave = true;

	m_bLButtonDownPoint.x = m_bLButtonDownPoint.y = -10;

	m_pXmlPropWindow = NULL;
	// Group:base
	m_pPropGroupBase = NULL;
	// base-类型名称
	m_pPropBase_TypeName = NULL;
	// base-objectid
	m_pPropBase_ObjectId = NULL;
	// base-name
	m_pPropBase_Name = NULL;
	// base-windowtitle
	m_pPropBase_WindowText = NULL;
	// base-在任务栏显示按钮
	m_pPropBase_ShowInTaskbar = NULL;
	// base-支持分层窗口
	m_pPropBase_Layered = NULL;
	// base-topmost
	m_pPropBase_TopMost = NULL;
	// base-TemporaryDrawMem
	m_pPropBase_TempDrawMem = NULL;
	m_pPropBase_BigTabOrder = NULL;

	// Group-size
	m_pPropGroupWindowSize = NULL;
	// size-width
	m_pPropSize_WindowWidth = NULL;
	// size-height
	m_pPropSize_WindowHeight = NULL;

	// Group-drag(拖拽窗口)
	m_pPropGroupDrag = NULL;
	// drag-enable
	m_pPropDrag_Enable = NULL;

	// Group-Size(窗口的大小)
	m_pPropGroupSize = NULL;
	m_pPropSize_Enable = NULL;
	// Size-leftspace
	m_pPropSize_MaxWidth = NULL;
	// Size-rightspace
	m_pPropSize_MaxHeight = NULL;
	// Size-topspace
	m_pPropSize_MinWidth = NULL;
	// Size-bottomspace
	m_pPropSize_MinHeight = NULL;

	// Group-stretching(拉伸窗口)
	m_pPropGroupStretching = NULL;
	// stretching-enable
	m_pPropStretching_Enable = NULL;
	// stretching-leftspace
	m_pPropStretching_LeftSpace = NULL;
	// stretching-rightspace
	m_pPropStretching_RightSpace = NULL;
	// stretching-topspace
	m_pPropStretching_TopSpace = NULL;
	// stretching-bottomspace
	m_pPropStretching_BottomSpace = NULL;
}

IWindowBaseImpl::~IWindowBaseImpl()
{
	ReleaseChildVec(&m_ChildCtrlsVec);
	if (m_pXmlPropWindow != NULL)
		m_pXmlPropWindow->ClearPropertyOwner();

	SAFE_DELETE(m_pSysTips);
	SAFE_DELETE(m_pHighTimer);
}

// 释放子控件列表
void IWindowBaseImpl::ReleaseChildVec(CHILD_CTRLS_VEC *pChildCtrlsVec)
{
	if (pChildCtrlsVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		// 释放子控件
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
	}
	pChildCtrlsVec->Clear();
}

// 当窗口的属性发生变化时需要通知窗口进行刷新时调用
void IWindowBaseImpl::RefreshWindowStyle()
{
	// 会触发 WM_NCCALCSIZE 消息
	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
}

// 内部接受到初始化消息，开始初始化窗口，加载皮肤
bool IWindowBaseImpl::OnInitWindowBase()
{
	if (m_pUiKernel == NULL || m_pSkinPropMgr == NULL)
	{
		// 向窗口发送通知：初始化皮肤异常
		::PostMessage(m_hWnd, UI_FEATURE_KERNEL_MSG, WID_INIT_WINDOW_ERROR, -1);
		return false;
	}

//// 设置对话框基本样式 //////////////////////////////////////////////////////////////////////
//	// 取得对话框基本样式属性
//	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
//	dwStyle &= (~WS_POPUP);
//	dwStyle &= (~WS_DLGFRAME);
//	// 对话框基本样式属性
//	dwStyle |= WS_OVERLAPPED | WS_THICKFRAME;
//	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
//////////////////////////////////////////////////////////////////////////

	// 初始化窗口皮肤
	IPropertyWindow* pWndProp = m_pSkinPropMgr->PG_InitWindowSkin(m_strSkinPath, m_strWindowObjectName);
	if (pWndProp == NULL)
	{
		// 向窗口发送通知：初始化皮肤异常
		::PostMessage(m_hWnd, UI_FEATURE_KERNEL_MSG, WID_INIT_WINDOW_ERROR, -2);
		return false;
	}

	// 设置对话框属性
	PP_SetWindowPropetry(pWndProp, false);

	// 创建控件
	if (!CreateChildCtrlVec(NULL, GetChildPropControlVec(), &m_ChildCtrlsVec))
	{
		// 向窗口发送通知：创建控件失败
		::PostMessage(m_hWnd, UI_FEATURE_KERNEL_MSG, WID_INIT_WINDOW_ERROR, -3);
		return false;
	}

//////////////////////////////////////////////////////////////////////////
	// 初始化拉伸窗口操作
	m_WndResize.InitResizeInfo(this, m_pPropBase_Layered,
		m_pPropSize_Enable, m_pPropSize_MaxWidth, m_pPropSize_MaxHeight, m_pPropSize_MinWidth, m_pPropSize_MinHeight,
		m_pPropStretching_Enable, m_pPropStretching_LeftSpace, m_pPropStretching_RightSpace, m_pPropStretching_TopSpace, m_pPropStretching_BottomSpace);

	// 根据窗口基本属性，设置窗口基本样式
	SetWindowStyleByProp();

	// 向窗口发送通知：初始化皮肤完成
	::PostMessage(m_hWnd, UI_FEATURE_KERNEL_MSG, WID_INIT_WINDOW_SUCCESS, NULL);

	SendWindowInitOkMsgToCtrl(&m_ChildCtrlsVec);
	return true;
}

// 向所有控件发送对话框初始化完成的消息
void IWindowBaseImpl::SendWindowInitOkMsgToCtrl(CHILD_CTRLS_VEC *pChildCtrlsVec)
{
	if (pChildCtrlsVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		pCtrl->OnWindowFinalCreate();
		SendWindowInitOkMsgToCtrl(pCtrl->GetChildControlsVec());
	}
}

// 根据窗口基本属性，设置窗口基本样式
void IWindowBaseImpl::SetWindowStyleByProp()
{
	// 设置窗体的透明特性
	SetWindowTransparence(PP_GetLayeredWindow());

	// 窗口标题
	if (m_pPropBase_WindowText != NULL)
		::SetWindowTextW(this->GetSafeHandle(), m_pPropBase_WindowText->GetString());

	// 设置窗口置顶
	if (m_pPropBase_TopMost != NULL)
	{
		SIZE wndSize = this->PP_GetWindowPropSize();
		::SetWindowPos(this->GetSafeHandle(), (m_pPropBase_TopMost->GetValue() ? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, wndSize.cx, wndSize.cy, SWP_NOSIZE | SWP_NOMOVE);
	}

	// 设置窗口在任务栏是否显示按钮
	if (m_pPropBase_ShowInTaskbar != NULL && !m_pPropBase_ShowInTaskbar->GetValue())
		HideInTaskbar();

	// 当窗口的属性发生变化时需要通知窗口进行刷新
	RefreshWindowStyle();
}

// 设置窗体的透明特性
void IWindowBaseImpl::SetWindowTransparence(bool bIsTrans)
{
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (bIsTrans)
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

// 创建控件
bool IWindowBaseImpl::CreateChildCtrlVec(IControlBase *pParentCtrl, PROP_CONTROL_VEC* pPropCtrlVec, CHILD_CTRLS_VEC* pCtrlVec)
{
	if (pPropCtrlVec == NULL || pCtrlVec == NULL || m_pUiKernel == NULL)
		return false;

	IWindowBase* pBaseThis = dynamic_cast<IWindowBase*>(this);
	if (pBaseThis == NULL)
		return false;

	for (PROP_CONTROL_VEC::iterator pCtrlItem = pPropCtrlVec->begin(); pCtrlItem != pPropCtrlVec->end(); pCtrlItem++)
	{
		IPropertyControl* pPropCtrl = *pCtrlItem;
		if (pPropCtrl == NULL)
			continue;

		IControlBase *pNewCtrl = m_pUiKernel->CreateControlByPropetry(pBaseThis, pParentCtrl, pPropCtrl);
		if (pNewCtrl == NULL)
			return false;

		if (!CreateChildCtrlVec(pNewCtrl, pPropCtrl->GetChildPropControlVec(), pNewCtrl->GetChildControlsVec()))
			return false;

		// new child 这里是运行时态加载子控件，没有创建操作
		pNewCtrl->CreateSubControl(false);
	}

	return true;
}

// 取得窗口控件指针
IControlBase* IWindowBaseImpl::GetControl(char *pszCtrlName)
{
	return GetSubControlByName(pszCtrlName, m_ChildCtrlsVec);
}

// 取得窗口控件指针
IControlBase* IWindowBaseImpl::GetControl(LPCWSTR pszCtrlName)
{
	return GetSubControlByName(pszCtrlName, m_ChildCtrlsVec);
}

// 取得窗口控件指针
IControlBase* IWindowBaseImpl::GetControl(WCHAR *pszCtrlName)
{
	return GetSubControlByName(pszCtrlName, m_ChildCtrlsVec);
}

// 初始化：由外部调用，内部发送开始初始化的消息真正开始初始化
bool IWindowBaseImpl::PG_InitWindowBase(HWND hWnd, LPCWSTR pszSkinPath, LPCWSTR pszWndName)
{
	if (m_hWnd != NULL || hWnd == NULL || pszSkinPath == NULL || pszWndName == NULL)
		return false;

	// 设置参数，通过句柄，外部可以得到 IWindowBase 指针
	IWindowBase* pThis = dynamic_cast<IWindowBase*>(this);
	::SetPropA(m_hWnd, "uifeaturewndpoint", (HANDLE)pThis);

	m_hWnd = hWnd;
	m_strSkinPath = pszSkinPath;
	m_strWindowObjectName = pszWndName;

	// 初始化
	return OnInitWindowBase();
}

// 取得窗口句柄
HWND IWindowBaseImpl::GetSafeHandle()
{
	return m_hWnd;
}

// 得到窗口屏幕坐标
RECT IWindowBaseImpl::GetWindowRect()
{
	RECT RetRect;
	INIT_RECT(RetRect);

	if (IS_SAFE_HANDLE(m_hWnd))
		::GetWindowRect(m_hWnd, &RetRect);

	return RetRect;
}

// 得到客户端坐标
RECT IWindowBaseImpl::GetClientRect()
{
	RECT RetRect;
	INIT_RECT(RetRect);

	if (IS_SAFE_HANDLE(m_hWnd))
	{
		::GetWindowRect(m_hWnd, &RetRect);
		int nW = RetRect.right - RetRect.left;
		int nH = RetRect.bottom - RetRect.top;

		INIT_RECT(RetRect);
		RetRect.right = nW;
		RetRect.bottom = nH;
	}
	else
	{
		// 取得窗口中设置属性的窗口大小
		SIZE WndSize = PP_GetWindowPropSize();
		RetRect.right = WndSize.cx;
		RetRect.bottom = WndSize.cy;
	}

	return RetRect;
}

// 将窗口移到屏幕中央
void IWindowBaseImpl::CenterWindow()
{
	RECT WndRect = GetClientRect();
	int cx = RECT_WIDTH(WndRect);
	int cy = RECT_HEIGHT(WndRect);

	if (IS_SAFE_HANDLE(m_hWnd) && cx > 0 && cy > 0)
	{
		// 设置默认大小
		RECT WorkArea = GetWorkAreaRect();
		RECT CenterRect;
		INIT_RECT(CenterRect);

		CenterRect.left = (RECT_WIDTH(WorkArea) - cx) / 2;
		CenterRect.right = CenterRect.left + cx;
		CenterRect.top = (RECT_HEIGHT(WorkArea) - cy) / 2;
		CenterRect.bottom = CenterRect.top + cy;

		::MoveWindow(m_hWnd, CenterRect.left, CenterRect.top, RECT_WIDTH(CenterRect), RECT_HEIGHT(CenterRect), TRUE);
	}
}

// 初始化对话框、加载窗口皮肤完成，在分层窗口模式下，初始化窗口后，显示窗口
void IWindowBaseImpl::OnInitWindowBaseEnd()
{
	if (PP_GetLayeredWindow() && m_pPropBase_TopMost != NULL)
	{
		// 在分层窗口模式下，初始化窗口后，显示窗口
		SIZE WndSize = this->PP_GetWindowPropSize();

		// 设置默认大小
		RECT WorkArea = GetWorkAreaRect();
		RECT CenterRect;
		INIT_RECT(CenterRect);

		CenterRect.left = (RECT_WIDTH(WorkArea) - WndSize.cx) / 2;
		CenterRect.right = CenterRect.left + WndSize.cx;
		CenterRect.top = (RECT_HEIGHT(WorkArea) - WndSize.cy) / 2;
		CenterRect.bottom = CenterRect.top + WndSize.cy;

		// 设置窗口置顶样式
		::SetWindowPos(m_hWnd, (m_pPropBase_TopMost->GetValue() ? HWND_TOPMOST : HWND_NOTOPMOST),
			0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		RedrawWindow(&CenterRect);
	}
}

// 立即重绘窗口
void IWindowBaseImpl::RedrawWindow(RECT *pDrawRect)
{
	if (IS_SAFE_HANDLE(m_hWnd))
	{
		if (PP_GetLayeredWindow())
		{
			if (!m_bStopDrawWindow)
			{
				HDC hDc = ::GetDC(m_hWnd);
				OnPaint(hDc, pDrawRect);
				::ReleaseDC(m_hWnd, hDc);
			}
		}
		else
		{
			::RedrawWindow(m_hWnd, pDrawRect, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
	}
}

// 本窗口的消息处理函数，bPassOn参数为true是，消息会继续传递处理；false时，处理完毕，不再下传
LRESULT IWindowBaseImpl::WindowProc(UINT nMsgId, WPARAM wParam, LPARAM lParam, bool &bPassOn)
{
	// bPassOn参数为true是，消息会继续传递处理；false时，处理完毕，不再下传
	bPassOn = true;
	if (!::IsWindow(m_hWnd))
		return 0;

	// 先将消息派发给已经注册了的子控件
	if (CTRL_REG_MSG_NOT_PASS_ON == DispatchRegisterMessage(nMsgId, wParam, lParam))
	{
		bPassOn = false;
		return -2;
	}

	switch (nMsgId)
	{
		// UI内核内部自定义消息值
	case UI_FEATURE_KERNEL_MSG:
		{
			switch ((int)wParam)
			{
				// 需要外部对话框接受的消息：使用皮肤初始化窗口正确结束
			case WID_INIT_WINDOW_SUCCESS:
				// 需要外部对话框接受的消息：使用皮肤初始化窗口异常
			case WID_INIT_WINDOW_ERROR:
				OnInitWindowBaseEnd();
				break;

				// 控件注册要取得的消息
			case WID_REG_CTRL_MSG:
				{
					LPARAM_DATA* pLpData = (LPARAM_DATA*)lParam;
					if (pLpData != NULL)
						DoRegisterControlMessage((IControlBase*)pLpData->wParam, (int)pLpData->lParam);
					SAFE_DELETE(pLpData);
				}
				break;

				// 控件注销要取得的消息
			case WID_UNREG_CTRL_MSG:
				{
					LPARAM_DATA* pLpData = (LPARAM_DATA*)lParam;
					if (pLpData != NULL)
						DoUnRegisterControlMessage((IControlBase*)pLpData->wParam, (int)pLpData->lParam);
					SAFE_DELETE(pLpData);
				}
				break;

				// 播放控件动画
			case WID_CTRL_ANIMATION:
				{
					LPARAM_DATA* pLpData = (LPARAM_DATA*)lParam;
					if (pLpData != NULL)
					{
						IControlBase* pCtrl = (IControlBase*)pLpData->wParam;
						if (pCtrl != NULL)
							OnCtrlMessage(pCtrl, (int)pLpData->lParam, pCtrl->PP_GetCtrlAnimationType(), NULL);
					}
					SAFE_DELETE(pLpData);
				}
				break;

				// 使用发消息的方式重绘整个窗口
			case WID_REDRAWWINDOW:
				RedrawWindow();
				break;

				// 清空整个窗口的绘制缓存
			case WID_CLEAR_DRAW_MEM_DC:
				{
					ClearCtrlDrawMemoryDc(&m_ChildCtrlsVec);
					m_WndMemDc.Delete();
				}
				break;

			default:
				break;
			}
		}
		break;

	case WM_GETMINMAXINFO:
		bPassOn = !OnGetMinMaxInfo((MINMAXINFO*)lParam);
		break;

		// 处理这个消息，进入菜单的时候手势就会恢复为箭头
	case WM_ENTERIDLE:
		SetWindowCursor(UF_IDC_ARROW);
		break;

	case WM_MOUSEMOVE:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
				break;

			POINT pt;
			pt.x = pt.y = 0;
			::GetCursorPos(&pt);
			::ScreenToClient(m_hWnd, &pt);
			OnMouseMove((int)wParam, pt);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
				break;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnLButtonDown((int)wParam, pt);
		}
		break;

	case WM_LBUTTONUP:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
				break;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnLButtonUp((int)wParam, pt);
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
				break;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnLButtonDbClick((int)wParam, pt);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
				break;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			OnRButtonDown((int)wParam, pt);
		}
		break;

	case WM_MOUSEWHEEL:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
				break;

			OnMouseWheel(wParam, lParam);
		}
		break;

	case WM_KEYDOWN:
		OnKeyDown(wParam, lParam);
		break;

	case WM_KEYUP:
		OnKeyUp(wParam, lParam);
		break;

	case WM_CHAR:
		OnChar(wParam, lParam);
		break;

	case WM_SYSCOMMAND:
		OnSysCommand(wParam, lParam);
		break;

	case WM_TIMER:
		OnTimer((UINT)wParam);
		break;

	case WM_KILLFOCUS:
		OnKillFocus(wParam, lParam);
		break;

	case WM_SETFOCUS:
		OnSetFocus(wParam, lParam);
		break;

	case WM_NCHITTEST:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
			{
				bPassOn = false;
				break;
			}

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			::ScreenToClient(m_hWnd, &pt);
			LRESULT lResult = OnNcHitTest(pt.x, pt.y);
			if (lResult != -1)
			{
				bPassOn = false;
				return lResult;
			}
		}
		break;

	case WM_SETCURSOR:
		bPassOn = !OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_CLOSE:
		OnClose();
		break;

	case WM_SIZE:
		OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_SIZING:
		OnSizing((UINT)wParam, (LPRECT)lParam);
		break;

	case WM_ENTERSIZEMOVE:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
			{
				bPassOn = false;
				break;
			}

			OnEnterSizeMove();
		}
		break;

	case WM_EXITSIZEMOVE:
		{
			// 如果正在动画过程中，不处理此消息
			if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
			{
				bPassOn = false;
				break;
			}

			OnExitSizeMove();
		}
		break;

	case WM_PAINT:
		{
			bPassOn = false;
			if (!m_bStopDrawWindow)
			{
				PAINTSTRUCT ps;
				HDC hDc = ::BeginPaint(m_hWnd, &ps);
				OnPaint(hDc);
				::EndPaint(m_hWnd, &ps);
			}
			return TRUE;
		}
		break;

	case WM_NCCALCSIZE:
		bPassOn = false;
		return FALSE;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_NCPAINT:
	case WM_NCACTIVATE:
	case 0x00AE:	// WM_NCUAHDRAWCAPTION
	case 0x00AF:	// WM_NCUAHDRAWFRAME
		bPassOn = false;
		return WM_PAINT;

	case WM_SHOWWINDOW:
		OnShowWindow((bool)(wParam != 0), (int)lParam);
		break;

	default:
		break;
	}

	if (m_pWindowMessageCtrl != NULL && m_pWindowMessageCtrl->CompareControl(m_pFocusCtrl))
	{
		// 一个需要得到窗口所有消息的控件，这个控件在取得焦点之后会收到所有窗口消息
		if (CTRL_REG_MSG_NOT_PASS_ON == m_pWindowMessageCtrl->OnCtrlNotify(nMsgId, wParam, lParam))
		{
			bPassOn = false;
			return -3;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// 消息处理 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 取得桌面工作区域大小
RECT IWindowBaseImpl::GetWorkAreaRect()
{
	RECT WorkArea;
	INIT_RECT(WorkArea);
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
	return WorkArea;
}

bool IWindowBaseImpl::OnGetMinMaxInfo(MINMAXINFO *pMinMaxInfo)
{
	if (pMinMaxInfo == NULL || m_pPropSize_Enable == NULL || !m_pPropSize_Enable->GetValue())
		return false;

	if (m_pPropSize_MaxWidth == NULL || m_pPropSize_MaxHeight == NULL ||
		m_pPropSize_MinWidth == NULL || m_pPropSize_MinHeight == NULL)
		return false;

	int nMaxWidth = 0;
	int nMaxHeight = 0;

	//DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	//dwStyle &= WS_MAXIMIZEBOX;
	//if (dwStyle != 0)
	//{
	//	RECT workRct = {0};  
	//	::SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&workRct, 0);
	//	nMaxWidth = RECT_WIDTH(workRct);
	//	nMaxHeight = RECT_HEIGHT(workRct);
	//}
	//else
	//{
		nMaxWidth = ::GetSystemMetrics(SM_CXSCREEN);
		nMaxHeight = ::GetSystemMetrics(SM_CYSCREEN);
	//}

	int nMiniWidth = 100;
	int nMiniHeight = 25;

	if (m_pPropSize_MaxWidth->GetValue() > 0)
		nMaxWidth = min(m_pPropSize_MaxWidth->GetValue(), nMaxWidth);

	if (m_pPropSize_MaxHeight->GetValue() > 0)
		nMaxHeight = min(m_pPropSize_MaxHeight->GetValue(), nMaxHeight);

	nMiniWidth = max(m_pPropSize_MinWidth->GetValue(), nMiniWidth);
	if (nMiniWidth <= 0 || nMiniWidth > nMaxWidth)
		nMiniWidth = 100;

	nMiniHeight = max(m_pPropSize_MinHeight->GetValue(), nMiniHeight);
	if (nMiniHeight <= 0 || nMiniHeight > nMaxHeight)
		nMiniHeight = 25;

	pMinMaxInfo->ptMaxTrackSize.x = nMaxWidth;
	pMinMaxInfo->ptMaxTrackSize.y = nMaxHeight;
	pMinMaxInfo->ptMinTrackSize.x = nMiniWidth;
	pMinMaxInfo->ptMinTrackSize.y = nMiniHeight;

	return true;
}

void IWindowBaseImpl::OnPaint(HDC hWndDc, RECT *pLayeredRect)
{
	if (!IS_SAFE_HANDLE(hWndDc) && !IsInit())
		return;

	RECT WndRect = this->GetWindowRect();
	m_WndMemDc.Create(RECT_WIDTH(WndRect), RECT_HEIGHT(WndRect), false, true);
	if (m_WndMemDc.GetBits() == NULL)
		return;

	// 循环遍历每个控件的绘制
	DrawControl(m_WndMemDc);

	// 绘制到窗口上
	if (PP_GetLayeredWindow())
	{
		POINT ptWinPos = {WndRect.left, WndRect.top};
		POINT ptSrc = {0, 0};
		SIZE sizeWindow = {RECT_WIDTH(WndRect), RECT_HEIGHT(WndRect)};
		
		if (pLayeredRect != NULL)
		{
			// 设置目标渲染窗口位置
			ptWinPos.x = pLayeredRect->left;
			ptWinPos.y = pLayeredRect->top;
			sizeWindow.cx = RECT_WIDTH(*pLayeredRect);
			sizeWindow.cy = RECT_HEIGHT(*pLayeredRect);
		}

		::UpdateLayeredWindow(m_hWnd, hWndDc, &ptWinPos, &sizeWindow, m_WndMemDc.GetSafeHdc(), &ptSrc, 0, &m_Blend, ULW_ALPHA);
	}
	else
	{
		::BitBlt(hWndDc, 0, 0, RECT_WIDTH(WndRect), RECT_HEIGHT(WndRect), m_WndMemDc.GetSafeHdc(), 0, 0, SRCCOPY);
	}

	if (PP_IsUseTempDrawMem())
		m_WndMemDc.Delete();
}

bool IWindowBaseImpl::CheckMouseInControl(CHILD_CTRLS_VEC *pCtrlVec, POINT pt, IControlBase **ppControl, int nMouseMsgId)
{
	if (pCtrlVec == NULL || ppControl == NULL)
		return false;

	ListObj* pPrior = NULL;
	for (ListObj* pItem = pCtrlVec->LastObj(); pItem != NULL; pItem = pPrior)
	{
		pPrior = pCtrlVec->PriorObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL || !pCtrl->IsVisible())
			continue;

		if (pCtrl->GetReceiveMouseMessage())
		{
			RECT CtrlRct = pCtrl->GetWindowRect();
			if ((::PtInRect(&CtrlRct, pt)) && (pCtrl->OnCheckMouseInRgn(GetCtrlMouseOffset(pt, pCtrl), nMouseMsgId)))
			{
				*ppControl = pCtrl;
				CHILD_CTRLS_VEC *pCtrlsVec = pCtrl->GetChildControlsVec();
				CheckMouseInControl(pCtrlsVec, pt, ppControl, nMouseMsgId);
				return true;
			}
		}
		else
		{
			CHILD_CTRLS_VEC *pCtrlsVec = pCtrl->GetChildControlsVec();
			if (CheckMouseInControl(pCtrlsVec, pt, ppControl, nMouseMsgId))
				return true;
		}
	}

	return false;
}

void IWindowBaseImpl::ResizeInLayeredWindow(RECT NewWndRect, bool bRedraw)
{
	if (m_pPropBase_TopMost == NULL || !PP_GetLayeredWindow())
		return;

	RECT wndRct = this->GetWindowRect();

	// 分层窗口不能直接渲染，必须先要设置窗口大小才可以
	::SetWindowPos(m_hWnd, (m_pPropBase_TopMost->GetValue() ? HWND_TOPMOST : HWND_NOTOPMOST),
		0, 0, RECT_WIDTH(NewWndRect), RECT_HEIGHT(NewWndRect), SWP_NOMOVE);

	if ((RECT_WIDTH(wndRct) != RECT_WIDTH(NewWndRect)) || (RECT_HEIGHT(wndRct) != RECT_HEIGHT(NewWndRect)))
		OnSize(0, RECT_WIDTH(NewWndRect), RECT_HEIGHT(NewWndRect));

	if (bRedraw)
		RedrawWindow(&NewWndRect);
}

// 移动窗口
void IWindowBaseImpl::MoveWindow(RECT toWndRct, bool bRedraw)
{
	if (!::IsWindow(m_hWnd))
		return;

	// 播放动画的时候，不能移动窗口、改变窗口大小
	if (IS_SAFE_HANDLE(m_hCtrlAnimateThread))
		return;

	if (PP_GetLayeredWindow())
	{
		ResizeInLayeredWindow(toWndRct, bRedraw);
	}
	else
	{
		::MoveWindow(m_hWnd, toWndRct.left, toWndRct.top, RECT_WIDTH(toWndRct), RECT_HEIGHT(toWndRct), (bRedraw == true));
	}
}

// 从相对窗口的鼠标相对坐标得到相对控件的鼠标坐标
POINT IWindowBaseImpl::GetCtrlMouseOffset(POINT WndPt, IControlBase* pCtrl)
{
	if (pCtrl == NULL)
		return WndPt;

	POINT CtrlMousePt = WndPt;
	RECT CtlRct = pCtrl->GetWindowRect();
	CtrlMousePt.x -= CtlRct.left;
	CtrlMousePt.y -= CtlRct.top;

	return CtrlMousePt;
}

void IWindowBaseImpl::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if (m_pMouseHoverCtrl != NULL)
		m_pMouseHoverCtrl->OnMouseWheel(wParam, lParam);
}

void IWindowBaseImpl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseLeave = true;
	if (m_pMouseHoverCtrl != NULL)
	{
		// 消除tips
		this->DestroySystemTips();
		m_pMouseHoverCtrl->SetMouseHover(false);
		POINT pt = {0, 0};
		m_pMouseHoverCtrl->OnMouseLeave(pt);
		m_pMouseHoverCtrl = NULL;
	}
}

void IWindowBaseImpl::OnMouseMove(int nVirtKey, POINT pt)
{
	// 当系统收到LButtonDown消息时，同时还会受到MouseMove消息，先要过滤掉这个MouseMove消息
	if (m_bLButtonDownPoint.x == pt.x && m_bLButtonDownPoint.y == pt.y && m_bLButtonDownPoint.x != -10)
		return;
	m_bLButtonDownPoint.x = m_bLButtonDownPoint.y = -10;

	if (!IsInit())
		return;

	// 取得鼠标离开消息
	if (m_bMouseLeave)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
		m_bMouseLeave = false;
	}

	// 窗口的拉伸操作
	RECT OutOldRect, OutNewRect;
	bool bInStretching = false;
	if (!IsFullScreen() && m_WndResize.ResizeInLayeredWindow(OutOldRect, OutNewRect, bInStretching))
	{
		ResizeInLayeredWindow(OutNewRect);
		return;
	}

	// 拉伸窗口过程中
	if (bInStretching)
		return;

	if (m_bIsLButtonDown)
	{
		if (m_pLButtonDownCtrl != NULL)
		{
			m_bMouseDragging = true;
			// 鼠标在控件内部拖动控件
			m_pLButtonDownCtrl->OnMouseDragging(GetCtrlMouseOffset(pt, m_pLButtonDownCtrl));
			return;
		}

		// 鼠标按下时，需要锁定一个控件进行鼠标移动处理
		if (m_pMouseHoverCtrl != NULL)
		{
			RECT CtrlRct = m_pMouseHoverCtrl->GetWindowRect();
			if (::PtInRect(&CtrlRct, pt))
			{
				if (m_pMouseHoverCtrl->IsMousehover())
				{
					m_pMouseHoverCtrl->OnMouseMove(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
				}
				else
				{
					m_pMouseHoverCtrl->SetMouseHover(true);
					m_pMouseHoverCtrl->OnMouseEnter(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
					// 显示全控件范围的tips
					m_pMouseHoverCtrl->ShowDefaultSystemTips();
				}
			}
			else
			{
				if (m_pMouseHoverCtrl->IsMousehover())
				{
					// 消除tips
					this->DestroySystemTips();
					m_pMouseHoverCtrl->SetMouseHover(false);
					m_pMouseHoverCtrl->OnMouseLeave(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
				}
			}
		}
	}
	else
	{
		// 鼠标是否移动到了窗口可以进行拉伸操作的边缘
		if (!IsFullScreen() && m_WndResize.MouseMoveInWindowFrame(pt) != HTNOWHERE)
			return;

		m_nCurMouseStyle = UF_IDC_ARROW;

		// 取得当前MouseMove的控件
		IControlBase *pControl = NULL;
		CheckMouseInControl(&m_ChildCtrlsVec, pt, &pControl, WM_MOUSEMOVE);
		if (pControl == NULL)
		{
			if (m_pMouseHoverCtrl != NULL)
			{
				// 消除tips
				this->DestroySystemTips();
				m_pMouseHoverCtrl->SetMouseHover(false);
				m_pMouseHoverCtrl->OnMouseLeave(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
				m_pMouseHoverCtrl = NULL;
			}
			return;
		}

		// 派发鼠标消息到控件
		if (m_pMouseHoverCtrl == pControl)
		{
			m_pMouseHoverCtrl->OnMouseMove(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
		}
		else
		{
			if (m_pMouseHoverCtrl != NULL)
			{
				// 消除tips
				this->DestroySystemTips();
				m_pMouseHoverCtrl->SetMouseHover(false);
				m_pMouseHoverCtrl->OnMouseLeave(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
			}

			m_pMouseHoverCtrl = pControl;
			m_pMouseHoverCtrl->SetMouseHover(true);
			m_pMouseHoverCtrl->OnMouseEnter(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
			// 显示全控件范围的tips
			m_pMouseHoverCtrl->ShowDefaultSystemTips();
		}
	}
}

// 显示自定义光标
void IWindowBaseImpl::SetWindowCursor(int nCursor)
{
	m_nCurMouseStyle = nCursor;
	::PostMessage(this->m_hWnd, WM_SETCURSOR, NULL, NULL);
}

bool IWindowBaseImpl::OnSetCursor(HWND hWnd, int nHitTest, int nMsgId)
{
	if (m_pMouseHoverCtrl != NULL && m_pMouseHoverCtrl->GetReceiveMouseMessage())
	{
		m_nCurMouseStyle = UF_IDC_ARROW;
		POINT pt;
		pt.x = pt.y = 0;
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		IPropertyCursor* pChildCursor = m_pMouseHoverCtrl->OnSetCursor(GetCtrlMouseOffset(pt, m_pMouseHoverCtrl));
		if (pChildCursor != NULL)
		{
			IPropertyCursorBase* pCursorBase = pChildCursor->GetCursorBaseProp();
			if (pCursorBase != NULL)
				m_nCurMouseStyle = pCursorBase->GetCursorId();
		}
	}

	HCURSOR hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(m_nCurMouseStyle));
	if (hCursor == NULL)
	{
		m_nCurMouseStyle = UF_IDC_ARROW;
		return false;
	}

	::SetCursor(hCursor);
	if (m_hPreviousCursor != NULL)
	{
		::DestroyCursor(m_hPreviousCursor);
		m_hPreviousCursor = NULL;
	}
	m_hPreviousCursor = hCursor;

	return true;
}

LRESULT IWindowBaseImpl::OnNcHitTest(int nX, int nY)
{
	if (!IsFullScreen() && !PP_GetLayeredWindow())
	{
		// 非分层模式下拉伸窗口的操作处理
		POINT pt;
		pt.x = nX;
		pt.y = nY;

		int nHit = m_WndResize.MouseMoveInWindowFrame(pt);

		if (m_nHitType != nHit)
		{
			m_nHitType = nHit;
			if (m_nHitType == HTNOWHERE)
				SetWindowCursor(UF_IDC_ARROW);
		}

		if (nHit != HTNOWHERE)
			return nHit;
	}

	return -1;
}

void IWindowBaseImpl::OnLButtonDbClick(int nVirtKey, POINT pt)
{
	if (!IsInit())
		return;

	// 取得当前鼠标按下的控件
	IControlBase *pControl = NULL;
	CheckMouseInControl(&m_ChildCtrlsVec, pt, &pControl, WM_LBUTTONDBLCLK);
	if (pControl == NULL)
		return;

	m_pLButtonDownCtrl = pControl;
	m_pLButtonDownCtrl->OnLButtonDbClick(GetCtrlMouseOffset(pt, m_pLButtonDownCtrl));
	SetFocusCtrl(m_pLButtonDownCtrl);
}

void IWindowBaseImpl::OnRButtonDown(int nVirtKey, POINT pt)
{
	if (!IsInit())
		return;

	// 取得当前鼠标按下的控件
	IControlBase *pControl = NULL;
	CheckMouseInControl(&m_ChildCtrlsVec, pt, &pControl, WM_RBUTTONDOWN);
	if (pControl == NULL)
		return;

	pControl->OnRButtonDown(GetCtrlMouseOffset(pt, pControl));
	SetFocusCtrl(pControl);
}

// 设置当前得到焦点的控件，如果参数为NULL，说明当前没有控件得到焦点，所有控件都失去焦点
void IWindowBaseImpl::SetFocusCtrl(IControlBase *pControl)
{
	if (m_pFocusCtrl != pControl)
	{
		if (m_pFocusCtrl != NULL)
		{
			m_pFocusCtrl->OnKillFocus();
			m_pFocusCtrl = NULL;
		}

		if (pControl != NULL)
		{
			m_pFocusCtrl = pControl;
			m_pFocusCtrl->OnSetFocus();
		}

		if (m_pFocusCtrl != NULL && m_pFocusCtrl->PP_GetControlTabOrder() > NOT_SET_TAB_FOCUS)
		{
			m_pTabCtrl = m_pFocusCtrl;
			if (m_pRegFocusCtrl != NULL)
				m_pRegFocusCtrl->OnContrlFocusChange(m_pTabCtrl);
		}
	}
}

void IWindowBaseImpl::OnLButtonDown(int nVirtKey, POINT pt)
{
	m_bLButtonDownPoint = pt;

	if (!IsInit())
		return;

	// 判断是否进入了拉伸窗口范围，进入则开始进行拉伸操作
	if (!IsFullScreen() && m_WndResize.BeginResizeInLayeredWindow())
		return;

	// 取得当前鼠标按下的控件
	IControlBase *pControl = NULL;
	CheckMouseInControl(&m_ChildCtrlsVec, pt, &pControl, WM_LBUTTONDOWN);
	if (pControl == NULL)
	{
		SetFocusCtrl(NULL);

		// 是否最大化
		if (IsFullScreen() && !GetMaximizeWndDragMove())
			return;

		// 是否支持鼠标点击移动窗口
		if (PP_GetDragWindow())
			::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));

		return;
	}

	::SetCapture(m_hWnd);
	m_bIsLButtonDown = true;

	// 派发鼠标消息到控件
	m_pLButtonDownCtrl = pControl;
	m_pLButtonDownCtrl->OnLButtonDown(GetCtrlMouseOffset(pt, m_pLButtonDownCtrl));

	SetFocusCtrl(m_pLButtonDownCtrl);
}

void IWindowBaseImpl::OnLButtonUp(int nVirtKey, POINT pt)
{
	m_bLButtonDownPoint.x = m_bLButtonDownPoint.y = -10;

	if (!IsInit())
		return;

	// 释放窗口拉伸操作
	m_WndResize.EndResizeInLayeredWindow();

	::ReleaseCapture();
	m_bIsLButtonDown = false;

	if (m_pLButtonDownCtrl != NULL)
	{
		if (m_bMouseDragging)
		{
			// 有拖拽的操作，将拖拽的动作传给目标控件
			IControlBase *pDragToControl = NULL;
			CheckMouseInControl(&m_ChildCtrlsVec, pt, &pDragToControl, WM_LBUTTONUP);
			if (pDragToControl != NULL && pDragToControl != m_pLButtonDownCtrl)
			{
				WPARAM wParam = NULL;
				LPARAM lParam = NULL;
				m_pLButtonDownCtrl->OnGetDragParam(wParam, lParam);
				pDragToControl->OnDragStop(m_pLButtonDownCtrl, GetCtrlMouseOffset(pt, pDragToControl), wParam, lParam);
			}

			m_bMouseDragging = false;
		}

		m_pLButtonDownCtrl->OnLButtonUp(GetCtrlMouseOffset(pt, m_pLButtonDownCtrl));
		m_pLButtonDownCtrl = NULL;
	}
}

void IWindowBaseImpl::OnSize(UINT nType, int cx, int cy)
{
	if (!IsInit())
		return;

	SIZE NSize;
	NSize.cx = cx;
	NSize.cy = cy;
	SetControlPostion(&m_ChildCtrlsVec, NSize);
}

void IWindowBaseImpl::SetControlPostion(CHILD_CTRLS_VEC *pCtrlVec, SIZE NewSize)
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

		RECT ParentRctInWnd;
		INIT_RECT(ParentRctInWnd);

		IControlBase* pParentCtrl = pCtrl->GetParentControl();
		if (pParentCtrl == NULL)
		{
			// 没有父控件，说明直接附着在对话框上
			ParentRctInWnd.right = NewSize.cx;
			ParentRctInWnd.bottom = NewSize.cy;
		}
		else
		{
			ParentRctInWnd = pParentCtrl->GetWindowRect();
		}

		// 计算当前控件的位置
		SetControlWindowLayoutPostion(pCtrl, ParentRctInWnd);
		pCtrl->RedrawControl();

		// 设置子控件的位置
		SetControlPostion(pCtrl->GetChildControlsVec(), NewSize);
	}
}

// 根据控件布局信息得到相对于窗口的位置
void IWindowBaseImpl::SetControlWindowLayoutPostion(IControlBase* pCtrl, RECT ParentRctInWnd)
{
	if (pCtrl == NULL)
		return;

	CONTROL_LAYOUT_INFO CtrlLayout = pCtrl->GetLayout();

	// 用户自定义位置需要父控件自行设置
	if (CtrlLayout.clType == CL_L_USER_DEF)
	{
		if (IsDesignMode())
		{
			// 设置模式下提供 左上角的预览
			CtrlLayout.clType = CL_G_LEFT_TOP;
		}
		else
		{
			return;
		}
	}

	RECT RctInWnd = pCtrl->GetWindowRect();
	int nCtrlW = RECT_WIDTH(RctInWnd);
	int nCtrlH = RECT_HEIGHT(RctInWnd);
	int nParentW = RECT_WIDTH(ParentRctInWnd);
	int nParentH = RECT_HEIGHT(ParentRctInWnd);

	if (IsDesignMode() && pCtrl->m_pPropBase_Layout_Width != NULL && pCtrl->m_pPropBase_Layout_Height != NULL)
	{
		nCtrlW = pCtrl->m_pPropBase_Layout_Width->GetValue();
		nCtrlH = pCtrl->m_pPropBase_Layout_Height->GetValue();
	}

	if (CtrlLayout.clType == CL_G_LEFT_TOP)
	{
		// 固定大小：左上角定位
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = RctInWnd.left + CtrlLayout.nWidth;
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = RctInWnd.top + CtrlLayout.nHeight;
	}
	else if (CtrlLayout.clType == CL_G_LEFT_BOTTOM)
	{
		// 固定大小：左下角定位
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = RctInWnd.left + CtrlLayout.nWidth;
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
		RctInWnd.top = RctInWnd.bottom - CtrlLayout.nHeight;
	}
	else if (CtrlLayout.clType == CL_G_RIGHT_TOP)
	{
		// 固定大小：右上角定位
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.left = RctInWnd.right - CtrlLayout.nWidth;
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = RctInWnd.top + CtrlLayout.nHeight;
	}
	else if (CtrlLayout.clType == CL_G_RIGHT_BOTTOM)
	{
		// 固定大小：右下角定位
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.left = RctInWnd.right - CtrlLayout.nWidth;
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
		RctInWnd.top = RctInWnd.bottom - CtrlLayout.nHeight;
	}
	else if (CtrlLayout.clType == CL_L_LEFT)
	{
		// 拉伸变动大小：撑满左侧
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = RctInWnd.left + CtrlLayout.nWidth;
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
	}
	else if (CtrlLayout.clType == CL_L_RIGHT)
	{
		// 拉伸变动大小：撑满右侧
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.left = RctInWnd.right - CtrlLayout.nWidth;
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
	}
	else if (CtrlLayout.clType == CL_L_TOP)
	{
		// 拉伸变动大小：撑满上方
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = RctInWnd.top + CtrlLayout.nHeight;
	}
	else if (CtrlLayout.clType == CL_L_BOTTOM)
	{
		// 拉伸变动大小：撑满下方
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
		RctInWnd.top = RctInWnd.bottom - CtrlLayout.nHeight;
	}
	else if (CtrlLayout.clType == CL_L_ALL)
	{
		// 拉伸变动大小：撑满全部
		RctInWnd = ParentRctInWnd;
	}
	else if (CtrlLayout.clType == CL_L_MIDDLE)
	{
		// 拉伸变动大小：居中
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
	}
	else if (CtrlLayout.clType == CL_G_TOP_MIDDLE)
	{
		// 固定大小：靠上居中
		RctInWnd.top = ParentRctInWnd.top + CtrlLayout.nTopSpace;
		RctInWnd.bottom = RctInWnd.top + nCtrlH;
		RctInWnd.left = ParentRctInWnd.left + (nParentW - nCtrlW) / 2;
		RctInWnd.right = RctInWnd.left + nCtrlW;
	}
	else if (CtrlLayout.clType == CL_G_BOTTOM_MIDDLE)
	{
		// 固定大小：靠下居中
		RctInWnd.bottom = ParentRctInWnd.bottom - CtrlLayout.nBottomSpace;
		RctInWnd.top = RctInWnd.bottom - nCtrlH;
		RctInWnd.left = ParentRctInWnd.left + (nParentW - nCtrlW) / 2;
		RctInWnd.right = RctInWnd.left + nCtrlW;
	}
	else if (CtrlLayout.clType == CL_G_LEFT_MIDDLE)
	{
		// 固定大小：靠左居中
		RctInWnd.left = ParentRctInWnd.left + CtrlLayout.nLeftSpace;
		RctInWnd.right = RctInWnd.left + nCtrlW;
		RctInWnd.top = ParentRctInWnd.top + (nParentH - nCtrlH) / 2;
		RctInWnd.bottom = RctInWnd.top + nCtrlH;
	}
	else if (CtrlLayout.clType == CL_G_RIGHT_MIDDLE)
	{
		// 固定大小：靠右居中
		RctInWnd.right = ParentRctInWnd.right - CtrlLayout.nRightSpace;
		RctInWnd.left = RctInWnd.right - nCtrlW;
		RctInWnd.top = ParentRctInWnd.top + (nParentH - nCtrlH) / 2;
		RctInWnd.bottom = RctInWnd.top + nCtrlH;
	}
	else if (CtrlLayout.clType == CL_G_MID_MIDDLE)
	{
		// 固定大小：居中
		RctInWnd.left = ParentRctInWnd.left + (nParentW - nCtrlW) / 2;
		RctInWnd.right = RctInWnd.left + nCtrlW;
		RctInWnd.top = ParentRctInWnd.top + (nParentH - nCtrlH) / 2;
		RctInWnd.bottom = RctInWnd.top + nCtrlH;
	}

	pCtrl->SetWindowRect(RctInWnd);
}

// 销毁子类化窗口、释放资源
void IWindowBaseImpl::OnDestroy()
{
}

void IWindowBaseImpl::OnClose()
{
	// 等待动画线程结束
	if (m_hCtrlAnimateThread != NULL)
	{
		if (::WaitForSingleObject(m_hCtrlAnimateThread, 3 * 1000) != WAIT_OBJECT_0)
		{
			::TerminateThread(m_hCtrlAnimateThread, -1);
			SAFE_CLOSE_HANDLE(m_hCtrlAnimateThread);
		}
	}

	// 向所有控件发送关闭对话框消息
	SendCloseWndMsgToCtrl(&m_ChildCtrlsVec);
}

// 向所有控件发送关闭对话框消息
void IWindowBaseImpl::SendCloseWndMsgToCtrl(CHILD_CTRLS_VEC *pChildCtrlsVec)
{
	if (pChildCtrlsVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		SendCloseWndMsgToCtrl(pCtrl->GetChildControlsVec());
		pCtrl->OnWindowClose();
	}
}

void IWindowBaseImpl::OnCreate()
{
}

void IWindowBaseImpl::OnTimer(UINT nTimerId)
{
	if (!IsInit() || m_pSkinPropMgr == NULL)
		return;

	bool bPropNeedDraw = false;
	if (nTimerId == UM_DFT_ANIMATION_TIMER)
	{
		IPropertyBaseMap* pImagePropMap = ((IPropertySkinManagerImpl*)m_pSkinPropMgr)->GetIPropertyBaseMap(this->GetSkinFileItem(), _T(PROP_TYPE_IMAGE_NAME));
		if (pImagePropMap == NULL)
			return;

		for (IPropertyBaseMap::iterator pImage = pImagePropMap->begin(); pImage != pImagePropMap->end(); pImage++)
		{
			IPropertyImage* pImageProp = dynamic_cast<IPropertyImage*>(pImage->second);
			if (pImageProp == NULL)
				continue;

			IFeatureObject* pOwner = pImageProp->GetOwnerObject();
			if (pOwner == NULL)
				continue;

			IControlBase* pCtrl = dynamic_cast<IControlBase*>(pOwner);
			if (pCtrl != NULL)
			{
				// 非自身窗口控件，不进行动画绘制
				IWindowBaseImpl* pWnd = dynamic_cast<IWindowBaseImpl*>(pCtrl->GetOwnerWindow());
				if (pWnd == NULL || pWnd != this)
					continue;

				// 不可见，不刷新控件界面
				if (!pCtrl->IsVisible())
					continue;
			}
			else
			{
				// 非自身窗口控件，不进行动画绘制
				IWindowBaseImpl* pWnd = dynamic_cast<IWindowBaseImpl*>(pOwner);
				if (pWnd == NULL || pWnd != this)
					continue;
			}

			if (pImageProp->OnDrawAnimation())
			{
				if (pCtrl == NULL)
				{
					this->InvalidateRect(NULL);
				}
				else
				{
					pCtrl->RedrawControl();
					this->InvalidateRect(&(pCtrl->GetWindowRect()));
				}

				bPropNeedDraw = true;
			}
		}
	}

	bool bChildNeedDraw = OnTimerToChild(m_ChildCtrlsVec, nTimerId);

	if (bPropNeedDraw || bChildNeedDraw)
		this->UpdateWindow();
}

bool IWindowBaseImpl::OnTimerToChild(CHILD_CTRLS_VEC &ChildCtrlsVec, UINT nTimerId)
{
	bool bNeedDraw = false;
	ListObj* pNext = NULL;
	for (ListObj* pItem = ChildCtrlsVec.TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = ChildCtrlsVec.NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (nTimerId == UM_DFT_ANIMATION_TIMER && pCtrl->IsVisible())
		{
			if (pCtrl->OnDrawAnimation())
			{
				bNeedDraw = true;
				pCtrl->RedrawControl();
				this->InvalidateRect(&(pCtrl->GetWindowRect()));
			}
		}
		else
		{
			pCtrl->OnTimer(nTimerId);
		}

		if (OnTimerToChild((*(pCtrl->GetChildControlsVec())), nTimerId))
			bNeedDraw = true;
	}

	return bNeedDraw;
}


void IWindowBaseImpl::InvalidateRect(RECT *lpRect)
{
	if (::IsWindow(m_hWnd))
		::InvalidateRect(m_hWnd, lpRect, FALSE);
}

void IWindowBaseImpl::UpdateWindow()
{
	if (::IsWindow(m_hWnd))
		::UpdateWindow(m_hWnd);
}

void IWindowBaseImpl::OnChar(WPARAM wParam, LPARAM lParam)
{
	if (!IsInit())
		return;

	if (m_pFocusCtrl != NULL)
		m_pFocusCtrl->OnChar(wParam, lParam);
}

void IWindowBaseImpl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	if (!IsInit())
		return;

	if (m_pFocusCtrl != NULL)
		m_pFocusCtrl->OnKeyDown(wParam, lParam);

	if (wParam == VK_TAB)
		OnTabChange();
}

void IWindowBaseImpl::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	if (!IsInit())
		return;

	if (m_pFocusCtrl != NULL)
		m_pFocusCtrl->OnKeyUp(wParam, lParam);

	if (wParam == VK_RETURN)
	{
		IControlBase* pEnterCtrl = GetDefaultEnterCtrl(&m_ChildCtrlsVec);
		if (pEnterCtrl != NULL)
			pEnterCtrl->OnDefaultEnterCtrl();
	}
}

void IWindowBaseImpl::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	if (!IsInit() || m_pCtrlMsgCallBack == NULL)
		return;

	if (wParam == SC_CLOSE)
		m_pCtrlMsgCallBack->OnCtrlMessage(NULL, KCM_SYS_COMMAND_CLOSE, NULL, NULL);
}

void IWindowBaseImpl::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	if (!IsInit())
		return;

	this->SetFocusCtrl(NULL);
	this->ResetActiveControl();
}

void IWindowBaseImpl::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	if (!IsInit())
		return;
}

void IWindowBaseImpl::OnSizing(UINT nSide, LPRECT lpRect)
{
	if (!IsInit())
		return;
}

void IWindowBaseImpl::OnEnterSizeMove()
{
	if (!IsInit())
		return;

	this->DestroySystemTips();

	SendEnterOrExitSizeMsgToCtrl(true, &m_ChildCtrlsVec);
}

void IWindowBaseImpl::OnExitSizeMove()
{
	if (!IsInit())
		return;

	SendEnterOrExitSizeMsgToCtrl(false, &m_ChildCtrlsVec);
}

// 向队列未插入一个控件
void IWindowBaseImpl::AppendChildContrl(IControlBase *pCtrl)
{
	if (pCtrl != NULL)
		m_ChildCtrlsVec.PushBackObj(pCtrl);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// 功能函数 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 循环遍历每个控件的绘制
void IWindowBaseImpl::DrawControl(CDrawingImage &ToDrawDc)
{
	RECT WndRct = this->GetClientRect();
	ListObj* pNext = NULL;
	for (ListObj* pItem = m_ChildCtrlsVec.TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = m_ChildCtrlsVec.NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl != NULL)
			pCtrl->OnPaintControl(ToDrawDc, WndRct);
	}
}

// 将窗口图片输出到一个目标画板上
void IWindowBaseImpl::PrintWindow(CDrawingImage &ToDrawDc)
{
	RECT WndRect = this->GetWindowRect();
	ToDrawDc.Create(RECT_WIDTH(WndRect), RECT_HEIGHT(WndRect), true);
	if (ToDrawDc.GetBits() == NULL)
		return;

	// 循环遍历每个控件的绘制
	DrawControl(ToDrawDc);
}

// 是否为设计模式
bool IWindowBaseImpl::IsDesignMode()
{
	if (m_pUiKernel == NULL)
		return false;

	return m_pUiKernel->IsDesignMode();
}

void IWindowBaseImpl::BD_InitWindowBase(LPCWSTR pszSkinPath, IPropertyWindow *pWindowProp, bool bSetDftProp)
{
	if (pszSkinPath == NULL || wcslen(pszSkinPath) <= 0 || pWindowProp == NULL)
		return;

	m_strSkinPath = pszSkinPath;

	// 在Builder中绘制用的边框色块
	BD_NewFrameImage();

	PP_SetWindowPropetry(pWindowProp, bSetDftProp);
	this->SetObjectId(pWindowProp->GetObjectId());
	if (m_pPropBase_Name == NULL || m_pPropBase_Name->GetString() == NULL || m_pPropBase_Name->GetLength() <= 0)
	{
		if (m_pPropBase_Name != NULL)
			m_pPropBase_Name->SetString(L"【新建窗体名称】");
		this->SetObjectName(L"【新建窗体名称】");
	}
	else
	{
		this->SetObjectName(m_pPropBase_Name->GetString());
	}
}

// 在Builder中绘制用的边框色块
void IWindowBaseImpl::BD_NewFrameImage()
{
	if (m_pSkinPropMgr == NULL)
		return;

	m_BuilderWndFrameImageBase.SetUiKernel(m_pUiKernel);
	m_BuilderCtrlFrameImageBase.SetUiKernel(m_pUiKernel);

	// 在Builder中绘制用的边框色块
	CStringW strPath = PathHelperW(_T("ControlsRes\\BuilderWindowFrame.bmp"));
	BD_NewFrameImageBase(&m_BuilderWndFrameImageBase, strPath);

	strPath = PathHelperW(_T("ControlsRes\\BuilderCtrlFrame.bmp"));
	BD_NewFrameImageBase(&m_BuilderCtrlFrameImageBase, strPath);
}

void IWindowBaseImpl::BD_NewFrameImageBase(IPropertyImageBase *pImgBase, LPCWSTR strImgPath)
{
	if (pImgBase == NULL || strImgPath == NULL || wcslen(strImgPath) <= 0)
		return;

	IMAGE_BASE_PROP ImgBase;
	InitPropImageBase(&ImgBase);

	ImgBase.bIsZipFile = false;
	wcscpy_s(ImgBase.strFileName, MAX_PATH, strImgPath);
	ImgBase.ImgShowType = IST_ALL_LASHEN;
	ImgBase.RectInImage.left = ImgBase.RectInImage.top = 0;
	ImgBase.RectInImage.right = ImgBase.RectInImage.bottom = FANGKUAI_SIZE;
	INIT_RECT(ImgBase.jggInfo);

	pImgBase->SetImageProp(&ImgBase);
}

bool IWindowBaseImpl::IsInit()
{
	return (m_pXmlPropWindow != NULL && m_pSkinPropMgr != NULL);
}

void IWindowBaseImpl::BD_DrawWindowView(CDrawingBoard &ViewMemDc)
{
	if (ViewMemDc.GetSafeHdc() == NULL || m_pUiEngine == NULL || m_pPropSize_WindowWidth == NULL || m_pPropSize_WindowHeight == NULL)
		return;

	int nWidth = m_pPropSize_WindowWidth->GetValue();
	int nHeight = m_pPropSize_WindowHeight->GetValue();
	
	// 创建内存dc
	m_WndMemDc.Create(nWidth, nHeight, false, true);
	if (m_WndMemDc.GetBits() == NULL)
		return;

	// 循环遍历每个控件的绘制
	DrawControl(m_WndMemDc);

	// 绘制到父DC上
	m_pUiEngine->AlphaBlend(ViewMemDc, m_BD_FangKuai8.EntityRct.left, m_BD_FangKuai8.EntityRct.top, nWidth, nHeight,
		m_WndMemDc, 0, 0, nWidth, nHeight);

	// 绘制窗口的选择状态
	BD_DrawSelectRect(ViewMemDc, m_BD_FangKuai8, true);

	// 绘制焦点控件的选择状态
	if (m_pFocusCtrl != NULL)
	{
		FANGKUAI_8 *pFangKuai8 = m_pFocusCtrl->BD_GetFangKuai8Rect();
		BD_DrawSelectRect(ViewMemDc, *pFangKuai8, false);
	}

	if (PP_IsUseTempDrawMem())
		m_WndMemDc.Delete();
}

// 绘制窗口和被选中的控件的边框的8个方块
void IWindowBaseImpl::BD_DrawSelectRect(CDrawingBoard &MemDc, FANGKUAI_8 &FangKuai8, bool bIsWndFrame)
{
	IPropertyImageBase *pPropImg = &m_BuilderCtrlFrameImageBase;
	if (bIsWndFrame)
		pPropImg = &m_BuilderWndFrameImageBase;

	FangKuai8.LeftTop.left = FangKuai8.EntityRct.left - FANGKUAI_SIZE;
	FangKuai8.LeftTop.top = FangKuai8.EntityRct.top - FANGKUAI_SIZE;
	FangKuai8.LeftTop.right = FangKuai8.LeftTop.left + FANGKUAI_SIZE;
	FangKuai8.LeftTop.bottom = FangKuai8.LeftTop.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.LeftTop);

	FangKuai8.LeftMid.left = FangKuai8.EntityRct.left - FANGKUAI_SIZE;
	FangKuai8.LeftMid.top = FangKuai8.EntityRct.top + (RECT_HEIGHT(FangKuai8.EntityRct) / 2) - (FANGKUAI_SIZE / 2);
	FangKuai8.LeftMid.right = FangKuai8.LeftMid.left + FANGKUAI_SIZE;
	FangKuai8.LeftMid.bottom = FangKuai8.LeftMid.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.LeftMid);

	FangKuai8.LeftBottom.left = FangKuai8.EntityRct.left - FANGKUAI_SIZE;
	FangKuai8.LeftBottom.top = FangKuai8.EntityRct.bottom;
	FangKuai8.LeftBottom.right = FangKuai8.LeftBottom.left + FANGKUAI_SIZE;
	FangKuai8.LeftBottom.bottom = FangKuai8.LeftBottom.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.LeftBottom);

	FangKuai8.MidTop.left = FangKuai8.EntityRct.left + (RECT_WIDTH(FangKuai8.EntityRct) / 2) - (FANGKUAI_SIZE / 2);
	FangKuai8.MidTop.top = FangKuai8.EntityRct.top - FANGKUAI_SIZE;
	FangKuai8.MidTop.right = FangKuai8.MidTop.left + FANGKUAI_SIZE;
	FangKuai8.MidTop.bottom = FangKuai8.MidTop.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.MidTop);

	FangKuai8.MidBottom.left = FangKuai8.EntityRct.left + (RECT_WIDTH(FangKuai8.EntityRct) / 2) - (FANGKUAI_SIZE / 2);
	FangKuai8.MidBottom.top = FangKuai8.EntityRct.bottom;
	FangKuai8.MidBottom.right = FangKuai8.MidBottom.left + FANGKUAI_SIZE;
	FangKuai8.MidBottom.bottom = FangKuai8.MidBottom.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.MidBottom);

	FangKuai8.RightTop.left = FangKuai8.EntityRct.right;
	FangKuai8.RightTop.top = FangKuai8.EntityRct.top - FANGKUAI_SIZE;
	FangKuai8.RightTop.right = FangKuai8.RightTop.left + FANGKUAI_SIZE;
	FangKuai8.RightTop.bottom = FangKuai8.RightTop.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.RightTop);

	FangKuai8.RightMid.left = FangKuai8.EntityRct.right;
	FangKuai8.RightMid.top = FangKuai8.EntityRct.top + (RECT_HEIGHT(FangKuai8.EntityRct) / 2) - (FANGKUAI_SIZE / 2);
	FangKuai8.RightMid.right = FangKuai8.RightMid.left + FANGKUAI_SIZE;
	FangKuai8.RightMid.bottom = FangKuai8.RightMid.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.RightMid);

	FangKuai8.RightBottom.left = FangKuai8.EntityRct.right;
	FangKuai8.RightBottom.top = FangKuai8.EntityRct.bottom;
	FangKuai8.RightBottom.right = FangKuai8.RightBottom.left + FANGKUAI_SIZE;
	FangKuai8.RightBottom.bottom = FangKuai8.RightBottom.top + FANGKUAI_SIZE;
	pPropImg->DrawImage(MemDc, FangKuai8.RightBottom);
}

// 取得窗口中设置属性的窗口大小
SIZE IWindowBaseImpl::PP_GetWindowPropSize()
{
	SIZE WndSize;
	WndSize.cx = WndSize.cy = 0;
	if (m_pPropSize_WindowWidth != NULL && m_pPropSize_WindowHeight != NULL)
	{
		WndSize.cx = m_pPropSize_WindowWidth->GetValue();
		WndSize.cy = m_pPropSize_WindowHeight->GetValue();
	}

	return WndSize;
}

// 得到在Builder中显示的四周8个方块的位置
FANGKUAI_8* IWindowBaseImpl::BD_GetFangKuai8Rect()
{
	return &m_BD_FangKuai8;
}

// 取得子控件列表
CHILD_CTRLS_VEC* IWindowBaseImpl::GetChildControlsVec()
{
	return &m_ChildCtrlsVec;
}

// 取得当前输入位置的控件
IControlBase* IWindowBaseImpl::BD_GetMouseInControl(POINT pt)
{
	if (!::PtInRect(&(m_BD_FangKuai8.EntityRct), pt))
		return NULL;

	IControlBase* pControl = NULL;
	BD_CheckMouseInControl(&m_ChildCtrlsVec, pt, &pControl);
	return pControl;
}

bool IWindowBaseImpl::BD_CheckMouseInControl(CHILD_CTRLS_VEC *pCtrlVec, POINT pt, IControlBase** ppControl)
{
	if (pCtrlVec == NULL || ppControl == NULL)
		return false;

	ListObj* pPrior = NULL;
	for (ListObj* pItem = pCtrlVec->LastObj(); pItem != NULL; pItem = pPrior)
	{
		pPrior = pCtrlVec->PriorObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl != NULL && pCtrl->IsVisible())
		{
			FANGKUAI_8 *pCtrlFangkuai8 = pCtrl->BD_GetFangKuai8Rect();
			if (::PtInRect(&pCtrlFangkuai8->EntityRct, pt))
			{
				*ppControl = pCtrl;
				CHILD_CTRLS_VEC *pCtrlsVec = pCtrl->GetChildControlsVec();
				BD_CheckMouseInControl(pCtrlsVec, pt, ppControl);
				return true;
			}
		}
	}

	return false;
}

// 取得当前激活的控件
IControlBase* IWindowBaseImpl::BD_GetFocusControl()
{
	return m_pFocusCtrl;
}

// 设置当前激活的控件
void IWindowBaseImpl::BD_SetFocusControl(IControlBase* pControl)
{
	m_pFocusCtrl = pControl;
}

// 设置控件在编辑模式下的位置和大小
void IWindowBaseImpl::BD_SetControlRect(IControlBase* pControl, RECT RctInView)
{
	if (pControl == NULL || pControl->BD_GetFangKuai8Rect() == NULL)
		return;

	FANGKUAI_8* pFk8 = pControl->BD_GetFangKuai8Rect();
	pFk8->EntityRct = RctInView;

	RECT InWindowRect;
	INIT_RECT(InWindowRect);
	InWindowRect.left = RctInView.left - m_BD_FangKuai8.EntityRct.left;
	InWindowRect.right = InWindowRect.left + RECT_WIDTH(RctInView);
	InWindowRect.top = RctInView.top - m_BD_FangKuai8.EntityRct.top;
	InWindowRect.bottom = InWindowRect.top + RECT_HEIGHT(RctInView);

	pControl->SetWindowRectLayoutWithChild(InWindowRect);
}

// 初始化所有控件在Builder中的显示位置
void IWindowBaseImpl::BD_SetAllCtrlRectInView()
{
	BD_SetControlRectInView(&m_ChildCtrlsVec);
}

void IWindowBaseImpl::BD_SetControlRectInView(CHILD_CTRLS_VEC *pChildCtrlsVec)
{
	if (pChildCtrlsVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL || pCtrl->GetOwnerWindow() == NULL)
			continue;

		FANGKUAI_8* pSetFk8 = pCtrl->BD_GetFangKuai8Rect();
		if (pSetFk8 == NULL)
			continue;

		FANGKUAI_8* pWndFk8 = pCtrl->GetOwnerWindow()->BD_GetFangKuai8Rect();
		if (pWndFk8 == NULL)
			continue;

		RECT CtrlRct = pCtrl->GetWindowRect();

		pSetFk8->EntityRct.left = pWndFk8->EntityRct.left + CtrlRct.left;
		pSetFk8->EntityRct.right = pSetFk8->EntityRct.left + RECT_WIDTH(CtrlRct);
		pSetFk8->EntityRct.top = pWndFk8->EntityRct.top + CtrlRct.top;
		pSetFk8->EntityRct.bottom = pSetFk8->EntityRct.top + RECT_HEIGHT(CtrlRct);

		BD_SetControlRectInView(pCtrl->GetChildControlsVec());
	}
}

// 重新计算子控件的位置和大小
void IWindowBaseImpl::ResetChildCtrlPostion(CHILD_CTRLS_VEC* pChildVec, bool bMemToProp)
{
	if (pChildVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL || pCtrl->GetOwnerWindow() == NULL)
			continue;

		RECT ParentRct;
		INIT_RECT(ParentRct);
		IControlBase* pParentCtrl = pCtrl->GetParentControl();
		if (pParentCtrl != NULL)
			ParentRct = pParentCtrl->GetWindowRect();
		else
			ParentRct = pCtrl->GetOwnerWindow()->GetClientRect();

		SetControlWindowLayoutPostion(pCtrl, ParentRct);

		if (bMemToProp)
			pCtrl->SetWindowRectLayoutWithChild(pCtrl->GetWindowRect());

		ResetChildCtrlPostion(pCtrl->GetChildControlsVec(), bMemToProp);
	}
}

// 在任务栏上隐藏主窗口按钮
void IWindowBaseImpl::HideInTaskbar()
{
	HRESULT hr = NULL;
	ITaskbarList* pTaskbarList = NULL;

	hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&pTaskbarList);
	if (hr == S_OK && pTaskbarList != NULL)
	{
		pTaskbarList->HrInit();

		::SetWindowTextA(this->GetSafeHandle(), "");

		DWORD dwLong = ::GetWindowLong(this->GetSafeHandle(), GWL_STYLE);
		dwLong &= (~WS_CAPTION);
		::SetWindowLong(this->GetSafeHandle(), GWL_STYLE, dwLong);

		dwLong = ::GetWindowLong(this->GetSafeHandle(), GWL_EXSTYLE);
		dwLong &= (~WS_EX_APPWINDOW);
		dwLong |= WS_EX_TOOLWINDOW;
		::SetWindowLong(this->GetSafeHandle(), GWL_EXSTYLE, dwLong);

		pTaskbarList->DeleteTab(this->GetSafeHandle());
		pTaskbarList->Release();
	}
}

// 设置普通定时器
int IWindowBaseImpl::SetTimer(DWORD uElapse)
{
	if (m_pUiKernel == NULL || !::IsWindow(m_hWnd) || uElapse <= 0)
		return -1;

	int nId = m_pUiKernel->GetTimerId();
	::SetTimer(m_hWnd, nId, uElapse, NULL);
	return nId;
}

void IWindowBaseImpl::KillTimer(int nId)
{
	::KillTimer(m_hWnd, nId);
}

// 设置轻量级通用动画定时器
void IWindowBaseImpl::SetAnimationTimer()
{
	if (m_pUiKernel == NULL || !::IsWindow(m_hWnd))
		return;

	::SetTimer(m_hWnd, UM_DFT_ANIMATION_TIMER, UM_DFT_ANIMATION_TIMER_100S, NULL);
}

// 设置高精度定时器
int IWindowBaseImpl::SetHighAccuracyTimer(DWORD uElapse)
{
	if (m_pHighTimer == NULL)
		m_pHighTimer = new CHighAccuracyTimer;

	if (m_pHighTimer == NULL)
		return -1;

	return m_pHighTimer->SetTimer(this, uElapse);
}

void IWindowBaseImpl::KillHighAccuracyTimer(int nId)
{
	if (m_pHighTimer != NULL)
		m_pHighTimer->KillTimer();
}

// 设置控件回调函数
void IWindowBaseImpl::SetCtrlMsgCallBack(IControlMessage* pCtrlMsg)
{
	m_pCtrlMsgCallBack = pCtrlMsg;
}

// 取得整个程序默认的字体信息
IPropertyFontBase* IWindowBaseImpl::GetDefaultFontBase()
{
	if (m_pSkinPropMgr == NULL)
		return NULL;

	return m_pSkinPropMgr->GetDefaultFontBase(GetSkinFileItem());
}

// 控件消息回调函数
LRESULT IWindowBaseImpl::OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (m_pCtrlMsgCallBack == NULL)
		return -1;

	if (pCtrl != NULL && pCtrl->GetControlMessage() != NULL)
		return pCtrl->GetControlMessage()->OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);
	else
		return m_pCtrlMsgCallBack->OnCtrlMessage(pCtrl, nMsgId, wParam, lParam);
}

// 向内核注册一个想要取到的消息：同步处理方式，控件请不要再自己注册了的消息的处理过程中执行同步处理
void IWindowBaseImpl::RegisterControlMessageSync(IControlBase* pCtrl, int nMsgId)
{
	DoRegisterControlMessage(pCtrl, nMsgId);
}

void IWindowBaseImpl::UnRegisterControlMessageSync(IControlBase* pCtrl, int nMsgId)
{
	DoUnRegisterControlMessage(pCtrl, nMsgId);
}

// 向内核注册一个想要取到的消息
void IWindowBaseImpl::RegisterControlMessage(IControlBase* pCtrl, int nMsgId)
{
	LPARAM_DATA *pLpData = new LPARAM_DATA;
	if (pLpData == NULL)
		return;

	pLpData->wParam = (WPARAM)pCtrl;
	pLpData->lParam = (LPARAM)nMsgId;

	::PostMessage(m_hWnd, UI_FEATURE_KERNEL_MSG, WID_REG_CTRL_MSG, (LPARAM)pLpData);
}

void IWindowBaseImpl::DoRegisterControlMessage(IControlBase* pCtrl, int nMsgId)
{
	if (pCtrl == NULL || nMsgId < 0)
		return;

	REG_MSG_MAP::iterator pMsgItem = m_CtrlRegMsgMap.find(nMsgId);
	if (pMsgItem == m_CtrlRegMsgMap.end())
	{
		REG_CTRL_VEC CtrlRegMap;
		CtrlRegMap.clear();
		m_CtrlRegMsgMap.insert(pair<int, REG_CTRL_VEC>(nMsgId, CtrlRegMap));
	}

	pMsgItem = m_CtrlRegMsgMap.find(nMsgId);
	if (pMsgItem == m_CtrlRegMsgMap.end())
		return;

	REG_CTRL_VEC& CtrlMap = pMsgItem->second;
	for (REG_CTRL_VEC::iterator pCtrlItem = CtrlMap.begin(); pCtrlItem != CtrlMap.end(); pCtrlItem++)
	{
		IControlBase* pComCtrl = *pCtrlItem;
		if (pComCtrl == pCtrl)
			return;
	}

	CtrlMap.push_back(pCtrl);
}

void IWindowBaseImpl::UnRegisterControlMessage(IControlBase* pCtrl, int nMsgId)
{
	LPARAM_DATA *pLpData = new LPARAM_DATA;
	if (pLpData == NULL)
		return;

	pLpData->wParam = (WPARAM)pCtrl;
	pLpData->lParam = (LPARAM)nMsgId;

	::PostMessage(m_hWnd, UI_FEATURE_KERNEL_MSG, WID_UNREG_CTRL_MSG, (LPARAM)pLpData);
}

void IWindowBaseImpl::DoUnRegisterControlMessage(IControlBase* pCtrl, int nMsgId)
{
	if (pCtrl == NULL || nMsgId < 0)
		return;

	REG_MSG_MAP::iterator pMsgItem = m_CtrlRegMsgMap.find(nMsgId);
	if (pMsgItem == m_CtrlRegMsgMap.end())
		return;

	REG_CTRL_VEC& CtrlMap = pMsgItem->second;
	for (REG_CTRL_VEC::iterator pCtrlItem = CtrlMap.begin(); pCtrlItem != CtrlMap.end(); pCtrlItem++)
	{
		IControlBase* pComCtrl = *pCtrlItem;
		if (pComCtrl == pCtrl)
		{
			CtrlMap.erase(pCtrlItem);
			break;
		}
	}

	if (CtrlMap.size() <= 0)
		m_CtrlRegMsgMap.erase(pMsgItem);
}

int IWindowBaseImpl::DispatchRegisterMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (m_CtrlRegMsgMap.size() <= 0)
		return S_OK;

	REG_MSG_MAP::iterator pMsgItem = m_CtrlRegMsgMap.find(nMsgId);
	if (pMsgItem == m_CtrlRegMsgMap.end())
		return S_OK;

	REG_CTRL_VEC& CtrlMap = pMsgItem->second;
	for (REG_CTRL_VEC::iterator pCtrlItem = CtrlMap.begin(); pCtrlItem != CtrlMap.end(); pCtrlItem++)
	{
		IControlBase* pCtrl = *pCtrlItem;
		if (pCtrl == NULL)
			continue;

		if (CTRL_REG_MSG_NOT_PASS_ON == pCtrl->OnCtrlNotify(nMsgId, wParam, lParam))
			return CTRL_REG_MSG_NOT_PASS_ON;
	}
	return S_OK;
}

// 发送消息:Send方式
LRESULT IWindowBaseImpl::SendMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	LRESULT lpRes = -1;
	if (IS_SAFE_HANDLE(m_hWnd))
		lpRes = ::SendMessage(m_hWnd, nMsgId, wParam, lParam);

	return lpRes;
}

// 发送消息:Post方式
bool IWindowBaseImpl::PostMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	bool bRet = false;
	if (IS_SAFE_HANDLE(m_hWnd))
		bRet = (::PostMessage(m_hWnd, nMsgId, wParam, lParam) == TRUE);

	return bRet;
}

// 强迫重绘窗口，所有控件均重绘
void IWindowBaseImpl::CompelRedrawWindow(RECT *pDrawRect)
{
	CompelRedrawControl(&m_ChildCtrlsVec);
	RedrawWindow();
}

// 强迫子控件重绘
void IWindowBaseImpl::CompelRedrawControl(CHILD_CTRLS_VEC *pCtrlVec)
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

		pCtrl->RedrawControl();
		CompelRedrawControl(pCtrl->GetChildControlsVec());
	}
}

// 向所有控件发送进入/离开对话框移动/拉伸消息
void IWindowBaseImpl::SendEnterOrExitSizeMsgToCtrl(bool bIsEnter, CHILD_CTRLS_VEC *pChildCtrlsVec)
{
	if (pChildCtrlsVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (bIsEnter)
			pCtrl->OnEnterSizeMove();
		else
			pCtrl->OnExitSizeMove();

		SendEnterOrExitSizeMsgToCtrl(bIsEnter, pCtrl->GetChildControlsVec());
	}
}

// 得到当前窗口的皮肤包数据
SKIN_FILE_ITEM* IWindowBaseImpl::GetSkinFileItem()
{
	if (m_strSkinPath.GetLength() <= 0 || m_pUiKernel == NULL || m_pUiKernel->GetSkinManager() == NULL)
		return NULL;

	return m_pUiKernel->GetSkinManager()->GetSkinFileItem(m_strSkinPath);
}

bool IWindowBaseImpl::ShowSystemTips(LPCWSTR pszTips, bool bBaloon, int nShowTime)
{
	if (pszTips == NULL || wcslen(pszTips) <= 0 || nShowTime <= 0)
		return false;

	if (m_pSysTips == NULL)
		m_pSysTips = new CSystemTips;

	if (m_pSysTips == NULL)
		return false;

	return m_pSysTips->ShowSysTips(m_hWnd, pszTips, bBaloon, nShowTime);
}

bool IWindowBaseImpl::DestroySystemTips()
{
	if (m_pSysTips == NULL)
		return false;

	return m_pSysTips->DestroySysTips();
}

// 删除控件后需要清空相关的控件指针
void IWindowBaseImpl::ResetActiveControl()
{
	m_pMouseHoverCtrl = NULL;
	m_pLButtonDownCtrl = NULL;
	m_pFocusCtrl = NULL;
	m_pTabCtrl = NULL;

	if (m_pRegFocusCtrl != NULL)
		m_pRegFocusCtrl->OnContrlFocusChange(m_pTabCtrl);
}

// 注册焦点控件，用于绘制用户自定义的焦点状态，当一个可以取得焦点的控件取得焦点时
// 被注册的焦点控件就会收到焦点改变的消息
void IWindowBaseImpl::RegisterFocusControl(IControlBase* pFocusCtrl)
{
	m_pRegFocusCtrl = pFocusCtrl;
}

// 取得当前的焦点控件
IControlBase* IWindowBaseImpl::GetFocusControl()
{
	return m_pFocusCtrl;
}

// 取得当前tab停留的控件
IControlBase* IWindowBaseImpl::GetCurrentTabControl()
{
	return m_pTabCtrl;
}

// 窗口需要tab切换
void IWindowBaseImpl::OnTabChange()
{
	if (m_pTabCtrl == NULL)
	{
		m_pTabCtrl = GetNextTabOrderControl(NOT_SET_TAB_FOCUS);
	}
	else
	{
		m_pTabCtrl = GetNextTabOrderControl(m_pTabCtrl->PP_GetControlTabOrder());
		if (m_pTabCtrl == NULL)
			m_pTabCtrl = GetNextTabOrderControl(NOT_SET_TAB_FOCUS);
	}

	if (m_pTabCtrl != NULL && m_pTabCtrl != m_pFocusCtrl)
	{
		if (m_pFocusCtrl != NULL)
		{
			m_pFocusCtrl->OnKillFocus();
			m_pFocusCtrl = NULL;
		}

		m_pFocusCtrl = m_pTabCtrl;
		m_pFocusCtrl->OnSetFocus();
	}

	if (m_pRegFocusCtrl != NULL)
		m_pRegFocusCtrl->OnContrlFocusChange(m_pTabCtrl);
}

// 取得下一个taborder的控件
IControlBase* IWindowBaseImpl::GetNextTabOrderControl(int nFromTabOrder)
{
	if (m_pPropBase_BigTabOrder == NULL || m_pPropBase_BigTabOrder->GetValue() <= NOT_SET_TAB_FOCUS)
		return NULL;

	if (nFromTabOrder < NOT_SET_TAB_FOCUS)
		nFromTabOrder = NOT_SET_TAB_FOCUS;

	int nBigTabOrder = m_pPropBase_BigTabOrder->GetValue();
	nFromTabOrder++;

	while (nFromTabOrder <= nBigTabOrder)
	{
		IControlBase* pFind = GetTabOrderCtrl(&m_ChildCtrlsVec, nFromTabOrder);
		if (pFind != NULL)
			return pFind;

		nFromTabOrder++;
	}

	return NULL;
}

// 取得指定taborder的控件
IControlBase* IWindowBaseImpl::GetTabOrderCtrl(CHILD_CTRLS_VEC* pChildVec, int nOrder)
{
	if (pChildVec == NULL || nOrder <= NOT_SET_TAB_FOCUS)
		return NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL || pCtrl->GetOwnerWindow() == NULL)
			continue;

		if (pCtrl->PP_GetControlTabOrder() == nOrder)
			return pCtrl;

		IControlBase* pFind = GetTabOrderCtrl(pCtrl->GetChildControlsVec(), nOrder);
		if (pFind != NULL)
			return pFind;
	}

	return NULL;
}

// 取得默认的回车键接受控件
IControlBase* IWindowBaseImpl::GetDefaultEnterCtrl(CHILD_CTRLS_VEC* pChildVec)
{
	if (pChildVec == NULL)
		return NULL;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		if (!pCtrl->IsVisible() || !pCtrl->IsEnable())
			continue;

		if (pCtrl->PP_IsDefaultEnterCtrl())
			return pCtrl;

		IControlBase* pFind = GetDefaultEnterCtrl(pCtrl->GetChildControlsVec());
		if (pFind != NULL)
			return pFind;
	}

	return NULL;
}

// 注册一个需要得到窗口所有消息的控件，这个控件在取得焦点之后会收到所有窗口消息
void IWindowBaseImpl::RegisterWindowMessageControl(IControlBase* pCtrl)
{
	m_pWindowMessageCtrl = pCtrl;
}
