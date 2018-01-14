
#include "StdAfx.h"
#include "..\..\Inc\IControlBase.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IControlBase.h"
#include "..\..\Inc\IWindowBase.h"
#include "..\..\Inc\IUiEngine.h"
#include "..\..\Inc\IPropertySkinManager.h"

// 绘制控件到指定的DC上
void IControlBase::PrintTo(CDrawingBoard &DstBoard, RECT ToRct)
{
	if (DstBoard.GetSafeHdc() == NULL || RECT_WIDTH(ToRct) <= 0 || RECT_HEIGHT(ToRct) <= 0 || m_pUiEngine == NULL || PP_GetNoDrawControl())
		return;

	if (RECT_WIDTH(m_RectInWindow) <= 0 || RECT_HEIGHT(m_RectInWindow) <= 0)
		return;

	CDrawingImage CtrlMemDc;
	CtrlMemDc.Create(RECT_WIDTH(m_RectInWindow), RECT_HEIGHT(m_RectInWindow), false, true);
	if (CtrlMemDc.GetBits() == NULL)
		return;

	OnPaint(CtrlMemDc);

	if (PP_GetControlAlphaBlendDraw())
	{
		m_pUiEngine->AlphaBlend(DstBoard, ToRct.left, ToRct.top, RECT_WIDTH(ToRct), RECT_HEIGHT(ToRct),
			CtrlMemDc, 0, 0, RECT_WIDTH(m_RectInWindow), RECT_HEIGHT(m_RectInWindow), PP_GetControlDrawAlpha());
	}
	else
	{
		m_pUiEngine->BitBlt(DstBoard.GetSafeHdc(), ToRct.left, ToRct.top, RECT_WIDTH(ToRct), RECT_HEIGHT(ToRct),
			CtrlMemDc.GetSafeHdc(), 0, 0, SRCCOPY);
	}
}

