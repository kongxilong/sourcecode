
#pragma once

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

///// 重要消息值，外界程序不能与之重复 /////////////////////////////////////////////////////////////////////
// UI内核内部自定义消息值，窗口不得占用此消息值，否则会引起系统异常
#define UI_FEATURE_KERNEL_MSG						(WM_APP + 0x0100)
// UI内核自定义消息的WPARAM参数值
enum WPARAM_ID
{
	// 需要外部对话框接受的消息：使用皮肤初始化窗口正确结束
	WID_INIT_WINDOW_SUCCESS				= 1,
	// 需要外部对话框接受的消息：使用皮肤初始化窗口异常
	WID_INIT_WINDOW_ERROR				= 2,
	// 需要外部对话框接受的消息：皮肤包的密码错误
	WID_INIT_PASSWORD_ERROR				= 3,
	// 自动化测试消息值
	WID_AUTO_TEST						= 4,
	// 控件注册要取得的消息
	WID_REG_CTRL_MSG					= 5,
	// 控件注销要取得的消息
	WID_UNREG_CTRL_MSG					= 6,
	// 播放控件动画
	WID_CTRL_ANIMATION					= 7,
	// 使用发消息的方式重绘整个窗口
	WID_REDRAWWINDOW					= 8,
	// 清空整个窗口的绘制缓存
	WID_CLEAR_DRAW_MEM_DC				= 9,
	// 窗口动画线程结束
	WID_ANIMATION_THREAD_END			= 10,
	// 控件向窗口发送：窗口需要设置RGN的消息
	WID_SET_WINDOW_RGN					= 11,
};
// 自定义消息的LPARAM参数，如果有的消息值需要用到内存参数传递，就使用这个结构
struct LPARAM_DATA
{
	WPARAM wParam;
	LPARAM lParam;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 内核发给对话框的内置控件消息，由对话框的OnCtrlMessage接受，开发控件时这些消息不能相同
enum KERNEL_CTRL_MSG
{
	KCM_BASE = 0x0BBB0000,
	// Windows系统向对话框发送了关闭消息，源自于：WM_SYSCOMMAND 的 SC_CLOSE 消息
	KCM_SYS_COMMAND_CLOSE,
};

// 控件得到自己注册的消息后，如果不想再往下派发，则在OnCtrlNotify直接返回这个值即可
#define CTRL_REG_MSG_NOT_PASS_ON					(0x1234ABCD)

// 用于绘制动画的固定定时器ID
#define UM_DFT_ANIMATION_TIMER						(0x0000F001)
#define UM_DFT_ANIMATION_TIMER_100S					(100)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NAME_UIFEATURE_XML							"UiFeature.xml"
#define NAME_EFFECT_DLL								"UiFeatureEffect.dll"
#define NAME_KERNEL_DLL								"UiFeatureKernel.dll"
#define NAME_FEATURE_RUN_TEST						"UiFeatureRun.exe"
#define NAME_UFP_TEMPLATE							"ufp.template"
#define NAME_SKIN_PROJ_EX_NAME						".ufp"
#define NAME_SKIN_FILE_EX_NAME						".ufd"

#define NAME_WINDOW_WIDTH							"@WindowWidth"
#define NAME_WINDOW_HEIGHT							"@WindowHeight"
#define NAME_SKIN_PROP_NAME_OBJ_ID					"@Object_Id"
#define NAME_SKIN_PROP_NAME_TYPE					"@Object_Type"
#define NAME_SKIN_PROP_NAME							"@Object_Name"
#define NAME_LAYOUT_TYPE							"@Layout_Type"
#define NAME_LAYOUT_WIDTH							"@Width"
#define NAME_LAYOUT_HEIGHT							"@Height"
#define NAME_LAYOUT_LEFTSPACE						"@LeftSpace"
#define NAME_LAYOUT_RIGHTSPACE						"@RightSpace"
#define NAME_LAYOUT_TOPSPACE						"@TopSpace"
#define NAME_LAYOUT_BOTTOMSPACE						"@BottomSpace"

// 属性资源xml文件
// Window配置资源xml文件
#define LAYOUT_XML_NAME								"Layout.xml"
#define WINDOWS_XML_NAME							"Windows.xml"
#define CONTROLS_XML_NAME							"Controls.xml"
#define RESOURCE_XML_NAME							"Resource.xml"

// 系统默认字体
#ifndef SYS_DEFAULT_FONT_NAME
#define SYS_DEFAULT_FONT_NAME						"SystemDefaultFont"
#endif


// 控件预览图、icon文件的存放路径，这个只有Builder使用，内核并不使用
#define CONTROL_IMAGE_DIR							"ControlsRes\\"
// 所有皮肤文件集中存放的地方
#define SKIN_DATA_DIR								"SkinData\\"
#define SKIN_OBJECT_ID								"objid"
// 用户密码最大长度：字节
#define USER_PASSWORD_KEY_LEN						(128)

#ifndef SAFE_CLOSE_HANDLE
#define SAFE_CLOSE_HANDLE(hHandle)					{if((hHandle)!=NULL)::CloseHandle((hHandle));(hHandle)=NULL;};
#endif

#ifndef SAFE_FREE_LIBRARY
#define SAFE_FREE_LIBRARY(hHandle)					{if((hHandle)!=NULL)::FreeLibrary((hHandle));(hHandle)=NULL;};
#endif

#ifndef SAFE_DELETE_OBJECT
#define SAFE_DELETE_OBJECT(hHandle)					{if((hHandle)!=NULL)::DeleteObject((hHandle));(hHandle)=NULL;};
#endif

#ifndef IS_SAFE_HANDLE
#define IS_SAFE_HANDLE(hWnd)						(((hWnd)!=NULL)&&((hWnd)!=INVALID_HANDLE_VALUE))
#endif

#ifndef IS_INVALID_HANDLE
#define IS_INVALID_HANDLE(hWnd)						(((hWnd)==NULL)||((hWnd)==INVALID_HANDLE_VALUE))
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(pMem)							{if((pMem)!=NULL){delete((pMem));(pMem)=NULL;}};
#endif

#ifndef SAFE_DELETE_LIST
#define SAFE_DELETE_LIST(pMem)						{if((pMem)!= NULL) {delete[]((pMem));(pMem)=NULL;}};
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(pMem)								{if((pMem)!=NULL){free((pMem));(pMem)=NULL;}};
#endif

// 初始化点
#define INIT_POINT(pt)								((pt).x=(pt).y=0)
// 得到矩形宽度
#define RECT_WIDTH(GetRct)							((GetRct).right-(GetRct).left)
// 得到矩形宽度
#define RECT_HEIGHT(GetRct)							((GetRct).bottom-(GetRct).top)
// 初始化矩形为空
#define INIT_RECT(SetEmRect)						((SetEmRect).left=(SetEmRect).right=(SetEmRect).top=(SetEmRect).bottom=0)
// 设置矩形
#define SET_RECT(SetRct,nL,nT,nR,nB)				((SetRct).left=(nL),(SetRct).top=(nT),(SetRct).right=(nR),(SetRct).bottom=(nB))
#define SET_RECT_WH(SetRct,nL,nT,nWidth,nHeight)	((SetRct).left=(nL),(SetRct).top=(nT),(SetRct).right=(nL)+(nWidth),(SetRct).bottom=(nT)+(nHeight))
// 判断矩形是否不为空
#define IS_RECT_NOT_EMPTY(CheckRect)				((RECT_WIDTH(CheckRect)>0)&&((RECT_HEIGHT(CheckRect)>0)))
// 判断矩形是否为空
#define IS_RECT_EMPTY(CheckRect)					((RECT_WIDTH(CheckRect)<=0)||(RECT_HEIGHT(CheckRect)<=0))

// 解决GDI+的Alpha融合中Alpha融合进位误差的问题，程序默认的Alpha的最大值为254，而非255
#define MAX_ALPHA									(254)

// 正常光标 标准的箭头
#define UF_IDC_ARROW			(32512)
// 十字架光标
#define UF_IDC_CROSS			(32515)
// 双箭头指向西北和东南
#define UF_IDC_SIZENWSE			(32642)
// 双箭头指向东北和西南
#define UF_IDC_SIZENESW			(32643)
// 双箭头指向东西
#define UF_IDC_SIZEWE			(32644)
// 双箭头指向南北
#define UF_IDC_SIZENS			(32645)
// 四向箭头指向东、西、南、北
#define UF_IDC_SIZEALL			(32646)
// 标准的箭头和小沙漏
#define UF_IDC_APPSTARTING		(32650)
// 标准的箭头和问号
#define UF_IDC_HELP				(32651)
// 工字光标
#define UF_IDC_IBEAM			(32513)
// 禁止圈
#define UF_IDC_NO				(32648)
// 垂直箭头
#define UF_IDC_UPARROW			(32516)
// 沙漏
#define UF_IDC_WAIT				(32514)
// 手势点击
#define UF_IDC_HAND				(32649)

// 改变控件、窗口大小的位置
enum SIZE_CTRL_TYPE
{
	SCT_NONE		= 0,
	SCT_LEFT_TOP,
	SCT_LEFT_MID,
	SCT_LEFT_BOTTOM,
	SCT_MID_TOP,
	SCT_MID_BOTTOM,
	SCT_RIGHT_TOP,
	SCT_RIGHT_MID,
	SCT_RIGHT_BOTTOM,
};
