
#pragma once

#include "UiFeatureBrowserDefs.h"
#include <atlstr.h>
using namespace ATL;


#define STR_URL_ABOUT_BLANK                      L"about:blank"
#define CLASSNAME_IECTRL_WND                     L"UiFeatureBrowserCtrl"
#define WM_IE_DOCUMENT_COMPLETE                   (WM_USER + 102)


template <UINT nID, class T>
class CDTExternalDispatch : public IDispEventSimpleImpl<nID, T, &IID_IDispatch>
{
public:
	STDMETHOD(GetIDsOfNames)(REFIID /*riid*/, LPOLESTR* rgszNames, UINT cNames, LCID /*lcid*/, DISPID* rgdispid)
	{
		return S_OK;
	}
};


class CUiFeatureBrowserCtrl : public CUiFeatureHtmlView,
	               public IDispEventSimpleImpl<1, CUiFeatureBrowserCtrl, &DIID_DWebBrowserEvents2>,
                   public CDTExternalDispatch<0, CUiFeatureBrowserCtrl>

{
public:
	DECLARE_WND_SUPERCLASS(CLASSNAME_IECTRL_WND, CAxWindow::GetWndClassName())

    CUiFeatureBrowserCtrl();
    ~CUiFeatureBrowserCtrl();

    // External funs
    HWND Create(IDispatch* pExternalDispatch, LPCWSTR lpURL, HWND hWndParent, _U_RECT rect = NULL,
        DWORD dwStyle = 0, DWORD dwExStyle = 0,
        _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL, DWORD dwDocHostFlags = 
        DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_SCROLL_NO |
        DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER |
        DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_FLAT_SCROLLBAR);

    // Messages map
	BEGIN_MSG_MAP(CUiFeatureBrowserCtrl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(CUiFeatureHtmlView)
	END_MSG_MAP()

    // Message handlers
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // IDispatch events function info	
    static _ATL_FUNC_INFO DocComplete_Info;
    static _ATL_FUNC_INFO NavError_Info;
    static _ATL_FUNC_INFO BeforeNav_Info;
    static _ATL_FUNC_INFO ProgressChange_Info;
    static _ATL_FUNC_INFO DTCommunicate_Info;
    static _ATL_FUNC_INFO DTReadFile_Info;
    static _ATL_FUNC_INFO DTWriteFile_Info;

	// Events map
	BEGIN_SINK_MAP(CUiFeatureBrowserCtrl)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete, &DocComplete_Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR, OnNavigateError, &NavError_Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2, &BeforeNav_Info)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, DISPID_PROGRESSCHANGE, OnProgressChange, &ProgressChange_Info)
	END_SINK_MAP()    

    // IE events handlers
	void __stdcall OnDocumentComplete(LPDISPATCH pDisp, VARIANT FAR* URL);
	void __stdcall OnNavigateError(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* Error, VARIANT_BOOL* bCancel);
	void __stdcall OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* pvURL, VARIANT* pvFlags, VARIANT* pvTargetFrameName, VARIANT* pvPostData, VARIANT* pvHeaders, VARIANT_BOOL* pbCancel);
	void __stdcall OnProgressChange(long nProgress, long nProgressMax);	

    // Call script methods	
	HRESULT NotifyReadFile(const wchar_t *pcszKey, const wchar_t *pcszContent);
    HRESULT NotifyCommunicate(const wchar_t *pcszCommandID, const wchar_t *pcszCommandParams);   

protected:
    HRESULT GetIEScriptDispatch(IDispatch **ppScript, IDispatchEx **ppScriptObject);
    BOOL DispIEEventAdvise();
    void DispIEEventUnAdvise();
    BOOL EnableIEDrop(BOOL bEnable);
    BOOL IsBlankUrl(const CString &strURL);
};

//////////////////////////////////////////////////////////////////////////
//
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::DocComplete_Info        = {CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT }};
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::NavError_Info           = {CC_STDCALL, VT_EMPTY, 5, {VT_BYREF | VT_BOOL, VT_BYREF | VT_VARIANT, VT_BYREF | VT_VARIANT, VT_BYREF | VT_VARIANT, VT_BYREF | VT_DISPATCH}};
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::BeforeNav_Info          = {CC_STDCALL, VT_EMPTY, 7, {VT_DISPATCH,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_BOOL}};
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::ProgressChange_Info     = {CC_STDCALL, VT_EMPTY, 2, {VT_I4,VT_I4}};
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::DTCommunicate_Info      = {CC_STDCALL, VT_VARIANT, 3, { VT_VARIANT, VT_VARIANT, VT_VARIANT }};
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::DTReadFile_Info         = {CC_STDCALL, VT_VARIANT, 1, { VT_VARIANT }};
__declspec(selectany) _ATL_FUNC_INFO CUiFeatureBrowserCtrl::DTWriteFile_Info        = {CC_STDCALL, VT_VARIANT, 2, { VT_VARIANT, VT_VARIANT }};

