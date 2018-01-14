
#include "StdAfx.h"
#include "ICtrlPanelImpl.h"

ICtrlPanel::ICtrlPanel(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlPanelImpl::ICtrlPanelImpl(IUiFeatureKernel *pUiKernel) : ICtrlPanel(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_BASE_PANEL);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;
	m_pPropImage = NULL;
	m_pChangeImgAnimation = NULL;
	m_pPropPhotoFrameImg = NULL;
	m_pPropMarkPhotoFrameImg = NULL;
	m_pPropCursor = NULL;

	m_pInSkinImgBase = NULL;
	m_pNewImgBase = NULL;
	m_pOldImgBase = NULL;
	m_nNewImgAlpha = 255;

	m_IsShowCursor = true;
}

ICtrlPanelImpl::~ICtrlPanelImpl()
{
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlPanelImpl::CreateControlPropetry(bool bIsNewCtrl)
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
		if (m_pPropBase_RcvMouseMsg != NULL)
			m_pPropBase_RcvMouseMsg->SetValue(false);
	}

	IPropertyGroup* pPhotoGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "PhotoFrameGroup", "相框功能");
	m_pPropPhotoFrameImg = (IPropertyImage*)CreatePropetry(pPhotoGroup, OTID_IMAGE, "PhotoFrame", "图片的相框");
	m_pPropMarkPhotoFrameImg = (IPropertyImage*)CreatePropetry(pPhotoGroup, OTID_IMAGE, "MarkPhotoFrame", "图片的相框的扣边图片，图片中黑色的部分代表需要扣掉的部分");

	if (m_pPropPhotoFrameImg != NULL && m_pPropMarkPhotoFrameImg != NULL)
	{
		m_pPropPhotoFrameImg->InitImage();
		m_pPropMarkPhotoFrameImg->InitImage();
	}

	m_pPropCursor = (IPropertyCursor*)CreatePropetry(NULL, OTID_CURSOR, "CursorStyle", "鼠标停留在图片上的时候的手势");

	return true;
}

// 是否显示手型指针开关
void ICtrlPanelImpl::SetIsShowCursor(bool bIsShow)
{
	m_IsShowCursor = bIsShow;
}

bool ICtrlPanelImpl::SetBackgroundImage(HBITMAP hbmp, bool bReload)
{
	if(!hbmp)
	{
		return false;
	}

	if(m_pPropImage->GetImageBaseProp() != m_pInSkinImgBase)
	{
		m_pSkinPropMgr->DeleteLocalImageBase(m_pPropImage->GetImageBaseProp());
	}

	m_pPropImage->SetImageBaseProp(m_pSkinPropMgr->LoadImageFromHBITMAP(hbmp));

	this->RedrawControl(bReload);
	return true;
}

// 设置动画类型
void ICtrlPanelImpl::SetImageAnimation(PANEL_ANIMATION paType)
{
	if (m_pChangeImgAnimation == NULL)
		return;

	m_pChangeImgAnimation->SetSelect(paType);
}

// 回复到皮肤包中的图片
void ICtrlPanelImpl::SetToDefault(bool bRedraw, bool bDeleteOld/* = true*/)
{
	//if (m_pInSkinImgBase == NULL)
	//	return;

	if(m_pPropImage->GetImageBaseProp() != m_pInSkinImgBase)
	{
		if(bDeleteOld)
		{
			m_pSkinPropMgr->DeleteLocalImageBase(m_pPropImage->GetImageBaseProp());
		}
	}

	m_pPropImage->SetImageBaseProp(m_pInSkinImgBase);

	this->RedrawControl(bRedraw);
}

PANEL_ANIMATION ICtrlPanelImpl::GetImageAnimation()
{
	if (m_pChangeImgAnimation == NULL)
		return PA_NONE;

	return (PANEL_ANIMATION)(m_pChangeImgAnimation->GetSelect());
}

