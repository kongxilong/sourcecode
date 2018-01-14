// 一个控件必须实现的所有接口

#include "stdafx.h"
#include "..\..\Inc\ICtrlInterface.h"

ICtrlInterface::ICtrlInterface(IUiFeatureKernel *pUiKernel) : IControlBase(pUiKernel)
{
}

ICtrlInterface::~ICtrlInterface()
{
}

// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
int ICtrlInterface::OnCtrlNotify(int nMsgId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// Builder刷新属性，此函数中只要重新加载控件属性即可
void ICtrlInterface::OnBuilderRefreshProp(IPropertyBase* pPropBase)
{
}

// 鼠标进入
void ICtrlInterface::OnMouseEnter(POINT pt)
{
}

// 鼠标移出
void ICtrlInterface::OnMouseLeave(POINT pt)
{
}

// 鼠标移动
void ICtrlInterface::OnMouseMove(POINT pt)
{
}

// 鼠标左键点击
void ICtrlInterface::OnLButtonDown(POINT pt)
{
}

// 鼠标左键抬起
void ICtrlInterface::OnLButtonUp(POINT pt)
{
}

// 鼠标左键双击
void ICtrlInterface::OnLButtonDbClick(POINT pt)
{
}

// 鼠标在控件内部拖动控件
void ICtrlInterface::OnMouseDragging(POINT pt)
{
}

// 鼠标右键点击
void ICtrlInterface::OnRButtonDown(POINT pt)
{
}

// 移动、设置控件位置
void ICtrlInterface::OnSize()
{
}

// 移动窗口开始
void ICtrlInterface::OnEnterSizeMove()
{
}

// 移动窗口结束
void ICtrlInterface::OnExitSizeMove()
{
}

// 控件取得焦点，通过Tab键跳转，激活控件
void ICtrlInterface::OnSetFocus()
{
}

// 控件失去焦点
void ICtrlInterface::OnKillFocus()
{
}

// 控件取得键盘输入消息
void ICtrlInterface::OnChar(WPARAM wParam, LPARAM lParam)
{
}

// 控件取得键盘输入消息
void ICtrlInterface::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
}

// 控件取得键盘输入消息
void ICtrlInterface::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
}

// 接受到默认回车键，执行控件操作
void ICtrlInterface::OnDefaultEnterCtrl()
{
}

// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
// 此函数由定时器触发，每100毫秒触发一次
bool ICtrlInterface::OnDrawAnimation()
{
	return false;
}

// 定时器
void ICtrlInterface::OnTimer(UINT nTimerId)
{
}

// 滚动条消息
void ICtrlInterface::OnVScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar)
{
}

void ICtrlInterface::OnHScroll(UINT nSBCode, UINT nPos, IControlBase* pScrollBar)
{
}

// 鼠标滚轮消息
void ICtrlInterface::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
}

// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
bool ICtrlInterface::OnCheckMouseInRgn(POINT pt, int nMouseMsgId)
{
	return true;
}

// 整个对话框包括对话框上的控件都初始化完毕
void ICtrlInterface::OnWindowFinalCreate()
{
}

// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
void ICtrlInterface::OnCreateSubControl(bool bIsCreate)
{
}

// 附属对话框即将关闭
void ICtrlInterface::OnWindowClose()
{
}

// 设置控件光标
IPropertyCursor* ICtrlInterface::OnSetCursor(POINT pt)
{
	return NULL;
}

// 鼠标拖动操作结束，抬起鼠标。pDragCtrl：拖拽来源控件
void ICtrlInterface::OnDragStop(IControlBase* pDragCtrl, POINT pt, WPARAM wParam, LPARAM lParam)
{
}

// 取得控件之间拖拽的数据参数，在触发了 OnDragStop 消息后会被调用
void ICtrlInterface::OnGetDragParam(WPARAM& wParam, LPARAM& lParam)
{
}

// 用户自定义强动画开始
void ICtrlInterface::OnUserDefHighAnimationBegin()
{
}

// 用户自定义强动画绘制接口，hWndDc：控件所在的窗口DC
void ICtrlInterface::OnUserDefHighAnimationDraw(CDrawingImage &CtrlAnimationMemDc, HDC &hWndDc)
{
}

// 用户自定义强动画结束
void ICtrlInterface::OnUserDefHighAnimationEnd()
{
}

// 收到需要清空绘制缓存的消息
void ICtrlInterface::OnClearDrawMemoryDc()
{
}

// 开始克隆，参数由Clone函数传入，如果函数返回 CAN_NOT_CLONE ，则不进行克隆操作
int ICtrlInterface::OnBeforeClone(WPARAM wParam)
{
	return S_OK;
}

void ICtrlInterface::OnAfterClone(IControlBase* pCloneCtrl, WPARAM lParam)
{

}

// 当一个控件的TabOrder属性被设置了之后，同时此控件得到焦点，窗口会将此消息
// 通过这个函数发送给注册了需要取得焦点消息的控件
void ICtrlInterface::OnContrlFocusChange(IControlBase* pFocusCtrl)
{

}

// 一个控件如果是另一个控件的子控件，并且此控件是通过CreateSubContrl函数创建的，那么这个控件的父控件被Clone的时候，当这个子控件也被Clone完成后，会受到此消息函数
void ICtrlInterface::OnCreateSubControlCloneEnd()
{

}
