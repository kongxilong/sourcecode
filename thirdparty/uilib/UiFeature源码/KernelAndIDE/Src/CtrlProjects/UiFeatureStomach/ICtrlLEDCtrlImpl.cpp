#include "StdAfx.h"
#include "ICtrlLEDCtrlImpl.h"
#include "..\..\Inc\UiFeatureDefs.h"


ICtrlLEDCtrl::ICtrlLEDCtrl(IUiFeatureKernel *pUiKernel) : ICtrlInterface(pUiKernel) {}

ICtrlLEDCtrlImpl::ICtrlLEDCtrlImpl(IUiFeatureKernel *pUiKernel) : ICtrlLEDCtrl(pUiKernel)
{
	// 添加控件，步骤3：向内核系统注册控件
	SetObjectType(CTRL_NAME_LEDCTRL);

	// 此控件不需要Tips属性，减少xml信息，优化性能，删除此行代码，重新编译控件，此控件就会支持系统Tips
	m_bHaveSysTipsProp = false;

	m_pImageList = NULL;
	m_pStrText = NULL;
	m_pStrMapIndex = NULL;
	m_pImageListCount = NULL;

}

ICtrlLEDCtrlImpl::~ICtrlLEDCtrlImpl()
{

}

// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
LRESULT ICtrlLEDCtrlImpl::ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

// 初始化控件
void ICtrlLEDCtrlImpl::OnCreate()
{
}

// 控件初始化完毕
void ICtrlLEDCtrlImpl::OnFinalCreate()
{

	CreateMapIndex();
	CreateVecIndex();
}

// 销毁控件
void ICtrlLEDCtrlImpl::OnDestroy()
{
}

// 绘制控件
void ICtrlLEDCtrlImpl::OnPaint(CDrawingBoard &DrawBoard)
{

	if (m_vecIndex.size()==0 || m_mapIndex.size()==0)
		return;

	if (m_pImageList->GetDrawingImage()==NULL)
		return;

	if(NULL == m_pStrText || m_pStrText->GetLength()==0)
		return;


	int nSize = (int)m_vecIndex.size();
	RECT rcClient = this->GetClientRect();
	
	IMAGE_BASE_PROP* pImageProp = m_pImageList->GetImageProp();
	RECT imageRect = pImageProp->RectInImage;

	RECT rcItem = rcClient;
	rcItem.right = rcItem.left;
	rcItem.left = rcItem.right - RECT_WIDTH(imageRect);
	rcItem.bottom = RECT_HEIGHT(imageRect);
	
	for (int i = 0; i < nSize; i++)
	{
		::OffsetRect(&rcItem,RECT_WIDTH(imageRect),0);

		//计算绘图的区域并画到DrawBoard上去。
		int nIndex = m_vecIndex[i];	//现在是绘制图片序列中的哪一项
		
		if (nIndex == -1 || nIndex > m_pImageListCount->GetValue())
		{
			return;
		}
		RECT rcPos = {0};
		rcPos.left = RECT_WIDTH(imageRect) * nIndex;
		rcPos.top = 0;
		rcPos.right = rcPos.left + RECT_WIDTH(imageRect);
		rcPos.bottom = rcPos.top + RECT_HEIGHT(imageRect);
		pImageProp->RectInImage = rcPos;
		m_pImageList->SetImageProp(pImageProp);

		m_pImageList->DrawImage(DrawBoard, rcItem);
	}
}

// 派生控件用于创建属于自己的控件属性
bool ICtrlLEDCtrlImpl::CreateControlPropetry(bool bIsNewCtrl)
{
	IPropertyGroup *pGroup = (IPropertyGroup*)CreatePropetry(NULL, OTID_GROUP, "LEDGroup", "LED组");
	m_pImageList = (IPropertyImage*)CreatePropetry(pGroup,OTID_IMAGE,"ImageList","一张由多个小图组成的序列图片",true);
	m_pStrText = (IPropertyString*)CreatePropetry(pGroup, OTID_STRING, "Text", "", true);
	m_pStrMapIndex = (IPropertyString*)CreatePropetry(pGroup, OTID_STRING, "MapIndex", "与图片序列一一对应的字符序列", true);
	m_pImageListCount = (IPropertyInt*)CreatePropetry(pGroup, OTID_INT, "ImageListCount", "图片一共有多少张小图组成", true);
	return true;
}


void ICtrlLEDCtrlImpl::SetText(LPCWSTR pText, bool bRedraw)
{
	if(NULL == pText)
		return;

	if(NULL == m_pStrText)
		m_pStrText = (IPropertyString*)CreatePropetry(NULL, OTID_STRING, "Text", "", true);

	if (NULL == m_pStrText)
		return;

	m_vecIndex.clear();
	m_pStrText->SetString(pText);
	int nSize = wcslen(pText);
	for (int i = 0; i< nSize; i++)
	{
		if (m_mapIndex.count(pText[i]))
		{
			m_vecIndex.push_back(m_mapIndex[pText[i]]);
		}
		else
		{
			m_vecIndex.push_back(-1);
		}
	}

	RedrawControl(bRedraw);
}


void ICtrlLEDCtrlImpl::SetIndexMap(LPCWSTR pText)
{
	if (pText == NULL || wcslen(pText)==0)
		return;
	
	m_mapIndex.clear();
	
	int nSize = wcslen(pText);
	for (int i = 0; i < nSize; i++)
	{
		m_mapIndex[pText[i]] = i;
	}
}


SIZE ICtrlLEDCtrlImpl::GetAutoSize()
{
	SIZE sz = {0,0};
	if (m_pImageList == NULL)
	{
		return sz;
	}
	//宽度 = 每个字符图片的宽度*字符数
	//高度 = 每个字符图片的高度
	IMAGE_BASE_PROP* pImageProp = m_pImageList->GetImageProp();
	RECT imageRect = pImageProp->RectInImage;
	int cx = RECT_WIDTH(imageRect) * m_vecIndex.size();
	int cy = RECT_HEIGHT(imageRect);
	sz.cx = cx;
	sz.cy = cy;
	return sz;
}


void ICtrlLEDCtrlImpl::CreateVecIndex()
{
	if(NULL == m_pStrText)
		return;
	
	m_vecIndex.clear();
	LPCWSTR pStrText = m_pStrText->GetString();
	int nSize = wcslen(pStrText);
	if (pStrText == NULL || nSize == 0)
		return;


	for (int i = 0; i< nSize; i++)
	{
		if (m_mapIndex.count(pStrText[i]))
		{
			m_vecIndex.push_back(m_mapIndex[pStrText[i]]);
		}
		else
		{
			m_vecIndex.push_back(-1);
		}
	}
}

void ICtrlLEDCtrlImpl::CreateMapIndex()
{
	if (NULL == m_pStrMapIndex)
		return;

	m_mapIndex.clear();

	LPCWSTR pMapText = m_pStrMapIndex->GetString();
	int nSize = wcslen(pMapText);
	if (pMapText == NULL || nSize == 0)
		return;

	for (int i = 0; i < nSize; i++)
	{
		m_mapIndex[pMapText[i]] = i;
	}

}
