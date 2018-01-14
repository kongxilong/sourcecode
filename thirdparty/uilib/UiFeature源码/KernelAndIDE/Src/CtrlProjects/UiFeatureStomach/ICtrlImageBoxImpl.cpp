
#include "StdAfx.h"
#include "ICtrlImageBoxImpl.h"
#include "..\..\Inc\IControlManager.h"

ICtrlImageBox::ICtrlImageBox(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlImageBoxImpl::ICtrlImageBoxImpl(IUiFeatureKernel *pUiKernel) : ICtrlImageBox(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_IMAGE_BOX);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_pPropImage = NULL;
	m_pChangeImgAnimation = NULL;

	m_pNewImgBase = NULL;
	m_pOldImgBase = NULL;
	m_nNewImgAlpha = 255;
}

ICtrlImageBoxImpl::~ICtrlImageBoxImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlImageBoxImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 设置动画类型
void ICtrlImageBoxImpl::SetImageAnimation(IMAGE_BOX_ANIMATION paType)
{
	if (m_pChangeImgAnimation == NULL)
		return;

	m_pChangeImgAnimation->SetSelect(paType);
}

// 初始化控件
void ICtrlImageBoxImpl::OnCreate()
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlImageBoxImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "Image", "背景图片");

	m_pChangeImgAnimation = (IPropertyComboBox*)CreatePropetry(NULL, OTID_COMBOBOX, "ChangeImageAnimation", "替换图片的过程中，替换图片显示的动画");
	if(NULL != m_pChangeImgAnimation && m_pWindowBase->IsDesignMode())
	{
		m_pChangeImgAnimation->AppendString(L"无动画");
		m_pChangeImgAnimation->AppendString(L"渐隐渐现");

		if (m_pChangeImgAnimation->GetSelect() < 0 || m_pChangeImgAnimation->GetSelect() >= m_pChangeImgAnimation->GetDataCounts())
			m_pChangeImgAnimation->SetSelect(0);
	}

	if (bIsNewCtrl)
	{
		// 设置默认不接受鼠标信息
		if (m_pPropBase_RcvMouseMsg != NULL)
			m_pPropBase_RcvMouseMsg->SetValue(false);
	}
	return true;
}

// 控件初始化完毕
void ICtrlImageBoxImpl::OnFinalCreate()
{
	if (m_pPropImage == NULL)
		return;

	m_pNewImgBase = m_pPropImage->GetImageBaseProp();
}

IMAGE_BOX_ANIMATION ICtrlImageBoxImpl::GetImageAnimation()
{
	if (m_pChangeImgAnimation == NULL)
		return IBA_NONE;

	return (IMAGE_BOX_ANIMATION)(m_pChangeImgAnimation->GetSelect());
}

// 销毁控件
void ICtrlImageBoxImpl::OnDestroy()
{
}

IPropertyImageBase* ICtrlImageBoxImpl::GetBackgroundImage()
{
	if (m_pPropImage == NULL)
		return NULL;

	return m_pPropImage->GetImageBaseProp();
}

bool ICtrlImageBoxImpl::SetBackgroundImage(IPropertyImageBase* pNewImageBase)
{
	if (m_pSkinPropMgr == NULL || m_pWindowBase == NULL)
		return false;

	// 如果没有属性，则强制创建属性
	if (m_pPropImage == NULL)
		m_pPropImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "Image", "", true);

	if (m_pPropImage == NULL)
		return false;

	IPropertyImageBase* pOldImgBase = m_pPropImage->GetImageBaseProp();
	m_pPropImage->SetImageBaseProp(pNewImageBase);

	if (GetImageAnimation() <= IBA_NONE)
	{
		// 没有动画
		m_pOldImgBase = NULL;
		m_pNewImgBase = NULL;
		this->RedrawControl(true);
		return true;
	}
	else if (GetImageAnimation() == IBA_JYJX)
	{
		// 渐隐渐现
		m_pNewImgBase = pNewImageBase;
		m_pOldImgBase = pOldImgBase;
		m_nNewImgAlpha = 0;
	}

	m_pWindowBase->SetAnimationTimer();
	return true;
}

// 设置背景图
bool ICtrlImageBoxImpl::SetBackgroundImage(LPCWSTR pszImagePath)
{
	if (pszImagePath == NULL || wcslen(pszImagePath) <= 0 || m_pSkinPropMgr == NULL || m_pWindowBase == NULL)
		return false;

	// 如果没有属性，则强制创建属性
	if (m_pPropImage == NULL)
		m_pPropImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "Image", "", true);

	if (m_pPropImage == NULL)
		return false;

	IPropertyImageBase* pImgBase = m_pSkinPropMgr->LoadLocalImage(pszImagePath);
	if (pImgBase == NULL)
		return false;

	IPropertyImageBase* pOldImgBase = m_pPropImage->GetImageBaseProp();
	m_pPropImage->SetImageBaseProp(pImgBase);

	if (GetImageAnimation() <= IBA_NONE)
	{
		// 没有动画
		m_pOldImgBase = NULL;
		m_pNewImgBase = NULL;
		this->RedrawControl(true);
		return true;
	}
	else if (GetImageAnimation() == IBA_JYJX)
	{
		// 渐隐渐现
		m_pNewImgBase = pImgBase;
		m_pOldImgBase = pOldImgBase;
		m_nNewImgAlpha = 0;
	}

	m_pWindowBase->SetAnimationTimer();
	return true;
}

bool ICtrlImageBoxImpl::OnDrawAnimation()
{
	if (GetImageAnimation() == IBA_JYJX)
	{
		// 渐隐渐现效果
		int nOldAlpha = m_nNewImgAlpha;

		m_nNewImgAlpha += 20;

		if (m_nNewImgAlpha < 0)
			m_nNewImgAlpha = 0;

		if (m_nNewImgAlpha > 255)
			m_nNewImgAlpha = 255;

		if (m_nNewImgAlpha != nOldAlpha)
			return true;
	}

	return false;
}

// 绘制控件
void ICtrlImageBoxImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (GetImageAnimation() <= IBA_NONE)
	{
		if (m_pPropImage != NULL)
			m_pPropImage->DrawImage(DrawBoard, this->GetClientRect());
	}
	else if (GetImageAnimation() == IBA_JYJX)
	{
		if (m_nNewImgAlpha < 0)
			m_nNewImgAlpha = 0;

		if (m_nNewImgAlpha > 255)
			m_nNewImgAlpha = 255;

		int nOldAlpha = 255 - m_nNewImgAlpha;
		if (nOldAlpha > 0 && m_pOldImgBase != NULL)
			m_pOldImgBase->DrawImage(DrawBoard, this->GetClientRect(), nOldAlpha);

		if (m_pNewImgBase != NULL)
			m_pNewImgBase->DrawImage(DrawBoard, this->GetClientRect(), m_nNewImgAlpha);
	}
}
