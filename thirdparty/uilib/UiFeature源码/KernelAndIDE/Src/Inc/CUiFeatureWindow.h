// 说明：UiFeature的高性能基本对话框

#pragma once
#include <Windows.h>
#include "IWindowBase.h"
#include "IControlBase.h"
#include "IUiFeatureKernel.h"
#include "CWin32Window.h"
#include "UiFeatureWindowDefs.h"
#include <atlstr.h>
using namespace ATL;


class CUiFeatureWindow : public CWin32Window, public IControlMessage
{
public:
	CUiFeatureWindow();
	virtual ~CUiFeatureWindow();

	// 创建一个窗口，参数：
	// char* pszPassword：皮肤包密码
	// LPCWSTR pszDllDir：UiFeatureKernel.dll的全路径
	// LPCWSTR pszSkinPath：*.ufd皮肤文件的全路径
	// LPCWSTR pszWndName：*.ufd皮肤文件中对话框的名字
	// HWND hParent：父窗口
	// RECT WndRect：窗口显示位置，坐标为屏幕位置
	// int nShow：显示模式，如：SW_SHOW、SW_HIDE
	// LPARAM lParam：创建窗口时传入的参数，可以通过 GetCreateWindowParam 函数取得
	virtual bool CreateFeatureWindow(char* pszPassword, LPCWSTR pszKernelDllPath, LPCWSTR pszSkinPath, LPCWSTR pszSkinWndName, HWND hParent, RECT WndRect, int nShow, int nStyle = WS_OVERLAPPED | WS_THICKFRAME, LPARAM lParam = NULL);
	virtual bool CreateFeatureWindowByKernel(char* pszPassword, IUiFeatureKernel* pUiKernel, LPCWSTR pszSkinPath, LPCWSTR pszSkinWndName, HWND hParent, RECT WndRect, int nShow, int nStyle = WS_OVERLAPPED | WS_THICKFRAME, LPARAM lParam = NULL);

	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// 取得
	IWindowBase* GetUiFeatureWindow();
	// 设置窗口标题文字
	void SetWindowText(char* pstrWndText);
	// 初始化对话框结束
	virtual void OnInitWindowEnd(int nErrorCode);

	virtual bool CreateWin32Window(HWND hParent, RECT WndRect, LPCWSTR pszWndText, int nShow, int nStyle = WS_OVERLAPPED | WS_THICKFRAME, LPARAM lParam = NULL);

	// 设置对话框启动的时候的动画控件
	void ShowWindowWithAnimation(IControlBase* pAnimationCtrl);
	// 播放关闭对话框动画，并关闭窗口
	bool CloseWindowWithAnimation(IControlBase* pAnimationCtrl);
	LPCWSTR GetSkinPath();

	// 控件发送给窗口的消息接口
	BEGIN_CTRL_COMMAND
		CTRL_COMMAND_HANDLER(m_pEndAnimationCtrl, CMB_ANIMATION_END, OnCloseWindowWithAnimation);
	END_CTRL_COMMAND

protected:
	virtual bool InitInstance();
	// 显示窗口
	virtual void OnShowWindow(bool bIsShow, int nShowStatus);
	virtual void OnCreateEnd();
	
	LRESULT OnCloseWindowWithAnimation(WPARAM wParam, LPARAM lParam);

protected:
	HMODULE m_hKernelDll;
	IUiFeatureKernel* m_pUiKernel;
	IPropertySkinManager* m_pSkinManager;
	// UiFeature界面的对话框资源指针
	IWindowBase* m_pWindowBase;
	CStringW m_strSkinPath;
	CStringW m_strSkinWndName;
	string m_strUserPassword;

	// 是否是对话框刚启动的时候显示的。
	bool m_bIsStartShow;
	IControlBase* m_pStartAnimationCtrl;
	IControlBase* m_pEndAnimationCtrl;
};
