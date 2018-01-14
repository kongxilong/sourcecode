
#pragma once
#include "..\ICtrlInterface.h"
#include "ICtrlShadowButton.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_WINDOW_TITLE_BAR						(L"WindowTitleBar")

// 控件向窗口发送的消息
enum CTRL_MSG_WND_TITLEBAR
{
	CM_TITLEBAR_BASE	= 0x10008000,
	// 设置窗口的RGN，必须要处理窗口的rgn，否则会导致窗口标题栏异常
	// 由于设置了 WS_CAPTION 属性，必须要通过设置RGN来消除系统自带的RGN，所以对话框必须要处理这个消息
	// 也可以直接设置控件的 UseDefaultRgn 属性为true，使用控件内部提供的默认矩形rgn，这样就不需要处理这个消息了
	CM_TITLEBAR_SET_WND_RGN,
	// Up消息双击了标题栏
	CM_TITLEBAR_DBCLICK,
	// 按下了关闭对话框按钮
	CM_TITLEBAR_CLOSEBTN_CLICK,
	// 按下了最小化按钮
	CM_TITLEBAR_MINIMIZEBTN_CLICK,
	// 按下了最大化按钮
	CM_TITLEBAR_MAXIMIZEBTN_CLICK,
	// 按下了还原按钮
	CM_TITLEBAR_RESTOREBTN_CLICK,
	// 窗口将会最大化
	CM_TITLEBAR_WILL_MAXIMIZE,
	// 窗口将会还原
	CM_TITLEBAR_WILL_RESTOREBTN,
};

class _declspec(novtable) ICtrlWindowTitleBar : public ICtrlInterface
{
public:
	ICtrlWindowTitleBar(IUiFeatureKernel *pUiKernel);
	virtual ICtrlShadowButton* GetMinimizeButton() = 0;
	virtual ICtrlShadowButton* GetMaximizeButton() = 0;
	virtual ICtrlShadowButton* GetRestoreButton() = 0;
	virtual ICtrlShadowButton* GetClostButton() = 0;
	virtual bool UseDefaultRgn() = 0;
};