// 初始化控件
void ICtrlPanelImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlPanelImpl::OnFinalCreate()
{
	if (m_pPropImage == NULL)
		return;

	m_pInSkinImgBase = m_pPropImage->GetImageBaseProp();
	m_pNewImgBase = m_pInSkinImgBase;
}

// 销毁控件
void ICtrlPanelImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlPanelImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (GetImageAnimation() <= PA_NONE)
	{
		if (m_pPropImage != NULL)
			m_pPropImage->DrawImage(DrawBoard, this->GetClientRect());
	}
	else if (GetImageAnimation() == PA_JYJX)
	{
		if (m_nNewImgAlpha < 0)
			m_nNewImgAlpha = 0;

		if (m_nNewImgAlpha > 255)
			m_nNewImgAlpha = 255;

		if (m_pOldImgBase != NULL && m_pSkinPropMgr != NULL)
		{
			int nAlpha = 255 - m_nNewImgAlpha;
			m_pOldImgBase->DrawImage(DrawBoard, this->GetClientRect(), nAlpha);
			if (nAlpha <= 0 && m_pOldImgBase != m_pInSkinImgBase)
			{
				m_pSkinPropMgr->DeleteLocalImageBase(m_pOldImgBase);
				m_pOldImgBase = NULL;
			}
		}

		if (m_pNewImgBase != NULL)
			m_pNewImgBase->DrawImage(DrawBoard, this->GetClientRect(), m_nNewImgAlpha);
	}

	// 绘制相框
	DrawPhotoFrame(DrawBoard);
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlPanelImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

void ICtrlPanelImpl::DrawPhotoFrame(CDrawingBoard &DrawBoard)
{
	if (m_pPropPhotoFrameImg == NULL || m_pPropMarkPhotoFrameImg == NULL || DrawBoard.GetBits() == NULL)
		return;

	IPropertyImageBase* pDelImg = m_pPropMarkPhotoFrameImg->GetImageBaseProp();
	if (pDelImg != NULL)
	{
		// 图片抠边
		CDrawingImage* pDelDraw = pDelImg->GetDrawingImage();
		if (pDelDraw != NULL && pDelDraw->GetBits() != NULL)
			CutImageData(*pDelDraw, DrawBoard);
	}

	m_pPropPhotoFrameImg->DrawImage(DrawBoard, this->GetClientRect());
}

// 设置背景图
bool ICtrlPanelImpl::SetBackgroundImage(LPCWSTR pszImagePath, bool bReload)
{
	if (pszImagePath == NULL || wcslen(pszImagePath) <= 0 || m_pSkinPropMgr == NULL || m_pPropImage == NULL || m_pWindowBase == NULL)
		return false;

	IPropertyImageBase* pImgBase = m_pSkinPropMgr->LoadLocalImage(pszImagePath, bReload);
	if (pImgBase == NULL)
		return false;

	IPropertyImageBase* pOldImgBase = m_pPropImage->GetImageBaseProp();
	m_pPropImage->SetImageBaseProp(pImgBase);

	if (GetImageAnimation() <= PA_NONE)
	{
		// 没有动画
		m_pOldImgBase = NULL;

		if (pOldImgBase != m_pInSkinImgBase)
			m_pSkinPropMgr->DeleteLocalImageBase(m_pOldImgBase);

		this->RedrawControl(true);
		return true;
	}
	else if (GetImageAnimation() == PA_JYJX)
	{
		// 渐隐渐现
		m_pNewImgBase = pImgBase;
		m_pOldImgBase = pOldImgBase;
		m_nNewImgAlpha = 0;

		m_pWindowBase->SetAnimationTimer();
	}

	return true;
}

