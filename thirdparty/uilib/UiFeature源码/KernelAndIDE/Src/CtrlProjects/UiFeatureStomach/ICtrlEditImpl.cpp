
#include "StdAfx.h"
#include "ICtrlEditImpl.h"
#include <assert.h>
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\ICommonFun.h"

ICtrlEdit::ICtrlEdit(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlEditImpl::ICtrlEditImpl(IUiFeatureKernel *pUiKernel) : ICtrlEdit(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_EDIT);

	m_pPropEditBackgroundImg = NULL;
	m_pPropMargins = NULL;
	m_pPropLeft = NULL;
	m_pPropTop = NULL;
	m_pPropRight = NULL;
	m_pPropBottom = NULL;
}

ICtrlEditImpl::~ICtrlEditImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlEditImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlEditImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropEditBackgroundImg = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "EditBackgroundImage", "纯粹EDIT的背景图片");

	m_pPropMargins = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "Margins", "编辑框的边距");
	m_pPropLeft = (IPropertyInt*)CreatePropetry(m_pPropMargins, OTID_INT, "Left", "编辑框的左边距");
	m_pPropTop = (IPropertyInt*)CreatePropetry(m_pPropMargins, OTID_INT, "Top", "编辑框的上边距");
	m_pPropRight = (IPropertyInt*)CreatePropetry(m_pPropMargins, OTID_INT, "Right", "编辑框的右边距");
	m_pPropBottom = (IPropertyInt*)CreatePropetry(m_pPropMargins, OTID_INT, "Bottom", "编辑框的下边距");

	m_EditWnd.CreateEditPropetry(dynamic_cast<IControlBase*>(this), bIsNewCtrl);
	return true;
}

// 初始化控件
void ICtrlEditImpl::OnCreate()
{
	RECT rcWindow = GetEditWindowRect();
	if (!m_EditWnd.CreateEditWindow(rcWindow, SW_SHOW))
	{
		assert(false);
		return;
	}
}

RECT ICtrlEditImpl::GetEditWindowRect()
{
	int nLeft = 0;
	int nTop = 0;
	int nRight = 0;
	int nBottom = 0;

	if (NULL != m_pPropLeft && NULL != m_pPropTop && NULL != m_pPropRight && NULL != m_pPropBottom)
	{
		nLeft = m_pPropLeft->GetValue();
		nTop = m_pPropTop->GetValue();
		nRight = m_pPropRight->GetValue();
		nBottom = m_pPropBottom->GetValue();
	}

	RECT rcWindow = GetClientRect();
	rcWindow.left += nLeft;
	rcWindow.top += nTop;
	rcWindow.right -= nRight;
	rcWindow.bottom -= nBottom;

	return rcWindow;
}

// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
int ICtrlEditImpl::OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase != NULL)
	{
		if (CM_NORMAL_EDIT_KEY_DOWN == nMsgId && wParam == VK_ESCAPE)
			::PostMessage(m_pWindowBase->GetSafeHandle(), WM_KEYDOWN, wParam, lParam);

		if (CM_NORMAL_EDIT_KEY_DOWN == nMsgId && wParam == VK_RETURN)
			::PostMessage(m_pWindowBase->GetSafeHandle(), WM_KEYDOWN, wParam, lParam);

		if (nMsgId > CM_NORMAL_EDIT_BASE && nMsgId < CM_NORMAL_EDIT_MESSAGE_END)
		{
			m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), nMsgId, wParam, lParam);
			return S_OK;
		}
	}
	m_EditWnd.OnEditNotify(nMsgId, wParam, lParam);

	return S_OK;
}

// 控件初始化完毕
void ICtrlEditImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlEditImpl::OnDestroy()
{
	m_EditWnd.CloseWindow();
}

// 绘制控件
void ICtrlEditImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (NULL != m_pPropEditBackgroundImg)
	{
		m_pPropEditBackgroundImg->DrawImage(DrawBoard, GetClientRect());
	}
}

// 移动、设置控件位置
void ICtrlEditImpl::OnSize()
{
	m_EditWnd.SetRectInOwner(GetEditWindowRect());
}

// 可见属性
void ICtrlEditImpl::SetVisible(bool bVisible, bool bSetChild)
{
	ICtrlEdit::SetVisible(bVisible, bSetChild);

	if (bVisible && this->IsVisible())
		m_EditWnd.ShowWindow(SW_SHOW);
	else
		m_EditWnd.ShowWindow(SW_HIDE);
}

LPCWSTR ICtrlEditImpl::GetEditText()
{
	CStringW strText = L"";
	m_EditWnd.GetEditText(strText);
	return strText;
}

void ICtrlEditImpl::SetEditText(LPCWSTR pszText)
{
	m_EditWnd.SetEditText(pszText);
}

void ICtrlEditImpl::SetEditFocus()
{
	m_EditWnd.PostActiveMessage();
}

// 判断当前edit是否得到输入焦点
bool ICtrlEditImpl::IsForegroundWindow()
{
	return m_EditWnd.IsForegroundWindow();
}

// 取得Edit托管窗口的句柄
HWND ICtrlEditImpl::GetWindowHwnd()
{
	return m_EditWnd.GetWindowHwnd();
}

// 取得Edit窗口的句柄
HWND ICtrlEditImpl::GetEditHwnd()
{
	return m_EditWnd.GetEditHwnd();
}

void ICtrlEditImpl::EnableEditWindow(BOOL bEnable)
{
	this->SetEnable((bEnable == TRUE));
	m_EditWnd.EnableEditWindow(bEnable);
}

void ICtrlEditImpl::SetReadOnly(BOOL bReadOnly)
{
	m_EditWnd.SetReadOnly(bReadOnly);
}

void ICtrlEditImpl::GetEditRemindText(CStringW &strText)
{
	m_EditWnd.GetEditRemindText(strText);
}

void ICtrlEditImpl::SetEditRemindText(WCHAR *pszText)
{
	m_EditWnd.SetEditRemindText(pszText);
}

void ICtrlEditImpl::SetEditToRemindText()
{
	m_EditWnd.SetEditToRemindText();
}

void ICtrlEditImpl::SetEditAutoVScroll(bool bVScroll)
{
	m_EditWnd.SetEditAutoVScroll(bVScroll);
}

void ICtrlEditImpl::SetEditAutoHScroll(bool bHScroll)
{
	m_EditWnd.SetEditAutoHScroll(bHScroll);
}

void ICtrlEditImpl::SetEditMultiLine(bool bMultiline/* = false*/)
{
	m_EditWnd.SetEditMultiLine(bMultiline);
}
