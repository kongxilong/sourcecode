// ThreadServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ThreadServer.h"
#include "ThreadServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThreadServerApp

BEGIN_MESSAGE_MAP(CThreadServerApp, CWinApp)
	//{{AFX_MSG_MAP(CThreadServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadServerApp construction

CThreadServerApp::CThreadServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CThreadServerApp object

CThreadServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CThreadServerApp initialization

BOOL CThreadServerApp::InitInstance()
{
	AfxEnableControlContainer();


#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CThreadServerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	
	return FALSE;
}
