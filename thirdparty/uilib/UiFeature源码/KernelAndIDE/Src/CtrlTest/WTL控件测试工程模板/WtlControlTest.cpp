// CWTLWindow.cpp : main source file for CWTLWindow.exe
//

#include "stdafx.h"
#include "WtlTestWindow.h"

CAppModule _Module;

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

//////////////////////////////////////////////////////////////////////////
	DWORD dwMainThreadId = ::GetCurrentThreadId();

	CWtlTestWindow RunWindow;
	if (!RunWindow.ShowWtlTestWindow(&_Module, dwMainThreadId))
	{
		MessageBox(NULL, _T("´´½¨WTL²âÊÔ´°¿Ú´íÎó£¡"), _T("Ô¤ÀÀ´°¿Ú"), MB_OK | MB_ICONERROR);
		return -1;
	}
//////////////////////////////////////////////////////////////////////////

	int nRet = theLoop.Run();
	_Module.RemoveMessageLoop();

//////////////////////////////////////////////////////////////////////////
	RunWindow.UnInitialized();
//////////////////////////////////////////////////////////////////////////

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
