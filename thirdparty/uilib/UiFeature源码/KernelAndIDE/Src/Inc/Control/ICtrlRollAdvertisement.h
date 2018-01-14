
#pragma once
#include "..\ICtrlInterface.h"


// 添加控件，步骤1：编写一个控件名称宏。注意，控件名称不能相同
#define CTRL_NAME_ROLL_ADVERTISEMENT						(L"RollAdvertisement")

// NoticeLable 控件向窗口发送的消息
enum CTRL_MSG_ROLL_ADVERTISEMENT
{
	CM_ROLL_ADVERTISEMENT_BASE	= 0x10026000,
	// WPARAM 参数为：ROLL_ADVERTISEMENT 的指针
	CM_ROLL_ADVERTISEMENT_DOWN,
	// WPARAM 参数为：ROLL_ADVERTISEMENT 的指针
	CM_ROLL_ADVERTISEMENT_UP,
};

struct ROLL_ADVERTISEMENT_ITEM
{
	int nId;
	int nConfActive;
	WCHAR* pStrShowInfo;
	WCHAR* pStrData;
	WPARAM wParam;

	ROLL_ADVERTISEMENT_ITEM()
	{
		nId = 0;
		nConfActive = 0;
		pStrShowInfo = pStrData = NULL;
		wParam = NULL;
	};
};
typedef vector<ROLL_ADVERTISEMENT_ITEM*> ROLL_ADVERTISEMENT_VEC;


class ICtrlRollAdvertisement : public ICtrlInterface
{
public:
	ICtrlRollAdvertisement(IUiFeatureKernel *pUiKernel);
	// 清空所有滚动信息
	virtual bool Clear() = 0;
	// 添加一个滚动信息
	virtual bool AddAdvertisement(ROLL_ADVERTISEMENT_ITEM &AdvertisementeInfo) = 0;
	// 当前滚动队列是否为空
	virtual bool IsEmpty() = 0;
	// 设置滚动显示的时间，单位秒
	virtual void SetShowTime(int nTime) = 0;
};
