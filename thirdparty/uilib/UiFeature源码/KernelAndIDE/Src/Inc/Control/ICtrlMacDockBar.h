
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_MAC_DOCK_BAR						(L"MacDockBar")

class _declspec(novtable) ICtrlMacDockBar : public ICtrlInterface
{
public:
	ICtrlMacDockBar(IUiFeatureKernel *pUiKernel);
	virtual bool AppendItem(LPCWSTR pszLocalImg, bool bRedraw) = 0;
};
