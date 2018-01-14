#include "StdAfx.h"
#include "ICtrlSplitBarImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"

// 鼠标按下标志
#define MOUSE_FLAG_DOWN								(0x0000001UL)

ICtrlSplitBar::ICtrlSplitBar(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlSplitBarImpl::ICtrlSplitBarImpl(IUiFeatureKernel *pUiKernel) : ICtrlSplitBar(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_SPLITBAR);
	m_bHaveSysTipsProp = false;

	m_bTracking = FALSE;
	m_bInSplit = FALSE;
	m_nSplitPos = 0;

	m_mouseFlag = 0;
	m_iDragBeam = -1;

	m_enumSplitSate = SplitStateNormal;
	for (int i = 0; i < RadioStateNum; i++)
	{
		m_pImage[i] = NULL;
		m_pGraphic[i] = NULL;
	}

	m_pIsHorz = NULL;
	m_pTopLeftMin = NULL;
	m_pRightBottomMin = NULL;
	m_pTopLeftName = NULL;
	m_pRightBottomName = NULL;
	m_pGraphLength = NULL;
	m_pSplitSize = NULL;
	m_pTopLeftSize = NULL;
	m_pRightBottomSize = NULL;


	m_pTopLeftPanel = NULL;
	m_pRightBottomPanel = NULL;

	m_pLTPriority = NULL;
	m_pRBPriority = NULL;
}

ICtrlSplitBarImpl::~ICtrlSplitBarImpl()
{

}

bool ICtrlSplitBarImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	IPropertyGroup *pImageGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "ImageGroup", "");
	IPropertyGroup *pNormalImageGroup = (IPropertyGroup*)CreatePropetry(pImageGroup,OTID_GROUP,"Normal","");
	IPropertyGroup *pHotImageGroup = (IPropertyGroup*)CreatePropetry(pImageGroup,OTID_GROUP,"Hot","");
	IPropertyGroup *pPressImageGroup = (IPropertyGroup*)CreatePropetry(pImageGroup,OTID_GROUP,"Press","");
	IPropertyGroup *pDisableImageGroup = (IPropertyGroup*)CreatePropetry(pImageGroup,OTID_GROUP,"Disable","");

	IPropertyGroup *pSplitGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "SplitGroup", "");

	m_pImage[SplitStateNormal] = (IPropertyImage*)CreatePropetry(pNormalImageGroup, OTID_IMAGE, "Image", "正常状态的背景图片");
	m_pGraphic[SplitStateNormal] = (IPropertyImage*)CreatePropetry(pNormalImageGroup, OTID_IMAGE, "Graphic", "正常状态的图标，默认水平和居中对齐");

	m_pImage[SplitStateHot] = (IPropertyImage*)CreatePropetry(pHotImageGroup, OTID_IMAGE, "Image", "高亮状态的背景图片");
	m_pGraphic[SplitStateHot] = (IPropertyImage*)CreatePropetry(pHotImageGroup, OTID_IMAGE, "Graphic", "高亮状态的图标，默认水平和居中对齐");

	m_pImage[SplitStatePressed] = (IPropertyImage*)CreatePropetry(pPressImageGroup, OTID_IMAGE, "Image", "按下状态的背景图片");
	m_pGraphic[SplitStatePressed] = (IPropertyImage*)CreatePropetry(pPressImageGroup, OTID_IMAGE, "Graphic", "按下状态的图标，默认水平和居中对齐");

	m_pImage[SplitStateDisabled] = (IPropertyImage*)CreatePropetry(pDisableImageGroup, OTID_IMAGE, "Image", "禁用状态的背景图片");
	m_pGraphic[SplitStateDisabled] = (IPropertyImage*)CreatePropetry(pDisableImageGroup, OTID_IMAGE, "Graphic", "禁用状态的图标，默认水平和居中对齐");

	m_pIsHorz = (IPropertyBool*)CreatePropetry(pSplitGroup, OTID_BOOL, "IsHorz", "是否是水平分隔条");
	m_pSplitSize = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "SplitSize", "分隔条的宽度或者高度");
	m_pTopLeftMin = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "TopLeftMin", "第一分割区域的最小值");
	m_pRightBottomMin = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "RightBottomMin", "第二分割区域的最小值");
	m_pTopLeftName = (IPropertyString*)CreatePropetry(pSplitGroup, OTID_STRING, "TopLeftName", "第一分割区域的Panel名称");
	m_pRightBottomName = (IPropertyString*)CreatePropetry(pSplitGroup, OTID_STRING, "RightBottomName", "第二分割区域的Panel名称");
	m_pTopLeftSize = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "TopLeftSize", "第一分割区域的宽度或者高度");
	m_pRightBottomSize = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "RightBottomSize", "第二分割区域的宽度或者高度");

	m_pLTPriority = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "LeftTopPriority", "第一分隔区域优先级");
	m_pRBPriority = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "RightBottomPriority", "第一分隔区域优先级");

	m_pGraphLength = (IPropertyInt*)CreatePropetry(pSplitGroup, OTID_INT, "GraphLength", "图标的长度");
	m_pCursor = (IPropertyCursor*)CreatePropetry(pSplitGroup, OTID_CURSOR, "Cursor", "拖拽时的光标");

	return TRUE;
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlSplitBarImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

