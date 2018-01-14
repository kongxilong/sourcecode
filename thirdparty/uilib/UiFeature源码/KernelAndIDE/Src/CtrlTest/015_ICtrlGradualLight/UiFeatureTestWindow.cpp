#include "StdAfx.h"
#include "UiFeatureTestWindow.h"
#include <assert.h>
#include "..\..\Inc\ICommonFun.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include <Shellapi.h>
#include <windows.h>

CUiFeatureTestWindow::CUiFeatureTestWindow()
{
	m_dwMainThreadId = 0;
	m_pBeginFlyBtn = NULL;
	m_pFlyImage = NULL;
	m_pGradualLight = NULL;
	m_pRoolAdver = NULL;
}

CUiFeatureTestWindow::~CUiFeatureTestWindow()
{
}

bool CUiFeatureTestWindow::ShowUiFeatureTestWindow(DWORD dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;

	// 取得UI内核DLL的路径
	CStringW strKernelDllPath = PathHelperW(_T(NAME_KERNEL_DLL));
	// 取得皮肤包的路径
	CStringW strSkinPath = PathHelperW(_T("..\\SkinPackage\\015_ICtrlGradualLight\\015_ICtrlGradualLight.ufd"));

	CRect WndRct(0, 0, 0, 0);
	// 根据皮肤包创建窗口，如果窗口大小为0，则会根据皮肤包中的大小自动设置窗口的大小。
	// 第一个参数为皮肤包密码
	return CreateFeatureWindow(NULL, strKernelDllPath, strSkinPath, L"TestWindow", NULL, WndRct, SW_SHOW, WS_OVERLAPPED | WS_THICKFRAME, NULL);
}

void CUiFeatureTestWindow::OnKeyDown(int nVirtKey, int nFlag)
{
	// 测试程序按下【Esc】就退出程序
	if (nVirtKey == VK_ESCAPE)
	{
		this->CloseWindow();
		::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	}
}

// WM_CREATE：对话框初始化	
void CUiFeatureTestWindow::OnCreate()
{
	GET_CONTROL_EX(m_pBeginFlyBtn, ICtrlShadowButton, "ToFly");
	GET_CONTROL_EX(m_pFlyImage, ICtrlStraightLineFlyImage, "FlyImg");
	GET_CONTROL_EX(m_pGradualLight, ICtrlGradualLight, "BLight");
	GET_CONTROL_EX(m_pRoolAdver, ICtrlRollAdvertisement, "RollAdver");
	if (m_pBeginFlyBtn == NULL || m_pFlyImage == NULL || m_pGradualLight == NULL || m_pRoolAdver == NULL)
	{
		assert(false);
		return;
	}

	// 播放呼吸灯效果
	m_pGradualLight->Play();

	// 设置滚动信息
	ROLL_ADVERTISEMENT_ITEM adver;
	adver.nId = adver.nConfActive = 1;
	adver.pStrData = L"http://fangshunbao.blog.sohu.com/";
	adver.pStrShowInfo = L"UiFeature的博客，欢迎光临！";
	m_pRoolAdver->AddAdvertisement(adver);

	adver.nId = adver.nConfActive = 2;
	adver.pStrData = L"http://zhuomian.360.cn/";
	adver.pStrShowInfo = L"下载一个360安全桌面研究一下，进入UiFeature的世界！";
	m_pRoolAdver->AddAdvertisement(adver);

	adver.nId = adver.nConfActive = 2;
	adver.pStrData = L"http://zhuomian.360.cn/";
	adver.pStrShowInfo = L"我们再来下载一个【360游戏盒子】研究一下，进入UiFeature的世界！";
	m_pRoolAdver->AddAdvertisement(adver);
}

void CUiFeatureTestWindow::UnInitialized()
{
	if (m_pUiKernel != NULL)
		m_pUiKernel->UnInitialized();
}

LRESULT CUiFeatureTestWindow::OnSysCommandClose(WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(m_dwMainThreadId, WM_QUIT, 0, 0);
	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnBeginFly(WPARAM wParam, LPARAM lParam)
{
	// 点击按钮，开始飞翔动画
	if (m_pFlyImage == NULL)
		return S_FALSE;

	RECT beginRct = {0}, endRct = {0};
	m_pFlyImage->GetFlyPostion(beginRct, endRct);
	m_pFlyImage->StartFly(beginRct, endRct);

	return S_OK;
}

LRESULT CUiFeatureTestWindow::OnClickAdrertisement(WPARAM wParam, LPARAM lParam)
{
	ROLL_ADVERTISEMENT_ITEM *pAdvertisementeInfo = (ROLL_ADVERTISEMENT_ITEM *)wParam;
	if (pAdvertisementeInfo == NULL || pAdvertisementeInfo->pStrData == NULL || wcslen(pAdvertisementeInfo->pStrData) <= 0)
		return S_FALSE;

	::ShellExecuteW(NULL, NULL, pAdvertisementeInfo->pStrData, NULL, NULL, SW_SHOWNORMAL);
	return S_OK;
}
