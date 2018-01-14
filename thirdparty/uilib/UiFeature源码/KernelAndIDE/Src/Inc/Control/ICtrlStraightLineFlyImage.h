
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_STRAIGHTLINE_FLY_IMAGE					(L"StraightLineFlyImage")

class _declspec(novtable) ICtrlStraightLineFlyImage : public ICtrlInterface
{
public:
	ICtrlStraightLineFlyImage(IUiFeatureKernel *pUiKernel);
	// 开始一个动画，设置动画开始和结束的位置，返回值是动画的一个KEY
	virtual VOID* StartFly(RECT rctBegin, RECT rctEnd) = 0;
	// 得到皮肤包中设置的飞行位置
	virtual void GetFlyPostion(RECT &rctBegin, RECT &rctEnd) = 0;
};
