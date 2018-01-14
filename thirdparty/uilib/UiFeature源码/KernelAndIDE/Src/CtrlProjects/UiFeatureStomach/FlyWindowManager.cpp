#include "StdAfx.h"
#include "FlyWindowManager.h"

CFlyWindowManager::CFlyWindowManager()
{
	m_FlyWndMap.clear();
	m_pCtrl = NULL;
	m_nTimerId = 0;
}

CFlyWindowManager::~CFlyWindowManager()
{
}

void CFlyWindowManager::OnTimer(int nTimerId)
{
	if (m_nTimerId != nTimerId)
		return;

	for (FLY_WND_MAP::iterator wndItem = m_FlyWndMap.begin(); wndItem != m_FlyWndMap.end(); wndItem++)
	{
		FLY_WND& FlyWnd = wndItem->second;
		if (FlyWnd.pFlyWnd != NULL && !::IsWindow(FlyWnd.pFlyWnd->GetSafeHandle()))
		{
			SAFE_DELETE(FlyWnd.pFlyWnd);
			m_FlyWndMap.erase(wndItem);
			break;
		}
	}

	if (m_FlyWndMap.size() <= 0 && m_pCtrl != NULL && m_pCtrl->GetOwnerWindow() != NULL)
	{
		m_pCtrl->GetOwnerWindow()->KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}
}

// 开始一个动画，设置动画开始和结束的位置，返回值是动画的一个KEY
CFlyWindow* CFlyWindowManager::StartFly(ICtrlStraightLineFlyImage* pCtrl, RECT rctBegin, RECT rctEnd, IPropertyImage* pPropFlyImage, IPropertyInt* pPropFlyAlpha, IPropertyInt* pPropFlyTime)
{
	if (pPropFlyImage == NULL || pPropFlyAlpha == NULL || pCtrl == NULL || pCtrl->GetOwnerWindow() == NULL)
		return NULL;

	m_pCtrl = pCtrl;
	FLY_WND FlyWnd;
	FlyWnd.pFlyWnd = new CFlyWindow;
	if (FlyWnd.pFlyWnd == NULL)
		return NULL;

	FlyWnd.BeginRct = rctBegin;
	FlyWnd.EndRct = rctEnd;

	if (!FlyWnd.pFlyWnd->StartFly(rctBegin, rctEnd, pPropFlyImage, pPropFlyAlpha, pPropFlyTime))
	{
		SAFE_DELETE(FlyWnd.pFlyWnd);
		return NULL;
	}

	if (m_nTimerId == 0 && m_pCtrl != NULL)
		m_nTimerId = m_pCtrl->GetOwnerWindow()->SetTimer(100);

	m_FlyWndMap.insert(pair<CFlyWindow*, FLY_WND>(FlyWnd.pFlyWnd, FlyWnd));
	return FlyWnd.pFlyWnd;
}
