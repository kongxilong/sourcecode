
#pragma once
#include "..\..\Inc\Control\ICtrlIEWebBrowser.h"
#include "WindowsIe\UiFeatureBrowserWindow.h"


class ICtrlIEWebBrowserImpl : public ICtrlIEWebBrowser
{
public:
	ICtrlIEWebBrowserImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlIEWebBrowserImpl();

	virtual VOID OpenUrl(LPCWSTR pszUrl);
	virtual LPCWSTR GetUrl();
	// 可见属性
	virtual void SetVisible(bool bVisible, bool bSetChild = false);

	// 控件需要处理的一些自定义的消息，消息基本上会由对话框或者其他控件传入
	BEGIN_CTRL_NOTIFY
		CTRL_NOTIFY_HANDLER(WM_IE_WEB_CREATE_UIF, OnCreateUiFeatureIeWindow)
		CTRL_NOTIFY_HANDLER(WM_IE_WEB_OPEN_URL, OnOpenUrl)
		CTRL_NOTIFY_HANDLER(WM_MOVE, OnWindowMove)
		CTRL_NOTIFY_HANDLER(WM_SYSCOMMAND, OnSysCommand)
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

	// 移动、设置控件位置
	virtual void OnSize();

	// 这个接口会在整个对话框初始化完成的时候调用
	// 控件通过派生这个接口，可以在这个接口函数里通过其他控件的名称取得当前对话框里的其他控件。
	virtual void OnWindowFinalCreate();
	// 附属对话框即将关闭
	virtual void OnWindowClose();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

	int OnCreateUiFeatureIeWindow(WPARAM wParam, LPARAM lParam);
	int OnOpenUrl(WPARAM wParam, LPARAM lParam);
	int OnWindowMove(WPARAM wParam, LPARAM lParam);
	int OnSysCommand(WPARAM wParam, LPARAM lParam);

private:
	void SetIeWindowPostion();
	BOOL SetIeWindowShow(BOOL* pIsWndVisible);

private:
	IPropertyString* m_pPropUrl;
	CUiFeatureBrowserWindow m_WebWindow;
};
