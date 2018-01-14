#include "StdAfx.h"
#include "ICtrlHwndObjImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\ICommonFun.h"
#include <algorithm>

ICtrlHwndObj::ICtrlHwndObj(IUiFeatureKernel *pUiKernel): ICtrlInterface(pUiKernel) {}


ICtrlHwndObjImpl::ICtrlHwndObjImpl(IUiFeatureKernel *pUiKernel) : ICtrlHwndObj(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_HWNDOBJ);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_theHwnd = NULL;
}

ICtrlHwndObjImpl::~ICtrlHwndObjImpl()
{
	
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlHwndObjImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlHwndObjImpl::CreateControlPropetry(bool bIsNewCtrl)
{

	if (bIsNewCtrl)
	{
		if (m_pPropBase_NoDrawCtrl != NULL)
			m_pPropBase_NoDrawCtrl->SetValue(true);
	}
	return true;
}

// 初始化控件
void ICtrlHwndObjImpl::OnCreate()
{

}

// 控件初始化完毕
void ICtrlHwndObjImpl::OnFinalCreate()
{
	this->RegisterControlMessage(WM_MOVE);
	this->RegisterControlMessage(WM_SHOWWINDOW);
	this->RegisterControlMessage(WM_SYSCOMMAND);
}

// 销毁控件
void ICtrlHwndObjImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlHwndObjImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 这个接口会在整个对话框初始化完成的时候调用
// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
void ICtrlHwndObjImpl::OnWindowFinalCreate()
{
	
}

// 附属对话框即将关闭
void ICtrlHwndObjImpl::OnWindowClose()
{
	this->UnRegisterControlMessage(WM_MOVE);
	this->UnRegisterControlMessage(WM_SHOWWINDOW);
	this->UnRegisterControlMessage(WM_SYSCOMMAND);
}


void ICtrlHwndObjImpl::SetHwndWindowPostion()
{
	if (m_theHwnd!=NULL && ::IsWindow(m_theHwnd) && ::IsWindowVisible(m_theHwnd))
	{
		/*vector<HWND>::iterator result = find(m_vectorHwnd.begin(),m_vectorHwnd.end(),m_theHwnd);
		if (result==m_vectorHwnd.end())
		{
			return;
		}*/
		if (!IsHwndExist(m_theHwnd))
		{
			return;
		}
		RECT CtrlRct = this->GetWindowRect();
		if (m_pWindowBase->PP_GetLayeredWindow())
		{
			RectClientToScreen(m_pWindowBase->GetSafeHandle(), CtrlRct);
			//::MoveWindow(m_theHwnd,CtrlRct.left,CtrlRct.top,RECT_WIDTH(CtrlRct),RECT_HEIGHT(CtrlRct),true);
			::SetWindowPos(m_theHwnd, NULL, CtrlRct.left, CtrlRct.top, 
				RECT_WIDTH(CtrlRct),RECT_HEIGHT(CtrlRct), SWP_SHOWWINDOW);
		}
		else
		{
			::SetWindowPos(m_theHwnd, NULL, CtrlRct.left, CtrlRct.top, 
				RECT_WIDTH(CtrlRct),RECT_HEIGHT(CtrlRct), SWP_SHOWWINDOW);
		}
	}
	
}

// 移动、设置控件位置
void ICtrlHwndObjImpl::OnSize()
{
	SetHwndWindowPostion();
}

int ICtrlHwndObjImpl::OnWindowMove(WPARAM wParam, LPARAM lParam)
{
	SetHwndWindowPostion();
	return S_OK;
}

int ICtrlHwndObjImpl::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	if (m_theHwnd!=NULL && ::IsWindow(m_theHwnd))
	{
		if (m_pWindowBase->PP_GetLayeredWindow())
		{
			if (wParam == SC_RESTORE)
			{
				DWORD dwStyle = ::GetWindowLong(m_theHwnd, GWL_STYLE);
				dwStyle &= (~WS_CHILD);
				dwStyle |= WS_POPUP;
				::SetWindowLong(m_theHwnd, GWL_STYLE, dwStyle);
			}
			else if (wParam == SC_MINIMIZE)
			{
				DWORD dwStyle = ::GetWindowLong(m_theHwnd, GWL_STYLE);
				dwStyle &= (~WS_POPUP);
				dwStyle |= WS_CHILD;
				::SetWindowLong(m_theHwnd, GWL_STYLE, dwStyle);
			}
		}
	}

	return S_OK;
}


