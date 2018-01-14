
#include "stdafx.h"
#include "windows.h"
#include "stddef.h"
#include "windowsx.h"
#include "memory.h"
#include <imm.h>
#include <tchar.h>
#include "IUiFeatureTextHost.h"

BOOL fInAssert = FALSE;

// HIMETRIC units per inch (used for conversion)
#define HIMETRIC_PER_INCH 2540

// Convert Himetric along the X axis to X pixels
LONG HimetricXtoDX(LONG xHimetric, LONG xPerInch)
{
	return (LONG)::MulDiv(xHimetric, xPerInch, HIMETRIC_PER_INCH);
}

// Convert Himetric along the Y axis to Y pixels
LONG HimetricYtoDY(LONG yHimetric, LONG yPerInch)
{
	return (LONG)::MulDiv(yHimetric, yPerInch, HIMETRIC_PER_INCH);
}

// Convert Pixels on the X axis to Himetric
LONG DXtoHimetricX(LONG dx, LONG xPerInch)
{
	return (LONG)::MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
}

// Convert Pixels on the Y axis to Himetric
LONG DYtoHimetricY(LONG dy, LONG yPerInch)
{
	return (LONG)::MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
}


// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0

const LONG cInitTextMax = (32 * 1024) - 1;
const LONG cResetTextMax = (64 * 1024);


#define ibPed 0
#define SYS_ALTERNATE 0x20000000




INT	cxBorder = 0, cyBorder = 0;	    // GetSystemMetricx(SM_CXBORDER)...
INT	cxDoubleClk, cyDoubleClk;   // Double click distances
INT	cxHScroll, cxVScroll;	    // Width/height of scrlbar arw bitmap
INT	cyHScroll, cyVScroll;	    // Width/height of scrlbar arw bitmap
INT	dct = 0;			    // Double Click Time in milliseconds
INT nScrollInset = 0;
COLORREF crAuto = 0;

LONG IUiFeatureTextHost::m_sl_xWidthSys = 0;		// average char width of system font
LONG IUiFeatureTextHost::m_sl_yHeightSys = 0;		// height of system font
LONG IUiFeatureTextHost::m_sl_yPerInch = 0;			// y pixels per inch
LONG IUiFeatureTextHost::m_sl_xPerInch = 0;			// x pixels per inch


EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    0x8d33f740,
    0xcf58,
    0x11ce,
    {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
  };

EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
    0xc5bdd8d0,
    0xd26e,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
  };


EXTERN_C const IID IID_ITextEditControl = { /* f6642620-d266-11ce-a89e-00aa006cadc5 */
    0xf6642620,
    0xd266,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
  };

void GetSysParms()
{
	crAuto		= GetSysColor(COLOR_WINDOWTEXT);
	cxBorder	= GetSystemMetrics(SM_CXBORDER);	// Unsizable window border
	cyBorder	= GetSystemMetrics(SM_CYBORDER);	//  widths
	cxHScroll	= GetSystemMetrics(SM_CXHSCROLL);	// Scrollbar-arrow bitmap 
	cxVScroll	= GetSystemMetrics(SM_CXVSCROLL);	//  dimensions
	cyHScroll	= GetSystemMetrics(SM_CYHSCROLL);	//
	cyVScroll	= GetSystemMetrics(SM_CYVSCROLL);	//
	cxDoubleClk	= GetSystemMetrics(SM_CXDOUBLECLK);
	cyDoubleClk	= GetSystemMetrics(SM_CYDOUBLECLK);
	dct			= ::GetDoubleClickTime();
    
	nScrollInset = ::GetProfileInt(_T("windows"), _T("ScrollInset"), DD_DEFSCROLLINSET);
}

HRESULT InitDefaultCharFormat(CHARFORMATW *pcf, HFONT hFont) 
{
	if (pcf == NULL)
		return S_FALSE;

	HWND hWnd = NULL;
	LOGFONT lf;
	HDC hdc = NULL;
	LONG yPixPerInch = 0;

	// Get LOGFONT for default font
	if (hFont == NULL)
		hFont = (HFONT)::GetStockObject(SYSTEM_FONT);

	// Get LOGFONT for passed hFont
	if (!::GetObject(hFont, sizeof(LOGFONT), &lf))
		return E_FAIL;

	// Set CHARFORMAT structure
	pcf->cbSize = sizeof(CHARFORMAT2);
	
	hWnd = GetDesktopWindow();
	hdc = GetDC(hWnd);
	yPixPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
	pcf->yHeight = lf.lfHeight * LY_PER_INCH / yPixPerInch;
	ReleaseDC(hWnd, hdc);

	pcf->yOffset = 0;
	pcf->crTextColor = crAuto;

	pcf->dwEffects = CFM_EFFECTS | CFE_AUTOBACKCOLOR;
	pcf->dwEffects &= ~(CFE_PROTECTED | CFE_LINK);

	if(lf.lfWeight < FW_BOLD)
		pcf->dwEffects &= ~CFE_BOLD;
	if(!lf.lfItalic)
		pcf->dwEffects &= ~CFE_ITALIC;
	if(!lf.lfUnderline)
		pcf->dwEffects &= ~CFE_UNDERLINE;
	if(!lf.lfStrikeOut)
		pcf->dwEffects &= ~CFE_STRIKEOUT;

	pcf->dwMask = CFM_ALL | CFM_BACKCOLOR;
	pcf->bCharSet = lf.lfCharSet;
	pcf->bPitchAndFamily = lf.lfPitchAndFamily;

#ifdef UNICODE
	_tcscpy_s(pcf->szFaceName, LF_FACESIZE, lf.lfFaceName);
#else
	//need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
	MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif
	
	return S_OK;
}

HRESULT InitDefaultParaFormat(PARAFORMAT * ppf) 
{
	if (ppf == NULL)
		return S_FALSE;

	memset(ppf, 0, sizeof(PARAFORMAT));

	ppf->cbSize = sizeof(PARAFORMAT);
	ppf->dwMask = PFM_ALL;
	ppf->wAlignment = PFA_LEFT;
	ppf->cTabCount = 1;
	ppf->rgxTabs[0] = lDefaultTab;
	return S_OK;
}



