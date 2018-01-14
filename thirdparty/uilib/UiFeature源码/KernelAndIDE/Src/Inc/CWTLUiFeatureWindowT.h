////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 重要说明
// 此头文件演示了如何使用WTL调用UiFeature创建对话框，理解这个示例后，也就自然能否使用MFC等其他的方式直接调用UiFeature内核了。
// WTL窗口的使用例子可以参考Demo：005_WTLWindowAnimation

#pragma once
#include "UiFeatureDefs.h"
#include "UiFeatureWindowDefs.h"
#include "IWindowBase.h"
#include "IControlBase.h"
#include "IUiFeatureKernel.h"
#include "ICommonFun.h"
#include <assert.h>

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlcrack.h>

#include <string>
#include <map>
#include <vector>
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class CWTLUiFeatureWindowT : public CWindowImpl<T>,
							 public CUpdateUI<T>,
							 public CMessageFilter,
							 public CIdleHandler,
							 public IControlMessage
{
public:
	// 调用 CreateWTLFeatureWindow 或者 CreateWTLFeatureWindowByKernel 函数创建窗口完毕的回调函数
	// 可以在此函数中取得控件指针
	virtual VOID OnWTLWindowCreate() = 0;
	// 控件发送的消息都发送到此函数
	virtual LRESULT OnCtrlMessage(IControlBase* pCtrl, int nMsgId, WPARAM wParam, LPARAM lParam) = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CWTLUiFeatureWindowT()
	{
		m_hWnd = NULL;
		m_pModule = NULL;

		m_hKernelDll = NULL;
		m_pSkinManager = NULL;
		m_pWindowBase = NULL;

		m_strKernelDllPath = "";
		m_strSkinPath = "";
		m_strSkinWndName = "";

		INIT_RECT(m_rctCreate);
	}

	~CWTLUiFeatureWindowT()
	{
		SAFE_FREE_LIBRARY(m_hKernelDll);
	}

	virtual BOOL CreateWTLFeatureWindow(CAppModule *_pModule, LPCWSTR pszKernelDllPath, LPCWSTR pszSkinPath, LPCWSTR pszSkinWndName, char *pszPassword, 
		HWND hWndParent, RECT wndRct, int nShow, DWORD dwStyle = 0, DWORD dwExStyle = 0, LPVOID lpCreateParam = NULL)
	{
		if (_pModule == NULL || m_hWnd != NULL || pszKernelDllPath == NULL || pszSkinPath == NULL || pszSkinWndName == NULL ||
			wcslen(pszKernelDllPath) <= 0 || wcslen(pszSkinPath) <= 0 || wcslen(pszSkinWndName) <= 0)
			return FALSE;

		m_rctCreate = wndRct;
		m_pModule = _pModule;

		m_strKernelDllPath = pszKernelDllPath;
		m_strSkinPath = pszSkinPath;
		m_strSkinWndName = pszSkinWndName;

		// 加载UiFeatureKernel.dll
		m_hKernelDll = ::LoadLibraryW(pszKernelDllPath);
		if (m_hKernelDll == NULL)
		{
			DWORD dwErr = ::GetLastError();
			return FALSE;
		}

		// 取得UiFeatureKernel的内核接口
		GETUIKERNELINTERFACE GetKernel = (GETUIKERNELINTERFACE)::GetProcAddress(m_hKernelDll, "GetUiKernelInterface");
		if (GetKernel == NULL)
		{
			DWORD dwErr = ::GetLastError();
			SAFE_FREE_LIBRARY(m_hKernelDll);
			return FALSE;
		}

		m_pUiKernel = GetKernel();
		if (m_pUiKernel == NULL)
		{
			DWORD dwErr = ::GetLastError();
			SAFE_FREE_LIBRARY(m_hKernelDll);
			return false;
		}

		// 取得皮肤包管理器
		m_pSkinManager = m_pUiKernel->GetSkinManager();
		if (m_pSkinManager == NULL)
		{
			m_pUiKernel = NULL;
			SAFE_FREE_LIBRARY(m_hKernelDll);
			return false;
		}

		// 创建并初始化窗口
		return InitUiFeatureInstance(pszPassword, hWndParent, wndRct, nShow, dwStyle, dwExStyle, lpCreateParam);
	}

	virtual BOOL CreateWTLFeatureWindowByKernel(CAppModule *_pModule, IUiFeatureKernel* pUiKernel, char* pszSkinPath, char *pszSkinWndName, char *pszPassword, 
		HWND hWndParent, RECT wndRct, int nShow, DWORD dwStyle = 0, DWORD dwExStyle = 0, LPVOID lpCreateParam = NULL)
	{
		if (_pModule == NULL || pUiKernel == NULL || pszSkinPath == NULL || strlen(pszSkinPath) <= 0 ||
			pszSkinWndName == NULL || strlen(pszSkinWndName) <= 0)
			return FALSE;

		m_rctCreate = wndRct;
		m_pModule = _pModule;
		m_pUiKernel = pUiKernel;
		m_strSkinPath = pszSkinPath;
		m_strSkinWndName = pszSkinWndName;

		m_pSkinManager = m_pUiKernel->GetSkinManager();
		if (m_pSkinManager == NULL)
		{
			m_pUiKernel = NULL;
			return FALSE;
		}

		return InitUiFeatureInstance(pszPassword, hWndParent, wndRct, nShow, dwStyle, dwExStyle, lpCreateParam);
	}

	BEGIN_UPDATE_UI_MAP(CWTLUiFeatureWindowT)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CWTLUiFeatureWindowT)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