//设置窗口与HwndObj进行相互绑定
BOOL ICtrlHwndObjImpl::Attach(HWND hwnd)
{
	if (hwnd == NULL || !::IsWindow(hwnd))
	{
		return FALSE;
	}
	vector<HWND>::iterator result = find(m_vectorHwnd.begin(),m_vectorHwnd.end(),hwnd);
	if (result!=m_vectorHwnd.end())
	{
		return TRUE;
	}

	m_vectorHwnd.push_back(hwnd);

	if(m_theHwnd!=NULL && ::IsWindow(m_theHwnd))
	{
		::ShowWindow(m_theHwnd,SW_HIDE);
	}
	//::ShowWindow(hwnd,SW_SHOW);
	m_theHwnd = hwnd;

	HWND hWndManager = GetOwnerWindowHwnd();
	if(hWndManager == NULL) return FALSE;

	if (m_pWindowBase->PP_GetLayeredWindow())
	{
		::SetParent(hwnd,NULL);
		DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
		dwStyle &= (~WS_CHILD);
		dwStyle |= WS_POPUP;
		dwStyle |= WS_CLIPCHILDREN;
		::SetWindowLong(hwnd, GWL_STYLE, dwStyle);
	}
	else
	{
		::SetParent(hwnd,hWndManager);

		DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
		dwStyle &= (~WS_POPUP);
		dwStyle |= WS_CHILD;
		::SetWindowLong(hwnd, GWL_STYLE, dwStyle);
	}

	SetHwndWindowPostion();
}

//解除窗口与HwndObj的绑定
BOOL ICtrlHwndObjImpl::Detach(HWND hwnd)
{
	if (hwnd == NULL || !::IsWindow(hwnd))
	{
		return FALSE;
	}

	for (vector<HWND>::iterator it = m_vectorHwnd.begin();it != m_vectorHwnd.end();)
	{
		if (*it == hwnd)
		{
			it = m_vectorHwnd.erase(it);
			break;
		}
		++it;
	}

	return TRUE;
}

//根据序号获取绑定的窗口句柄
HWND ICtrlHwndObjImpl::GetSafeHwnd(int nIndex)
{
	if (nIndex >= m_vectorHwnd.size())
	{
		return NULL;
	}

	return m_vectorHwnd.at(nIndex);
}

//获取HwndObj当前绑定的窗口句柄
HWND ICtrlHwndObjImpl::GetBindHwnd()
{
	return m_theHwnd;
}

//显示HideBindWindow隐藏起来的窗口
BOOL ICtrlHwndObjImpl::ShowBindWindow()
{
	if(m_theHwnd!=NULL && ::IsWindow(m_theHwnd))
	{
		::ShowWindow(m_theHwnd,SW_SHOW);
		SetHwndWindowPostion();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

//隐藏当前绑定的窗口
BOOL ICtrlHwndObjImpl::HideBindWindow()
{
	if(m_theHwnd!=NULL && ::IsWindow(m_theHwnd))
	{
		::ShowWindow(m_theHwnd,SW_HIDE);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//移动与HwndObj进行绑定的窗口到指定区域
void ICtrlHwndObjImpl::MoveHwndToRect(RECT rect)
{
	if(m_theHwnd==NULL || !::IsWindow(m_theHwnd))
	{
		return;
	}

	if (m_pWindowBase->PP_GetLayeredWindow())
	{
		RectClientToScreen(m_pWindowBase->GetSafeHandle(), rect);
		::SetWindowPos(m_theHwnd, NULL, rect.left, rect.top,RECT_WIDTH(rect),RECT_HEIGHT(rect),SWP_SHOWWINDOW);
	}
	else
	{
		::SetWindowPos(m_theHwnd, NULL, rect.left, rect.top, 
			RECT_WIDTH(rect),RECT_HEIGHT(rect), SWP_SHOWWINDOW);
	}
}

//根据窗口句柄来显示与HwndObj绑定的窗口
BOOL ICtrlHwndObjImpl::ShowWindow(HWND hwnd)
{
	if (hwnd == m_theHwnd || hwnd == NULL || !IsWindow(hwnd) || !IsHwndExist(hwnd))
	{
		return FALSE;
	}
	
	HideBindWindow();
	m_theHwnd = hwnd;
	ShowBindWindow();
}

//根据索引来显示与HwndObj绑定的窗口
BOOL ICtrlHwndObjImpl::ShowWindowByIndex(int nIndex)
{
	if (nIndex >= m_vectorHwnd.size())
	{
		return FALSE;
	}
	if ( m_vectorHwnd.at(nIndex)!= NULL && ::IsWindow(m_vectorHwnd.at(nIndex)))
	{
		HideBindWindow();
		m_theHwnd = m_vectorHwnd.at(nIndex);
		ShowBindWindow();

		return TRUE;
	}

	return FALSE;
}



BOOL ICtrlHwndObjImpl::IsHwndExist(HWND hwnd)
{
	if (hwnd == NULL || !::IsWindow(hwnd))
	{
		return FALSE;
	}
	vector<HWND>::iterator result = find(m_vectorHwnd.begin(),m_vectorHwnd.end(),hwnd);
	if (result!=m_vectorHwnd.end())
	{
		return TRUE;
	}

	return FALSE;
}