#pragma once
#include "..\..\Inc\Control\ICtrlHwndObj.h"

class ICtrlHwndObjImpl : public ICtrlHwndObj
{
public:
	ICtrlHwndObjImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlHwndObjImpl();

	// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
	BEGIN_CTRL_NOTIFY
		CTRL_NOTIFY_HANDLER(WM_MOVE, OnWindowMove)
		CTRL_NOTIFY_HANDLER(WM_SYSCOMMAND, OnSysCommand)
	END_CTRL_NOTIFY

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

	virtual BOOL IsHwndExist(HWND hwnd);

protected:
	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);

	// 移动、设置控件位置
	virtual void OnSize();

	// 这个接口会在整个对话框初始化完成的时候调用
	// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
	virtual void OnWindowFinalCreate();
	// 附属对话框即将关闭
	virtual void OnWindowClose();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

	int OnWindowMove(WPARAM wParam, LPARAM lParam);
	int OnSysCommand(WPARAM wParam, LPARAM lParam);


private:
	void SetHwndWindowPostion();

private:
	HWND m_theHwnd;			//当前显示的窗口句柄
	vector<HWND> m_vectorHwnd;	//窗口列表

};