#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"

class CDockBarWindow : public CUiFeatureWindow
{
public:
	CDockBarWindow();
	virtual ~CDockBarWindow();

	bool ShowDockBarWindow();

//////////////////////////////////////////////////////////////////////////
	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
	WIN32_WND_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pShowDirBtn, CM_BUTTON_UP, OnDirBtnClick)
	END_CTRL_COMMAND
//////////////////////////////////////////////////////////////////////////
	// 本窗口的消息处理函数
	BEGIN_WND_PROC
//	WND_MSG_HANDLER(WM_UIF_CRATE_SUB_WINDOW, OnCreateSubWnd)
	END_WND_PROC(CUiFeatureWindow)
//////////////////////////////////////////////////////////////////////////

protected:
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	LRESULT OnDirBtnClick(WPARAM wParam, LPARAM lParam);

private:
	ICtrlImageBox* m_pDialogBkImgBox;
	ICtrlShadowButton* m_pShowDirBtn;
};
