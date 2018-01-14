#pragma once
#include "..\ICtrlInterface.h"	//包含控件接口文件

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_HWNDOBJ							(L"HwndObj")

class _declspec(novtable) ICtrlHwndObj : public ICtrlInterface
{
public:
	ICtrlHwndObj(IUiFeatureKernel *pUiKernel);

	//设置窗口与HwndObj进行相互绑定
	virtual BOOL Attach(HWND hwnd);

	//解除窗口与HwndObj的绑定
	virtual BOOL Detach(HWND hwnd);

	//根据序号获取绑定的窗口句柄
	virtual HWND GetSafeHwnd(int nIndex);

	//获取HwndObj当前绑定的窗口句柄
	virtual HWND GetBindHwnd();

	//显示HideBindWindow隐藏起来的窗口
	virtual BOOL ShowBindWindow();

	//隐藏当前绑定的窗口
	virtual BOOL HideBindWindow();

	//移动与HwndObj进行绑定的窗口到指定区域
	virtual void MoveHwndToRect(RECT rect); 

	//根据窗口句柄来显示与HwndObj绑定的窗口
	virtual BOOL ShowWindow(HWND hwnd);

	//根据索引来显示与HwndObj绑定的窗口
	virtual BOOL ShowWindowByIndex(int nIndex);

	//判断被绑定的窗口序列当中是否存在该窗口句柄
	virtual BOOL IsHwndExist(HWND hwnd);

};