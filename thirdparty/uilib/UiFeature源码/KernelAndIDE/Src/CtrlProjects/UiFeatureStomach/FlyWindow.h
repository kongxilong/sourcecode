#pragma once
#include "..\..\Inc\CUiFeatureWindow.h"
#include "..\..\Inc\IPropertyImage.h"
#include "..\..\Inc\IPropertyInt.h"

class CFlyWindow : public CWin32Window
{
public:
	CFlyWindow();
	virtual ~CFlyWindow();

	// 开始动画
	bool StartFly(RECT rctBegin, RECT rctEnd, IPropertyImage* pPropFlyImage, IPropertyInt* pPropFlyAlpha, IPropertyInt* pPropFlyTime);
	//线程动画函数
	static DWORD WINAPI DoFlyThreadFunc(LPVOID lpParam);

protected:
	// 本窗口的消息处理函数
	virtual LRESULT WndProc(UINT nMsgId, WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDc);
	virtual void OnCreate();
	DWORD DoFly();
	void FlyAnimationWindow(HDC hWndDc);
	bool SetFlyRect();

private:
	int m_nFrameCtns;
	RECT m_BeginRct;
	RECT m_FlyRct;
	RECT m_EndRct;
	CDrawingBoard m_AnimationMemDc;
	IPropertyImage* m_pPropFlyImage;
	IPropertyInt* m_pPropFlyAlpha;

	HANDLE m_hAnimateThread;
	int m_nAnimationTime;
	int m_nPropFlyTime;
	BLENDFUNCTION m_Blend;
};