bool ICtrlPanelImpl::OnDrawAnimation()
{
	if (GetImageAnimation() == PA_JYJX)
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

// 设置控件光标
IPropertyCursor* ICtrlPanelImpl::OnSetCursor(POINT pt)
{
	if (this->GetReceiveMouseMessage() && m_IsShowCursor)
	{
		// 只有在接受鼠标信息的情况下才有手势
		return m_pPropCursor;
	}

	return NULL;
}

void ICtrlPanelImpl::OnMouseEnter(POINT pt)
{
	if (GetReceiveMouseMessage() && m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_PANEL_ENTER, NULL, NULL);
}

// 鼠标移出
void ICtrlPanelImpl::OnMouseLeave(POINT pt)
{
	if (GetReceiveMouseMessage() && m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_PANEL_LEAVE, NULL, NULL);
}

// 鼠标左键点击
void ICtrlPanelImpl::OnLButtonDown(POINT pt)
{
	if (GetReceiveMouseMessage() && m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_PANEL_DOWN, NULL, NULL);
}

// 鼠标左键双击
void ICtrlPanelImpl::OnLButtonDbClick(POINT pt)
{
	if (GetReceiveMouseMessage() && m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_PANEL_DBCLICK, NULL, NULL);
}

// 鼠标左键抬起
void ICtrlPanelImpl::OnLButtonUp(POINT pt)
{
	RECT ctrlRct = this->GetClientRect();
	if (!::PtInRect(&ctrlRct, pt))
		return;

	if (GetReceiveMouseMessage() && m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_PANEL_UP, NULL, NULL);
}

// 鼠标右键点击
void ICtrlPanelImpl::OnRButtonDown(POINT pt)
{
	if (GetReceiveMouseMessage() && m_pWindowBase != NULL && this->IsEnable())
		m_pWindowBase->OnCtrlMessage(dynamic_cast<IControlBase*>(this), CM_PANEL_R_BTN_DOWN, NULL, NULL);
}

IPropertyImageBase* ICtrlPanelImpl::GetBackgroundImage()
{
	if (m_pPropImage == NULL)
		return NULL;

	return m_pPropImage->GetImageBaseProp();
}

bool ICtrlPanelImpl::SetBackgroundImage(IPropertyImageBase* pNewImageBase)
{
	if (m_pSkinPropMgr == NULL || m_pPropImage == NULL || m_pWindowBase == NULL)
		return false;

	IPropertyImageBase* pOldImgBase = m_pPropImage->GetImageBaseProp();
	m_pPropImage->SetImageBaseProp(pNewImageBase);

	if (GetImageAnimation() <= PA_NONE)
	{
		// 没有动画
		m_pOldImgBase = NULL;
		m_pNewImgBase = NULL;
		this->RedrawControl(true);
		return true;
	}
	else if (GetImageAnimation() == PA_JYJX)
	{
		// 渐隐渐现
		m_pNewImgBase = pNewImageBase;
		m_pOldImgBase = pOldImgBase;
		m_nNewImgAlpha = 0;
		m_pWindowBase->SetAnimationTimer();
	}

	return true;
}

// 设置关键字，方便做容器的时候排序用
void ICtrlPanelImpl::SetKey(LPCWSTR key)
{
	m_key = key;
}

// getkey
LPCWSTR ICtrlPanelImpl::GetKey()
{
	return m_key;
}

// 图片抠边，MarkDrawBoard 图片中黑色部分将会从 DstDrawBoard 中抠出
bool ICtrlPanelImpl::CutImageData(CDrawingBoard &MarkDrawBoard, CDrawingBoard &DstDrawBoard)
{
	if (MarkDrawBoard.GetBits() == NULL || DstDrawBoard.GetBits() == NULL)
		return false;

	if (MarkDrawBoard.GetDcSize().cx != DstDrawBoard.GetDcSize().cx || MarkDrawBoard.GetDcSize().cy != DstDrawBoard.GetDcSize().cy)
		return false;

	BYTE* pDataMask = MarkDrawBoard.GetBits();
	BYTE* pDataImg = DstDrawBoard.GetBits();

	for(int i = 0; i < MarkDrawBoard.GetDcSize().cx * MarkDrawBoard.GetDcSize().cy; i++)
	{
		if((*(pDataMask+1)) == 0 && (*(pDataMask+2)) == 0)
			memset(pDataImg, 0, 4);

		pDataImg += 4;
		pDataMask += 4;
	}

	return true;
}