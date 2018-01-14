
#pragma once
#include "..\ICtrlInterface.h"

// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_IE_WEB_BROWSER						(L"IEWebBrowser")

// 创建IE窗口的窗口消息
#define WM_IE_WEB_CREATE_UIF							(WM_APP + 11)
#define WM_IE_WEB_OPEN_URL								(WM_APP + 12)

class _declspec(novtable) ICtrlIEWebBrowser : public ICtrlInterface
{
public:
	ICtrlIEWebBrowser(IUiFeatureKernel *pUiKernel);
	virtual VOID OpenUrl(LPCWSTR pszUrl) = 0;
	virtual LPCWSTR GetUrl() = 0;
};
