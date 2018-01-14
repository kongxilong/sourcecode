#pragma once
#include "..\..\Inc\Control\ICtrlLEDCtrl.h"

class ICtrlLEDCtrlImpl : public ICtrlLEDCtrl
{
public:
	ICtrlLEDCtrlImpl(IUiFeatureKernel *pUiKernel);
	virtual ~ICtrlLEDCtrlImpl();

	virtual void SetText(LPCWSTR pText, bool bRedraw);
	virtual void SetIndexMap(LPCWSTR pText);
	virtual SIZE GetAutoSize();

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
	// 为了整个项目的向下兼容，扩展控件的执行命令接口，每个控件都必须实现，但实现可以为空
	virtual LRESULT ExecuteControlCommand(int nCommandId, WPARAM wParam, LPARAM lParam);

private:
	//初始化操作
	virtual void CreateVecIndex();
	virtual void CreateMapIndex();

private:
	map<TCHAR,int>	  m_mapIndex;	//字符串到图片索引的映射
	vector<int>       m_vecIndex;   //当前要显示的索引序列

	IPropertyImage*	  m_pImageList;			//一张由多个小图组成的序列图片
	IPropertyString*  m_pStrText;			//要显示的LED文字
	IPropertyString*  m_pStrMapIndex;		//例如：0123456789:-
	IPropertyInt*	  m_pImageListCount;	//图片序列中一共有多少张小图组合而成

};