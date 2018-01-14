
#include "StdAfx.h"
#include "UiFeatureBrowserCtrl.h"


CUiFeatureBrowserCtrl::CUiFeatureBrowserCtrl()
{
}

CUiFeatureBrowserCtrl::~CUiFeatureBrowserCtrl()
{
}

HWND CUiFeatureBrowserCtrl::Create(IDispatch* pExternalDispatch, LPCWSTR lpURL, HWND hWndParent, _U_RECT rect /*= NULL*/,
			DWORD dwStyle /*= 0*/, DWORD dwExStyle /*= 0*/,
			_U_MENUorID MenuOrID /*= 0U*/, LPVOID lpCreateParam /*= NULL*/, DWORD dwDocHostFlags /*= 
			DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU | DOCHOSTUIFLAG_SCROLL_NO |
			DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE | DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER |
			DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_FLAT_SCROLLBAR*/)
{
	return CUiFeatureHtmlView::Create((IDispatch*)(CDTExternalDispatch<0, CUiFeatureBrowserCtrl>*)this, lpURL, hWndParent, rect, dwStyle, dwExStyle, MenuOrID, lpCreateParam, dwDocHostFlags);
}

LRESULT CUiFeatureBrowserCtrl::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    LRESULT lRet = CUiFeatureHtmlView::OnCreate(uMsg, wParam, lParam, bHandled);

    DispIEEventAdvise();
    EnableIEDrop(FALSE);

    bHandled = TRUE;

    return lRet;
}

LRESULT CUiFeatureBrowserCtrl::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    DispIEEventUnAdvise();
    bHandled = FALSE;
    return 0;
}

void __stdcall CUiFeatureBrowserCtrl::OnDocumentComplete( LPDISPATCH pDisp, VARIANT FAR* URL )
{
}

void __stdcall CUiFeatureBrowserCtrl::OnNavigateError( LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* Error, VARIANT_BOOL* bCancel )
{
	CComBSTR bstrUrl(URL->bstrVal);

	CString strUrl(bstrUrl);
	strUrl = strUrl.MakeLower();
	strUrl.Remove(' ');
	strUrl.Remove('\t');
	if (strUrl.Find(L"javascript:") >= 0)
		return;

	DWORD dwError = Error->intVal;
	if (INET_E_RESOURCE_NOT_FOUND == dwError /* or some other errors */)
	{
		// #404
	}

	*bCancel = VARIANT_FALSE;
	CComPtr<IWebBrowser2> spWebBrowser2;
	HRESULT hRet = QueryControl (IID_IWebBrowser2, (void**)&spWebBrowser2) ;
	if (FAILED(hRet))
		return;

	BSTR bsLocationUrl = NULL;
	spWebBrowser2->get_LocationURL(&bsLocationUrl);
	CString strLocationUrl(bsLocationUrl);
	if (!strLocationUrl.IsEmpty() && strLocationUrl.Compare(strUrl))
		return;

	spWebBrowser2->Stop();
}

void __stdcall CUiFeatureBrowserCtrl::OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* pvURL, VARIANT* pvFlags, VARIANT* pvTargetFrameName, VARIANT* pvPostData, VARIANT* pvHeaders, VARIANT_BOOL* pbCancel)
{
}

void __stdcall CUiFeatureBrowserCtrl::OnProgressChange(long nProgress, long nProgressMax)
{
    CComPtr<IWebBrowser2> spWebBrowser2;
    HRESULT hRet = QueryControl (IID_IWebBrowser2, (void**)&spWebBrowser2);

    BSTR bsLocationURL = NULL;
    spWebBrowser2->get_LocationURL(&bsLocationURL);
    CString strLocationURL(bsLocationURL);

    CComPtr<IDispatch> pDoc;
    hRet = spWebBrowser2->get_Document(&pDoc);
    if (SUCCEEDED(hRet) && pDoc != NULL && !IsBlankUrl(strLocationURL))// && nProgressMax == nProgress)
    {
        HWND hParentWnd = NULL;
        hParentWnd = GetParent().m_hWnd;
        if (::IsWindow(hParentWnd))            
            ::PostMessage(hParentWnd, WM_IE_DOCUMENT_COMPLETE, 0, 0);
    }
}

