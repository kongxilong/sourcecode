
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_FOCUS_CTRL						(L"FocusControl")

class _declspec(novtable) ICtrlFocus : public ICtrlInterface
{
public:
	ICtrlFocus(IUiFeatureKernel *pUiKernel);
};
