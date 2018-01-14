
#include "StdAfx.h"
#include "ICtrlMacDockBarImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"
#include "..\..\Inc\ICommonFun.h"


ICtrlMacDockBar::ICtrlMacDockBar(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlMacDockBarImpl::ICtrlMacDockBarImpl(IUiFeatureKernel *pUiKernel) : ICtrlMacDockBar(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_MAC_DOCK_BAR);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	INIT_RECT(m_BkShowRct);

	m_nItemSpace = 0;
	m_nTimerId = 0;
	m_nMouseSelNo = -1;
	m_DockItemVec.clear();
	m_nCurrentItemSize.cx = m_nCurrentItemSize.cy = 0;

	m_pPropItemSpace = NULL;
	m_pPropBkTopSpace = NULL;
	m_pPropItemAnimationSize = NULL;
	m_pPropBackgroundImage = NULL;
	m_pPropItemBaseSize = NULL;
	m_pPropItemMaxSize = NULL;
	m_pPropItemLevel = NULL;
	m_pPropItemBottomSpace = NULL;
	m_pPropItemLeftSpace = NULL;
	m_pPropItemRightSpace = NULL;
	m_pPropMaxItemNum = NULL;
}

ICtrlMacDockBarImpl::~ICtrlMacDockBarImpl()
{
}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlMacDockBarImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

void ICtrlMacDockBarImpl::ReleaseDockItem()
{
	return;
	for (DOCK_ITEM_VEC::iterator pDockItem = m_DockItemVec.begin(); pDockItem != m_DockItemVec.end(); pDockItem++)
	{
		DOCK_ITEM *pItem = *pDockItem;
		if (pItem == NULL)
			continue;

		m_pSkinPropMgr->DeleteLocalImageBase(pItem->pDockImage);
		SAFE_DELETE(pItem);
	}
	m_DockItemVec.clear();
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlMacDockBarImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	m_pPropBackgroundImage = (IPropertyImage*)CreatePropetry(NULL, OTID_IMAGE, "BackgroundImage", "DockBar背景图片");
	m_pPropBkTopSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "BkTopSpace", "背景图片距离控件上方的绘制距离");

	m_pPropItemSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "ItemSpace", "Item 之间的横向间距");
	m_pPropMaxItemNum = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "MaxItemNum", "最多的Item数量");

	m_pPropItemBaseSize = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "BaseSize", "Item显示的最基本尺寸");
	m_pPropItemMaxSize = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "MaxSize", "放大后最大的Item的尺寸");

	m_pPropItemLevel = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "LevelSize", "每一级别的Item的尺寸差");
	m_pPropItemAnimationSize = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "AnimationSize", "每一次动画渐变的像素值");

	m_pPropItemBottomSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "BottomSpace", "Item距离下方的边距");
	m_pPropItemLeftSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "LeftSpace", "最左侧的Item距离左侧的边距");
	m_pPropItemRightSpace = (IPropertyInt*)CreatePropetry(NULL, OTID_INT, "RightSpace", "最右侧Item距离右侧的边距");

	return true;
}

// 初始化控件
void ICtrlMacDockBarImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlMacDockBarImpl::OnFinalCreate()
{
	m_nItemSpace = GetIntValue(m_pPropItemSpace);
}

// 销毁控件
void ICtrlMacDockBarImpl::OnDestroy()
{
	ReleaseDockItem();
}

// 鼠标进入
void ICtrlMacDockBarImpl::OnMouseEnter(POINT pt)
{
	if (m_nTimerId == 0 && m_pWindowBase != NULL)
		m_nTimerId = m_pWindowBase->SetTimer(5);
}

// 鼠标移出
void ICtrlMacDockBarImpl::OnMouseLeave(POINT pt)
{
	m_nMouseSelNo = -1;
	OnMouseEnter(pt);
}

int ICtrlMacDockBarImpl::GetIntValue(IPropertyInt* pIntProp)
{
	int nRet = -1;

	if (pIntProp != NULL)
		nRet = pIntProp->GetValue();

	return nRet;
}

// 定时器
void ICtrlMacDockBarImpl::OnTimer(UINT nTimerId)
{
	if (m_nTimerId != 0 && m_nTimerId == nTimerId)
		SetItemPostion(true, false);
}

