
#include "StdAfx.h"
#include "ICtrl_XXXX_Impl.h"

ICtrl_XXXX_::ICtrl_XXXX_(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrl_XXXX_Impl::ICtrl_XXXX_Impl(IUiFeatureKernel *pUiKernel) : ICtrl_XXXX_(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_BASE_PANEL);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;
}

ICtrl_XXXX_Impl::~ICtrl_XXXX_Impl()
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrl_XXXX_Impl::CreateControlPropetry(bool bIsNewCtrl)
{
	return true;
}

// 初始化控件
void ICtrl_XXXX_Impl::OnCreate()
{
}

// 控件初始化完毕
void ICtrl_XXXX_Impl::OnFinalCreate()
{
}

// 销毁控件
void ICtrl_XXXX_Impl::OnDestroy()
{
}

// 绘制控件
void ICtrl_XXXX_Impl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrl_XXXX_Impl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}
