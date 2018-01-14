
#pragma once
#include "..\..\Inc\Control\ICtrlMacDockBar.h"
#include "..\..\Inc\IPropertyInt.h"

struct DOCK_ITEM
{
	IPropertyImageBase* pDockImage;
	RECT itemRct;

	DOCK_ITEM()
	{
		pDockImage = NULL;
		INIT_RECT(itemRct);
	};
};

typedef vector<DOCK_ITEM*>	DOCK_ITEM_VEC;

class ICtrlMacDockBarImpl : public ICtrlMacDockBar
{
public:
	ICtrlMacDockBarImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlMacDockBarImpl();

	virtual bool AppendItem(LPCWSTR pszLocalImg, bool bRedraw);

protected:
	// 派生控件用于创建属于自己的控件属性
	// bIsNewCtrl：true时说明是Builder调用，新创建一个控件，需要初始化属性的各个默认值
	// false：说明整个皮肤包已经初始化完毕，属性创建完毕的时候，从xml中读取的属性值已经赋值完毕了，不能再初始化，否则xml中的值会被冲掉
	virtual bool CreateControlPropetry(bool bIsNewCtrl);
	// 初始化控件
	virtual void OnCreate();
	// 控件初始化完毕
	virtual void OnFinalCreate();
	// 销毁控件
	virtual void OnDestroy();
	// 绘制控件
	virtual void OnPaint(CDrawingBoard &DrawBoard);
	// 鼠标进入
	virtual void OnMouseEnter(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);
	// 定时器
	virtual void OnTimer(UINT nTimerId);
	// 鼠标移动
	virtual void OnMouseMove(POINT pt);
	// 移动、设置控件位置
	virtual void OnSize();
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	void ReleaseDockItem();
	int GetIntValue(IPropertyInt* pIntProp);
	void SetItemPostion(bool bRedraw, bool bNeedReset);

	// 此算法是自动计算各个 item 之间的间距，不足的是右侧的item在鼠标快速移动的时候会有一些左右抖动
	// 原因是 item 之间的缝隙的差值和 item 增大的差值不匹配导致的。
	void SetItemPostion1(bool bRedraw, bool bNeedReset);

private:
	// 背景显示的矩形
	RECT m_BkShowRct;
	// 多媒体定时器
	int m_nTimerId;
	// Item的间距
	int m_nItemSpace;
	// 当前选中的Item
	int m_nMouseSelNo;
	DOCK_ITEM_VEC m_DockItemVec;
	// 当前显示的Item的大小
	SIZE m_nCurrentItemSize;

	// 背景图片
	IPropertyImage* m_pPropBackgroundImage;

	// Item显示的最基本尺寸
	IPropertyInt* m_pPropItemBaseSize;
	// 放大后最大的Item的尺寸
	IPropertyInt* m_pPropItemMaxSize;

	// 每个 item 的固定间距
	IPropertyInt* m_pPropItemSpace;
	// 每一级别的Item的尺寸差
	IPropertyInt* m_pPropItemLevel;
	IPropertyInt* m_pPropItemAnimationSize;

	// Item距离下方的边距
	IPropertyInt* m_pPropItemBottomSpace;
	// Item距离左右两侧的边距
	IPropertyInt* m_pPropItemLeftSpace;
	IPropertyInt* m_pPropItemRightSpace;

	// 最多的Item数量
	IPropertyInt* m_pPropMaxItemNum;
	// 背景图片距离控件上方的距离
	IPropertyInt* m_pPropBkTopSpace;
};
