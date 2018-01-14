
#pragma once
#include "..\..\Inc\Control\ICtrlRollAdvertisement.h"
#include <vector>
using namespace ATL;


class ICtrlRollAdvertisementImpl : public ICtrlRollAdvertisement
{
public:
	ICtrlRollAdvertisementImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlRollAdvertisementImpl();

	virtual bool Clear();
	virtual bool AddAdvertisement(ROLL_ADVERTISEMENT_ITEM &AdvertisementeInfo);
	virtual bool IsEmpty();
	virtual void SetShowTime(int nTime);

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

	// 设置控件光标
	virtual IPropertyCursor* OnSetCursor(POINT pt);
	// 定时器
	virtual void OnTimer(UINT nTimerId);
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 鼠标是否落在了控件指定的RGN区域，默认返回true，整个控件就是RGN区域
	virtual bool OnCheckMouseInRgn(POINT pt, INT nMouseMsgId);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

	// 鼠标进入
	virtual void OnMouseEnter(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);


private:
	void BeginTimer();
	void EndTimer();
	void SetCurrentAdvertisementeInfo(int nNum);
	bool GetCurrentAdvertisementInfo(ROLL_ADVERTISEMENT_ITEM **ppAdvertisementeInfo);
	bool GetAdvertisementInfo(ROLL_ADVERTISEMENT_ITEM **ppAdvertisementeInfo, int nNum);
	void KillGunDongTimer();
	void BeginGunDongTimer();
	void DrawCurrentAdvertisement(CDrawingBoard &DrawBoard, RECT DrawRct);
	bool CopyStringData(WCHAR* pszFrom, WCHAR** ppszTo);
	bool ReleaseStringData(WCHAR** ppszRelease);

private:
	IPropertyCursor* m_pPropCursor;
	IPropertyFont* m_pPropFont_Normal;
	IPropertyFont* m_pPropFont_Hover;
	IPropertyFont* m_pPropFont_Down;
	IPropertyInt* m_pPropGunDongTimeCtns;

	ROLL_ADVERTISEMENT_VEC m_AdvertisementVec;
	int m_nOldAdvertisement;
	int m_nCurrentAdvertisenment;
	int m_nChangeTimerId;
	int m_nTimeCtns;
	bool m_bIsLButtonDown;
	RECT m_TextRct;

	int m_nAnimationTimerId;
	RECT m_OldAdvertisementRct;
	RECT m_CurrentAdvertisementRct;
};