LRESULT MapHresultToLresult(HRESULT hr, UINT msg)
{
	LRESULT lres = hr;

	switch(msg)
	{
	case EM_GETMODIFY:
		lres = (hr == S_OK) ? -1 : 0;
		break;

		// These messages must return TRUE/FALSE rather than an hresult.
	case EM_UNDO:
	case WM_UNDO:
	case EM_CANUNDO:
	case EM_CANPASTE:
	case EM_LINESCROLL:

		// Hresults are backwards from TRUE and FALSE so we need
		// to do that remapping here as well.

		lres = (hr == S_OK) ? TRUE : FALSE;
		break;

	case EM_EXLINEFROMCHAR:
	case EM_LINEFROMCHAR:

		// If success, then hr a number. If error, it s/b 0.
		lres = SUCCEEDED(hr) ? (LRESULT) hr : 0;
		break;
			
	case EM_LINEINDEX:

		// If success, then hr a number. If error, it s/b -1.
		lres = SUCCEEDED(hr) ? (LRESULT) hr : -1;
		break;	

	default:
		lres = (LRESULT) hr;		
	}

	return lres;
}


BOOL GetIconic(HWND hWnd) 
{
	while (hWnd)
	{
		if(::IsIconic(hWnd))
			return TRUE;

		hWnd = ::GetParent(hWnd);
	}
	return FALSE;
}

IUiFeatureTextHost::IUiFeatureTextHost()
{
	ZeroMemory(&m_pNotifyCall, sizeof(IUiFeatureTextHost) - offsetof(IUiFeatureTextHost, m_pNotifyCall));
	cchTextMost = cInitTextMax;
	laccelpos = -1;
	m_pTextServ = NULL;
	m_hWnd = NULL;
	m_dwStyle = 0;
	m_dwExStyle = 0;
}

IUiFeatureTextHost::~IUiFeatureTextHost()
{
	// Revoke our drop target
	RevokeDragDrop();
	if (m_pTextServ != NULL)
	{
		m_pTextServ->OnTxInPlaceDeactivate();
		m_pTextServ->Release();
	}
}

////////////////////// Create/Init/Destruct Commands ///////////////////////
BOOL CreateHost(HWND hWnd, const CREATESTRUCT *pcs, CHostCallBack* pNotifyCall, IUiFeatureTextHost **ppTextSev)
{
	if (hWnd == NULL || pcs == NULL || ppTextSev == NULL)
		return FALSE;

	::GdiSetBatchLimit(1);

	IUiFeatureTextHost *phost = new IUiFeatureTextHost();
	if (phost == NULL)
		return FALSE;

	if (!phost->Init(hWnd, pcs, pNotifyCall))
	{
		delete phost;
		return FALSE;
	}

	*ppTextSev = phost;
	return TRUE;
}


/*
 *	IUiFeatureTextHost::Init
 *
 *	Purpose:
 *		Initializes this IUiFeatureTextHost
 *
 *	TODO:	Format Cache logic needs to be cleaned up. 
 */
BOOL IUiFeatureTextHost::Init(HWND hWnd, const CREATESTRUCT *pcs, CHostCallBack* p_nc)
{
	HDC hdc = NULL;
	HFONT hfontOld = NULL;
	TEXTMETRIC tm;
	IUnknown *pUnk = NULL;
	HRESULT hr = NULL;

	// Initialize Reference count
	m_ul_cRefs = 1;

	// Set up the notification callback
	m_pNotifyCall = p_nc;	

	m_hWnd = hWnd;

	// Create and cache CHARFORMAT for this control
	if(FAILED(InitDefaultCharFormat(&cf, NULL)))
		return FALSE;

	// Create and cache PARAFORMAT for this control
	if(FAILED(InitDefaultParaFormat(&pf)))
		return FALSE;

	// edit controls created without a window are multiline by default
	// so that paragraph formats can be
	m_dwStyle = ES_MULTILINE;
	fHidden = TRUE;

	// edit controls are rich by default
	fRich = TRUE;

	if (pcs)
	{
		m_dwExStyle = pcs->dwExStyle;
		m_dwStyle = pcs->style;

		fBorder = !!(m_dwStyle & WS_BORDER);

		if(m_dwStyle & ES_SUNKEN)
		{
			fBorder = TRUE;
		}

		if (!(m_dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)))
		{
			fWordWrap = TRUE;
		}
	}

	if(!(m_dwStyle & ES_LEFT))
	{
		if(m_dwStyle & ES_CENTER)
			pf.wAlignment = PFA_CENTER;
		else if(m_dwStyle & ES_RIGHT)
			pf.wAlignment = PFA_RIGHT;
	}

	// Init system metrics
	hdc = GetDC(m_hWnd);
	if(!hdc)
		return FALSE;

	hfontOld = (HFONT)SelectObject(hdc, GetStockObject(SYSTEM_FONT));

	if(!hfontOld)
		return FALSE;

	GetTextMetrics(hdc, &tm);
	SelectObject(hdc, hfontOld);

	m_sl_xWidthSys = (INT) tm.tmAveCharWidth;
	m_sl_yHeightSys = (INT) tm.tmHeight;
	m_sl_xPerInch = GetDeviceCaps(hdc, LOGPIXELSX); 
	m_sl_yPerInch =	GetDeviceCaps(hdc, LOGPIXELSY); 

	ReleaseDC(m_hWnd, hdc);

	// At this point the border flag is set and so is the pixels per inch
	// so we can initalize the inset.
	SetDefaultInset();

	fInplaceActive = TRUE;

	// Create Text Services component
	if(FAILED(CreateTextServices(NULL, this, &pUnk)))
		return FALSE;

	hr = pUnk->QueryInterface(IID_ITextServices,(void **)&m_pTextServ);

	// Whether the previous call succeeded or failed we are done
	// with the private interface.
	pUnk->Release();

	if(FAILED(hr))
		return FALSE;

	// Set window text
	if(pcs && pcs->lpszName)
	{
#ifdef UNICODE		
		if(FAILED(m_pTextServ->TxSetText((TCHAR *)pcs->lpszName)))
			return FALSE;
#else
		// have to thunk the string to a unicode string.
		WCHAR wsz_name[MAX_PATH] ;
		ZeroMemory(wsz_name, MAX_PATH *sizeof WCHAR) ;
		MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, lstrlen(pcs->lpszName),(LPWSTR)&wsz_name, MAX_PATH) ;
		if(FAILED(m_pTextServ->TxSetText((LPWSTR)&wsz_name)))
			return FALSE;