// 绘制当前控件，参数为父窗口/父控件的内存DC
void IControlBase::OnPaintControl(CDrawingBoard &WndMemDc, RECT ActiveDrawRct)
{
	if (WndMemDc.GetSafeHdc() == NULL || !IsVisible() || m_pUiEngine == NULL || GetOwnerWindow() == NULL || RECT_WIDTH(ActiveDrawRct) <= 0 || RECT_HEIGHT(ActiveDrawRct) <= 0)
		return;

	// 超出父窗口不再绘制
	if ((m_RectInWindow.right <= ActiveDrawRct.left)
	|| (m_RectInWindow.bottom <= ActiveDrawRct.top)
	|| (m_RectInWindow.left >= ActiveDrawRct.right)
	|| (m_RectInWindow.top >= ActiveDrawRct.bottom))
		return;

	// 计算有效绘制区域
	RECT DrawRct;
	INIT_RECT(DrawRct);
	DrawRct.left = (m_RectInWindow.left >= ActiveDrawRct.left) ? m_RectInWindow.left : ActiveDrawRct.left;
	DrawRct.right = (m_RectInWindow.right >= ActiveDrawRct.right) ? ActiveDrawRct.right : m_RectInWindow.right;
	DrawRct.top = (m_RectInWindow.top >= ActiveDrawRct.top) ? m_RectInWindow.top : ActiveDrawRct.top;
	DrawRct.bottom = (m_RectInWindow.bottom >= ActiveDrawRct.bottom) ? ActiveDrawRct.bottom : m_RectInWindow.bottom;

	// 绘制到窗口的DC上
	int nWidth = RECT_WIDTH(DrawRct);
	int nHeight = RECT_HEIGHT(DrawRct);

	if (m_bIsCtrlAnimationBegin)
	{
		// 绘制控件动画，如果为无绘制控件，不进行绘制操作
		// 这个绘制只针对内置动画，不针对用户自定义强动画
		// 用户自定义强动画都在 OnUserDefHighAnimationDraw 函数进行绘制
		if (!PP_GetNoDrawControl() && m_pCtrlAnimationMemDc != NULL)
		{
			m_pUiEngine->AlphaBlend(WndMemDc, DrawRct.left, DrawRct.top, nWidth, nHeight,
				*m_pCtrlAnimationMemDc, DrawRct.left - m_RectInWindow.left, DrawRct.top - m_RectInWindow.top, nWidth, nHeight);
		}
	}
	else
	{
		// 如果为无绘制控件，不进行绘制操作
		if (!PP_GetNoDrawControl())
		{
			if (m_pCtrlMemDc == NULL)
				m_pCtrlMemDc = new CDrawingImage;

			if (m_pCtrlMemDc != NULL)
			{
				// 正常绘制控件
				bool bIsCreateNew = m_pCtrlMemDc->Create(RECT_WIDTH(m_RectInWindow), RECT_HEIGHT(m_RectInWindow), false, m_bNeedRedraw);
				if (m_pCtrlMemDc->GetBits() == NULL)
					return;

				if (bIsCreateNew)
					m_bNeedRedraw = true;

				// 重绘控件自身
				if (m_bNeedRedraw || PP_IsUseTempDrawMem())
				{
					OnPaint(*m_pCtrlMemDc);
					m_bNeedRedraw = false;
				}

				if (PP_GetControlAlphaBlendDraw())
				{
					m_pUiEngine->AlphaBlend(WndMemDc, DrawRct.left, DrawRct.top, nWidth, nHeight,
						*m_pCtrlMemDc, DrawRct.left - m_RectInWindow.left, DrawRct.top - m_RectInWindow.top, nWidth, nHeight, PP_GetControlDrawAlpha());
				}
				else
				{
					m_pUiEngine->BitBlt(WndMemDc.GetSafeHdc(), DrawRct.left, DrawRct.top, nWidth, nHeight,
						m_pCtrlMemDc->GetSafeHdc(), DrawRct.left - m_RectInWindow.left, DrawRct.top - m_RectInWindow.top, SRCCOPY); // CAPTUREBLT | SRCCOPY
				}
			}
		}

		// 绘制子控件
		ListObj* pNext = NULL;
		for (ListObj* pItem = m_ChildCtrlsVec.TopObj(); pItem != NULL; pItem = pNext)
		{
			pNext = m_ChildCtrlsVec.NextObj(pItem);
			IControlBase* pCtrl = (IControlBase *)pItem;
			if (pCtrl != NULL)
				pCtrl->OnPaintControl(WndMemDc, DrawRct);
		}
	}

	// 使用零时内存或者无绘制控件，需要清空内存
	if (PP_IsUseTempDrawMem() || PP_GetNoDrawControl())
	{
		if (m_pCtrlMemDc != NULL)
			m_pCtrlMemDc->Delete();
	}
}

// 向内核注册一个想要取到的消息
void IControlBase::RegisterControlMessage(int nMsgId)
{
	if (m_pWindowBase == NULL)
		return;

	m_pWindowBase->RegisterControlMessage(this, nMsgId);
}

void IControlBase::UnRegisterControlMessage(int nMsgId)
{
	if (m_pWindowBase == NULL)
		return;

	m_pWindowBase->UnRegisterControlMessage(this, nMsgId);
}

// 向内核注册一个想要取到的消息：同步处理方式，控件请不要再自己注册了的消息的处理过程中执行同步处理
void IControlBase::RegisterControlMessageSync(int nMsgId)
{
	if (m_pWindowBase == NULL)
		return;

	m_pWindowBase->RegisterControlMessageSync(this, nMsgId);
}

void IControlBase::UnRegisterControlMessageSync(int nMsgId)
{
	if (m_pWindowBase == NULL)
		return;

	m_pWindowBase->UnRegisterControlMessageSync(this, nMsgId);
}

// 发送消息:Send方式
LRESULT IControlBase::SendMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL)
		return -1;

	return m_pWindowBase->SendMessage(nMsgId, wParam, lParam);
}

// 发送消息:Post方式
bool IControlBase::PostMessage(UINT nMsgId, WPARAM wParam, LPARAM lParam)
{
	if (m_pWindowBase == NULL)
		return false;

	return m_pWindowBase->PostMessage(nMsgId, wParam, lParam);
}

