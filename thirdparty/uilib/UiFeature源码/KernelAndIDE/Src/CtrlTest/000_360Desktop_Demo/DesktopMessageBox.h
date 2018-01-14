
#pragma once
#include "..\..\inc\CUiFeatureWindow.h"
#include "..\..\inc\Control\ICtrlShadowButton.h"
#include "..\..\inc\Control\ICtrlStatic.h"
#include "..\..\inc\Control\ICtrlImageBox.h"
#include <winuser.h>
#include <atlstr.h>
using namespace ATL;

// 当不是采用模态的方式弹出 CDesktopMessageBox 的时候，如果需要 CDesktopMessageBox 的返回值，那需要继承这个接口进行回调
class CNotDoModalMessageBoxCallBack
{
public:
	virtual void NDMMsgBoxCallBack(int nMsgId, int nMsgReturn) = 0;
};

class CDesktopMessageBox : public CUiFeatureWindow
{
public:
	CDesktopMessageBox();
	virtual ~CDesktopMessageBox();

	// dwMsgBoxType 可以为下列宏的组合
	// MB_OKCANCEL、MB_OK 二选一
	// MB_ICONINFORMATION、MB_ICONWARNING、MB_ICONERROR 三选一
	// 以上两组可以组合
	// 返回值为：IDOK、IDCANCEL 的其中一个
	DWORD MessageBox(HWND hParent, IUiFeatureKernel* pUiKernel, LPCWSTR pszSkinPath, LPCWSTR pszMsgTitle, LPCWSTR pszMsgInfo,
		DWORD dwMsgBoxType = MB_OK | MB_ICONINFORMATION, INT nDlgIconId = 0,
		BOOL IsDoModal = TRUE, int nMsgId = 0, CNotDoModalMessageBoxCallBack* pNDMCallBack = NULL);

protected:
	// 控件发送给窗口的消息接口
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam);
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	// WM_CREATE：对话框初始化	
	virtual void OnCreate();

	virtual void OnKeyDown(int nVirtKey, int nFlag);
	void EndMessageBox(DWORD nReturn);

private:
	HWND m_hParent;
	HICON m_DlgIcon;
	INT m_nDlgIconId;

	DWORD m_dwThreadId;
	DWORD m_dwMsgBoxType;
	DWORD m_dwMsgReturn;
	DWORD m_dwMsgIconType;
	CStringW m_strMsgTitle;
	CStringW m_strMsgInfo;

	BOOL m_bIsDoModal;
	int m_nMsgId;
	CNotDoModalMessageBoxCallBack* m_pNDMCallBack;

	ICtrlShadowButton* m_pOk2Btn;

	ICtrlShadowButton* m_pCloseBtn;
	ICtrlShadowButton* m_pCancelBtn;
	ICtrlShadowButton* m_pOkBtn;
	ICtrlStatic* m_pTitle;
	ICtrlStatic* m_pInfomation;

	ICtrlImageBox* m_IconI;
	ICtrlImageBox* m_IconW;
	ICtrlImageBox* m_IconE;
};