#endif // UNICODE
	}

	rcClient.left = pcs->x;
	rcClient.top = pcs->y;
	rcClient.right = pcs->x + pcs->cx;
	rcClient.bottom = pcs->y + pcs->cy;

	// The extent matches the full client rectangle in HIMETRIC
	sizelExtent.cx = DXtoHimetricX(pcs->cx - 2 * HOST_BORDER, m_sl_xPerInch);
	sizelExtent.cy = DYtoHimetricY(pcs->cy - 2 * HOST_BORDER, m_sl_yPerInch);

	// notify Text Services that we are in place active
	if(FAILED(m_pTextServ->OnTxInPlaceActivate(&rcClient)))
		return FALSE;

	// Hide all scrollbars to start
	if(m_hWnd && !(m_dwStyle & ES_DISABLENOSCROLL))
	{
		LONG dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		dwStyle &= ~(WS_VSCROLL | WS_HSCROLL);
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	}

	if (!(m_dwStyle & ES_READONLY))
	{
		// This isn't a read only window so we need a drop target.
		RegisterDragDrop();
	}

	return TRUE;
}

/////////////////////////////////  IUnknown ////////////////////////////////
HRESULT IUiFeatureTextHost::QueryInterface(REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_ITextEditControl))
	{
		*ppvObject = (ITextEditControl *) this;
		AddRef();
		hr = S_OK;
	}
	else if (IsEqualIID(riid, IID_IUnknown) 
		|| IsEqualIID(riid, IID_ITextHost)) 
	{
		AddRef();
		*ppvObject = (ITextHost *) this;
		hr = S_OK;
	}

	return hr;
}

ULONG IUiFeatureTextHost::AddRef(void)
{
	return ++m_ul_cRefs;
}

ULONG IUiFeatureTextHost::Release(void)
{
	ULONG c_Refs = --m_ul_cRefs;

	if (c_Refs == 0)
	{
		delete this;
	}

	return c_Refs;
}

//////////////////////////// System API wrapper ////////////////////////////



///////////////////////  Windows message dispatch methods  ///////////////////////////////
LRESULT IUiFeatureTextHost::TxWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT	lres = 0;
	HRESULT hr;

	switch(msg)
	{
	case WM_NCCALCSIZE:
		// we can't rely on WM_WININICHANGE so we use WM_NCCALCSIZE since
		// changing any of these should trigger a WM_NCCALCSIZE
		GetSysParms();
		break;

	case WM_KEYDOWN:
		lres = OnKeyDown((WORD) wparam, (DWORD) lparam);
		if(lres != 0)
			goto serv;
		break;		   

	case WM_CHAR:
		lres = OnChar((WORD) wparam, (DWORD) lparam);
		if(lres != 0)
			goto serv;
		break;

	case WM_SYSCOLORCHANGE:
		OnSysColorChange();

		// Notify the text services that there has been a change in the 
		// system colors.
		goto serv;

	case EM_HIDESELECTION:
		if((BOOL)lparam)
		{
			DWORD dwPropertyBits = 0;

			if((BOOL)wparam)
			{
				m_dwStyle &= ~(DWORD) ES_NOHIDESEL;
				dwPropertyBits = TXTBIT_HIDESELECTION;
			}
			else
				m_dwStyle |= ES_NOHIDESEL;

			// Notify text services of change in status.
			m_pTextServ->OnTxPropertyBitsChange(TXTBIT_HIDESELECTION, 
				dwPropertyBits);
		}

		goto serv;



    case EM_LIMITTEXT:

        lparam = wparam;

        // Intentionally fall through. These messages are duplicates.

	case EM_EXLIMITTEXT:

        if (lparam == 0)
        {
            // 0 means set the control to the maximum size. However, because
            // 1.0 set this to 64K will keep this the same value so as not to
            // supprise anyone. Apps are free to set the value to be above 64K.
            lparam = (LPARAM) cResetTextMax;
        }

		cchTextMost = (LONG) lparam;
		m_pTextServ->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, 
					TXTBIT_MAXLENGTHCHANGE);

		break;

	case EM_SETREADONLY:
		OnSetReadOnly(BOOL(wparam));
		lres = 1;
		break;

	case EM_GETEVENTMASK:
		lres = OnGetEventMask();
		break;

	case EM_SETEVENTMASK:
		OnSetEventMask((DWORD) lparam);
		goto serv;

	case EM_GETOPTIONS:
		lres = OnGetOptions();
		break;

	case EM_SETOPTIONS:
		OnSetOptions((WORD) wparam, (DWORD) lparam);
		lres = (m_dwStyle & ECO_STYLES);
		if(fEnableAutoWordSel)
			lres |= ECO_AUTOWORDSELECTION;
		break;

	case WM_SETFONT:
		lres = OnSetFont((HFONT) wparam);
		break;

	case EM_SETRECT:
        OnSetRect((LPRECT)lparam);
        break;
        
	case EM_GETRECT:
        OnGetRect((LPRECT)lparam);
        break;

	case EM_SETBKGNDCOLOR:

		lres = (LRESULT) crBackground;
		fNotSysBkgnd = !wparam;
		crBackground = (COLORREF) lparam;

		if(wparam)
			crBackground = GetSysColor(COLOR_WINDOW);

		// Notify the text services that color has changed
		m_pTextServ->TxSendMessage(WM_SYSCOLORCHANGE, 0, 0, 0);

		if(lres != (LRESULT) crBackground)
			TxInvalidateRect(NULL, TRUE);

		break;

	case EM_SETCHARFORMAT:
		if(!FValidCF((CHARFORMAT *) lparam))
		{
			return 0;
		}

		if(wparam & SCF_SELECTION)
			goto serv;								// Change selection format
		OnSetCharFormat((CHARFORMAT *) lparam);		// Change default format
		break;

	case EM_SETPARAFORMAT:
		if(!FValidPF((PARAFORMAT *) lparam))
		{
			return 0;
		}

		// check to see if we're setting the default.
		// either SCF_DEFAULT will be specified *or* there is no
		// no text in the document (richedit1.0 behaviour).
		if (!(wparam & SCF_DEFAULT))
		{
			hr = m_pTextServ->TxSendMessage(WM_GETTEXTLENGTH, 0, 0, 0);
			if (hr == 0)
			{
				wparam |= SCF_DEFAULT;
			}
		}

		if(wparam & SCF_DEFAULT)
		{								
			OnSetParaFormat((PARAFORMAT *) lparam);	// Change default format
		}
		else
		{
			goto serv;								// Change selection format
		}
		break;

    case WM_SETTEXT:

        // For RichEdit 1.0, the max text length would be reset by a settext so 
        // we follow pattern here as well.

		hr = m_pTextServ->TxSendMessage(msg, wparam, lparam, 0);

        if (SUCCEEDED(hr))
        {
            // Update succeeded.
            LONG cNewText = _tcslen((LPCTSTR) lparam);

            // If the new text is greater than the max set the max to the new
            // text length.
            if (cNewText > cchTextMost)
            {
                cchTextMost = cNewText;                
            }

			lres = 1;
        }

        break;

	case WM_SIZE:
		lres = OnSize(hWnd, wparam, LOWORD(lparam), HIWORD(lparam));
		break;

	case WM_SETCURSOR:
		//Only set cursor when over us rather than a child; this
		//			helps prevent us from fighting it out with an inplace child
		if((HWND)wparam == hWnd)
		{
			if(!(lres = ::DefWindowProc(hWnd, msg, wparam, lparam)))
			{
				POINT pt;
				GetCursorPos(&pt);
				::ScreenToClient(hWnd, &pt);
				m_pTextServ->OnTxSetCursor(
					DVASPECT_CONTENT,	
					-1,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,			// Client rect - no redraw 
					pt.x, 
					pt.y);
				lres = TRUE;
			}
		}
		break;

	case WM_SHOWWINDOW:
		hr = OnTxVisibleChange((BOOL)wparam);
		break;

	case WM_PAINT:
		{
			// Put a frame around the control so it can be seen
			FrameRect((HDC) wparam, &rcClient, 
				(HBRUSH) GetStockObject(BLACK_BRUSH));

			RECT rcClient;
			RECT *prc = NULL;
			LONG lViewId = TXTVIEW_ACTIVE;

			if (!fInplaceActive)
			{
				GetControlRect(&rcClient);
				prc = &rcClient;
				lViewId = TXTVIEW_INACTIVE;
			}
			
			// Remember wparam is actually the hdc and lparam is the update
			// rect because this message has been preprocessed by the window.
			m_pTextServ->TxDraw(
	    		DVASPECT_CONTENT,  		// Draw Aspect
				/*-1*/0,						// Lindex
				NULL,					// Info for drawing optimazation
				NULL,					// target device information
	        	(HDC) wparam,			// Draw device HDC
	        	NULL, 				   	// Target device HDC
				(RECTL *) prc,			// Bounding client rectangle
				NULL, 					// Clipping rectangle for metafiles
				(RECT *) lparam,		// Update rectangle
				NULL, 	   				// Call back function
				NULL,					// Call back parameter
				lViewId);				// What view of the object				
		}

		break;

	default:
