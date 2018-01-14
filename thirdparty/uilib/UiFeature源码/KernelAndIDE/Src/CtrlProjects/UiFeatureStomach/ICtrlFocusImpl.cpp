
#include "StdAfx.h"
#include "ICtrlFocusImpl.h"
#include <complex>
#include <time.h>
#include <mmsystem.h>

ICtrlFocus::ICtrlFocus(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlFocusImpl::ICtrlFocusImpl(IUiFeatureKernel *pUiKernel) : ICtrlFocus(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_FOCUS_CTRL);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	INIT_RECT(m_BeginRct);
	INIT_RECT(m_EndRct);
	INIT_RECT(m_MoveRct);

	m_pPropFrameSpace = 0;
	m_pFocusCtrl = NULL;
	m_pPropFrameImage = NULL;
	m_pPropUseAnimation = NULL;

	m_bIsInAnimation = false;
}

ICtrlFocusImpl::~ICtrlFocusImpl()
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlFocusImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropFrameSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "FrameSpace", "绘制焦点控件的边框距离");
	m_pPropFrameImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "FrameImage", "边框图片");
	m_pPropUseAnimation = (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "UseAnimation", "是否使用移动动画");
	m_pPropAnimationTime = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "AnimationTime", "移动动画的总时间，单位：毫秒，有效值：20-1000毫秒");

	return true;
}

bool ICtrlFocusImpl::IsUseAnimation()
{
	if (m_pPropUseAnimation == NULL)
		return false;

	return m_pPropUseAnimation->GetValue();
}

// 初始化控件
void ICtrlFocusImpl::OnCreate()
{
	if (m_pWindowBase != NULL)
		m_pWindowBase->RegisterFocusControl(dynamic_cast<IControlBase*>(this));
}

// 控件初始化完毕
void ICtrlFocusImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlFocusImpl::OnDestroy()
{
	if (m_pWindowBase != NULL)
		m_pWindowBase->RegisterFocusControl(NULL);
}

// 绘制控件
void ICtrlFocusImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (m_pFocusCtrl == NULL || m_pPropFrameImage == NULL || m_bIsInAnimation)
		return;

	m_pPropFrameImage->DrawImage(DrawBoard, this->GetClientRect());
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlFocusImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 当一个控件的TabOrder属性被设置了之后，同时此控件得到焦点，窗口会将此消息
// 通过这个函数发送给注册了需要取得焦点消息的控件
void ICtrlFocusImpl::OnContrlFocusChange(IControlBase* pFocusCtrl)
{
	if (m_pFocusCtrl == pFocusCtrl || m_pWindowBase == NULL || m_bIsInAnimation)
		return;

	IControlBase* pOldFocusCtrl = m_pFocusCtrl;

	m_pFocusCtrl = pFocusCtrl;
	if (m_pFocusCtrl == NULL)
	{
		this->RedrawControl(true);
		return;
	}

	m_BeginRct = this->GetWindowRect();

	CONTROL_LAYOUT_INFO layoutInfo = m_pFocusCtrl->GetLayout();
	this->SetControlLayoutType(layoutInfo.clType);

	RECT ctrlRct = m_pFocusCtrl->GetWindowRect();
	int nSpace = 0;
	if (m_pPropFrameSpace != NULL)
		nSpace = m_pPropFrameSpace->GetValue();

	ctrlRct.left -= nSpace;
	ctrlRct.right += nSpace;
	ctrlRct.top -= nSpace;
	ctrlRct.bottom += nSpace;
	this->SetWindowRect(ctrlRct, true);

	m_EndRct = this->GetWindowRect();

	if (IsUseAnimation() && pOldFocusCtrl != NULL)
	{
//		// 控件自定义强动画
//		this->PP_SetCtrlAnimationType(1);
//		// 开始动画
//		m_pWindowBase->AppendAnimationControl(dynamic_cast<IControlBase*>(this));
//		m_pWindowBase->DoControlAnimation();

		OnMoveAnimationDraw();
	}
	else
	{
		this->RedrawControl(true);
	}
}