void ICtrlSplitBarImpl::OnCreate()
{

}

void ICtrlSplitBarImpl::OnFinalCreate()
{
	this->RegisterControlMessage(WM_WINDOWPOSCHANGED);
}

void ICtrlSplitBarImpl::OnDestroy()
{
	this->UnRegisterControlMessage(WM_WINDOWPOSCHANGED);
}

void ICtrlSplitBarImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (IsEnable())
	{
		if (m_enumSplitSate == SplitStateDisabled)
		{
			m_enumSplitSate = SplitStateNormal;
		}
	}
	else
	{
		m_enumSplitSate = SplitStateDisabled;
	}

	SPLITBARSTATE SplitSate = m_enumSplitSate;

	RECT rcClient = GetClientRect();

	if (m_pWindowBase->IsDesignMode())
	{
		return;
	}
	RECT rcSep = rcClient;

	BOOL bColMode = (!m_pIsHorz->GetValue());
	if(bColMode)
		rcSep.right=rcClient.left;
	else
		rcSep.bottom=rcSep.top;


	RECT rcPane = m_pTopLeftPanel->GetClientRect();
	if (bColMode)
	{
		rcSep.left = rcSep.left+RECT_WIDTH(rcPane);
		rcSep.right = rcSep.left+m_pSplitSize->GetValue();
	}
	else
	{
		rcSep.top = rcSep.top + RECT_HEIGHT(rcPane);
		rcSep.bottom = rcSep.top +m_pSplitSize->GetValue();
	}

	if (m_pImage[SplitSate]!=NULL)
	{
		m_pImage[SplitSate]->DrawImage(DrawBoard,rcSep);
	}

	if (m_pGraphic[SplitSate]!=NULL)
	{
		SIZE graphicSize = m_pGraphic[SplitSate]->GetImageSize();
		if (m_pIsHorz->GetValue())
		{
			graphicSize.cx+=m_pGraphLength->GetValue();
		}
		else
		{
			graphicSize.cy+=m_pGraphLength->GetValue();
		}
		RECT rcGraphicImage = {0};
		if (!m_pIsHorz->GetValue())
		{
			rcGraphicImage.left = rcSep.left + ((rcSep.right - rcSep.left)/2 - (graphicSize.cx/2));
			rcGraphicImage.top = (rcSep.bottom - rcSep.top)/2 - (graphicSize.cy/2);
			rcGraphicImage.right = rcGraphicImage.left + graphicSize.cx;
			rcGraphicImage.bottom = rcGraphicImage.top+graphicSize.cy;
		}
		else
		{
			rcGraphicImage.left = (rcSep.right - rcSep.left)/2 - (graphicSize.cx/2);
			rcGraphicImage.top = rcSep.top+ ((rcSep.bottom - rcSep.top)/2 - (graphicSize.cy/2));
			rcGraphicImage.right = rcGraphicImage.left + graphicSize.cx;
			rcGraphicImage.bottom = rcGraphicImage.top+graphicSize.cy;
		}

		m_pGraphic[SplitSate]->DrawImage(DrawBoard,rcGraphicImage);
	}
}