serv:
		{
			hr = m_pTextServ->TxSendMessage(msg, wparam, lparam, &lres);

			if (hr == S_FALSE)
			{
				lres = ::DefWindowProc(hWnd, msg, wparam, lparam);
			}
		}
	}

	return lres;
}
	

///////////////////////////////  Keyboard Messages  //////////////////////////////////


LRESULT IUiFeatureTextHost::OnKeyDown(WORD vkey, DWORD dwFlags)
{
	switch(vkey)
	{
	case VK_ESCAPE:
	//	::PostMessage(m_hWnd, WM_KEYDOWN, (WPARAM)VK_ESCAPE, 0);
	//	break;

	case VK_RETURN:
	//	::PostMessage(m_hWnd, WM_KEYDOWN, (WPARAM)VK_RETURN, 0);
	//	break;

	case VK_TAB:
	//	::PostMessage(m_hWnd, WM_KEYDOWN, (WPARAM)VK_RETURN, 0);
	//	break;

	default:
		break;
	}

	return 1;
}

LRESULT IUiFeatureTextHost::OnChar(WORD vkey, DWORD dwFlags)
{
	switch(vkey)
	{
	case VK_ESCAPE:
	case VK_RETURN:
	case VK_TAB:
	default:
		break;
	}
	
	return 1;
}


////////////////////////////////////  View rectangle //////////////////////////////////////


void IUiFeatureTextHost::OnGetRect(LPRECT prc)
{
    RECT rcInset;

	// Get view inset (in HIMETRIC)
    TxGetViewInset(&rcInset);

	// Convert the himetric inset to pixels
	rcInset.left = HimetricXtoDX(rcInset.left, m_sl_xPerInch);
	rcInset.top = HimetricYtoDY(rcInset.top , m_sl_yPerInch);
	rcInset.right = HimetricXtoDX(rcInset.right, m_sl_xPerInch);
	rcInset.bottom = HimetricYtoDY(rcInset.bottom, m_sl_yPerInch);
    
	// Get client rect in pixels
    TxGetClientRect(prc);

	// Modify the client rect by the inset 
    prc->left += rcInset.left;
    prc->top += rcInset.top;
    prc->right -= rcInset.right;
    prc->bottom -= rcInset.bottom;
}

void IUiFeatureTextHost::OnSetRect(LPRECT prc)
{
	RECT rcClient;
	
	if(!prc)
	{
		SetDefaultInset();
	}	
	else	
    {
    	// For screen display, the following intersects new view RECT
    	// with adjusted client area RECT
    	TxGetClientRect(&rcClient);

        // Adjust client rect
        // Factors in space for borders
        if(fBorder)
        {																					  
    	    rcClient.top		+= m_sl_yHeightSys / 4;
    	    rcClient.bottom 	-= m_sl_yHeightSys / 4 - 1;
    	    rcClient.left		+= m_sl_xWidthSys / 2;
    	    rcClient.right	-= m_sl_xWidthSys / 2;
        }
	
        // Ensure we have minimum width and height
        rcClient.right = max(rcClient.right, rcClient.left + m_sl_xWidthSys);
        rcClient.bottom = max(rcClient.bottom, rcClient.top + m_sl_yHeightSys);

        // Intersect the new view rectangle with the 
        // adjusted client area rectangle
        if(!IntersectRect(&rcViewInset, &rcClient, prc))
    	    rcViewInset = rcClient;

        // compute inset in pixels
        rcViewInset.left -= rcClient.left;
        rcViewInset.top -= rcClient.top;
        rcViewInset.right = rcClient.right - rcViewInset.right;
        rcViewInset.bottom = rcClient.bottom - rcViewInset.bottom;

		// Convert the inset to himetric that must be returned to ITextServices
        rcViewInset.left = DXtoHimetricX(rcViewInset.left, m_sl_xPerInch);
        rcViewInset.top = DYtoHimetricY(rcViewInset.top, m_sl_yPerInch);
        rcViewInset.right = DXtoHimetricX(rcViewInset.right, m_sl_xPerInch);
        rcViewInset.bottom = DYtoHimetricY(rcViewInset.bottom, m_sl_yPerInch);
    }

    m_pTextServ->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
}

