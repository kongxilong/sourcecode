
#pragma once
#include "..\..\Inc\Control\ICtrlPanel.h"

class ICtrlPanelImpl : public ICtrlPanel
{
public:
	ICtrlPanelImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlPanelImpl();

	virtual IPropertyImageBase* GetBackgroundImage();
	virtual bool SetBackgroundImage(IPropertyImageBase* pNewImageBase);
	// 设置背景图
	virtual bool SetBackgroundImage(LPCWSTR pszImagePath, bool bReload);
	// 设置动画类型
	virtual void SetImageAnimation(PANEL_ANIMATION paType);
	virtual PANEL_ANIMATION GetImageAnimation();
	// 回复到皮肤包中的图片
	virtual void SetToDefault(bool bRedraw, bool bDeleteOld = true);
	// 是否显示手型指针开关
	virtual void SetIsShowCursor(bool bIsShow);
	// 设置背景图
	virtual bool SetBackgroundImage(HBITMAP hbmp, bool bReload);
	// 设置关键字，方便做容器的时候排序用
	virtual void SetKey(LPCWSTR key);
	// getkey
	virtual LPCWSTR GetKey();

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
	// 绘制控件的动画，如果需要重新刷新界面，返回true，否则返回false
	// 此函数由定时器触发，每100毫秒触发一次
	virtual bool OnDrawAnimation();
	// 设置控件光标
	virtual IPropertyCursor* OnSetCursor(POINT pt);

	// 鼠标进入
	virtual void OnMouseEnter(POINT pt);
	// 鼠标移出
	virtual void OnMouseLeave(POINT pt);
	// 鼠标左键点击
	virtual void OnLButtonDown(POINT pt);
	// 鼠标左键双击
	virtual void OnLButtonDbClick(POINT pt);
	// 鼠标左键抬起
	virtual void OnLButtonUp(POINT pt);
	// 鼠标右键点击
	virtual void OnRButtonDown(POINT pt);
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	void DrawPhotoFrame(CDrawingBoard &DrawBoard);
	bool CutImageData(CDrawingBoard &MarkDrawBoard, CDrawingBoard &DstDrawBoard);
private:
	IPropertyImage* m_pPropImage;
	IPropertyComboBox* m_pChangeImgAnimation;
	IPropertyImage* m_pPropPhotoFrameImg;
	IPropertyImage* m_pPropMarkPhotoFrameImg;

	IPropertyImageBase* m_pInSkinImgBase;
	IPropertyImageBase* m_pOldImgBase;
	IPropertyImageBase* m_pNewImgBase;
	IPropertyCursor* m_pPropCursor;

	int m_nNewImgAlpha;
	bool m_IsShowCursor;

	CStringW m_key;
};
