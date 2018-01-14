
#include "StdAfx.h"
#include "ICtrlAndroidListImpl.h"

ICtrlAndroidList::ICtrlAndroidList(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlAndroidListImpl::ICtrlAndroidListImpl(IUiFeatureKernel *pUiKernel) : ICtrlAndroidList(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_ANDROID_LIST);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;
}

ICtrlAndroidListImpl::~ICtrlAndroidListImpl()
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlAndroidListImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	return true;
}

// 初始化控件
void ICtrlAndroidListImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlAndroidListImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlAndroidListImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlAndroidListImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlAndroidListImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
void ICtrlAndroidListImpl::OnCreateSubControl(bool bIsCreate)
{

}

// 隐藏 ListItem 中的一个子控件，并腾出这个控件的地方
void ICtrlAndroidListImpl::Gone(IControlBase* pGoneCtrl)
{

}

// 隐藏/显示 ListItem 中的一个子控件，被 gone 的控件也可以显示，并会重新占用原来的腾出来的空间
void ICtrlAndroidListImpl::Visible(IControlBase* pCtrl, bool bVisible)
{

}

// 在 UiFeatureBuilder 中设计出来的 ListItem 的模板，在运行时态一行要显示多少个
int ICtrlAndroidListImpl::GetTempletColumnCount()
{
	return 1;
}

// 整个列表是否为空
bool ICtrlAndroidListImpl::IsEmpty()
{
	return true;
}
