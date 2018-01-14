
#pragma once
#include "..\..\Inc\Control\ICtrlWindowTitleBar.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\IPropertyBool.h"

class ICtrlWindowTitleBarImpl : public ICtrlWindowTitleBar, public IControlMessage
{
public:
	ICtrlWindowTitleBarImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlWindowTitleBarImpl();

	virtual ICtrlShadowButton* GetMinimizeButton();
	virtual ICtrlShadowButton* GetMaximizeButton();
	virtual ICtrlShadowButton* GetRestoreButton();
	virtual ICtrlShadowButton* GetClostButton();
	virtual bool UseDefaultRgn();

	BEGIN_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pMinimizeBtn, CM_BUTTON_UP, OnMinimizeBtnClick)
		CTRL_COMMAND_HANDLER(m_pMaximizeBtn, CM_BUTTON_UP, OnMaximizeBtnClick)
		CTRL_COMMAND_HANDLER(m_pRestoreBtn, CM_BUTTON_UP, OnRestoreBtnClick)
		CTRL_COMMAND_HANDLER(m_pClostBtn, CM_BUTTON_UP, OnClostBtnClick)
	END_CTRL_COMMAND

	BEGIN_CTRL_NOTIFY
		// 拦截这个自定义消息目的是为了通知窗口可以设置rgn区域了
		CTRL_NOTIFY_HANDLER(UI_FEATURE_KERNEL_MSG, OnSetWindowRgn)
		// 拦截这个消息目的是为了设置最大化、还原按钮的状态
		CTRL_NOTIFY_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		CTRL_NOTIFY_HANDLER(WM_SIZE, OnWindowSize)
	END_CTRL_NOTIFY

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
	// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
	virtual bool OnCheckMouseInRgn(POINT pt, INT nMouseMsgId);
	// 鼠标左键双击
	virtual void OnLButtonDbClick(POINT pt);
	// 当一个控件包含了其他子控件的时候，系统会发送创建或加载子控件的消息
	virtual void OnCreateSubControl(bool bIsCreate);
	// 这个接口会在整个对话框初始化完成的时候调用
	// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
	virtual void OnWindowFinalCreate();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);
	// pCloneCtrl 是最终克隆出来的控件，可以在这个消息响应函数中再克隆一些控件私有的属性
	virtual void OnAfterClone(IControlBase* pCloneCtrl, WPARAM lParam);

	LRESULT OnMinimizeBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnMaximizeBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnRestoreBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnClostBtnClick(WPARAM wParam, LPARAM lParam);

	int OnSetWindowRgn(WPARAM wParam, LPARAM lParam);
	int OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
	int OnWindowSize(WPARAM wParam, LPARAM lParam);

private:
	// 鼠标是否落在了子控件范围内
	BOOL PointInSubCtrl(POINT pt, IControlBase* pCtrlBase);
	bool HaveMaximizeButton();
	bool HaveMinimizeButton();
	// 是否为Win7以后的操作系统
	bool IsWin7Later();
	void SetDefaultWindowRgn();
	void GetMaxWndRgnOffset(int &nCxOffset, int &nCyOffset);

private:
	ICtrlShadowButton* m_pMinimizeBtn;
	ICtrlShadowButton* m_pMaximizeBtn;
	ICtrlShadowButton* m_pRestoreBtn;
	ICtrlShadowButton* m_pClostBtn;
	IControlBase* m_pWndBasePanelCtrl;

	IPropertyBool* m_pPropNoMinimizeBtn;
	IPropertyBool* m_pPropNoMaximizeBtn;
	IPropertyString* m_pPropWndBasePanelName;
	IPropertyBool* m_pPropSetDftRgn;

	bool m_bOldFullScreenStyle;
};