// 用户自定义强动画绘制接口，WndMemDc：动画使用的内存DC，动画结束后会被自动清空；hWndDc：控件所在的窗口DC
void ICtrlFocusImpl::OnMoveAnimationDraw()
{
	if (m_pWindowBase == NULL || m_pPropAnimationTime == NULL || m_pUiKernel == NULL)
		return;

	m_bIsInAnimation = true;
//////////////////////////////////////////////////////////////////////////
	// 1. 取得绘制用的图片
	HDC hWndDc = ::GetDC(this->GetOwnerWindowHwnd());

	CDrawingImage WndMemDc;
	m_pWindowBase->PrintWindow(WndMemDc);

	// 2. 循环绘制动画
	int nPropMoveTime = m_pPropAnimationTime->GetValue();
	if (nPropMoveTime < 20 || nPropMoveTime > 1000)
		nPropMoveTime = 100;

	BLENDFUNCTION TempBlend;
	TempBlend.BlendOp = AC_SRC_OVER;
	TempBlend.BlendFlags = 0;
	TempBlend.AlphaFormat = AC_SRC_ALPHA;
	TempBlend.SourceConstantAlpha = 255;

	clock_t begin_clock = clock();
	while (true)
	{
		clock_t end_clock = clock();
		int nAnimationTime = end_clock - begin_clock;

		// 动画时间到了，退出
		if (nAnimationTime >= nPropMoveTime)
			break;

		bool bNeedDraw = SetMoveRect(nAnimationTime, nPropMoveTime);
		if (bNeedDraw)
		{
			DoAnimation(WndMemDc, hWndDc, TempBlend);
		}
		else
		{
			Sleep(1);
		}

		if (m_EndRct.left == m_MoveRct.left && m_EndRct.right == m_MoveRct.right &&
			m_EndRct.top == m_MoveRct.top && m_EndRct.bottom == m_MoveRct.bottom)
			break;
	}
//////////////////////////////////////////////////////////////////////////
	m_bIsInAnimation = false;
	m_pWindowBase->RedrawWindowInNextMessage();
}

void ICtrlFocusImpl::DoAnimation(CDrawingImage &WndMemDc, HDC &hWndDc, BLENDFUNCTION &Blend)
{
	if (hWndDc == NULL || m_pWindowBase == NULL || m_pPropFrameImage == NULL)
		return;

	CDrawingImage AnimationTempMemDc;
	AnimationTempMemDc.Clone(WndMemDc, m_pUiKernel);
	m_pPropFrameImage->DrawImage(AnimationTempMemDc, m_MoveRct);

	RECT CrurentWndRct = m_pWindowBase->GetWindowRect();

	if (m_pWindowBase->PP_GetLayeredWindow())
	{
		RECT wndRct = {0, 0, 0, 0};
		wndRct.right = RECT_WIDTH(CrurentWndRct);
		wndRct.bottom = RECT_HEIGHT(CrurentWndRct);

		POINT ptWinPos = {CrurentWndRct.left, CrurentWndRct.top};
		POINT ptSrc = {0, 0};
		SIZE sizeWindow = {RECT_WIDTH(CrurentWndRct), RECT_HEIGHT(CrurentWndRct)};

		::UpdateLayeredWindow(this->GetOwnerWindowHwnd(), hWndDc, &ptWinPos, &sizeWindow, AnimationTempMemDc.GetSafeHdc(), &ptSrc, 0, &Blend, ULW_ALPHA);
	}
	else
	{
		::BitBlt(hWndDc, 0, 0, RECT_WIDTH(CrurentWndRct), RECT_HEIGHT(CrurentWndRct), AnimationTempMemDc.GetSafeHdc(), 0, 0, SRCCOPY);
	}
}

bool ICtrlFocusImpl::SetMoveRect(int nAnimationTime, int nPropMoveTime)
{
	RECT MoveRct = m_MoveRct;
	double dbTime = (double)nAnimationTime / (double)nPropMoveTime - 1.0;
	m_MoveRct.left = m_BeginRct.left - (int)ceil((double)(-1 * (m_BeginRct.left - m_EndRct.left)) * ((double)pow(dbTime, 6.0) - 1.0));
	m_MoveRct.right = m_BeginRct.right - (int)ceil((double)(-1 * (m_BeginRct.right - m_EndRct.right)) * ((double)pow(dbTime, 6.0) - 1.0));
	m_MoveRct.top = m_BeginRct.top - (int)ceil((double)(-1 * (m_BeginRct.top - m_EndRct.top)) * ((double)pow(dbTime, 6.0) - 1.0));
	m_MoveRct.bottom = m_BeginRct.bottom - (int)ceil((double)(-1 * (m_BeginRct.bottom - m_EndRct.bottom)) * ((double)pow(dbTime, 6.0) - 1.0));

	if (MoveRct.left == m_MoveRct.left && MoveRct.right == m_MoveRct.right &&
		MoveRct.top == m_MoveRct.top && MoveRct.bottom == m_MoveRct.bottom)
		return false;

	return true;
}
