#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_CHECK_BOX							(L"CheckBox")

// 动画控件向窗口发送的消息
enum CTRL_MSG_CHECK_BOX
{
	// CHECK_BOX消息Base，此消息值不允许被发送，只是作为消息值的Base
	CM_CHECK_BOX_BASE	= 0x10009000,
	// CHECK_BOX Down消息
	CM_CHECK_BOX_DOWN,
	// CHECK_BOX Up消息
	CM_CHECK_BOX_UP
};

class _declspec(novtable) ICtrlCheckBox : public ICtrlInterface
{
public:
	ICtrlCheckBox(IUiFeatureKernel *pUiKernel);
	// 设置CheckBox文字
	virtual void SetText(LPCWSTR pText, bool bRedraw) = 0;
	// 设置选中属性
	virtual void SetCheck(bool bCheck) = 0;
	// 取得选中属性
	virtual bool GetCheck() = 0;
};
