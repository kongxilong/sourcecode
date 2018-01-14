
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_GRADUAL_LIGHT						(L"GradualLight")

class _declspec(novtable) ICtrlGradualLight : public ICtrlInterface
{
public:
	ICtrlGradualLight(IUiFeatureKernel *pUiKernel);
	virtual void Play() = 0;
	virtual void Stop(bool bIsKillTimer) = 0;
};