void ICtrlMacDockBarImpl::SetItemPostion(bool bRedraw, bool bNeedReset)
{
	// 先计算每一个item的尺寸
	bool bNeedDraw = false;

	int nBaseSize = GetIntValue(m_pPropItemBaseSize);
	int nMaxSize = GetIntValue(m_pPropItemMaxSize);
	int nLevelCha = GetIntValue(m_pPropItemLevel);
	int nAnimationSize = GetIntValue(m_pPropItemAnimationSize);

	int nAllItemWidth = 0;
	for (int nItemNo = 0; nItemNo < (int)m_DockItemVec.size(); nItemNo++)
	{
		DOCK_ITEM *pItem = m_DockItemVec[nItemNo];
		if (pItem == NULL)
			continue;

		int nItemSize = nBaseSize;
		if (m_nMouseSelNo != -1)
		{
			int nLevel = m_nMouseSelNo - nItemNo;
			int nLevelItemSize = nMaxSize - abs(nLevelCha * nLevel);
			nItemSize = max(nLevelItemSize, nBaseSize);
		}

		int nCurrentSize = RECT_WIDTH(pItem->itemRct);
		if (nCurrentSize <= 0)
		{
			nCurrentSize = nBaseSize;
			pItem->itemRct.right = pItem->itemRct.bottom = nBaseSize;
		}

		if (nCurrentSize != nItemSize)
		{
			bNeedDraw = true;
			if (nCurrentSize > nItemSize)
			{
				nCurrentSize -= nAnimationSize;
				if (nCurrentSize < nItemSize)
					nCurrentSize = nItemSize;
			}
			else
			{
				nCurrentSize += nAnimationSize;
				if (nCurrentSize > nItemSize)
					nCurrentSize = nItemSize;
			}

			pItem->itemRct.right = pItem->itemRct.left + nCurrentSize;
		}

		nAllItemWidth += RECT_WIDTH(pItem->itemRct);

		if (nItemNo != (int)m_DockItemVec.size() - 1)
			nAllItemWidth += m_nItemSpace;
	}

	RECT wndRct = this->GetClientRect();
	int nBottomSpace = GetIntValue(m_pPropItemBottomSpace);
	int nLeftSpace = (RECT_WIDTH(wndRct) - nAllItemWidth) / 2;

	m_BkShowRct = wndRct;
	m_BkShowRct.left += nLeftSpace;
	m_BkShowRct.right -= nLeftSpace;
	m_BkShowRct.top += (RECT_HEIGHT(wndRct) / 2);

	// 不需要绘制
	if (!bNeedReset && !bNeedDraw)
	{
		m_pWindowBase->KillTimer(m_nTimerId);
		m_nTimerId = 0;
		return;
	}

	// 计算每一个item的位置
	wndRct.left += nLeftSpace;
	wndRct.bottom -= nBottomSpace;

	for (int nItemNo = 0; nItemNo < (int)m_DockItemVec.size(); nItemNo++)
	{
		DOCK_ITEM *pItem = m_DockItemVec[nItemNo];
		if (pItem == NULL)
			continue;

		int nItemSize = RECT_WIDTH(pItem->itemRct);

		pItem->itemRct.left = wndRct.left;
		pItem->itemRct.right = pItem->itemRct.left + nItemSize;
		pItem->itemRct.bottom = wndRct.bottom;
		pItem->itemRct.top = pItem->itemRct.bottom - nItemSize;

		wndRct.left = pItem->itemRct.right + m_nItemSpace;
	}

	this->RedrawControl(bRedraw);
}

