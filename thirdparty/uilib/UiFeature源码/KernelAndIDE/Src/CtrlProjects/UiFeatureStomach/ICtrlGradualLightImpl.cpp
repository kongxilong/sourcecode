
#include "StdAfx.h"
#include "ICtrlGradualLightImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"

ICtrlGradualLight::ICtrlGradualLight(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlGradualLightImpl::ICtrlGradualLightImpl(IUiFeatureKernel *pUiKernel) : ICtrlGradualLight(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_GRADUAL_LIGHT);

	m_bHaveSysTipsProp = false;

	m_pPropImage = NULL;
	m_pPropPlay = NULL;

	m_nDrawAlpha = 0;
	m_nChangeAlpha = 20;
}

ICtrlGradualLightImpl::~ICtrlGradualLightImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlGradualLightImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlGradualLightImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "BreathLightImage", "呼吸灯图片");
	m_pPropPlay = (IPropertyBool*)CreatePropetry(NULL, OTID_BOOL, "Play", "是否播放呼吸灯");

	return true;
}

// 初始化控件
void ICtrlGradualLightImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlGradualLightImpl::OnFinalCreate()
{
}

// 销毁控件
void ICtrlGradualLightImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlGradualLightImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (m_pPropImage == NULL)
		return;

	if (IsNeedPlay())
		m_pPropImage->DrawImage(DrawBoard, GetClientRect(), m_nDrawAlpha);
}

bool ICtrlGradualLightImpl::OnDrawAnimation()
{
	if (!this->IsVisible() && !IsNeedPlay())
		return false;

	// 渐隐渐现效果
	int nOldAlpha = m_nDrawAlpha;

	if (m_nDrawAlpha >= 255)
		m_nChangeAlpha = -20;

	if (m_nDrawAlpha <= 0)
		m_nChangeAlpha = 20;

	m_nDrawAlpha += m_nChangeAlpha;

	if (m_nDrawAlpha < 0)
		m_nDrawAlpha = 0;

	if (m_nDrawAlpha > 255)
		m_nDrawAlpha = 255;

	return (nOldAlpha != m_nDrawAlpha);
}

void ICtrlGradualLightImpl::Play()
{
	if (m_pPropPlay == NULL)
		return;

	if (m_pWindowBase != NULL)
		m_pWindowBase->SetAnimationTimer();

	m_nDrawAlpha = 0;
	m_nChangeAlpha = 20;

	m_pPropPlay->SetValue(true);
	this->RedrawControl(true);
}

void ICtrlGradualLightImpl::Stop(bool bIsKillTimer)
{
	if (m_pPropPlay == NULL)
		return;

	m_pPropPlay->SetValue(false);
	this->RedrawControl(true);

	if (bIsKillTimer && m_pWindowBase != NULL)
		m_pWindowBase->KillTimer(UM_DFT_ANIMATION_TIMER);
}

bool ICtrlGradualLightImpl::IsNeedPlay()
{
	if (m_pPropPlay == NULL)
		return false;

	return m_pPropPlay->GetValue();
}
