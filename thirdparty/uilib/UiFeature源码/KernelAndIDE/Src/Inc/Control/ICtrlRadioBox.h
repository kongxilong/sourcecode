#pragma once
#include "..\ICtrlInterface.h"	//包含控件接口文件

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_RADIO_BOX							(L"RadioBox")

enum CTRL_MSG_RADIO_BOX
{
	// RADIO_BOX消息Base，此消息值不允许被发送，只是作为消息值的Base
	CM_RADIO_BOX_BASE	= 0x20009000,
	// RADIO_BOX Down消息
	CM_RADIO_BOX_DOWN,
	// RADIO_BOX Up消息
	CM_RADIO_BOX_UP
};


class _declspec(novtable) ICtrlRadioBox : public ICtrlInterface
{
public:
	ICtrlRadioBox(IUiFeatureKernel *pUiKernel);

	virtual void SetText(LPCWSTR pText, bool bRedraw) = 0;

	// 设置选中属性
	virtual void SetCheck(bool bCheck) = 0;

	// 取得选中属性
	virtual bool GetCheck() = 0;

	//设置分组ID
	virtual void SetGroupID(int nGroupId);

	//获取分组ID
	virtual int GetGroupID();

};