void ICtrlSplitBarImpl::OnMouseMove(POINT pt)
{
	RECT rcClient = GetClientRect();

	RECT rcSep = rcClient;

	BOOL bColMode = (!m_pIsHorz->GetValue());
	if(bColMode)
		rcSep.right=rcClient.left;
	else
		rcSep.bottom=rcSep.top;


	RECT rcPane = m_pTopLeftPanel->GetClientRect();
	if (bColMode)
	{
		rcSep.left = rcSep.left+RECT_WIDTH(rcPane);
		rcSep.right = rcSep.left+m_pSplitSize->GetValue();
	}
	else
	{
		rcSep.top = rcSep.top + RECT_HEIGHT(rcPane);
		rcSep.bottom = rcSep.top +m_pSplitSize->GetValue();
	}

	if(PtInRect(&rcSep,pt))
	{
		m_bInSplit = TRUE;
	}
	else
	{
		m_bInSplit = FALSE;
	}

	if (m_bInSplit)
	{
		if (m_mouseFlag & MOUSE_FLAG_DOWN)
		{
			ChangeSplitState(SplitStatePressed);

		}
		else
		{
			ChangeSplitState(SplitStateHot);
		}
	}
	else
	{
		ChangeSplitState(SplitStateNormal);
	}
}


void ICtrlSplitBarImpl::OnMouseEnter(POINT pt)
{

}

void ICtrlSplitBarImpl::OnMouseLeave(POINT pt)
{
	m_bInSplit = FALSE;
	ChangeSplitState(SplitStateNormal);
}

void ICtrlSplitBarImpl::OnLButtonDown(POINT pt)
{
	RECT rcWindow = this->GetWindowRect();
	RECT rcBeam = rcWindow;

	BOOL bColMode = (!m_pIsHorz->GetValue());
	if(bColMode) rcBeam.right=rcBeam.left;
	else rcBeam.bottom=rcBeam.top;

	long & nLT= bColMode?rcBeam.left:rcBeam.top;
	long & nRB= bColMode?rcBeam.right:rcBeam.bottom;

	int i = 0;
	RECT rcPane = m_pTopLeftPanel->GetWindowRect();
	nLT=bColMode?rcPane.right:rcPane.bottom;
	nRB=nLT+m_pSplitSize->GetValue();
	if (::PtInRect(&rcBeam,pt))
	{
		m_iDragBeam=i;
	}

	++i;
	rcPane = m_pRightBottomPanel->GetWindowRect();
	nLT=bColMode?rcPane.right:rcPane.bottom;
	nRB=nLT+m_pSplitSize->GetValue();
	if (::PtInRect(&rcBeam,pt))
	{
		m_iDragBeam=i;
	}

	m_ptClick = pt;
	m_bTracking = TRUE;

	m_mouseFlag |= MOUSE_FLAG_DOWN;
	ChangeSplitState(SplitStatePressed);
}

void ICtrlSplitBarImpl::OnLButtonUp(POINT pt)
{
	m_iDragBeam=-1;
	m_bTracking = FALSE;
	m_mouseFlag &= ~MOUSE_FLAG_DOWN;
	if (IsMousehover())
	{
		ChangeSplitState(SplitStateHot);
	}
	else
	{
		ChangeSplitState(SplitStateNormal);
	}
}

//设置为用户自定义的布局
//如果是垂直的话，在Builder中必须计算左右容器的初始宽度，同时考虑到容器中的SplitBar的宽度，水平同理
void ICtrlSplitBarImpl::OnWindowFinalCreate()
{
	if (m_pWindowBase == NULL || m_pTopLeftName == NULL || m_pTopLeftName->GetLength()==0 ||
		m_pRightBottomName == NULL || m_pRightBottomName->GetLength()==0)
		return;

	m_pTopLeftPanel = m_pWindowBase->GetControl(m_pTopLeftName->GetString());
	m_pRightBottomPanel = m_pWindowBase->GetControl(m_pRightBottomName->GetString());
	if (m_pTopLeftPanel == NULL || m_pTopLeftPanel == NULL)
		return;

	m_pTopLeftPanel->SetControlLayoutType(CL_L_USER_DEF);
	m_pTopLeftPanel->SetControlLayoutType(CL_L_USER_DEF);
}


