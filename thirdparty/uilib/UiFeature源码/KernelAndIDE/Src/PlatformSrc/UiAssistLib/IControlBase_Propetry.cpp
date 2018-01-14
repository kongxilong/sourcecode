
#include "StdAfx.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\IControlBase.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\IControlBase.h"
#include "..\..\Inc\IWindowBase.h"
#include "..\..\Inc\ICommonFun.h"


// 初始化在builder中的属性
bool IControlBase::InitControlPropetry(IPropertyControl *pCtrlProp, bool bIsNewCtrl)
{
	if (pCtrlProp == NULL)
		return false;

	IFeatureObject* pOwnerObj = dynamic_cast<IFeatureObject*>(this);
	pCtrlProp->SetOwnerObject(pOwnerObj);

	m_pXmlPropCtrl = pCtrlProp;
	CreateCtrlAllPropetry(bIsNewCtrl);
	// 控件创建
	OnCreate();
	return true;
}

IPropertyControl* IControlBase::PP_GetControlPropetry()
{
	return m_pXmlPropCtrl;
}

IPropertyBase* IControlBase::CreateCtrlPublicPropetry(SKIN_FILE_ITEM* pSkinFileItem, IPropertyGroup* pGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo)
{
	if (pSkinFileItem == NULL || m_pSkinPropMgr == NULL || m_pXmlPropCtrl == NULL ||
		pszPropName == NULL || strlen(pszPropName) <= 0 || propType <= OTID_NONE || propType >= OTID_LAST)
		return NULL;

	if (pGroup == NULL)
		pGroup = m_pXmlPropCtrl->GetControlPropGroup();

	return CreateResourcePropetry(pSkinFileItem, this, m_pSkinPropMgr, pGroup, propType, pszPropName, pszPropInfo, true);
}

// 创建空的属性队列
void IControlBase::CreateCtrlAllPropetry(bool bIsNewCtrl)
{
	if (m_pXmlPropCtrl == NULL || m_pWindowBase == NULL)
		return;

	m_pSkinFileItem = m_pWindowBase->GetSkinFileItem();
	if (m_pSkinFileItem == NULL)
		return;

	// Group:base
	m_pPropGroupBase = (IPropertyGroup*)CreateCtrlPublicPropetry(m_pSkinFileItem, NULL, OTID_GROUP, "ControlBase", "控件基本属性");
	if (m_pPropGroupBase == NULL)
		return;

	// base-objectid
	m_pPropBase_ObjectId = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_INT, NAME_SKIN_PROP_NAME_OBJ_ID, "控件 Object Id");
	if (m_pPropBase_ObjectId == NULL)
		return;
	m_pPropBase_ObjectId->SetValue((int)this->GetObjectId());
	m_pPropBase_ObjectId->EnableInBuilder(false);

	// base-类型名称
	m_pPropBase_TypeName = (IPropertyString*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_STRING, NAME_SKIN_PROP_NAME_TYPE, "控件类型");
	if (m_pPropBase_TypeName == NULL)
		return;
	m_pPropBase_TypeName->SetString(this->GetObjectType());
	m_pPropBase_TypeName->EnableInBuilder(false);

	// base-name
	m_pPropBase_Name = (IPropertyString*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_STRING, NAME_SKIN_PROP_NAME, "控件名称");
	if (m_pPropBase_Name == NULL)
		return;
	// 设置控件名称
	if (m_pPropBase_Name->GetString() == NULL
		|| (m_pPropBase_Name->GetString() != NULL && m_pPropBase_Name->GetLength() <= 0))
	{
		if (GetObjectName() != NULL && wcslen(GetObjectName()) > 0)
			m_pPropBase_Name->SetString(GetObjectName());
		else
			m_pPropBase_Name->SetString(L"新建控件");
	}

	// base-lock
	m_pPropBase_Lock = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "Lock", "是否锁定在Builder中鼠标改变控件大小和位置");
	if (m_pPropBase_Lock == NULL)
		return;

	// base-visible
	m_pPropBase_Visible = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "Visible", "是否可见");
	if (m_pPropBase_Visible == NULL)
		return;

	// base-enable
	m_pPropBase_Enable = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "Enable", "是否可用");
	if (m_pPropBase_Enable == NULL)
		return;

	// base-ReceiveMouseMessage
	m_pPropBase_RcvMouseMsg = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "ReceiveMouseMessage", "是否接收鼠标消息");
	if (m_pPropBase_RcvMouseMsg == NULL)
		return;

	// tab 键顺序
	// base-taborder
	m_pPropBase_TabOrder = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_INT, "TabOrder", "tab键的跳转序号，0为不接受tab键");
	if (m_pPropBase_TabOrder == NULL)
		return;

	// 是否为默认回车接受键
	// base-defaultenterctrl
	m_pPropBase_DefaultEnterCtrl = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_BOOL, "DefaultEnterCtrl", "控件是否为当前对话框默认回车键接收控件");
	if (m_pPropBase_DefaultEnterCtrl == NULL)
		return;

	// 如果一个控件没有tips属性，则不创建tips属性，加快性能
	if (m_bHaveSysTipsProp)
	{
		// Group:Tips
		m_pPropGroupTips = (IPropertyGroup*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_GROUP, "SystemTips", "Tips信息");
		if (m_pPropGroupTips == NULL)
			return;

		// Tips-ShowInUserDefPos
		m_pPropTips_ShowInUserDefPos = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupTips, OTID_BOOL, "ShowInUserDefPos", "是否在用户自定义的控件范围内显示。比如：有的控件比较复杂，如：List控件，可能会在每个节点都显示不同的tips，此时选择TRUE，用户可以在同一个控件不同地方显示不同的tips；当选择为FALSE的时候，鼠标只要进入控件范围内就会显示tips");
		if (m_pPropTips_ShowInUserDefPos == NULL)
			return;

		// Tips-Baloon
		m_pPropTips_Baloon = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupTips, OTID_BOOL, "Baloon", "是否为气泡样子的Tips，TRUE：气泡的样子；FALSE：长方形的Tips");
		if (m_pPropTips_Baloon == NULL)
			return;

		// Tips-tipsData
		m_pPropTips_TipsData = (IPropertyString*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupTips, OTID_STRING, "TipsData", "Tips信息");
		if (m_pPropTips_TipsData == NULL)
			return;

		// Tips-ShowTime
		m_pPropTips_ShowTimes = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupTips, OTID_INT, "ShowTime", "tips显示的时间，单位秒");
		if (m_pPropTips_ShowTimes == NULL)
			return;
	}