//	Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//	LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
//	LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_pModule == NULL)
			return -1;

		CMessageLoop* pLoop = m_pModule->GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_pModule == NULL)
			return -1;

		CMessageLoop* pLoop = m_pModule->GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
		bHandled = FALSE;
		return 0;
	}

private:
	BOOL InitUiFeatureInstance(char *pszPassword, HWND hWndParent, RECT wndRct, int nShow, DWORD dwStyle = 0, DWORD dwExStyle = 0, LPVOID lpCreateParam = NULL)
	{
		if (m_pModule == NULL)
			return FALSE;

		// WTL 的方式创建窗口
		this->Create(hWndParent, &wndRct, _T("WtlUiFeatureWindow"), dwStyle, dwExStyle, 0U, lpCreateParam);
		if (IS_INVALID_HANDLE(m_hWnd))
			return FALSE;

		IControlMessage *pCtrlMsg = dynamic_cast<IControlMessage*>(this);
		if (pCtrlMsg == NULL)
		{
			assert(false);
			return FALSE;
		}

		// 使用UiFeature内核创建对话框
		m_pWindowBase = m_pUiKernel->PG_InitFeatureSkin(m_hWnd, m_strSkinPath, m_strSkinWndName, pszPassword);
		if (m_pWindowBase == NULL)
		{
			assert(false);
			return FALSE;
		}

		// 设置控件回调
		m_pWindowBase->SetCtrlMsgCallBack(pCtrlMsg);
		if (IS_RECT_EMPTY(m_rctCreate))
		{
			SIZE WndSize = m_pWindowBase->PP_GetWindowPropSize();
			INIT_RECT(m_rctCreate);
			m_rctCreate.right = WndSize.cx;
			m_rctCreate.bottom = WndSize.cy;
			this->MoveWindow(&m_rctCreate);
			this->CenterWindow();
		}
		else
		{
			this->MoveWindow(&m_rctCreate);
		}

		// 窗口创建完毕
		OnWTLWindowCreate();
		::ShowWindow(m_hWnd, nShow);
		return IS_SAFE_HANDLE(m_hWnd);
	}

protected:
	CAppModule*				m_pModule;
	// UiFeature DLL 句柄
	HMODULE					m_hKernelDll;
	// UiFeature 内核指针
	IUiFeatureKernel*		m_pUiKernel;
	// 皮肤包管理器
	IPropertySkinManager*	m_pSkinManager;
	// UiFeature 的窗口指针
	IWindowBase*			m_pWindowBase;
	// UiFeatureKernel.dll 的路径
	CStringW				m_strKernelDllPath;
	// 当前对话框对应的皮肤包的路径
	CStringW				m_strSkinPath;
	// 当前对话框对于皮肤包中的窗口名称
	CStringW				m_strSkinWndName;
	// 当前对话框创建时的窗口大小
	RECT					m_rctCreate;
};