void ICtrlSplitBarImpl::OnSize()
{
	Relayout();
}

IPropertyCursor* ICtrlSplitBarImpl::OnSetCursor(POINT pt)
{
	if (!m_bInSplit)
	{
		return NULL;
	}
	return m_pCursor;
}


void ICtrlSplitBarImpl::SetSplitterType(bool bHorz)
{
	if (m_pIsHorz == NULL)
	{
		return;
	}
	m_pIsHorz->SetValue(bHorz);
}

void ICtrlSplitBarImpl::SetSplitterHorzMin(int nTopMin, int nBottomMin)
{
	if (m_pRightBottomMin == NULL || m_pTopLeftMin == NULL)
		return;

	m_pTopLeftMin->SetValue(nTopMin);
	m_pRightBottomMin->SetValue(nBottomMin);

	Relayout();
}

void ICtrlSplitBarImpl::SetSplitterVertMin(int nLeftMin, int nRightMin)
{
	if (m_pRightBottomMin == NULL || m_pTopLeftMin == NULL)
		return;

	m_pTopLeftMin->SetValue(nLeftMin);
	m_pRightBottomMin->SetValue(nRightMin);

	Relayout();
}

int ICtrlSplitBarImpl::GetTopLeftMin()
{
	if(m_pTopLeftMin == NULL)
		return 0;

	return m_pTopLeftMin->GetValue();
}

int ICtrlSplitBarImpl::GetBottomRightMin()
{
	if (m_pRightBottomMin == NULL)
		return 0;

	return m_pRightBottomMin->GetValue();
}

int ICtrlSplitBarImpl::GetGraphLength()
{
	if (m_pGraphLength == NULL)
		return 0;

	return m_pGraphLength->GetValue();
}

void ICtrlSplitBarImpl::SetGraphLength(int nGraphLength, bool bRedraw)
{
	m_pGraphLength->SetValue(nGraphLength);
	this->RedrawControl(bRedraw);
}

void ICtrlSplitBarImpl::ChangeSplitState(SPLITBARSTATE state)
{
	if(state != m_enumSplitSate)
	{
		m_enumSplitSate = state;
		RedrawControl(true);
	}
}