//////////////////////////////////////////////////////////////////////////
	// base-Draw
	m_pPropBase_DrawGroup = (IPropertyGroup*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_GROUP, "Draw", "绘制信息");
	if (m_pPropBase_DrawGroup == NULL)
		return;

	// 是否为无绘制控件
	// base-Draw-NoDrawControl
	m_pPropBase_NoDrawCtrl = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_DrawGroup, OTID_BOOL, "NoDrawControl", "控件是否为无绘制控件。如果一个控件是没有任何绘制操作的控件，设置此属性为true，将会减少内存使用，并提高整个系统的绘制速度。");
	if (m_pPropBase_NoDrawCtrl == NULL)
		return;

	// base-Draw-AlphaBlendDraw
	m_pPropBase_Draw_AlphaBlendDraw = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_DrawGroup, OTID_BOOL, "AlphaBlendDraw", "是否使用 AlphaBlend 函数直接将控件绘制到窗口上。true：使用 AlphaBlend 函数将控件绘制到窗口上；false：使用 BitBlt 函数将控件绘制到窗口上。");
	if (m_pPropBase_Draw_AlphaBlendDraw == NULL)
		return;

	// base-Draw-DrawAlpha
	m_pPropBase_Draw_DrawAlpha = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_DrawGroup, OTID_INT, "DrawAlpha", "使用AlphaBlend函数将控件绘制到窗口上时，使用的Alpha值，必须 0<=Alpha<=255 。");
	if (m_pPropBase_Draw_DrawAlpha == NULL)
		return;

	// base-Draw-NoDrawControl
	m_pPropBase_Draw_TempDrawMem = (IPropertyBool*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_DrawGroup, OTID_BOOL, "TempDrawMem", "控件是否使用临时内存绘制。是：在绘制完控件之后就释放内存；否：在绘制完控件之后还保有内存下次使用。如果控件只是纯粹的贴一个图片，建议选【是】减少内存使用，同时也不会降低速度。");
	if (m_pPropBase_Draw_TempDrawMem == NULL)
		return;

