// UiFeatureRun.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UiFeatureTestWindow.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwMainThreadId = ::GetCurrentThreadId();

	CUiFeatureTestWindow RunWindow;
	if (!RunWindow.ShowUiFeatureTestWindow(dwMainThreadId))
	{
		MessageBox(NULL, _T("创建测试窗口错误！"), _T("预览窗口"), MB_OK | MB_ICONERROR);
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

	// 退出程序，释放相关资源
	RunWindow.UnInitialized();

	return 0;
}
