
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_STATIC						(L"Static")

class _declspec(novtable) ICtrlStatic : public ICtrlInterface
{
public:
	ICtrlStatic(IUiFeatureKernel *pUiKernel);
	// 设置控件的文本
	virtual bool SetText(LPCWSTR pszText) = 0;
	virtual LPCWSTR GetText() = 0;
};
