#pragma once

#include "FlyWindow.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\Control\ICtrlStraightLineFlyImage.h"
#include <map>
using namespace std;

struct FLY_WND
{
	CFlyWindow* pFlyWnd;
	RECT BeginRct;
	RECT EndRct;

	FLY_WND()
	{
		pFlyWnd = NULL;
		INIT_RECT(BeginRct);
		INIT_RECT(EndRct);
	}
};

typedef map<CFlyWindow*, FLY_WND>	FLY_WND_MAP;

class CFlyWindowManager
{
public:
	CFlyWindowManager();
	virtual ~CFlyWindowManager();

	// 开始一个动画，设置动画开始和结束的位置，返回值是动画的一个KEY
	CFlyWindow* StartFly(ICtrlStraightLineFlyImage* pCtrl, RECT rctBegin, RECT rctEnd,
		IPropertyImage* pPropFlyImage, IPropertyInt* pPropFlyAlpha, IPropertyInt* pPropFlyTime);
	void OnTimer(int nTimerId);

private:
	ICtrlStraightLineFlyImage* m_pCtrl;
	FLY_WND_MAP m_FlyWndMap;
	int m_nTimerId;
};
