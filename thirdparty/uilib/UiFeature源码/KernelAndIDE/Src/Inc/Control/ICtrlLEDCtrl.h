#pragma once
#include "..\ICtrlInterface.h"	//包含控件接口文件

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_LEDCTRL							(L"LEDCtrl")

class _declspec(novtable) ICtrlLEDCtrl : public ICtrlInterface
{
public:
	ICtrlLEDCtrl(IUiFeatureKernel *pUiKernel);

	virtual void SetText(LPCWSTR pText, bool bRedraw) = 0;
	virtual void SetIndexMap(LPCWSTR pText) = 0;
	virtual SIZE GetAutoSize() = 0;

};
