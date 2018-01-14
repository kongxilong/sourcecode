
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_LINK							(L"Link")

// 动画控件向窗口发送的消息
enum CTRL_MSG_LINK
{
	// LINK_LABLE消息Base，此消息值不允许被发送，只是作为动画消息值的Base
	CM_LINK_BASE	= 0x10004000,
	// LINK_LABLE Down消息
	CM_LINK_DOWN,
	// LINK_LABLE Up消息
	CM_LINK_UP
};

class _declspec(novtable) ICtrlLink : public ICtrlInterface
{
public:
	ICtrlLink(IUiFeatureKernel *pUiKernel);
	// 设置Link文字
	virtual void SetLinkText(LPCWSTR pText, bool bRedraw) = 0;
	// 设置Link链接地址URL
	virtual void SetLinkUrl(LPCWSTR pUrl) = 0;
	// 取得Link链接地址URL
	virtual LPCWSTR GetLinkUrl() = 0;
};