////////////////////////////////////  System notifications  //////////////////////////////////
void IUiFeatureTextHost::OnSysColorChange()
{
	crAuto = GetSysColor(COLOR_WINDOWTEXT);
	if(!fNotSysBkgnd)
		crBackground = GetSysColor(COLOR_WINDOW);
	TxInvalidateRect(NULL, TRUE);
}

/////////////////////////////////  Other messages  //////////////////////////////////////
LRESULT IUiFeatureTextHost::OnGetOptions() const
{
	LRESULT lres = (m_dwStyle & ECO_STYLES);

	if(fEnableAutoWordSel)
		lres |= ECO_AUTOWORDSELECTION;
	
	return lres;
}

void IUiFeatureTextHost::OnSetOptions(WORD wOp, DWORD eco)
{
	const BOOL fAutoWordSel = !!(eco & ECO_AUTOWORDSELECTION);
	DWORD dwStyleNew = m_dwStyle;
	DWORD dw_Style = 0 ;

	DWORD dwChangeMask = 0;

	// single line controls can't have a selection bar
	// or do vertical writing
	if(!(dw_Style & ES_MULTILINE))
	{
#ifdef DBCS
		eco &= ~(ECO_SELECTIONBAR | ECO_VERTICAL);
#else
		eco &= ~ECO_SELECTIONBAR;
#endif
	}
	dw_Style = (eco & ECO_STYLES);

	switch(wOp)
	{
	case ECOOP_SET:
		dwStyleNew = ((dwStyleNew) & ~ECO_STYLES) | m_dwStyle;
		fEnableAutoWordSel = fAutoWordSel;
		break;

	case ECOOP_OR:
		dwStyleNew |= dw_Style;
		if(fAutoWordSel)
			fEnableAutoWordSel = TRUE;
		break;

	case ECOOP_AND:
		dwStyleNew &= (dw_Style | ~ECO_STYLES);
		if(fEnableAutoWordSel && !fAutoWordSel)
			fEnableAutoWordSel = FALSE;
		break;

	case ECOOP_XOR:
		dwStyleNew ^= dw_Style;
		fEnableAutoWordSel = (!fEnableAutoWordSel != !fAutoWordSel);
		break;
	}

	if(fEnableAutoWordSel != (unsigned)fAutoWordSel)
	{
		dwChangeMask |= TXTBIT_AUTOWORDSEL; 
	}

	if(dwStyleNew != dw_Style)
	{
		DWORD dwChange = dwStyleNew ^ dw_Style;
#ifdef DBCS
		USHORT	usMode;
#endif

		m_dwStyle = dwStyleNew;
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyleNew);

		if(dwChange & ES_NOHIDESEL)	
		{
			dwChangeMask |= TXTBIT_HIDESELECTION;
		}

		if(dwChange & ES_READONLY)
		{
			dwChangeMask |= TXTBIT_READONLY;

			// Change drop target state as appropriate.
			if (dwStyleNew & ES_READONLY)
			{
				RevokeDragDrop();
			}
			else
			{
				RegisterDragDrop();
			}
		}

		if(dwChange & ES_VERTICAL)
		{
			dwChangeMask |= TXTBIT_VERTICAL;
		}

		// no action require for ES_WANTRETURN
		// no action require for ES_SAVESEL
		// do this last
		if(dwChange & ES_SELECTIONBAR)
		{
			lSelBarWidth = 212;
			dwChangeMask |= TXTBIT_SELBARCHANGE;
		}
	}

	if (dwChangeMask)
	{
		DWORD dwProp = 0;
		TxGetPropertyBits(dwChangeMask, &dwProp);
		m_pTextServ->OnTxPropertyBitsChange(dwChangeMask, dwProp);
	}
}

void IUiFeatureTextHost::OnSetReadOnly(BOOL fReadOnly)
{
	DWORD dwUpdatedBits = 0;

	if(fReadOnly)
	{
		m_dwStyle |= ES_READONLY;

		// Turn off Drag Drop 
		RevokeDragDrop();
		dwUpdatedBits |= TXTBIT_READONLY;
	}
	else
	{
		m_dwStyle &= ~(DWORD) ES_READONLY;

		// Turn drag drop back on
		RegisterDragDrop();	
	}

	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_READONLY, dwUpdatedBits);
}

void IUiFeatureTextHost::OnSetEventMask(DWORD mask)
{
	LRESULT lres = (LRESULT) dwEventMask;
	dwEventMask = (DWORD) mask;

}


LRESULT IUiFeatureTextHost::OnGetEventMask() const
{
	return (LRESULT) dwEventMask;
}

/*
 *	IUiFeatureTextHost::OnSetFont(hFont)
 *
 *	Purpose:	
 *		Set new font from hFont
 *
 *	Arguments:
 *		hFont	new font (NULL for system font)
 */
BOOL IUiFeatureTextHost::OnSetFont(HFONT hFont)
{
	if(SUCCEEDED(InitDefaultCharFormat(&cf, hFont)))
	{
		m_pTextServ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
			TXTBIT_CHARFORMATCHANGE);
		return TRUE;
	}

	return FALSE;
}

/*
 *	IUiFeatureTextHost::OnSetCharFormat(pcf)
 *
 *	Purpose:	
 *		Set new default CharFormat
 *
 *	Arguments:
 *		pch		ptr to new CHARFORMAT
 */
BOOL IUiFeatureTextHost::OnSetCharFormat(CHARFORMAT *pcf)
{
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
		TXTBIT_CHARFORMATCHANGE);

	return TRUE;
}

/*
 *	IUiFeatureTextHost::OnSetParaFormat(ppf)
 *
 *	Purpose:	
 *		Set new default ParaFormat
 *
 *	Arguments:
 *		pch		ptr to new PARAFORMAT
 */