//////////////////////////////////////////////////////////////////////////
	// 控件布局
	// Group:base-layout
	m_pPropBase_LayoutGroup = (IPropertyGroup*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_GROUP, "Layout", "控件布局信息");
	if (m_pPropBase_LayoutGroup == NULL)
		return;

	// layout-width
	m_pPropBase_Layout_Width = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_INT, NAME_LAYOUT_WIDTH, "窗口宽度");
	if (m_pPropBase_Layout_Width == NULL)
		return;

	// layout-height
	m_pPropBase_Layout_Height = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_INT, NAME_LAYOUT_HEIGHT, "窗口高度");
	if (m_pPropBase_Layout_Height == NULL)
		return;

	// layout-layoutType
	m_pPropBase_Layout_Layout = (IPropertyComboBox*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_COMBOBOX, NAME_LAYOUT_TYPE, "控件相对于父控件/窗口的布局类型");
	if (m_pPropBase_Layout_Layout == NULL)
		return;

	if (m_pWindowBase->IsDesignMode())
	{
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_LEFT_TOP));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_LEFT_BOTTOM));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_RIGHT_TOP));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_RIGHT_BOTTOM));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_LEFT));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_RIGHT));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_TOP));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_BOTTOM));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_ALL));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_MIDDLE));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_L_USER_DEF));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_TOP_MIDDLE));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_BOTTOM_MIDDLE));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_LEFT_MIDDLE));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_RIGHT_MIDDLE));
		m_pPropBase_Layout_Layout->AppendString(_T(TN_CL_G_MID_MIDDLE));

		if (m_pPropBase_Layout_Layout->GetSelect() < 0 || m_pPropBase_Layout_Layout->GetSelect() >= m_pPropBase_Layout_Layout->GetDataCounts())
			m_pPropBase_Layout_Layout->SetSelect(0);
	}

	// layout-leftspace
	m_pPropBase_Layout_LeftSpace = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_INT, NAME_LAYOUT_LEFTSPACE, "距离父控件/窗口左侧距离");
	if (m_pPropBase_Layout_LeftSpace == NULL)
		return;

	// layout-rightspace
	m_pPropBase_Layout_RightSpace = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_INT, NAME_LAYOUT_RIGHTSPACE, "距离父控件/窗口右侧距离");
	if (m_pPropBase_Layout_RightSpace == NULL)
		return;

	// layout-topspace
	m_pPropBase_Layout_TopSpace = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_INT, NAME_LAYOUT_TOPSPACE, "距离父控件/窗口上方距离");
	if (m_pPropBase_Layout_TopSpace == NULL)
		return;

	// layout-bottomspace
	m_pPropBase_Layout_BottomSpace = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_LayoutGroup, OTID_INT, NAME_LAYOUT_BOTTOMSPACE, "距离父控件/窗口下方距离");
	if (m_pPropBase_Layout_BottomSpace == NULL)
		return;

//////////////////////////////////////////////////////////////////////////

	m_pPropBase_AnimationGroup = (IPropertyGroup*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropGroupBase, OTID_GROUP, "Animation", "控件动画");
	if (m_pPropBase_AnimationGroup == NULL)
		return;

	// ControlAnimation
	m_pPropBase_AnimationType = (IPropertyComboBox*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_AnimationGroup, OTID_COMBOBOX, "AnimationType", "控件动画");
	if (m_pPropBase_AnimationType == NULL)
		return;

	if (m_pWindowBase->IsDesignMode() && m_pWindowBase->GetUiKernel() != NULL)
	{
		// 取得所有动画名称，写入builder
		m_pPropBase_AnimationType->AppendString(L"0-无动画");
		m_pPropBase_AnimationType->AppendString(L"1-用户自定义强动画");

		const char *pList = NULL;
		int dwCtns = m_pWindowBase->GetUiKernel()->GetAnimationList(pList);
		if (pList != NULL || dwCtns > 0)
		{
			for (int i = 0; i < (int)dwCtns; i++)
			{
				WCHAR szAnimationName[MAX_PATH];
				memset(szAnimationName, 0, sizeof(szAnimationName));
				CStringW strDataW = L"";
				strDataW = pList;
				swprintf_s(szAnimationName, MAX_PATH - 1, L"%d-%s", i + 2, strDataW);
				m_pPropBase_AnimationType->AppendString(szAnimationName);

				pList = pList + strlen(pList) + 1;
				if ((*pList) == '\0')
					break;
			}
		}

		if (m_pPropBase_AnimationType->GetSelect() < 0 || m_pPropBase_AnimationType->GetSelect() >= m_pPropBase_AnimationType->GetDataCounts())
			m_pPropBase_AnimationType->SetSelect(0);
	}

	// Animation-FrameTime
	m_pPropBase_Animation_FrameTime = (IPropertyInt*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_AnimationGroup, OTID_INT, "FrameTime", "每一帧动画的时间间隔，单位：毫秒");
	if (m_pPropBase_Animation_FrameTime == NULL)
		return;

	// Animation-AnimationDirection
	m_pPropBase_Animation_Direction = (IPropertyComboBox*)CreateCtrlPublicPropetry(m_pSkinFileItem, m_pPropBase_AnimationGroup, OTID_COMBOBOX, "AnimationDirection", "动画的方向，比如：翻转分为正面翻转和反面翻转。");
	if (m_pPropBase_Animation_Direction == NULL)
		return;

	if (m_pWindowBase->IsDesignMode())
	{
		m_pPropBase_Animation_Direction->AppendString(L"正向动画");
		m_pPropBase_Animation_Direction->AppendString(L"反向动画");

		if (m_pPropBase_Animation_Direction->GetSelect() < 0 || m_pPropBase_Animation_Direction->GetSelect() >= m_pPropBase_Animation_Direction->GetDataCounts())
			m_pPropBase_Animation_Direction->SetSelect(0);
	}

