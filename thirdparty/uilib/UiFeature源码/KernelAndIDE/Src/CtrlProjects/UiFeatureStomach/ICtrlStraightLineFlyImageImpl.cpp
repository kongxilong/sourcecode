
#include "StdAfx.h"
#include "ICtrlStraightLineFlyImageImpl.h"
#include "..\..\Inc\IControlManager.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"
#include <complex>

ICtrlStraightLineFlyImage::ICtrlStraightLineFlyImage(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlStraightLineFlyImageImpl::ICtrlStraightLineFlyImageImpl(IUiFeatureKernel *pUiKernel) : ICtrlStraightLineFlyImage(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_STRAIGHTLINE_FLY_IMAGE);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_pPropFlyImage = NULL;
	m_pPropFlyAlpha = NULL;

	m_pPropBeginRctLeft = NULL;
	m_pPropBeginRctTop = NULL;
	m_pPropBeginWidth = NULL;
	m_pPropBeginHeight = NULL;

	m_pPropEndRctLeft = NULL;
	m_pPropEndRctTop = NULL;
	m_pPropEndWidth = NULL;
	m_pPropEndHeight = NULL;
}

ICtrlStraightLineFlyImageImpl::~ICtrlStraightLineFlyImageImpl(void)
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlStraightLineFlyImageImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropFlyImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "FlyImage", "动画过程用于移动的图片");
	m_pPropFlyAlpha = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "FlyAlpha", "动画移动时候的Alpha值");
	m_pPropFlyTime = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "FlyTime", "动画移动时间，单位毫秒");

	IPropertyGroup* pGroupBegin = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "BeginPos", "开始动画的位置");
	m_pPropBeginRctLeft = (IPropertyInt*)CreatePropetry(pGroupBegin, OTID_INT, "Left", "开始动画的位置：Left坐标");
	m_pPropBeginRctTop = (IPropertyInt*)CreatePropetry(pGroupBegin, OTID_INT, "Top", "开始动画的位置：Top坐标");
	m_pPropBeginWidth = (IPropertyInt*)CreatePropetry(pGroupBegin, OTID_INT, "Width", "动画图片开始时的宽度");
	m_pPropBeginHeight = (IPropertyInt*)CreatePropetry(pGroupBegin, OTID_INT, "Height", "动画图片开始时的高度");

	IPropertyGroup* pGroupEnd = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "EndPos", "动画结束的位置");
	m_pPropEndRctLeft = (IPropertyInt*)CreatePropetry(pGroupEnd, OTID_INT, "Left", "结束动画的位置：Left坐标");
	m_pPropEndRctTop = (IPropertyInt*)CreatePropetry(pGroupEnd, OTID_INT, "Top", "结束动画的位置：Top坐标");
	m_pPropEndWidth = (IPropertyInt*)CreatePropetry(pGroupEnd, OTID_INT, "Width", "动画图片结束时的宽度");
	m_pPropEndHeight = (IPropertyInt*)CreatePropetry(pGroupEnd, OTID_INT, "Height", "动画图片结束时的高度");

	if (bIsNewCtrl)
	{
		// 设置默认不接受鼠标信息
		m_pPropBase_RcvMouseMsg->SetValue(false);
	}

	return true;
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlStraightLineFlyImageImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 初始化控件
void ICtrlStraightLineFlyImageImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlStraightLineFlyImageImpl::OnFinalCreate()
{
}

// 绘制控件
void ICtrlStraightLineFlyImageImpl::OnPaint(CDrawingBoard &DrawBoard)
{
}

// 销毁控件
void ICtrlStraightLineFlyImageImpl::OnDestroy()
{
}

// 定时器
void ICtrlStraightLineFlyImageImpl::OnTimer(UINT nTimerId)
{
	m_FlyWindowMgr.OnTimer(nTimerId);
}

// 开始一个动画，设置动画开始和结束的位置，返回值是动画的一个KEY
VOID* ICtrlStraightLineFlyImageImpl::StartFly(RECT rctBegin, RECT rctEnd)
{
	if (m_pPropBeginRctLeft == NULL || m_pPropBeginRctTop == NULL || m_pPropBeginWidth == NULL || m_pPropBeginHeight == NULL ||
		m_pPropEndRctLeft == NULL || m_pPropEndRctTop == NULL || m_pPropEndWidth == NULL || m_pPropEndHeight == NULL)
		return NULL;

	m_pPropBeginRctLeft->SetValue(rctBegin.left);
	m_pPropBeginRctTop->SetValue(rctBegin.top);
	m_pPropBeginWidth->SetValue(RECT_WIDTH(rctBegin));
	m_pPropBeginHeight->SetValue(RECT_HEIGHT(rctBegin));

	m_pPropEndRctLeft->SetValue(rctEnd.left);
	m_pPropEndRctTop->SetValue(rctEnd.top);
	m_pPropEndWidth->SetValue(RECT_WIDTH(rctEnd));
	m_pPropEndHeight->SetValue(RECT_HEIGHT(rctEnd));

	return m_FlyWindowMgr.StartFly(dynamic_cast<ICtrlStraightLineFlyImage*>(this), rctBegin, rctEnd, m_pPropFlyImage, m_pPropFlyAlpha, m_pPropFlyTime);
}

void ICtrlStraightLineFlyImageImpl::GetFlyPostion(RECT &rctBegin, RECT &rctEnd)
{
	INIT_RECT(rctBegin);
	INIT_RECT(rctEnd);

	if (m_pPropBeginRctLeft == NULL || m_pPropBeginRctTop == NULL || m_pPropBeginWidth == NULL || m_pPropBeginHeight == NULL ||
		m_pPropEndRctLeft == NULL || m_pPropEndRctTop == NULL || m_pPropEndWidth == NULL || m_pPropEndHeight == NULL)
		return;

	rctBegin.left = m_pPropBeginRctLeft->GetValue();
	rctBegin.top = m_pPropBeginRctTop->GetValue();
	rctBegin.right = rctBegin.left +  m_pPropBeginWidth->GetValue();
	rctBegin.bottom = rctBegin.top + m_pPropBeginHeight->GetValue();

	rctEnd.left = m_pPropEndRctLeft->GetValue();
	rctEnd.top = m_pPropEndRctTop->GetValue();
	rctEnd.right = rctEnd.left +  m_pPropEndWidth->GetValue();
	rctEnd.bottom = rctEnd.top + m_pPropEndHeight->GetValue();
}