BOOL IUiFeatureTextHost::OnSetParaFormat(PARAFORMAT *pPF)
{
	pf = *pPF;									// Copy it

	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 
		TXTBIT_PARAFORMATCHANGE);

	return TRUE;
}



////////////////////////////  Event firing  /////////////////////////////////



void * IUiFeatureTextHost::CreateNmhdr(UINT uiCode, LONG cb)
{
	NMHDR *pnmhdr;

	pnmhdr = (NMHDR*) new char[cb];
	if(!pnmhdr)
		return NULL;

	memset(pnmhdr, 0, cb);

	pnmhdr->hwndFrom = m_hWnd;
	pnmhdr->idFrom = GetWindowLong(m_hWnd, GWL_ID);
	pnmhdr->code = uiCode;

	return (VOID *) pnmhdr;
}


////////////////////////////////////  Helpers  /////////////////////////////////////////
void IUiFeatureTextHost::SetDefaultInset()
{
	// Generate default view rect from client rect.
	if(fBorder)
	{
		// Factors in space for borders
		rcViewInset.top = DYtoHimetricY(m_sl_yHeightSys / 4, m_sl_yPerInch);
		rcViewInset.bottom	= DYtoHimetricY(m_sl_yHeightSys / 4 - 1, m_sl_yPerInch);
		rcViewInset.left = DXtoHimetricX(m_sl_xWidthSys / 2, m_sl_xPerInch);
		rcViewInset.right = DXtoHimetricX(m_sl_xWidthSys / 2, m_sl_xPerInch);
	}
	else
	{
		rcViewInset.top = rcViewInset.left = rcViewInset.bottom = rcViewInset.right = 0;
	}
}


/////////////////////////////////  Far East Support  //////////////////////////////////////

HIMC IUiFeatureTextHost::TxImmGetContext(void)
{
#if 0
	HIMC himc;

	himc = ImmGetContext( m_hWnd );

	return himc;
#endif // 0

	return NULL;
}

void IUiFeatureTextHost::TxImmReleaseContext(HIMC himc)
{
	ImmReleaseContext( m_hWnd, himc );
}

void IUiFeatureTextHost::RevokeDragDrop(void)
{
	if (fRegisteredForDrop)
	{
		::RevokeDragDrop(m_hWnd);
		fRegisteredForDrop = FALSE;
	}
}

void IUiFeatureTextHost::RegisterDragDrop(void)
{
	IDropTarget *pdt;

	if(!fRegisteredForDrop && m_pTextServ->TxGetDropTarget(&pdt) == NOERROR)
	{
		HRESULT hr = ::RegisterDragDrop(m_hWnd, pdt);

		if(hr == NOERROR)
		{	
			fRegisteredForDrop = TRUE;
		}

		pdt->Release();
	}
}

VOID DrawRectFn(
	HDC hdc, 
	RECT *prc, 
	INT icrTL, 
	INT icrBR,
	BOOL fBot, 
	BOOL fRght)
{
	COLORREF cr;
	COLORREF crSave;
	RECT rc;

	cr = GetSysColor(icrTL);
	crSave = SetBkColor(hdc, cr);

	// top
	rc = *prc;
	rc.bottom = rc.top + 1;
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

	// left
	rc.bottom = prc->bottom;
	rc.right = rc.left + 1;
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

	if(icrTL != icrBR)
	{
		cr = GetSysColor(icrBR);
		SetBkColor(hdc, cr);
	}

	// right
	rc.right = prc->right;
	rc.left = rc.right - 1;
	if(!fBot)
		rc.bottom -= cyHScroll;
	if(fRght)
	{
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
	}

	// bottom
	if(fBot)
	{
		rc.left = prc->left;
		rc.top = rc.bottom - 1;
		if(!fRght)
			rc.right -= cxVScroll;
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
	}
	SetBkColor(hdc, crSave);
}

LRESULT IUiFeatureTextHost::OnSize(HWND hWnd, WORD fwSizeType, int nWidth, int nHeight)
{
	// Update our client rectangle
	rcClient.right = rcClient.left + nWidth;
	rcClient.bottom = rcClient.top + nHeight;

	if(!fVisible)
	{
		fIconic = GetIconic(hWnd);
		if(!fIconic)
			fResized = TRUE;
	}
	else
	{
		if(GetIconic(hWnd))
		{
			fIconic = TRUE;
		}
		else
		{
			m_pTextServ->OnTxPropertyBitsChange(TXTBIT_CLIENTRECTCHANGE, 
				TXTBIT_CLIENTRECTCHANGE);

			if(fIconic)
			{
				InvalidateRect(hWnd, NULL, FALSE);
				fIconic = FALSE;
			}
		}
	}
	return 0;
}

HRESULT IUiFeatureTextHost::OnTxVisibleChange(BOOL fVisible)
{
	fVisible = fVisible;

	if(!fVisible && fResized)
	{
		RECT rc;
		// Control was resized while hidden,
		// need to really resize now
		TxGetClientRect(&rc);
		fResized = FALSE;
		m_pTextServ->OnTxPropertyBitsChange(TXTBIT_CLIENTRECTCHANGE, 
			TXTBIT_CLIENTRECTCHANGE);
	}

	return S_OK;
}



//////////////////////////// ITextHost Interface  ////////////////////////////

HDC IUiFeatureTextHost::TxGetDC()
{
	return ::GetDC(m_hWnd);
}


int IUiFeatureTextHost::TxReleaseDC(HDC hdc)
{
	return ::ReleaseDC (m_hWnd, hdc);
}


BOOL IUiFeatureTextHost::TxShowScrollBar(INT fnBar,	BOOL fShow)
{
	return ::ShowScrollBar(m_hWnd, fnBar, fShow);
}

BOOL IUiFeatureTextHost::TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags)
{
	return ::EnableScrollBar(m_hWnd, fuSBFlags, fuArrowflags) ;//SB_HORZ, ESB_DISABLE_BOTH);
}


BOOL IUiFeatureTextHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
{
	return ::SetScrollRange(m_hWnd, fnBar, nMinPos, nMaxPos, fRedraw);
}


BOOL IUiFeatureTextHost::TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw)
{
	return ::SetScrollPos(m_hWnd, fnBar, nPos, fRedraw);
}

void IUiFeatureTextHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
{
	::InvalidateRect(m_hWnd, prc, fMode);
}