void ICtrlSplitBarImpl::OnMouseDragging(POINT pt)
{
	//如果不在拖动状态的话
	if (!m_bTracking)
		return;

	//如果两个布局子控件为空的话
	if (m_pTopLeftPanel == NULL || m_pRightBottomPanel == NULL)
	{
		return;
	}

	RECT rcPane1 = m_pTopLeftPanel->GetWindowRect();
	RECT rcPane2 = m_pRightBottomPanel->GetWindowRect();

	POINT ptMove;
	ptMove.x = pt.x - m_ptClick.x;
	ptMove.y = pt.y - m_ptClick.y;

	if (!m_pIsHorz->GetValue())
	{
		rcPane1.right+=ptMove.x;
		rcPane2.left+=ptMove.x;
		if(ptMove.x<0)
		{
			//decrease left pane
			int iTest=RECT_WIDTH(rcPane1)- m_pTopLeftMin->GetValue();
			if(iTest<0)
			{
				rcPane1.right-=iTest;
				rcPane2.left-=iTest;
			}
		}
		else
		{
			//decrease right pane
			int iTest=RECT_WIDTH(rcPane2)-m_pRightBottomMin->GetValue();
			if(iTest<0)
			{
				rcPane1.right+=iTest;
				rcPane2.left+=iTest;
			}
		}
	}
	else
	{
		rcPane1.bottom+=ptMove.y;
		rcPane2.top+=ptMove.y;
		if(ptMove.y<0)
		{
			//decrease top pane
			int iTest=RECT_HEIGHT(rcPane1)-m_pTopLeftMin->GetValue();
			if(iTest<0)
			{
				rcPane1.bottom-=iTest;
				rcPane2.top-=iTest;
			}
		}
		else
		{
			//decrease bottom pane
			int iTest=RECT_HEIGHT(rcPane2)-m_pRightBottomMin->GetValue();
			if(iTest<0)
			{
				rcPane1.bottom+=iTest;
				rcPane2.top+=iTest;
			}
		}
	}

	m_pTopLeftPanel->SetWindowRect(rcPane1);
	m_pTopLeftPanel->SetWindowRectLayoutWithChild(rcPane1);
	m_pRightBottomPanel->SetWindowRect(rcPane2);
	m_pRightBottomPanel->SetWindowRectLayoutWithChild(rcPane2);

	BOOL bColMode = (!m_pIsHorz->GetValue());

	if(m_pLTPriority->GetValue()<=m_pRBPriority->GetValue())
	{
		m_pTopLeftSize->SetValue(bColMode?RECT_WIDTH(rcPane1):RECT_HEIGHT(rcPane1));
	}
	else
	{
		m_pRightBottomSize->SetValue(bColMode?RECT_WIDTH(rcPane2):RECT_HEIGHT(rcPane2));
	}

	m_ptClick = pt;
}


// 鼠标拖动操作结束，抬起鼠标。pDragCtrl：拖拽来源控件
void ICtrlSplitBarImpl::OnDragStop(IControlBase* pDragCtrl, POINT pt, WPARAM wParam, LPARAM lParam)
{

}

int ICtrlSplitBarImpl::FunComp( const void * p1,const void * p2 )
{
	const PANEORDER *pPane1=(PANEORDER*)p1;
	const PANEORDER *pPane2=(PANEORDER*)p2;

	return pPane1->pr - pPane2->pr;
}

