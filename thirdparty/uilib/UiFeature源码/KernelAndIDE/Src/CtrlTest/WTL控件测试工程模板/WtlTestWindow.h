
#pragma once

#include "..\..\Inc\CWTLUiFeatureWindowT.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\Control\ICtrlLink.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"

class CWtlTestWindow : public CWTLUiFeatureWindowT<CWtlTestWindow>
{
public:
	CWtlTestWindow();
	virtual ~CWtlTestWindow();

	bool ShowWtlTestWindow(CAppModule *_pModule, DWORD dwMainThreadId);
	void UnInitialized();

	DECLARE_WND_CLASS(_T("UiFeatureWtlWindow"))

	BEGIN_MSG_MAP(CWtlTestWindow)
		CHAIN_MSG_MAP(CWTLUiFeatureWindowT)
		MESSAGE_HANDLER(UI_FEATURE_KERNEL_MSG, OnUiFeatureMessage)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
		KERNEL_COMMAND_HANDLER(KCM_SYS_COMMAND_CLOSE, OnSysCommandClose)
	END_CTRL_COMMAND

protected:
	// WM_CREATE：对话框初始化	
	virtual VOID OnWTLWindowCreate();

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUiFeatureMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommandClose(WPARAM wParam, LPARAM lParam);

private:
	void CloseTestWindow();

private:
	DWORD m_dwMainThreadId;
	CAppModule *m_pModule;
	HICON m_DlgIcon;

	ICtrlImageBox* m_pWindowbkPanel;
};