HRESULT CUiFeatureBrowserCtrl::NotifyReadFile(const wchar_t *pcszKey, const wchar_t *pcszContent)
{
    CComPtr<IDispatch>   spScript;
    CComPtr<IDispatchEx> spScriptObject;

    if (FAILED(GetIEScriptDispatch(&spScript, &spScriptObject)))
        return E_FAIL;

    CStringW strKey;
    CStringW strContent;

    if (NULL != pcszKey)
        strKey = pcszKey;
    if (NULL != pcszContent)
        strContent = pcszContent;    

    spScriptObject.PutProperty(L"KeyName", strKey);
    spScriptObject.PutProperty(L"FileContent", strContent);

    CComVariant vRet;
    spScript.Invoke1(L"NotifyReadFile", &CComVariant(spScriptObject), &vRet);

    return S_OK;
}

HRESULT CUiFeatureBrowserCtrl::NotifyCommunicate(const wchar_t *pcszCommandID, const wchar_t *pcszCommandParams)
{
    CComPtr<IDispatch>   spScript;
    CComPtr<IDispatchEx> spScriptObject;

    if (FAILED(GetIEScriptDispatch(&spScript, &spScriptObject)))
        return E_FAIL;

    CStringW strCommandID;
    CStringW strCommandParams;

    if (NULL != pcszCommandID)
        strCommandID = pcszCommandID;
    if (NULL != pcszCommandParams)
        strCommandParams = pcszCommandParams;

    spScriptObject.PutProperty(L"CommandID", strCommandID);
    spScriptObject.PutProperty(L"CommandParams", strCommandParams);

    CComVariant vRet;
    spScript.Invoke1(L"NotifyCommunicate", &CComVariant(spScriptObject), &vRet);

    return S_OK;
}

HRESULT CUiFeatureBrowserCtrl::GetIEScriptDispatch(IDispatch **ppScript, IDispatchEx **ppScriptObject)
{
    ATLASSERT(ppScript && ppScriptObject);
    do 
    {
        CComPtr<IDispatch> spScript;
        if (FAILED(GetScriptDispatch(&spScript)))
            break;

        CComPtr<IDispatchEx> spScriptObject;

        if (FAILED(CreateScriptObject(&spScriptObject)))
            break;

        *ppScript		= spScript.Detach();
        *ppScriptObject	= spScriptObject.Detach();

        return S_OK;

    } while (false);

    return E_FAIL;
}

BOOL CUiFeatureBrowserCtrl::DispIEEventAdvise()
{
    CComPtr<IWebBrowser2> spWebBrowser2;
    HRESULT hRet = QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2);
    if(!SUCCEEDED(hRet))
        return FALSE;

    HRESULT hr = IDispEventSimpleImpl<1, CUiFeatureBrowserCtrl, &DIID_DWebBrowserEvents2>::DispEventAdvise(spWebBrowser2, &DIID_DWebBrowserEvents2);
    if(FAILED(hr))
        return FALSE;

    return TRUE;
}

void CUiFeatureBrowserCtrl::DispIEEventUnAdvise()
{
    CComPtr<IWebBrowser2> spWebBrowser2;
    HRESULT hRet = QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2);
    if(SUCCEEDED(hRet))
        IDispEventSimpleImpl<1, CUiFeatureBrowserCtrl, &DIID_DWebBrowserEvents2>::DispEventUnadvise(spWebBrowser2, &DIID_DWebBrowserEvents2);
}

BOOL CUiFeatureBrowserCtrl::EnableIEDrop(BOOL bEnable)
{
    CComPtr<IWebBrowser2> WebBrowser2;
    HRESULT hRet = QueryControl(IID_IWebBrowser2, (void**)&WebBrowser2);
    if(!SUCCEEDED(hRet))
        return FALSE;

    WebBrowser2->put_RegisterAsDropTarget(bEnable ? VARIANT_TRUE : VARIANT_FALSE);

    return TRUE;
}

BOOL CUiFeatureBrowserCtrl::IsBlankUrl(const CString &strURL)
{
    CString str(strURL);

    str.Trim();
    str.MakeLower();
    if (str.IsEmpty() || 0==str.CompareNoCase(STR_URL_ABOUT_BLANK))	
        return TRUE;

    return FALSE;
}