void ICtrlSplitBarImpl::Relayout()
{
	if (m_pTopLeftPanel == NULL || m_pTopLeftPanel == NULL)
		return;

	int nTotalIdeal=0,nTotalMin=0;
	nTotalIdeal+=m_pTopLeftSize->GetValue();
	nTotalIdeal+=m_pRightBottomSize->GetValue();
	nTotalMin+=m_pTopLeftMin->GetValue();
	nTotalMin+=m_pRightBottomMin->GetValue();

	RECT rcClient = this->GetWindowRect();	//原为GetClientRect
	int nInter = m_pSplitSize->GetValue();
	BOOL bColMode = !(m_pIsHorz->GetValue());
	int nSize = bColMode ? RECT_WIDTH(rcClient) : RECT_HEIGHT(rcClient);

	RECT rcPane = rcClient;
	if(bColMode)
	{
		rcPane.right=rcPane.left;
	}
	else
	{
		rcPane.bottom=rcPane.top;
	}

	long & nLT= bColMode ? rcPane.left : rcPane.top;
	long & nRB= bColMode ? rcPane.right : rcPane.bottom;

	if(nTotalMin+nInter > nSize)
	{
		nRB+=m_pTopLeftMin->GetValue();
		m_pTopLeftPanel->SetWindowRect(rcPane);
		m_pTopLeftPanel->SetWindowRectLayoutWithChild(rcPane);
		nLT=nRB+m_pSplitSize->GetValue();
		nRB=nLT;

		nRB+=m_pRightBottomMin->GetValue();
		m_pRightBottomPanel->SetWindowRect(rcPane);
		m_pRightBottomPanel->SetWindowRectLayoutWithChild(rcPane);
		nLT=nRB+m_pSplitSize->GetValue();
		nRB=nLT;

	}
	else if (nTotalIdeal+nInter<nSize)
	{
		int iLowest=0,nPriority=-1;
		if (m_pLTPriority->GetValue() > m_pRBPriority->GetValue())
		{
			iLowest=0;
		}
		else
		{
			iLowest=1;
		}

		if (iLowest != 0)
		{
			nRB+=m_pTopLeftSize->GetValue();
		}
		else
		{
			nRB+=m_pTopLeftSize->GetValue()+nSize-(nTotalIdeal+nInter);
		}
		m_pTopLeftPanel->SetWindowRect(rcPane);
		m_pTopLeftPanel->SetWindowRectLayoutWithChild(rcPane);
		nLT=nRB+m_pSplitSize->GetValue();
		nRB=nLT;

		if (iLowest !=1)
		{
			nRB+=m_pRightBottomSize->GetValue();
		}
		else
		{
			nRB+=m_pRightBottomSize->GetValue()+nSize-(nTotalIdeal+nInter);
		}
		m_pRightBottomPanel->SetWindowRect(rcPane);
		m_pRightBottomPanel->SetWindowRectLayoutWithChild(rcPane);
		nLT=nRB+m_pSplitSize->GetValue();
		nRB=nLT;
	}
	else
	{
		PANEORDER *pnPriority=new PANEORDER[2];
		int *pPaneSize=new int [2];
		pnPriority[0].idx = 0;
		pnPriority[0].pPane = m_pTopLeftPanel;
		pnPriority[0].pr = m_pLTPriority->GetValue();
		pnPriority[0].min = m_pTopLeftMin->GetValue();
		pnPriority[0].sz = m_pTopLeftSize->GetValue();

		pnPriority[1].idx = 1;
		pnPriority[1].pPane = m_pRightBottomPanel;
		pnPriority[1].pr = m_pRBPriority->GetValue();
		pnPriority[1].min = m_pRightBottomMin->GetValue();
		pnPriority[1].sz = m_pRightBottomSize->GetValue();

		qsort(pnPriority,2,sizeof(PANEORDER),FunComp);

		int nRemainSize=nSize-nInter;
		BOOL bMinimize=FALSE;
		//////////////////////////////////////////////////////////////////////////
		int i = 0;
		if(!bMinimize)
		{
			int nRequiredMin=0;
			for(int j=i+1; j<2; j++)
			{
				nRequiredMin+=pnPriority[j].min;
			}
			if(nRequiredMin<nRemainSize-pnPriority[i].sz)
			{
				pPaneSize[pnPriority[i].idx]=pnPriority[i].sz;
			}
			else
			{
				pPaneSize[pnPriority[i].idx]=nRemainSize-nRequiredMin;
				bMinimize=TRUE;
			}
		}
		else
		{
			pPaneSize[pnPriority[i].idx]=pnPriority[i].min;
		}
		nRemainSize-=pPaneSize[pnPriority[i].idx];

		++i;

		if(!bMinimize)
		{
			int nRequiredMin=0;
			for(int j=i+1; j<2; j++)
			{
				nRequiredMin+=pnPriority[j].min;
			}
			if(nRequiredMin<nRemainSize-pnPriority[i].sz)
			{
				pPaneSize[pnPriority[i].idx]=pnPriority[i].sz;
			}
			else
			{
				pPaneSize[pnPriority[i].idx]=nRemainSize-nRequiredMin;
				bMinimize=TRUE;
			}
		}
		else
		{
			pPaneSize[pnPriority[i].idx]=pnPriority[i].min;
		}
		nRemainSize-=pPaneSize[pnPriority[i].idx];

		//////////////////////////////////////////////////////////////////////////

		nRB+=pPaneSize[0];
		m_pTopLeftPanel->SetWindowRect(rcPane);
		m_pTopLeftPanel->SetWindowRectLayoutWithChild(rcPane);
		nLT=nRB+m_pSplitSize->GetValue();
		nRB=nLT;

		nRB+=pPaneSize[1];
		m_pRightBottomPanel->SetWindowRect(rcPane);
		m_pRightBottomPanel->SetWindowRectLayoutWithChild(rcPane);
		nLT=nRB+m_pSplitSize->GetValue();
		nRB=nLT;

		delete [] pPaneSize;
		delete [] pnPriority;
	}

}

int ICtrlSplitBarImpl::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
{
	Relayout();
	return S_OK;
}