// 此算法是自动计算各个 item 之间的间距，不足的是右侧的item在鼠标快速移动的时候会有一些左右抖动
// 原因是 item 之间的缝隙的差值和 item 增大的差值不匹配导致的。
void ICtrlMacDockBarImpl::SetItemPostion1(bool bRedraw, bool bNeedReset)
{
	// 1.先计算每一个item的尺寸
	bool bNeedDraw = false;

	int nBaseSize = GetIntValue(m_pPropItemBaseSize);
	int nMaxSize = GetIntValue(m_pPropItemMaxSize);
	int nLevelCha = GetIntValue(m_pPropItemLevel);
	int nAnimationSize = GetIntValue(m_pPropItemAnimationSize);

	int nAllItemWidth = 0;
	for (int nItemNo = 0; nItemNo < (int)m_DockItemVec.size(); nItemNo++)
	{
		DOCK_ITEM *pItem = m_DockItemVec[nItemNo];
		if (pItem == NULL)
			continue;

		int nItemSize = nBaseSize;
		if (m_nMouseSelNo != -1)
		{
			int nLevel = m_nMouseSelNo - nItemNo;
			int nLevelItemSize = nMaxSize - abs(nLevelCha * nLevel);
			nItemSize = max(nLevelItemSize, nBaseSize);
		}

		int nCurrentSize = RECT_WIDTH(pItem->itemRct);
		if (nCurrentSize <= 0)
		{
			nCurrentSize = nBaseSize;
			pItem->itemRct.right = pItem->itemRct.bottom = nBaseSize;
		}

		if (nCurrentSize != nItemSize)
		{
			bNeedDraw = true;
			if (nCurrentSize > nItemSize)
			{
				nCurrentSize -= nAnimationSize;
				if (nCurrentSize < nItemSize)
					nCurrentSize = nItemSize;
			}
			else
			{
				nCurrentSize += nAnimationSize;
				if (nCurrentSize > nItemSize)
					nCurrentSize = nItemSize;
			}

			pItem->itemRct.right = pItem->itemRct.left + nCurrentSize;
		}

		nAllItemWidth += RECT_WIDTH(pItem->itemRct);
	}

	// 不需要绘制
	if (!bNeedReset && !bNeedDraw)
	{
		m_pWindowBase->KillTimer(m_nTimerId);
		m_nTimerId = 0;
		return;
	}

	RECT wndRct = this->GetClientRect();

	int nBottomSpace = GetIntValue(m_pPropItemBottomSpace);
	int nLeftSpace = GetIntValue(m_pPropItemLeftSpace);
	int nRightSpace = GetIntValue(m_pPropItemRightSpace);

	// 2.计算item的间距
	m_nItemSpace = (RECT_WIDTH(wndRct) - nAllItemWidth - nLeftSpace - nRightSpace) / ((int)m_DockItemVec.size() + 1);

	// 3. 计算每一个item的位置
	wndRct.left += nLeftSpace;
	wndRct.bottom -= nBottomSpace;

	for (int nItemNo = 0; nItemNo < (int)m_DockItemVec.size(); nItemNo++)
	{
		DOCK_ITEM *pItem = m_DockItemVec[nItemNo];
		if (pItem == NULL)
			continue;

		int nItemSize = RECT_WIDTH(pItem->itemRct);

		pItem->itemRct.left = wndRct.left + m_nItemSpace;
		pItem->itemRct.right = pItem->itemRct.left + nItemSize;
		pItem->itemRct.bottom = wndRct.bottom;
		pItem->itemRct.top = pItem->itemRct.bottom - nItemSize;

		wndRct.left = pItem->itemRct.right;
	}

	this->RedrawControl(bRedraw);
}

// 绘制控件
void ICtrlMacDockBarImpl::OnPaint(CDrawingBoard &DrawBoard)
{
	if (m_pPropBackgroundImage != NULL)
		m_pPropBackgroundImage->DrawImage(DrawBoard, m_BkShowRct);

	for (DOCK_ITEM_VEC::iterator pDockItem = m_DockItemVec.begin(); pDockItem != m_DockItemVec.end(); pDockItem++)
	{
		DOCK_ITEM *pItem = *pDockItem;
		if (pItem == NULL || pItem->pDockImage == NULL)
			continue;

		pItem->pDockImage->DrawImage(DrawBoard, pItem->itemRct);
	}
}

// 鼠标移动
void ICtrlMacDockBarImpl::OnMouseMove(POINT pt)
{
	int nOldMouseSelNo = m_nMouseSelNo;
	for (m_nMouseSelNo = 0; m_nMouseSelNo < (int)m_DockItemVec.size(); m_nMouseSelNo++)
	{
		DOCK_ITEM *pItem = m_DockItemVec[m_nMouseSelNo];
		if (pItem == NULL)
			continue;

		if (pt.x >= (pItem->itemRct.left - m_nItemSpace / 2) && pt.x <= (pItem->itemRct.right + m_nItemSpace / 2))
			break;
	}

	// 没有选中的
	if (m_nMouseSelNo >= (int)m_DockItemVec.size())
		m_nMouseSelNo = -1;

	if (nOldMouseSelNo != m_nMouseSelNo)
		OnMouseEnter(pt);
}

// 移动、设置控件位置
void ICtrlMacDockBarImpl::OnSize()
{
	SetItemPostion(false, true);
}

bool ICtrlMacDockBarImpl::AppendItem(LPCWSTR pszLocalImg, bool bRedraw)
{
	if (pszLocalImg == NULL || m_pWindowBase == NULL || m_pSkinPropMgr == NULL)
		return false;

	DOCK_ITEM* pNewItem = new DOCK_ITEM;
	if (pNewItem == NULL)
		return false;

	pNewItem->pDockImage = m_pSkinPropMgr->LoadLocalImage(pszLocalImg);
	if (pNewItem->pDockImage == NULL)
	{
		SAFE_DELETE(pNewItem);
		return false;
	}

	m_DockItemVec.push_back(pNewItem);
	SetItemPostion(bRedraw, false);
	return true;
}
