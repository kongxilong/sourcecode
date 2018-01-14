#include "StdAfx.h"
#include <complex>
#include <time.h>
#include <mmsystem.h>
#include "FlyWindow.h"
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"


DWORD WINAPI CFlyWindow::DoFlyThreadFunc( LPVOID lpParam )
{
	CFlyWindow* pThis = (CFlyWindow*)lpParam;
	return pThis->DoFly();
}

CFlyWindow::CFlyWindow()
{
	INIT_RECT(m_BeginRct);
	INIT_RECT(m_FlyRct);
	INIT_RECT(m_EndRct);

	m_nFrameCtns = 0;
	m_nPropFlyTime = 0;
	m_nAnimationTime = 0;

	m_hAnimateThread = NULL;

	m_pPropFlyImage = NULL;
	m_pPropFlyAlpha = NULL;

	m_Blend.BlendOp = AC_SRC_OVER;
	m_Blend.BlendFlags = 0;
	m_Blend.AlphaFormat = AC_SRC_ALPHA;
	m_Blend.SourceConstantAlpha = 255;
}

CFlyWindow::~CFlyWindow()
{
}

void CFlyWindow::OnCreate()
{
}

// 本窗口的消息处理函数
LRESULT CFlyWindow::WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	return CWin32Window::WndProc(nMsgId, wParam, lParam);
}

// 开始动画
bool CFlyWindow::StartFly(RECT rctBegin, RECT rctEnd, IPropertyImage* pPropFlyImage, IPropertyInt* pPropFlyAlpha, IPropertyInt* pPropFlyTime)
{
	if (m_hWnd != NULL || pPropFlyImage == NULL || pPropFlyAlpha == NULL || pPropFlyTime == NULL)
		return false;

	m_pPropFlyImage = pPropFlyImage;
	m_pPropFlyAlpha = pPropFlyAlpha;

	m_nPropFlyTime = pPropFlyTime->GetValue();
	if (m_nPropFlyTime <= 0)
		m_nPropFlyTime = 1000;

	RECT WndRct = {0, 0, 0, 0};
	if (!CreateWin32Window(NULL, WndRct, L"", SW_HIDE, WS_POPUP, (LPARAM)this))
		return false;

	SetWindowTransparence(true);
	HideInTaskbar();

	m_BeginRct = rctBegin;
	m_EndRct = rctEnd;

	m_hAnimateThread = ::CreateThread(NULL, 0, DoFlyThreadFunc, (LPVOID)this, 0, NULL);
	if (m_hAnimateThread == NULL)
	{
		::DestroyWindow(this->GetSafeHandle());
		return false;
	}

	return true;
}

DWORD CFlyWindow::DoFly()
{
	if (m_pPropFlyAlpha == NULL || m_pPropFlyImage == NULL)
		return -1;

	// 提高线程优先级
	HANDLE hThreadMe = ::GetCurrentThread();
	INT priority = ::GetThreadPriority(hThreadMe);
	::SetThreadPriority(hThreadMe, THREAD_PRIORITY_HIGHEST);

	//设置最小时间周期
	TIMECAPS ptc={0,0};
	MMRESULT mr = MMSYSERR_ERROR;
	mr = timeGetDevCaps(&ptc, sizeof(ptc));
	if (mr != MMSYSERR_ERROR)
		mr = timeBeginPeriod(ptc.wPeriodMin);

	StopDrawWin32Window(true);

	HDC hWndDc = ::GetDC(m_hWnd);
	if (hWndDc != NULL)
	{
		m_nFrameCtns = 0;
		clock_t begin_clock = clock();
		while (true)
		{
			m_nFrameCtns++;
			clock_t end_clock = clock();
			m_nAnimationTime = end_clock - begin_clock;

			if (m_nAnimationTime >= m_nPropFlyTime)
			{
				m_nAnimationTime = 0;
				m_FlyRct = m_EndRct;
				break;
			}

			bool bNeedDraw = SetFlyRect();
			if (bNeedDraw)
				FlyAnimationWindow(hWndDc);
			else
				Sleep(1);

			if (m_EndRct.left == m_FlyRct.left && m_EndRct.right == m_FlyRct.right &&
				m_EndRct.top == m_FlyRct.top && m_EndRct.bottom == m_FlyRct.bottom)
				break;
		}
		m_AnimationMemDc.Delete();

		::ReleaseDC(m_hWnd, hWndDc);
	}

	StopDrawWin32Window(false);

	::SetThreadPriority(hThreadMe, priority);

	//恢复最小时间周期
	if (mr != MMSYSERR_ERROR)
		timeEndPeriod(ptc.wPeriodMin);

	SAFE_CLOSE_HANDLE(m_hAnimateThread);
	this->CloseWindow();
	return 0;
}