void IUiFeatureTextHost::TxViewChange(BOOL fUpdate) 
{
	::UpdateWindow (m_hWnd);
}


BOOL IUiFeatureTextHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
{
	return ::CreateCaret (m_hWnd, hbmp, xWidth, yHeight);
}


BOOL IUiFeatureTextHost::TxShowCaret(BOOL fShow)
{
	if(fShow)
		return ::ShowCaret(m_hWnd);
	else
		return ::HideCaret(m_hWnd);
}

BOOL IUiFeatureTextHost::TxSetCaretPos(INT x, INT y)
{
	return ::SetCaretPos(x, y);
}


BOOL IUiFeatureTextHost::TxSetTimer(UINT idTimer, UINT uTimeout)
{
	fTimer = TRUE;
	return ::SetTimer(m_hWnd, idTimer, uTimeout, NULL);
}


void IUiFeatureTextHost::TxKillTimer(UINT idTimer)
{
	::KillTimer(m_hWnd, idTimer);
	fTimer = FALSE;
}

void IUiFeatureTextHost::TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)	
{
	::ScrollWindowEx(m_hWnd, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate, fuScroll);
}

void IUiFeatureTextHost::TxSetCapture(BOOL fCapture)
{
	if (fCapture)
		::SetCapture(m_hWnd);
	else
		::ReleaseCapture();
}

void IUiFeatureTextHost::TxSetFocus()
{
	::SetFocus(m_hWnd);
}

void IUiFeatureTextHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
{
	::SetCursor(hcur);
}

BOOL IUiFeatureTextHost::TxScreenToClient(LPPOINT lppt)
{
	return ::ScreenToClient(m_hWnd, lppt);	
}

BOOL IUiFeatureTextHost::TxClientToScreen(LPPOINT lppt)
{
	return ::ClientToScreen(m_hWnd, lppt);
}

HRESULT IUiFeatureTextHost::TxActivate(LONG *plOldState)
{
    return S_OK;
}

HRESULT IUiFeatureTextHost::TxDeactivate(LONG lNewState)
{
    return S_OK;
}
    

HRESULT IUiFeatureTextHost::TxGetClientRect(LPRECT prc)
{
	*prc = rcClient;

	GetControlRect(prc);

	return NOERROR;
}


HRESULT IUiFeatureTextHost::TxGetViewInset(LPRECT prc) 
{

    *prc = rcViewInset;
    
    return NOERROR;	
}

HRESULT IUiFeatureTextHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
	*ppCF = &cf;
	return NOERROR;
}

HRESULT IUiFeatureTextHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
	*ppPF = &pf;
	return NOERROR;
}


COLORREF IUiFeatureTextHost::TxGetSysColor(int nIndex) 
{
	if (nIndex == COLOR_WINDOW)
	{
		if(!fNotSysBkgnd)
			return GetSysColor(COLOR_WINDOW);
		return crBackground;
	}

	return GetSysColor(nIndex);
}



HRESULT IUiFeatureTextHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
	*pstyle = !fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
	return NOERROR;
}


HRESULT IUiFeatureTextHost::TxGetMaxLength(DWORD *pLength)
{
	*pLength = cchTextMost;
	return NOERROR;
}



HRESULT IUiFeatureTextHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
	*pdwScrollBar =  m_dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
						ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

	return NOERROR;
}


HRESULT IUiFeatureTextHost::TxGetPasswordChar(TCHAR *pch)
{
#ifdef UNICODE
	*pch = chPasswordChar;
#else
	WideCharToMultiByte(CP_ACP, 0, &chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
	return NOERROR;
}

HRESULT IUiFeatureTextHost::TxGetAcceleratorPos(LONG *pcp)
{
	*pcp = laccelpos;
	return S_OK;
} 										   

HRESULT IUiFeatureTextHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
{
	return S_OK;
}


HRESULT IUiFeatureTextHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
{
	pf = *ppf;
	return S_OK;
}


HRESULT IUiFeatureTextHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) 
{
	DWORD dwProperties = 0;

	if (fRich)
	{
		dwProperties = TXTBIT_RICHTEXT;
	}

	if (m_dwStyle & ES_MULTILINE)
	{
		dwProperties |= TXTBIT_MULTILINE;
	}

	if (m_dwStyle & ES_READONLY)
	{
		dwProperties |= TXTBIT_READONLY;
	}


	if (m_dwStyle & ES_PASSWORD)
	{
		dwProperties |= TXTBIT_USEPASSWORD;
	}

	if (!(m_dwStyle & ES_NOHIDESEL))
	{
		dwProperties |= TXTBIT_HIDESELECTION;
	}

	if (fEnableAutoWordSel)
	{
		dwProperties |= TXTBIT_AUTOWORDSEL;
	}

	if (fVertical)
	{
		dwProperties |= TXTBIT_VERTICAL;
	}
					
	if (fWordWrap)
	{
		dwProperties |= TXTBIT_WORDWRAP;
	}

	if (fAllowBeep)
	{
		dwProperties |= TXTBIT_ALLOWBEEP;
	}

	if (fSaveSelection)
	{
		dwProperties |= TXTBIT_SAVESELECTION;
	}

	*pdwBits = dwProperties & dwMask; 
	return NOERROR;
}


HRESULT IUiFeatureTextHost::TxNotify(DWORD iNotify, void *pv)
{
	if( iNotify == EN_REQUESTRESIZE )
	{
		RECT rc;
		REQRESIZE *preqsz = (REQRESIZE *)pv;
		
		GetControlRect(&rc);
		rc.bottom = rc.top + preqsz->rc.bottom + HOST_BORDER;
		rc.right  = rc.left + preqsz->rc.right + HOST_BORDER;
		rc.top -= HOST_BORDER;
		rc.left -= HOST_BORDER;

		SetClientRect(&rc, TRUE);
		return S_OK;
	} 

	// Forward this to the container
	if (m_pNotifyCall != NULL)
		return m_pNotifyCall->TxNotify(iNotify, pv);

	return S_OK;
}



HRESULT IUiFeatureTextHost::TxGetExtent(LPSIZEL lpExtent)
{

	// Calculate the length & convert to himetric
	*lpExtent = sizelExtent;

	return S_OK;
}

HRESULT	IUiFeatureTextHost::TxGetSelectionBarWidth (LONG *plSelBarWidth)
{
	*plSelBarWidth = lSelBarWidth;
	return S_OK;
}