// 将当前控件，包括子控件，都打印到一个内存DC上
bool IControlBase::PrintToCoverChild(CDrawingBoard &DstBoard, bool bPrintWhenInvisible)
{
	if (m_pUiEngine == NULL || m_pWindowBase == NULL || m_pWindowBase->GetUiKernel() == NULL)
		return false;

	RECT CtrlRct = this->GetClientRect();
	if (RECT_WIDTH(CtrlRct) <= 0 || RECT_HEIGHT(CtrlRct) <= 0)
		return false;

	CDrawingBoard PrintMemDc;
	PrintMemDc.Create(RECT_WIDTH(CtrlRct), RECT_HEIGHT(CtrlRct), false, true);
	if (PrintMemDc.GetSafeHdc() == NULL)
		return false;

	bool bHaveDraw = false;
	if (this->IsVisible())
	{
		if (!PP_GetNoDrawControl())
		{
			bHaveDraw = true;
			OnPaint(PrintMemDc);
		}
	}
	else
	{
		if (bPrintWhenInvisible)
		{
			if (!PP_GetNoDrawControl())
			{
				bHaveDraw = true;
				OnPaint(PrintMemDc);
			}
		}
		else
		{
			return true;
		}
	}

	DstBoard.Create(RECT_WIDTH(CtrlRct), RECT_HEIGHT(CtrlRct), false, true);
	if (DstBoard.GetSafeHdc() == NULL)
		return false;

	if (bHaveDraw)
	{
		if (PP_GetControlAlphaBlendDraw())
		{
			m_pUiEngine->AlphaBlend(DstBoard, 0, 0, RECT_WIDTH(CtrlRct), RECT_HEIGHT(CtrlRct),
				PrintMemDc, 0, 0, RECT_WIDTH(CtrlRct), RECT_HEIGHT(CtrlRct), PP_GetControlDrawAlpha());
		}
		else
		{
			m_pUiEngine->BitBlt(DstBoard.GetSafeHdc(), 0, 0, RECT_WIDTH(CtrlRct), RECT_HEIGHT(CtrlRct),
				PrintMemDc.GetSafeHdc(), 0, 0, SRCCOPY);
		}
	}

	POINT offsetPt = {this->GetWindowRect().left, this->GetWindowRect().top};
	return ChildPrint(&m_ChildCtrlsVec, DstBoard, offsetPt, bPrintWhenInvisible);
}

// 递归打印子控件到一个内存DC上
bool IControlBase::ChildPrint(CHILD_CTRLS_VEC *pChildCtrlsVec, CDrawingBoard& DstBoard, POINT offsetPt, bool bPrintWhenInvisible)
{
	if (pChildCtrlsVec == NULL)
		return false;

	// 绘制子控件
	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL)
			continue;

		RECT CtrlRct = pCtrl->GetWindowRect();
		int nH = RECT_HEIGHT(CtrlRct);
		int nW = RECT_WIDTH(CtrlRct);

		CtrlRct.left -= offsetPt.x;
		CtrlRct.right = CtrlRct.left + nW;
		CtrlRct.top -= offsetPt.y;
		CtrlRct.bottom = CtrlRct.top + nH;

		if (pCtrl->IsVisible())
		{
			pCtrl->PrintTo(DstBoard, CtrlRct);
		}
		else
		{
			if (bPrintWhenInvisible)
				pCtrl->PrintTo(DstBoard, CtrlRct);
			else
				continue;
		}

		if (!ChildPrint(pCtrl->GetChildControlsVec(), DstBoard, offsetPt, bPrintWhenInvisible))
			return false;
	}

	return true;
}

// 设置/取得发送控件消息的目标
void IControlBase::SetControlMessage(IControlMessage* pCtrlMsg)
{
	m_pSendMsgCtrl = pCtrlMsg;
}

IControlMessage* IControlBase::GetControlMessage()
{
	return m_pSendMsgCtrl;
}

IUiEngine* IControlBase::GetUiEngine()
{
	return m_pUiEngine;
}

// 皮肤包管理类
IPropertySkinManager* IControlBase::GetSkinManager()
{
	return m_pSkinPropMgr;
}
