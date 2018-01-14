#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\Control\ICtrlImageBox.h"
#include "..\..\Inc\Control\ICtrlShadowButton.h"
#include "..\..\Inc\Control\ICtrlStraightLineFlyImage.h"
#include "..\..\Inc\Control\ICtrlGradualLight.h"
#include "..\..\Inc\Control\ICtrlRollAdvertisement.h"

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
		CTRL_COMMAND_HANDLER(m_pBeginFlyBtn, CM_BUTTON_UP, OnBeginFly)
		KERNEL_COMMAND_HANDLER(KCM_SYS_COMMAND_CLOSE, OnSysCommandClose)
		CTRL_COMMAND_HANDLER(m_pRoolAdver, CM_ROLL_ADVERTISEMENT_UP, OnClickAdrertisement)
	END_CTRL_COMMAND

	BEGIN_WNDPROC_MESSAGE
	END_WNDPROC_MESSAGE

protected:
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);

	LRESULT OnBeginFly(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommandClose(WPARAM wParam, LPARAM lParam);
	LRESULT OnClickAdrertisement(WPARAM wParam, LPARAM lParam);

private:
	DWORD m_dwMainThreadId;
	ICtrlShadowButton* m_pBeginFlyBtn;
	ICtrlStraightLineFlyImage* m_pFlyImage;
	ICtrlGradualLight* m_pGradualLight;
	ICtrlRollAdvertisement* m_pRoolAdver;
};