//////////////////////////////////////////////////////////////////////////
	// Group:base
	m_pPropGroupCtrlDefs = (IPropertyGroup*)CreateCtrlPublicPropetry(m_pSkinFileItem, NULL, OTID_GROUP, "ControlPropetry", "控件自定义属性信息");
	if (m_pPropGroupCtrlDefs == NULL)
		return;

	if (bIsNewCtrl)
	{
		if (m_pPropBase_Lock != NULL)
			m_pPropBase_Lock->SetValue(false);
		if (m_pPropBase_Visible != NULL)
			m_pPropBase_Visible->SetValue(true);
		if (m_pPropBase_RcvMouseMsg != NULL)
			m_pPropBase_RcvMouseMsg->SetValue(true);
		if (m_pPropBase_Enable != NULL)
			m_pPropBase_Enable->SetValue(true);
		if (m_pPropBase_TabOrder != NULL)
			m_pPropBase_TabOrder->SetValue(NOT_SET_TAB_FOCUS);
		if (m_pPropBase_DefaultEnterCtrl != NULL)
			m_pPropBase_DefaultEnterCtrl->SetValue(false);
		if (m_pPropBase_Layout_Width != NULL)
			m_pPropBase_Layout_Width->SetValue(20);
		if (m_pPropBase_Layout_Height != NULL)
			m_pPropBase_Layout_Height->SetValue(20);
		if (m_pPropBase_Layout_LeftSpace != NULL)
			m_pPropBase_Layout_LeftSpace->SetValue(0);
		if (m_pPropBase_Layout_RightSpace != NULL)
			m_pPropBase_Layout_RightSpace->SetValue(0);
		if (m_pPropBase_Layout_TopSpace != NULL)
			m_pPropBase_Layout_TopSpace->SetValue(0);
		if (m_pPropBase_Layout_BottomSpace != NULL)
			m_pPropBase_Layout_BottomSpace->SetValue(0);
		if (m_pPropBase_Draw_AlphaBlendDraw != NULL)
			m_pPropBase_Draw_AlphaBlendDraw->SetValue(true);
		if (m_pPropBase_Draw_DrawAlpha != NULL)
			m_pPropBase_Draw_DrawAlpha->SetValue(255);
		if (m_pPropBase_Animation_FrameTime != NULL)
			m_pPropBase_Animation_FrameTime->SetValue(20);
		if (m_pPropBase_Draw_TempDrawMem != NULL)
			m_pPropBase_Draw_TempDrawMem->SetValue(true);
	}

	if (m_pPropBase_Layout_Width != NULL && m_pPropBase_Layout_Height != NULL)
	{
		SET_RECT(m_RectInWindow, 0, 0, m_pPropBase_Layout_Width->GetValue(), m_pPropBase_Layout_Height->GetValue());
	}

	// 从属性更新数据到成员变量
	PropetyValueToMemberValue(false, NULL);

	// 控件创建自定义的属性
	CreateControlPropetry(bIsNewCtrl);
}

IPropertyBase* IControlBase::CreatePropetry(IPropertyGroup* pPropGroup, OBJECT_TYPE_ID propType, const char* pszPropName, const char *pszPropInfo, bool bNotExistCreate)
{
	if (m_pSkinFileItem == NULL || m_pPropGroupCtrlDefs == NULL || m_pSkinPropMgr == NULL || m_pXmlPropCtrl == NULL || m_pWindowBase == NULL ||
		pszPropName == NULL || strlen(pszPropName) <= 0 || propType <= OTID_NONE || propType >= OTID_LAST)
		return NULL;

	if (pPropGroup == NULL)
		pPropGroup = m_pPropGroupCtrlDefs;

	if (!bNotExistCreate)
	{
		// 如果设计时态下就无条件创建属性，如果是运行时态就根据xml的存在与否创建
		bNotExistCreate = m_pWindowBase->IsDesignMode() || IsCloneControl();
	}

	return CreateResourcePropetry(m_pSkinFileItem, this, m_pSkinPropMgr, pPropGroup, propType, pszPropName, pszPropInfo, bNotExistCreate);
}

// 从属性更新数据到成员变量
void IControlBase::PropetyValueToMemberValue(bool bCallRefresh, IPropertyBase* pPropBase)
{
	if (m_pWindowBase == NULL)
		return;

	RECT ParentRct;
	INIT_RECT(ParentRct);
	FANGKUAI_8* pFk8 = NULL;
	if (m_pParentCtrl == NULL)
	{
		ParentRct = m_pWindowBase->GetClientRect();
		pFk8 = m_pWindowBase->BD_GetFangKuai8Rect();
	}
	else
	{
		ParentRct = m_pParentCtrl->GetWindowRect();
		pFk8 = m_pParentCtrl->BD_GetFangKuai8Rect();
	}

	if (pFk8 == NULL)
		return;

	m_pWindowBase->SetControlWindowLayoutPostion(this, ParentRct);

	m_BD_FangKuai8.EntityRct.left = pFk8->EntityRct.left + (m_RectInWindow.left - ParentRct.left);
	m_BD_FangKuai8.EntityRct.right = m_BD_FangKuai8.EntityRct.left + RECT_WIDTH(m_RectInWindow);
	m_BD_FangKuai8.EntityRct.top = pFk8->EntityRct.top + (m_RectInWindow.top - ParentRct.top);
	m_BD_FangKuai8.EntityRct.bottom = m_BD_FangKuai8.EntityRct.top + RECT_HEIGHT(m_RectInWindow);

	ResetChildPropetyValueToMemberValue(this, bCallRefresh, pPropBase);

	if (bCallRefresh)
		this->OnBuilderRefreshProp(pPropBase);
}

