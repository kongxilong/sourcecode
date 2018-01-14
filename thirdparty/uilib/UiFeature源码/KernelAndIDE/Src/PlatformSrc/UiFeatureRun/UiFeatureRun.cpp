// UiFeatureRun.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UiFeatureRun.h"
#include "FeatureRunWindow.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (lpCmdLine == NULL || wcslen(lpCmdLine) <= 0)
	{
		MessageBox(NULL, _T("ÊäÈë²ÎÊý´íÎó£¡"), _T("Ô¤ÀÀ´°¿Ú"), MB_OK | MB_ICONERROR);
		return -1;
	}

	DWORD dwMainThreadId = ::GetCurrentThreadId();

	CFeatureRunWindow RunWindow;
	if (!RunWindow.ShowFeatureRunWindow(lpCmdLine, dwMainThreadId))
	{
		MessageBox(NULL, _T("´´½¨Ô¤ÀÀ´°¿Ú´íÎó£¡"), _T("Ô¤ÀÀ´°¿Ú"), MB_OK | MB_ICONERROR);
		return -1;
	}

	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		if (!::TranslateAccelerator(msg.hwnd, NULL, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	RunWindow.UnInitialized();
	return 0;
}