bool CFlyWindow::SetFlyRect()
{
	RECT FlyRct = m_FlyRct;
	double dbTime = (double)m_nAnimationTime / (double)m_nPropFlyTime - 1.0;
	m_FlyRct.left = m_BeginRct.left - (int)ceil((double)(-1 * (m_BeginRct.left - m_EndRct.left)) * ((double)pow(dbTime, 6.0) - 1.0));
	m_FlyRct.right = m_BeginRct.right - (int)ceil((double)(-1 * (m_BeginRct.right - m_EndRct.right)) * ((double)pow(dbTime, 6.0) - 1.0));
	m_FlyRct.top = m_BeginRct.top - (int)ceil((double)(-1 * (m_BeginRct.top - m_EndRct.top)) * ((double)pow(dbTime, 6.0) - 1.0));
	m_FlyRct.bottom = m_BeginRct.bottom - (int)ceil((double)(-1 * (m_BeginRct.bottom - m_EndRct.bottom)) * ((double)pow(dbTime, 6.0) - 1.0));

	if (FlyRct.left == m_FlyRct.left && FlyRct.right == m_FlyRct.right &&
		FlyRct.top == m_FlyRct.top && FlyRct.bottom == m_FlyRct.bottom)
		return false;

	return true;
}

void CFlyWindow::FlyAnimationWindow(HDC hWndDc)
{
	if (hWndDc == NULL)
		return;

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, RECT_WIDTH(m_FlyRct), RECT_HEIGHT(m_FlyRct), SWP_NOMOVE);

	RECT wndRct = {0, 0, 0, 0};
	wndRct.right = RECT_WIDTH(m_FlyRct);
	wndRct.bottom = RECT_HEIGHT(m_FlyRct);

	m_AnimationMemDc.Create(RECT_WIDTH(wndRct), RECT_HEIGHT(wndRct), false, true);
	if (m_AnimationMemDc.GetSafeHdc() == NULL)
		return;

	OnPaint(m_AnimationMemDc.GetSafeHdc());

	POINT ptWinPos = {m_FlyRct.left, m_FlyRct.top};
	POINT ptSrc = {0, 0};
	SIZE sizeWindow = {RECT_WIDTH(m_FlyRct), RECT_HEIGHT(m_FlyRct)};

	::UpdateLayeredWindow(m_hWnd, hWndDc, &ptWinPos, &sizeWindow, m_AnimationMemDc.GetSafeHdc(), &ptSrc, 0, &m_Blend, ULW_ALPHA);
	if (m_nFrameCtns > 1)
		this->ShowWindow(SW_SHOW);
}

void CFlyWindow::OnPaint(HDC hDc)
{
	if (m_hWnd == NULL || hDc == NULL || m_pPropFlyImage == NULL || m_pPropFlyAlpha == NULL)
		return;

	RECT wndRct = {0, 0, 0, 0};
	::GetClientRect(m_hWnd, &wndRct);

	int nAlpha = m_pPropFlyAlpha->GetValue();

	if (nAlpha < 0)
		nAlpha = 0;

	if (nAlpha > 255)
		nAlpha = 255;

	m_pPropFlyImage->DrawImage(hDc, wndRct, nAlpha);
}