// 重新计算子控件的位置和大小
void IControlBase::ResetChildPropetyValueToMemberValue(IControlBase* pParentCtrl, bool bCallRefresh, IPropertyBase* pPropBase)
{
	if (pParentCtrl == NULL)
		return;

	CHILD_CTRLS_VEC *pChildCtrlsVec = pParentCtrl->GetChildControlsVec();
	if (pChildCtrlsVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pChildCtrlsVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pChildCtrlsVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL || pCtrl->GetOwnerWindow() == NULL)
			continue;

		pCtrl->PropetyValueToMemberValue(bCallRefresh, pPropBase);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// 可用属性
void IControlBase::SetEnable(bool bEnable, bool bSetChild)
{
	if (m_pPropBase_Enable != NULL)
		m_pPropBase_Enable->SetValue(bEnable);

	// 设置重绘标志
	this->RedrawControl();

	// 设置子控件
	if (bSetChild)
	{
		ListObj* pNext = NULL;
		for (ListObj* pItem = m_ChildCtrlsVec.TopObj(); pItem != NULL; pItem = pNext)
		{
			pNext = m_ChildCtrlsVec.NextObj(pItem);
			IControlBase* pCtrl = (IControlBase *)pItem;
			if (pCtrl == NULL)
				continue;

			pCtrl->SetEnable(bEnable, bSetChild);
		}
	}
}

bool IControlBase::IsEnable()
{
	if (m_pPropBase_Enable == NULL)
		return true;

	return m_pPropBase_Enable->GetValue();
}

// 可见属性
void IControlBase::SetVisible(bool bVisible, bool bSetChild)
{
	if (m_pPropBase_Visible != NULL)
		m_pPropBase_Visible->SetValue(bVisible);

	// 设置子控件
	if (bSetChild)
	{
		ListObj* pNext = NULL;
		for (ListObj* pItem = m_ChildCtrlsVec.TopObj(); pItem != NULL; pItem = pNext)
		{
			pNext = m_ChildCtrlsVec.NextObj(pItem);
			IControlBase* pCtrl = (IControlBase *)pItem;
			if (pCtrl == NULL)
				continue;

			pCtrl->SetVisible(bVisible, bSetChild);
		}
	}
}

bool IControlBase::IsVisible(bool bIncludeParent/* = true*/)
{
	if (m_pPropBase_Visible == NULL)
		return true;

	if (!m_pPropBase_Visible->GetValue())
		return false;

	if (bIncludeParent)
	{
		return IsParentVisible(this->GetParentControl(), m_pPropBase_Visible->GetValue());
	}
	else
	{
		return true;
	}
}

bool IControlBase::IsParentVisible(IControlBase* pParentCtrl, bool bSelfVisible)
{
	if (pParentCtrl == NULL || pParentCtrl->m_pPropBase_Visible == NULL)
		return bSelfVisible;

	if (!pParentCtrl->m_pPropBase_Visible->GetValue())
		return false;

	return IsParentVisible(pParentCtrl->GetParentControl(), bSelfVisible);
}

LPCWSTR IControlBase::PP_GetControlObjectName()
{
	if (m_pPropBase_Name != NULL)
		return m_pPropBase_Name->GetString();

	return GetObjectName();
}

// 是否接受鼠标消息
void IControlBase::SetReceiveMouseMessage(bool bIsReceive)
{
	if (m_pPropBase_RcvMouseMsg != NULL)
		m_pPropBase_RcvMouseMsg->SetValue(bIsReceive);
}

bool IControlBase::GetReceiveMouseMessage()
{
	if (m_pPropBase_RcvMouseMsg == NULL)
		return true;

	return m_pPropBase_RcvMouseMsg->GetValue();
}

// 设置附属控件
void IControlBase::SetPropertySkinManager(IPropertySkinManager *pMgr)
{
	m_pSkinPropMgr = pMgr;
}

IPropertyGroup* IControlBase::PP_GetControlPropetryGroup()
{
	if (m_pXmlPropCtrl == NULL)
		return NULL;

	return m_pXmlPropCtrl->GetControlPropGroup();
}

// 取得子控件的属性列表：Layout.xml 中的布局
PROP_CONTROL_VEC* IControlBase::GetChildPropControlVec()
{
	if (m_pXmlPropCtrl == NULL)
		return NULL;

	return m_pXmlPropCtrl->GetChildPropControlVec();
}

// 是否锁定在Builder中鼠标改变控件大小和位置
void IControlBase::SetLockControl(bool bLock)
{
	if (m_pPropBase_Lock == NULL)
		return;

	m_pPropBase_Lock->SetValue(bLock);	
}

bool IControlBase::GetLockControl()
{
	if (m_pPropBase_Lock == NULL)
		return false;

	return m_pPropBase_Lock->GetValue();
}

CONTROL_LAYOUT_INFO IControlBase::GetLayout()
{
	CONTROL_LAYOUT_INFO LayoutInfo;
	memset(&LayoutInfo, 0, sizeof(CONTROL_LAYOUT_INFO));

	if (m_pPropBase_Layout_Width == NULL || m_pPropBase_Layout_Height == NULL || m_pPropBase_Layout_Layout == NULL
		|| m_pPropBase_Layout_LeftSpace == NULL || m_pPropBase_Layout_RightSpace == NULL || m_pPropBase_Layout_TopSpace == NULL || m_pPropBase_Layout_BottomSpace == NULL)
		return LayoutInfo;

	LayoutInfo.clType = (CONTROL_LAYOUT)m_pPropBase_Layout_Layout->GetSelect();
	LayoutInfo.nBottomSpace = m_pPropBase_Layout_BottomSpace->GetValue();
	LayoutInfo.nHeight = m_pPropBase_Layout_Height->GetValue();
	LayoutInfo.nLeftSpace = m_pPropBase_Layout_LeftSpace->GetValue();
	LayoutInfo.nRightSpace = m_pPropBase_Layout_RightSpace->GetValue();
	LayoutInfo.nTopSpace = m_pPropBase_Layout_TopSpace->GetValue();
	LayoutInfo.nWidth = m_pPropBase_Layout_Width->GetValue();

	return LayoutInfo;
}

// 控件显示位置和大小，这个位置是相对于附着的窗口的
void IControlBase::SetWindowRect(RECT CtrlInWndRct, bool bRememberToProp)
{
	m_RectInWindow = CtrlInWndRct;

	if (bRememberToProp)
	{
		if (m_pWindowBase != NULL && m_pPropBase_Layout_Width != NULL && m_pPropBase_Layout_Height != NULL && m_pPropBase_Layout_Layout != NULL
			&& m_pPropBase_Layout_LeftSpace != NULL && m_pPropBase_Layout_RightSpace != NULL
			&& m_pPropBase_Layout_TopSpace != NULL && m_pPropBase_Layout_BottomSpace != NULL)
		{
			m_pPropBase_Layout_Width->SetValue(RECT_WIDTH(m_RectInWindow));
			m_pPropBase_Layout_Height->SetValue(RECT_HEIGHT(m_RectInWindow));

			RECT ParentRct;
			INIT_RECT(ParentRct);
			IControlBase* pParentCtrl = GetParentControl();
			if (pParentCtrl != NULL)
				ParentRct = pParentCtrl->GetWindowRect();
			else
				ParentRct = m_pWindowBase->GetClientRect();

			m_pPropBase_Layout_LeftSpace->SetValue(m_RectInWindow.left - ParentRct.left);
			m_pPropBase_Layout_TopSpace->SetValue(m_RectInWindow.top - ParentRct.top);
			m_pPropBase_Layout_RightSpace->SetValue(ParentRct.right - m_RectInWindow.right);
			m_pPropBase_Layout_BottomSpace->SetValue(ParentRct.bottom - m_RectInWindow.bottom);
		}
	}

	RedrawControl();
	if (m_pCtrlMemDc != NULL)
		m_pCtrlMemDc->Delete();
	OnSize();
}

RECT IControlBase::GetWindowRect()
{
	return m_RectInWindow;
}

RECT IControlBase::GetClientRect()
{
	RECT ClientRct;
	INIT_RECT(ClientRct);
	ClientRct.right = RECT_WIDTH(m_RectInWindow);
	ClientRct.bottom = RECT_HEIGHT(m_RectInWindow);
	return ClientRct;
}

// 移动控件，会将最终的位置信息存为布局信息
// 参数：CtrlInWndRct控件位于窗口的位置
// bSetChild：是否递归设置子控件
void IControlBase::SetWindowRectLayoutWithChild(RECT CtrlInWndRct, bool bSetChild)
{
	if (m_pWindowBase == NULL || m_pPropBase_Layout_Width == NULL || m_pPropBase_Layout_Height == NULL || m_pPropBase_Layout_Layout == NULL
		|| m_pPropBase_Layout_LeftSpace == NULL || m_pPropBase_Layout_RightSpace == NULL
		|| m_pPropBase_Layout_TopSpace == NULL || m_pPropBase_Layout_BottomSpace == NULL)
		return;

	m_RectInWindow = CtrlInWndRct;

	m_pPropBase_Layout_Width->SetValue(RECT_WIDTH(m_RectInWindow));
	m_pPropBase_Layout_Height->SetValue(RECT_HEIGHT(m_RectInWindow));

	RECT ParentRct;
	INIT_RECT(ParentRct);
	IControlBase* pParentCtrl = GetParentControl();
	if (pParentCtrl != NULL)
		ParentRct = pParentCtrl->GetWindowRect();
	else
		ParentRct = GetOwnerWindow()->GetClientRect();

	// 设置本控件相对父控件的布局位置
	m_pPropBase_Layout_LeftSpace->SetValue(m_RectInWindow.left - ParentRct.left);
	m_pPropBase_Layout_TopSpace->SetValue(m_RectInWindow.top - ParentRct.top);
	m_pPropBase_Layout_RightSpace->SetValue(ParentRct.right - m_RectInWindow.right);
	m_pPropBase_Layout_BottomSpace->SetValue(ParentRct.bottom - m_RectInWindow.bottom);

	SetWindowRect(m_RectInWindow);

	if (bSetChild)
		SetChildControlLayoutPostion(&m_ChildCtrlsVec);
}

// 控件显示位置和大小，会根据布局信息连带修改子控件的位置
void IControlBase::SetChildControlLayoutPostion(CHILD_CTRLS_VEC* pVec)
{
	if (m_pWindowBase == NULL || pVec == NULL)
		return;

	ListObj* pNext = NULL;
	for (ListObj* pItem = pVec->TopObj(); pItem != NULL; pItem = pNext)
	{
		pNext = pVec->NextObj(pItem);
		IControlBase* pCtrl = (IControlBase *)pItem;
		if (pCtrl == NULL || pCtrl->GetParentControl() == NULL)
			continue;

		RECT ParentRctInWnd = pCtrl->GetParentControl()->GetWindowRect();

		// 计算当前控件的位置
		m_pWindowBase->SetControlWindowLayoutPostion(pCtrl, ParentRctInWnd);
		pCtrl->RedrawControl();

		// 设置子控件的位置
		SetChildControlLayoutPostion(pCtrl->GetChildControlsVec());
	}
}

// 是否为无绘制控件
void IControlBase::PP_SetNoDrawControl(bool bIsNoDrawCtrl)
{
	if (m_pPropBase_NoDrawCtrl == NULL)
		return;

	m_pPropBase_NoDrawCtrl->SetValue(bIsNoDrawCtrl);
}

bool IControlBase::PP_GetNoDrawControl()
{
	if (m_pPropBase_NoDrawCtrl == NULL)
		return false;

	return m_pPropBase_NoDrawCtrl->GetValue();
}

// 是否使用临时内存绘制
bool IControlBase::PP_IsUseTempDrawMem()
{
	if (m_pPropBase_Draw_TempDrawMem == NULL)
		return false;

	return m_pPropBase_Draw_TempDrawMem->GetValue();
}

// 是否使用临时内存绘制
void IControlBase::PP_SetUseTempDrawMem(bool bUseTempMem)
{
	if (m_pPropBase_Draw_TempDrawMem == NULL)
		return;

	m_pPropBase_Draw_TempDrawMem->SetValue(bUseTempMem);
}

// 设置控件提示信息
void IControlBase::PP_SetTipsInfomation(LPCWSTR pszTips)
{
	if (m_pPropTips_TipsData == NULL)
		return;

	if (pszTips == NULL)
	{
		m_pPropTips_TipsData->SetString(L"");
		return;
	}

	m_pPropTips_TipsData->SetString(pszTips);
}

LPCWSTR IControlBase::PP_GetTipsInfomation()
{
	if (m_pPropTips_TipsData == NULL)
		return NULL;

	return m_pPropTips_TipsData->GetString();
}

void IControlBase::PP_SetTipsBaloon(bool bBaloon)
{
	if (m_pPropTips_Baloon == NULL)
		return;

	m_pPropTips_Baloon->SetValue(bBaloon);
}

bool IControlBase::PP_GetTipsBaloon()
{
	if (m_pPropTips_Baloon == NULL)
		return NULL;

	return m_pPropTips_Baloon->GetValue();
}

void IControlBase::PP_SetTipsShowTime(int nTime)
{
	if (m_pPropTips_ShowTimes == NULL)
		return;

	m_pPropTips_ShowTimes->SetValue(nTime);
}

int IControlBase::PP_GetTipsShowTime()
{
	if (m_pPropTips_ShowTimes == NULL)
		return NULL;

	return m_pPropTips_ShowTimes->GetValue();
}

bool IControlBase::IsNeedSystemTips()
{
	LPCWSTR pszTips = PP_GetTipsInfomation();
	if (pszTips == NULL || wcslen(pszTips) <= 0)
		return false;

	return (PP_GetTipsShowTime() > 0);
}

void IControlBase::PP_SetTipsShowInUserDefPos(bool bBaloon)
{
	if (m_pPropTips_ShowInUserDefPos == NULL)
		return;

	m_pPropTips_ShowInUserDefPos->SetValue(bBaloon);
}

bool IControlBase::PP_GetTipsShowInUserDefPos()
{
	if (m_pPropTips_ShowInUserDefPos == NULL)
		return NULL;

	return m_pPropTips_ShowInUserDefPos->GetValue();
}

void IControlBase::ShowSystemTips()
{
	if (!IsNeedSystemTips() || m_pWindowBase == NULL)
		return;

	m_pWindowBase->ShowSystemTips(PP_GetTipsInfomation(), PP_GetTipsBaloon(), PP_GetTipsShowTime());
}

void IControlBase::ShowDefaultSystemTips()
{
	if (!IsNeedSystemTips() || PP_GetTipsShowInUserDefPos())
		return;

	ShowSystemTips();
}

// 取得控件自己创建的属性
IPropertyGroup* IControlBase::GetCtrlPrivatePropGroup()
{
	return m_pPropGroupCtrlDefs;
}

// 取得控件共有创建的属性
IPropertyGroup* IControlBase::GetCtrlPublicPropGroup()
{
	return m_pPropGroupBase;
}

// TabOrder属性
void IControlBase::PP_SetControlTabOrder(int nTabOrder)
{
	if (nTabOrder <= NOT_SET_TAB_FOCUS)
		nTabOrder = NOT_SET_TAB_FOCUS;

	if (m_pPropBase_TabOrder != NULL)
		m_pPropBase_TabOrder->SetValue(nTabOrder);
}

int IControlBase::PP_GetControlTabOrder()
{
	if (m_pPropBase_TabOrder == NULL)
		return NOT_SET_TAB_FOCUS;

	int nTabOrder = m_pPropBase_TabOrder->GetValue();
	if (nTabOrder <= NOT_SET_TAB_FOCUS)
		return NOT_SET_TAB_FOCUS;

	return nTabOrder;
}

// 销毁设计时态使用的属性，减少运行时态内存占用
void IControlBase::ClearDesignModePropetry()
{
	return;
	// 还需要完善，先增加接口
	if (m_pSkinPropMgr == NULL || m_pWindowBase == NULL || m_pWindowBase->IsDesignMode() || m_pPropGroupBase == NULL || m_pPropGroupBase->GetPropVec() == NULL)
		return;

	GROUP_PROP_VEC* pPropVec = m_pPropGroupBase->GetPropVec();

	pPropVec->DelObj(m_pPropBase_Name);
	pPropVec->DelObj(m_pPropBase_TypeName);
	pPropVec->DelObj(m_pPropBase_Lock);

	m_pSkinPropMgr->ReleaseIPropertyBase(m_pWindowBase->GetSkinFileItem(), m_pPropBase_Name);
	m_pSkinPropMgr->ReleaseIPropertyBase(m_pWindowBase->GetSkinFileItem(), m_pPropBase_TypeName);
	m_pSkinPropMgr->ReleaseIPropertyBase(m_pWindowBase->GetSkinFileItem(), m_pPropBase_Lock);

	m_pPropBase_Name = NULL;
	m_pPropBase_TypeName = NULL;
	m_pPropBase_Lock = NULL;
}

// 是否为默认回车键接受函数
bool IControlBase::PP_IsDefaultEnterCtrl()
{
	if (m_pPropBase_DefaultEnterCtrl == NULL)
		return false;
	
	return m_pPropBase_DefaultEnterCtrl->GetValue();
}

void IControlBase::PP_SetDefaultEnterCtrl(bool bIsDftEnterCtrl)
{
	if (m_pPropBase_DefaultEnterCtrl == NULL)
		return;

	m_pPropBase_DefaultEnterCtrl->SetValue(bIsDftEnterCtrl);
}

// 设置动画时间：每一帧动画的时间间隔，单位：毫秒
void IControlBase::PP_SetCtrlAnimationFrameTime(int nTime)
{
	if (m_pPropBase_Animation_FrameTime == NULL)
		return;

	return m_pPropBase_Animation_FrameTime->SetValue(nTime);
}

int IControlBase::PP_GetCtrlAnimationFrameTime()
{
	if (m_pPropBase_Animation_FrameTime == NULL)
		return 0;

	return m_pPropBase_Animation_FrameTime->GetValue();
}

// 设置动画方向：0：正向动画；1：反向动画
void IControlBase::PP_SetCtrlAnimationDirection(int nDirection)
{
	if (m_pPropBase_Animation_Direction == NULL || nDirection < 0 || nDirection > 1)
		return;

	m_pPropBase_Animation_Direction->SetSelect(nDirection);
}

int IControlBase::PP_GetCtrlAnimationDirection()
{
	if (m_pPropBase_Animation_Direction == NULL)
		return 0;

	return m_pPropBase_Animation_Direction->GetSelect();
}
