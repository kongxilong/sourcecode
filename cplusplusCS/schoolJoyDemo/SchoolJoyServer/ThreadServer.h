// ThreadServer.h : main header file for the THREADSERVER application
//

#if !defined(AFX_THREADSERVER_H__5622D43E_D9E2_4AAA_949A_551484907CA6__INCLUDED_)
#define AFX_THREADSERVER_H__5622D43E_D9E2_4AAA_949A_551484907CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CThreadServerApp:
// See ThreadServer.cpp for the implementation of this class
//

class CThreadServerApp : public CWinApp
{
public:
	CThreadServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CThreadServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREADSERVER_H__5622D43E_D9E2_4AAA_949A_551484907CA6__INCLUDED_)
