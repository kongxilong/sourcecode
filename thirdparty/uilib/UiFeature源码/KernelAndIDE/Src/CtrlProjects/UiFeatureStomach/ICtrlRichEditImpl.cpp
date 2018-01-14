
#include "StdAfx.h"
#include "ICtrlRichEditImpl.h"

ICtrlRichEdit::ICtrlRichEdit(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlRichEditImpl::ICtrlRichEditImpl(IUiFeatureKernel *pUiKernel) : ICtrlRichEdit(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_RICH_EDIT);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;
	m_pTextHost = NULL;
}

ICtrlRichEditImpl::~ICtrlRichEditImpl()
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlRichEditImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	return true;
}

// 初始化控件
void ICtrlRichEditImpl::OnCreate()
{
	if (m_pWindowBase == NULL)
		return;

	CREATESTRUCT createS;
	memset(&createS, 0, sizeof(CREATESTRUCT));

	createS.style = ES_MULTILINE;

	if (!CreateHost(m_pWindowBase->GetSafeHandle(), &createS, dynamic_cast<CHostCallBack*>(this), &m_pTextHost))
		return;
}

// 控件初始化完毕
void ICtrlRichEditImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlRichEditImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlRichEditImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlRichEditImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

//@cmember Notify host of events
HRESULT ICtrlRichEditImpl::TxNotify(DWORD iNotify, void *pv)
{

	return S_OK;
}

// 控件取得焦点，通过Tab键跳转，激活控件
void ICtrlRichEditImpl::OnSetFocus()
{
	if (m_pTextHost == NULL || m_pWindowBase == NULL)
		return;

	m_pWindowBase->RegisterWindowMessageControl(dynamic_cast<IControlBase*>(this));
}

// 控件失去焦点
void ICtrlRichEditImpl::OnKillFocus()
{
	m_pWindowBase->RegisterWindowMessageControl(NULL);
}

// 移动、设置控件位置
void ICtrlRichEditImpl::OnSize()
{
	if (m_pTextHost != NULL)
	{
	}
}

// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
int ICtrlRichEditImpl::OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (m_pTextHost == NULL || m_pWindowBase == NULL)
		return S_FALSE;

	m_pTextHost->TxWindowProc(m_pWindowBase->GetSafeHandle(), nMsgId, wParam, lParam);
	return S_OK;
}
