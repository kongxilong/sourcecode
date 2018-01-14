#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\Control\ICtrlContextMenu.h"
#include "..\..\Inc\Control\ICtrlContextMenuItem.h"

class CUiFeatureTestWindow : public CUiFeatureWindow
{
public:
	CUiFeatureTestWindow();
	virtual ~CUiFeatureTestWindow();

	bool ShowUiFeatureTestWindow(DWORD dwMainThreadId);
	void UnInitialized();

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
	WIN32_WND_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pShowMenuBtn, CM_BUTTON_UP, OnBtnClick)
		CTRL_COMMAND_HANDLER(m_pContextMenu, CM_MENU_ITEM_UP, OnMenuItem)
		KERNEL_COMMAND_HANDLER(KCM_SYS_COMMAND_CLOSE, OnSysCommandClose)
	END_CTRL_COMMAND

	BEGIN_WNDPROC_MESSAGE
	END_WNDPROC_MESSAGE

protected:
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);

	LRESULT OnBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnMenuItem(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommandClose(WPARAM wParam, LPARAM lParam);

private:
	DWORD m_dwMainThreadId;
	ICtrlImageBox* m_pDialogBkImgBox;
	ICtrlShadowButton* m_pShowMenuBtn;
	ICtrlContextMenu*	m_pContextMenu;
};
