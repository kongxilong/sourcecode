#pragma once
#include "..\ICtrlInterface.h"	//包含控件接口文件

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_SPLITBAR							(L"SplitBar")

class _declspec(novtable) ICtrlSplitBar : public ICtrlInterface
{
public:
	ICtrlSplitBar(IUiFeatureKernel *pUiKernel);

	//设置分割条控件的类型，bHorz为TRUE时表示水平分割条，否则为垂直分割条
	virtual void SetSplitterType(bool bHorz) = 0;

	//设置水平分割条区域的最小值，nTopMin为顶部区域最小值,nBottomMin为底部区域的最小值
	virtual void SetSplitterHorzMin(int nTopMin, int nBottomMin) = 0;

	//设置垂直分割条区域的最小值,nLeftMin为左侧区域最小值,nRightMin为右侧区域的最小值
	virtual void SetSplitterVertMin(int nLeftMin, int nRightMin) = 0;

	//获取左或者上最小值
	virtual int GetTopLeftMin() = 0;

	//获取底或者右最小值
	virtual int GetBottomRightMin() = 0;

	//获取图标的长度
	virtual int GetGraphLength() = 0;

	//设置图标的长度
	virtual void SetGraphLength(int nGraphLength, bool bRedraw) = 0;

};