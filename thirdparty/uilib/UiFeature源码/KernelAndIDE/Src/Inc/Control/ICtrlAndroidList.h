
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_ANDROID_LIST						(L"AndroidList")


// 动画控件向窗口发送的消息
enum CTRL_MSG_ANDROID_LIST
{
	CM_ANDROID_LIST_BASE				= 0x10027000,
	// 列表数据发生变化
	CM_ANDROID_LIST_DATA_SET_CHANGED,
	// ListItem 中有子控件被按下
	CM_ANDROID_LIST_L_BUTTON_DOWN,
	// ListItem 中有子控件被按下后抬起
	CM_ANDROID_LIST_L_BUTTON_UP,
	// ListItem 中有子控件item被鼠标选中
	CM_ANDROID_LIST_MOUSE_MOVE,
};


class _declspec(novtable) ICtrlAndroidList : public ICtrlInterface
{
public:
	ICtrlAndroidList(IUiFeatureKernel *pUiKernel);

	// 隐藏 ListItem 中的一个子控件，并腾出这个控件的地方
	virtual void Gone(IControlBase* pGoneCtrl) = 0;
	// 隐藏/显示 ListItem 中的一个子控件，被 gone 的控件也可以显示，并会重新占用原来的腾出来的空间
	virtual void Visible(IControlBase* pCtrl, bool bVisible) = 0;
	// 在 UiFeatureBuilder 中设计出来的 ListItem 的模板，在运行时态一行要显示多少个
	virtual int GetTempletColumnCount() = 0;
	// 整个列表是否为空
	virtual bool IsEmpty() = 0;
};