BOOL IUiFeatureTextHost::GetReadOnly()
{
	return (m_dwStyle & ES_READONLY) != 0;
}

void IUiFeatureTextHost::SetReadOnly(BOOL fReadOnly)
{
	if (fReadOnly)
	{
		m_dwStyle |= ES_READONLY;
	}
	else
	{
		m_dwStyle &= ~ES_READONLY;
	}

	// Notify control of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_READONLY, 
		fReadOnly ? TXTBIT_READONLY : 0);
}

BOOL IUiFeatureTextHost::GetAllowBeep()
{
	return fAllowBeep;
}

void IUiFeatureTextHost::SetAllowBeep(BOOL fAllowBeep)
{
	fAllowBeep = fAllowBeep;

	// Notify control of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP, 
		fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
}

void IUiFeatureTextHost::SetViewInset(RECT *prc)
{
	rcViewInset = *prc;

	// Notify control of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, 0);
}

WORD IUiFeatureTextHost::GetDefaultAlign()
{
	return pf.wAlignment;
}


void IUiFeatureTextHost::SetDefaultAlign(WORD wNewAlign)
{
	pf.wAlignment = wNewAlign;

	// Notify control of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

BOOL IUiFeatureTextHost::GetRichTextFlag()
{
	return fRich;
}

void IUiFeatureTextHost::SetRichTextFlag(BOOL fNew)
{
	fRich = fNew;

	// Notify control of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, 
		fNew ? TXTBIT_RICHTEXT : 0);
}

LONG IUiFeatureTextHost::GetDefaultLeftIndent()
{
	return pf.dxOffset;
}


void IUiFeatureTextHost::SetDefaultLeftIndent(LONG lNewIndent)
{
	pf.dxOffset = lNewIndent;

	// Notify control of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

void IUiFeatureTextHost::SetClientRect(RECT *prc, BOOL fUpdateExtent) 
{
	// If the extent matches the client rect then we assume the extent should follow
	// the client rect.
	LONG lTestExt = DYtoHimetricY(
		(rcClient.bottom - rcClient.top)  - 2 * HOST_BORDER, m_sl_yPerInch);

	if (fUpdateExtent 
		&& (sizelExtent.cy == lTestExt))
	{
		sizelExtent.cy = DXtoHimetricX((prc->bottom - prc->top) - 2 * HOST_BORDER, 
			m_sl_xPerInch);
		sizelExtent.cx = DYtoHimetricY((prc->right - prc->left) - 2 * HOST_BORDER,
			m_sl_yPerInch);
	}

	rcClient = *prc; 
}

BOOL IUiFeatureTextHost::SetSaveSelection(BOOL f_SaveSelection)
{
	BOOL fResult = f_SaveSelection;

	fSaveSelection = f_SaveSelection;

	// notify text services of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION, 
		fSaveSelection ? TXTBIT_SAVESELECTION : 0);

	return fResult;		
}

HRESULT	IUiFeatureTextHost::OnTxInPlaceDeactivate()
{
	HRESULT hr = m_pTextServ->OnTxInPlaceDeactivate();

	if (SUCCEEDED(hr))
	{
		fInplaceActive = FALSE;
	}

	return hr;
}

HRESULT	IUiFeatureTextHost::OnTxInPlaceActivate(LPCRECT prcClient)
{
	fInplaceActive = TRUE;

	HRESULT hr = m_pTextServ->OnTxInPlaceActivate(prcClient);

	if (FAILED(hr))
	{
		fInplaceActive = FALSE;
	}

	return hr;
}

BOOL IUiFeatureTextHost::DoSetCursor(RECT *prc, POINT *pt)
{
	RECT rc = prc ? *prc : rcClient;

	// Give some space for our border
	rc.top += HOST_BORDER;
	rc.bottom -= HOST_BORDER;
	rc.left += HOST_BORDER;
	rc.right -= HOST_BORDER;

	// Is this in our rectangle?
	if (PtInRect(&rc, *pt))
	{
		RECT *prcClient = (!fInplaceActive || prc) ? &rc : NULL;

		HDC hdc = GetDC(m_hWnd);

		m_pTextServ->OnTxSetCursor(
			DVASPECT_CONTENT,	
			-1,
			NULL,
			NULL,
			hdc,
			NULL,
			prcClient,
			pt->x, 
			pt->y);

		ReleaseDC(m_hWnd, hdc);

		return TRUE;
	}

	return FALSE;
}

void IUiFeatureTextHost::GetControlRect(
	LPRECT prc			//@parm	Where to put client coordinates
)
{
	// Give some space for our border
	prc->top = rcClient.top + HOST_BORDER;
	prc->bottom = rcClient.bottom - HOST_BORDER;
	prc->left = rcClient.left + HOST_BORDER;
	prc->right = rcClient.right - HOST_BORDER;
}

void IUiFeatureTextHost::SetTransparent(BOOL f_Transparent)
{
	fTransparent = f_Transparent;

	// notify text services of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
}

LONG IUiFeatureTextHost::SetAccelPos(LONG l_accelpos)
{
	LONG laccelposOld = l_accelpos;

	laccelpos = l_accelpos;

	// notify text services of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

	return laccelposOld;
}

WCHAR IUiFeatureTextHost::SetPasswordChar(WCHAR ch_PasswordChar)
{
	WCHAR chOldPasswordChar = chPasswordChar;

	chPasswordChar = ch_PasswordChar;

	// notify text services of property change
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, 
		(chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

	return chOldPasswordChar;
}

void IUiFeatureTextHost::SetDisabled(BOOL fOn)
{
	cf.dwMask	  |= CFM_COLOR	   | CFM_DISABLED;
	cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

	if( !fOn )
	{
		cf.dwEffects &= ~CFE_DISABLED;
	}
	
	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
		TXTBIT_CHARFORMATCHANGE);
}

LONG IUiFeatureTextHost::SetSelBarWidth(LONG l_SelBarWidth)
{
	LONG lOldSelBarWidth = lSelBarWidth;

	lSelBarWidth = l_SelBarWidth;

	if (lSelBarWidth)
	{
		m_dwStyle |= ES_SELECTIONBAR;
	}
	else
	{
		m_dwStyle &= (~ES_SELECTIONBAR);
	}

	m_pTextServ->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

	return lOldSelBarWidth;
}

BOOL IUiFeatureTextHost::GetTimerState()
{
	return fTimer;
}
