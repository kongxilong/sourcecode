// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类


#include <WINSOCK2.H>
#include <afxsock.h>
//#include <list>
#include <string>

#include <afxcmn.h>


#pragma comment(lib, "ws2_32.lib")
using namespace std;

#include "ExListCtrl.h"         //可编辑list
#include "ADOExcel.h"




#define		BACKGROUNDCOLOR          RGB(185,211,255)     //对话框背景色
#define     CONCTRLBKCOLOR   RGB(223, 223, 223)   //控件背景色
///////**********自定义消息*********/////////////
#define		WM_LOGINSUCCESS						(WM_USER+100)    //登录成功的消息
#define     WM_CURSORTONORMAL					(WM_USER+101)    //改变鼠标为正常状态
#define     WM_LOGINFAIL						(WM_USER+102)    //登录失败的消息
#define     WM_PAITMTTREE						(WM_USER+103)   //绘制材料树控件的消息
#define     WM_UPDATESTATUS_MtSetProp			(WM_USER+104)
#define     WM_UPDATEOPERATIONSTATUS			(WM_USER+105)  //更新状态栏的操作信息
#define     WM_REQNOTENEW_MtSetProp				(WM_USER+106)   //发起新建材料树节点的请求消息
#define     WM_STRATREQMT_MtSetProp				(WM_USER+107)
#define     WM_ENDREQMT_MtSetProp				(WM_USER+108)
#define     WM_CLEARMTTREE_MtSetProp			(WM_USER+109)
#define     WM_REQNODEEDIT_MtSetProp			(WM_USER+110)
#define     WM_SHOWNOTEINEDIT_MainView			(WM_USER+111)    //调整格式 显示最新公告信息 或者是显示特定公告信息
#define     WM_REQNOTEPUBLIC_NotePublic			(WM_USER+112)
#define     WM_ENDNOTEPUBLIC_NotePublic			(WM_USER+113)
#define     WM_NOTEPUBLICSUCCESS_NotePublic		(WM_USER+114)
#define     WM_REQSTARTINITNOTE_MainView		(WM_USER+115)
#define	    WM_REQINITNOTEEND_MainView			(WM_USER+116)
#define		WM_DELSELNOTE_MainView				(WM_USER+117)
#define		WM_REQEMPNEWSTART_EmpNew			(WM_USER+118)
#define		WM_REQEMPNEWEND_EmpNew				(WM_USER+119)
#define     WM_STARTTASK_EmpInfoView			(WM_USER+120)
#define     WM_ENDTASK_EmpInfoView				(WM_USER+121)
#define     WM_STARTTASK_EmpEdit				(WM_USER+122)
#define     WM_ENDTASK_EmpEdit					(WM_USER+123)
#define     WM_DELSELEMPINFO					(WM_USER+124)
#define     WM_STARTTASK_FctEdit				(WM_USER+125)
#define     WM_ENDTASK_FctEdit					(WM_USER+126)
#define     WM_STARTTASK_FctNew					(WM_USER+127)
#define     WM_ENDTASK_FctNew					(WM_USER+128)
#define		WM_STARTTASK_FctSet					(WM_USER+129)
#define		WM_ENDTASK_FctSet					(WM_USER+130)
#define     WM_STARTTASK_CtmInfo				(WM_USER+131)
#define		WM_ENDTASK_CtmInfo					(WM_USER+132)
#define     WM_STARTTASK_CtmNew					(WM_USER+133)
#define		WM_ENDTASK_CtmNew					(WM_USER+134)
#define     WM_STARTTASK_CtmEdit				(WM_USER+135)
#define     WM_ENDTASK_CtmEdit					(WM_USER+136)
#define     WM_DELCTM_CtmInfo					(WM_USER+137)
#define     WM_STARTTASK_StoreInfo				(WM_USER+138)
#define     WM_ENDTASK_StoreInfo				(WM_USER+139)
#define     WM_STARTTASK_StoreAdd				(WM_USER+140)
#define     WM_ENDTASK_StoreAdd				    (WM_USER+141)
#define     WM_STARTTASK_StoreOut				(WM_USER+142)
#define     WM_ENDTASK_StoreOut					(WM_USER+143)
#define     WM_STARTTASK_StoreAlertSet			(WM_USER+144)
#define     WM_ENDTASK_StoreAlertSet			(WM_USER+145)
#define     WM_STARTTASK_StoreHistory			(WM_USER+146)
#define     WM_ENDTASK_StoreHistory			    (WM_USER+147)
#define     WM_DELHISTORY_StoreHistory          (WM_USER+148)
#define     WM_STARTTASK_ServReg				(WM_USER+149)
#define     WM_ENDTASK_ServReg					(WM_USER+150)
#define     WM_STARTTASK_ServModify				(WM_USER+151)
#define     WM_ENDTASK_ServModify				(WM_USER+152)
#define     WM_STARTTASK_ServiceSearch			(WM_USER+153)
#define     WM_ENDTASK_ServiceSearch			(WM_USER+154)
#define     WM_DELSERV_ServiceSearch			(WM_USER+155)
#define     WM_STARTTASK_OrderContent			(WM_USER+156)
#define     WM_ENDTASK_OrderContent				(WM_USER+157)
#define     WM_STARTUPLOADFILE_OrderContent		(WM_USER+158)
#define     WM_CONFIRMUPLOADFILE_OrderContent   (WM_USER+159)
#define     WM_RESETUPLOAD_OrderContent			(WM_USER+160)
#define     WM_STARTTASK_DealListView			(WM_USER+161)
#define     WM_ENDTASK_DealListView			    (WM_USER+162)
#define     WM_RESETDLSTATUS_DealListView       (WM_USER+163)
#define     WM_VALID_FILEDL_DealListView        (WM_USER+164)						
#define     WM_STARTTASK_OrderEdit				(WM_USER+165)
#define     WM_ENDTASK_OrderEdit				(WM_USER+166)
#define     WM_STARTTASK_DeductSet				(WM_USER+167)
#define     WM_ENDTASK_DeductSet				(WM_USER+168)
#define     WM_LOADSETOVER_DeductSet			(WM_USER+169)
#define     WM_STRATDLOAD_DealListView			(WM_USER+170)
#define     WM_ENDDLOAD_DealListView			(WM_USER+171)
#define     WM_DEL_DEAL_DealListView			(WM_USER+172)
#define     WM_STARTTASK_DBNew					(WM_USER+173)
#define     WM_ENDTASK_DBNew					(WM_USER+174)
#define     WM_STARTTASK_PwdEdit				(WM_USER+175)
#define     WM_ENDTASK_PwdEdit					(WM_USER+176)
#define     WM_STARTTASK_EmpInfoExProp			(WM_USER+177)
#define     WM_ENDTASK_EmpInfoExProp			(WM_USER+178)
#define     WM_STARTTASK_EmpInfoProp            (WM_USER+179)
#define     WM_ENDTASK_EmpInfoProp              (WM_USER+180)
#define     WM_STARTTASK_ProdInfoExProp			(WM_USER+181)
#define     WM_ENDTASK_ProdInfoExProp           (WM_USER+182) 
#define     WM_STARTTASK_CustomExProp			(WM_USER+183)
#define     WM_ENDTASK_CustomExProp			    (WM_USER+184)
#define     WM_HANDLE_BATCH_CTM_CtmEx			(WM_USER+185)
#define     WM_STARTTASK_ProdIm					(WM_USER+186)
#define     WM_ENDTASK_ProdIm					(WM_USER+187)
#define     WM_STARTTASK_CtmIm					(WM_USER+188)
#define     WM_ENDTASK_CtmIM					(WM_USER+189)
#define     WM_STARTTASK_OrderIncomplete		(WM_USER+190)
#define     WM_ENDTASK_OrderIncomplete			(WM_USER+191)
#define     WM_FILLLIST_OrderIncomplete			(WM_USER+192)
#define     WM_HANDLE_BATCH_ProdEx				(WM_USER+193)
#define     WM_STARTTASK_EmpPerformance			(WM_USER+194)
#define     WM_ENDTASK_EmpPerformance			(WM_USER+195)
#define		WM_FILLDATATOLIST_EmpPerformance	(WM_USER+196)
#define     WM_ENDTASK_SysPostion				(WM_USER+197)
#define     WM_STARTTASK_SysPostion				(WM_USER+198)
#define     WM_STARTTASK_PosAdd					(WM_USER+199)
#define     WM_ENDTASK_PosAdd					(WM_USER+200)
#define     WM_FILLLIST_SysPostion				(WM_USER+201)
#define     WM_DEL_USERS_SysPostion				(WM_USER+202)
#define		WM_STARTTASK_PosEdit				(WM_USER+203)
#define		WM_ENDTASK_PosEdit				    (WM_USER+204)
#define		WM_INITUSER_AUTHORITY_PosEdit		(WM_USER+205)
#define		WM_DEL_FCT_FctSet					(WM_USER+206)
///////*********自定义消息*********/////////////
#define		RECV_BUFF               (1024*20)          //接收文件缓冲区大小
#define		SEND_BUFF				(1024*20)         //上传文件和下载文件的缓冲区大小
#define		HEADLEN					sizeof(hdr)     //数据包结构体的大小
#define		SERVPORT				6686            //定义默认的通讯端口
#define		TIMEHEARTPACKAGE		(60*1000)     //心跳包间隔
#define		MAX_RECVFILE_THREAD		2           //接收文件的最大线程数      

#define NORMAL		0			//正常接收数据
#define EXCEPT		1			//接收数据异常
#define FINISH		2			//数据接收完毕

#define DATA_START		-1    //数据开始部分
#define DATA_MID		0      //数据中间部分
#define DATA_END		1      //数据结尾部分


#define WT					USES_CONVERSION
#define BUF_TIMES			10			//设置系统的发送和接收数据缓冲区为默认的10倍，在xp32下为 8k*10
#define BUF_SIZE			(1024 * 60)	//缓冲区的大小
#define THREAD_SLEEP		500         //线程休眠时间


///////以下为包头的请求类型///////////////
#define		LOGIN									1      
#define     SEARCH_ALL_EMP_INFO						3
#define     HEART_PACKAGE							5
#define     INIT_MATERIAL_TREE						7
#define     EXPAND_MATERIAL_TREE					9
#define     MT_NODE_NEW								11
#define     MT_NODE_RENAME							13
#define     MT_NODE_DELETE							15
#define     NOTE_PUBLIC_MainView					17
#define		INIT_NOTE_MainView						19
#define     DEL_NOTE_MainVIew						21
#define     EMP_NEW_EmpNew							23
#define     EMP_EDIT_EmpEdit						25
#define     EMP_DEL_EmpInfoView						27
#define     EMP_SEARCHBYKEYWORD_EmpInfoView			29
#define     FCT_NEW_FctNew							31
#define		FCT_FIND_ALL							33
#define     FCT_EDIT_FactoryEdit					35
#define		FCT_DEL_FctSet							37
#define     CTM_NEW_CtmNew							39
#define     CTM_EDIT_CtmEdit						41
#define		CTM_FINDALL_CtmInfo						43
#define     CTM_DEL_CtmInfo							45
#define     STORE_FINDALL_StoreInfo					47
#define     STORE_ADD_StoreAdd						49
#define     STORE_OUT_StoreOut						51
#define     STORE_GOODS_ALERT_SET	                53	
#define     STOREHISTORY_FINDALL					55
#define     STORYHISTORY_DEL						57
#define     SERV_REG_ServReg						59	
#define     SERV_MODIFY_ServModify					61
#define		SERV_FINDALL							63
#define     SERV_DEL								65
#define     SERV_FIND_BY_KEYWORD					67
#define     CHECK_FILE_EXIST                        69
#define     FILE_UPLOAD								71
#define     SEND_FILEINFO                           73
#define     FILE_SIZE							    75
#define     ORDER_NEW_OrderContent                  77
#define     ORDER_FIND_DealListView                 79
#define     ORDER_EDIT_OrderEdit					81
#define     DEDUCT_INIT                             83
#define     DEDUCT_SET                              85
#define     REQ_FILE_DOWNLOAD                       87
#define     DEL_DEAL                                89
#define     DATABASE_NEW                            91
#define     PWD_EDIT								93
#define     NOTE_DETAIL                             95
#define     EMP_INFO_FOR_EX                         97
#define     EMP_INFO_IMPORT                         99
#define     CTM_EX                                  101
#define     CTM_IM								    103
#define		FIND_ORDER_INCOMPLETE					105
#define     UPDATE_ORDER_BATCH                      107
#define     PROD_IM								    109
#define     PROD_EX									111
#define     FIND_EMP_PERPFORMANCE					113
#define     POS_ADD									115
#define     POS_FIND_ALL							117
#define     POS_DEL									119
#define     USER_INIT								121
#define		POS_EDIT								123
///////以上为包头的请求类型///////////////


///////以下为包头的回复类型///////////////
#define		REPLY_LOGIN										2
#define     REPLY_SEARCH_ALL_EMP_INFO						4
#define     REPLY_HEART_PACKAGE								6
#define     REPLY_INIT_MATERIAL_TREE						8 
#define     REPLY_EXPAND_MATERIAL_TREE						10
#define     REPLY_MT_NODE_NEW								12
#define     REPLY_MT_NODE_RENAME							14
#define     REPLY_MT_NODE_DELETE							16
#define     REPLY_NOTE_PUBLIC_MainView						18
#define		REPLY_INIT_NOTE_MainView						20
#define     REPLY_DEL_NOTE_MainVIew							22
#define     REPLY_EMP_NEW_EmpNew							24
#define     REPLY_EMP_EDIT_EmpEdit						    26
#define     REPLY_EMP_DEL_EmpInfoView						28
#define     REPLY_EMP_SEARCHBYKEYWORD_EmpInfoView			30
#define     REPLY_FCT_NEW_FctNew							32
#define		REPLY_FCT_FIND_ALL								34
#define     REPLY_FCT_EDIT_FactoryEdit						36
#define		REPLY_FCT_DEL_FctSet							38
#define     REPLY_CTM_NEW_CtmNew							40
#define     REPLY_CTM_EDIT_CtmEdit							42
#define		REPLY_CTM_FINDALL_CtmInfo						44
#define     REPLY_CTM_DEL_CtmInfo							46
#define     REPLY_STORE_FINDALL_StoreInfo					48
#define     REPLY_STORE_ADD_StoreAdd						50
#define     REPLY_STORE_OUT_StoreOut						52
#define     REPLY_STORE_GOODS_ALERT_SET						54
#define     REPLY_STOREHISTORY_FINDALL						56
#define     REPLY_STORYHISTORY_DEL						    58
#define     REPLY_SERV_REG_ServReg							60	
#define     REPLY_SERV_MODIFY_ServModify					62
#define		REPLY_SERV_FINDALL								64
#define     REPLY_SERV_DEL								    66
#define     REPLY_SERV_FIND_BY_KEYWORD					    68
#define     REPLY_CHECK_FILE_EXIST                          70
#define     REPLY_FILE_UPLOAD                               72
#define     REPLY_FILE_DLINFO                               74    //接收下载文件的信息
#define     REPLY_FILE_SIZE							        76
#define     REPLY_ORDER_NEW_OrderContent                    78
#define     REPLY_ORDER_FIND_DealListView                   80
#define     REPLY_ORDER_EDIT_OrderEdit					    82
#define     REPLY_DEDUCT_INIT                               84
#define     REPLY_DEDUCT_SET                                86
#define     REPLY_REQ_FILE_DOWNLOAD                         88
#define     REPLY_REQ_DEL_DEAL                              90
#define     REPLY_DATABASE_NEW                              92
#define     REPLY_PWD_EDIT								    94
#define     REPLY_NOTE_DETAIL                               96
#define     REPLY_EMP_INFO_FOR_EX                           98
#define     REPLY_EMP_INFO_IMPORT                           100
#define     REPLY_CTM_EX                                    102
#define     REPLY_CTM_IM								    104
#define		REPLY_FIND_ORDER_INCOMPLETE					    106
#define     REPLY_UPDATE_ORDER_BATCH                        108
#define     REPLY_PROD_IM								    110
#define     REPLY_PROD_EX									112
#define     REPLY_FIND_EMP_PERPFORMANCE						114
#define     REPLY_POS_ADD									116
#define     REPLY_POS_FIND_ALL							    118
#define     REPLY_POS_DEL									120
#define     REPLY_USER_INIT								    122
#define		REPLY_POS_EDIT								    124
#define     REPLY_POS_AUTHORITY								126
///////以上为包头的回复类型///////////////


//////////以下为用户权限////////////////////////
#define		 NUM_AUTHORITY                          44

#define      AUTHORITY_DEAL_NEW						0
#define      AUTHORITY_DEAL_EDIT					1
#define		 AUTHORITY_DEAL_FIND					2
#define		 AUTHORITY_DEAL_DEL						3
#define		 AUTHORITY_FILE_UPLOAD					4
#define		 AUTHORITY_FILE_DOWNLOAD				5	
#define		 AUTHORITY_FILE_DEL						6
#define		 AUTHORITY_DEAL_INCOMPLETE_FIND			7
#define		 AUTHORITY_DEAL_INCOMPLETE_EDIT			8
#define		 AUTHORITY_STORE_FIND					9
#define		 AUTHORITY_ALERT_SET					10
#define		 AUTHORITY_STORE_ADD					11	
#define		 AUTHORITY_STORE_OUT					12	
#define		 AUTHORITY_HISTORY_FIND					13	
#define		 AUTHORITY_HISTORY_DEL					14	
#define		 AUTHORITY_EMP_FIND					    15	
#define		 AUTHORITY_EMP_EDIT					    16	
#define		 AUTHORITY_EMP_NEW						17	
#define		 AUTHORITY_EMP_DEL					    18	
#define		 AUTHORITY_EMP_PERFERENCE_FIND			19	
#define		 AUTHORITY_CTM_FIND						20	
#define		 AUTHORITY_CTM_NEW						21	
#define		 AUTHORITY_CTM_EDIT						22	
#define		 AUTHORITY_CTM_DEL						23	
#define		 AUTHORITY_POS_ADD						24	
#define		 AUTHORITY_POS_FIND						25	
#define		 AUTHORITY_POS_DEL					    26	
#define		 AUTHORITY_POS_EDIT						27	
#define		 AUTHORITY_PROD_IM						28	
#define		 AUTHORITY_PROD_EX						29	
#define		 AUTHORITY_CTM_IM						30	
#define		 AUTHORITY_CTM_EX						31	
#define		 AUTHORITY_EMP_IM						32	
#define		 AUTHORITY_EMP_EX						33	
#define		 AUTHORITY_MT_FIND						34	
#define		 AUTHORITY_NODE_NEW						35	
#define		 AUTHORITY_NODE_EDIT					36	
#define		 AUTHORITY_NODE_DEL						37	
#define		 AUTHORITY_FCT_FIND					    38
#define      AUTHORITY_FCT_NEW						39
#define		 AUTHORITY_FCT_EDIT					    40
#define		 AUTHORITY_FCT_DEL						41
#define		 AUTHORITY_NOTE_PUBLIC					42
#define		 AUTHORITY_NOTE_DEL						43
//////////以上为用户权限////////////////////////
enum REQTYPE{									//请求类型
		REQLOGIN,								//请求登录
		REQSEARCHALLEMPINFO,					//请求搜索所有员工信息
		REQKEEPHEART,							//心跳包
		REQ_INIT_MATERIAL_TREE,					//请求初始化材料树
		REQ_Expant_MATERIAL_TREE,				//请求材料树的子目录
		REQ_MT_NODE_NEW,						//请求新建材料树节点
		REQ_MT_NODE_RENAME,						//请求编辑材料节点
		REQ_MT_NODE_DELETE,						//请求删除某个节点
		REQ_MAINVIEW_NOTE_PUBLIC,				//请求发布公告
		REQ_INIT_NOTE_MAINVIEW,					//请求初始化通知栏
		REQ_DEL_NOTE_MAINVIEW,					//请求删除通知栏的一些行
		REQ_EMP_NEW_EmpNew,						//请求新建员工
		REQ_EMP_EDIT_EmpNew,					//请求编辑员工
		REQ_EMP_DEL_EmpInfoView,				//请求删除员工资料
		REQ_EMP_SEARCHBYKEYWORD_EmpInfoView,   //请求按关键字搜索员工资料
		REQ_FCT_NEW_FctNew,						//请求新建厂家资料
		REQ_FCT_FINDALL_FctSet,					//请求搜索所有厂家资料
		REQ_FCT_EDIT_FctEdit,					//请求修改厂家资料
		REQ_FCT_DEL_FctSet,						//请求删除厂家资料
		REQ_CTM_NEW_CtmNew,						//请求新建客户资料
		REQ_CTM_EDIT_CtmEdit,					//请求编辑客户资料
		REQ_CTM_FINDALL_CtmInfo,				//请求查找所有客户资料
		REQ_CTM_DEL_CtmInfo,					//请求删除客户资料
		REQ_STORE_FINDALL_StoreInfo,			//请求查询所有的库存资料
		REQ_STORE_ADD_StoreAdd,					//请求添加库存物品
		REQ_STORE_OUT_StoreOut,					//请求出库
		REQ_GOODS_ALERT_NUM_StoreAlertSet,		//请求库存预警信息
		REQ_FIND_ALL_STOREHISTORY_StoryHistory,			//请求搜索所有库存信息
		REQ_DEL_STOREHISTORY,					//请求删除库存历史记录
		REQ_SERV_REG_ServReg,					//请求注册服务
		REQ_SERV_MODIFY_ServModify,				//请求修改服务
		REQ_SERV_FINDALL,						//请求查询所有服务
		REQ_SERV_DEL,							//请求删除服务
		REQ_SERV_FIND_BY_KEYWORD,				//根据关键字查找服务
		REQ_CHECK_FILEEXIST,                    //检查上传的附件是否存在
		REQ_UPLOADFILE,							//请求上传文件
		REQ_FILE_SIZE,                            //请求下载文件
		REQ_ORDER_NEW,                          //请求新建订单
		REQ_FIND_DEAL,                          //请求查找订单
		REQ_ORDER_EDIT,                        //请求编辑订单
		REQ_INIT_DEDUCT,                       //请求初始化提成设置
		REQ_SET_DEDUCT,                       //请求设置提成
		REQ_DEL_DEAL,                         //请求删除订单
		REQ_PSW_EDIT,                          //修改密码
		REQ_NOTE_DETAIL,                       //请求加载公告详细信息
		REQ_EMP_EX,                            //请求批量导出员工数据
		REQ_EMP_IM,                            //批量提交员工数据到数据库
		REQ_CTM_EX,                            //请求客户资料以供导出
		REQ_CTM_IM,                             //请求批量提交客户资料到数据库
		REQ_FIND_ORDER_INCOMPLETE,               //请求查询未完成订单
		REQ_UPDATE_ORDER_BATCH,              //批量更新未完成订单数据
		REQ_UPDATE_PROD_BATCH,               //批量提交产品数据
		REQ_PROD_EX,                          //批量导出产品数据
		REQ_EMP_PERFORMANCE,                  //查找员工业绩表
		REQ_POS_ADD,                           //创建新用户
		REQ_FINDALL_USERS,                     //查找所有用户
		REQ_DELE_USERS,                       //删除用户
		REQ_LOAD_USER_AUTHORITY,				//加载用户权限
		REQ_EDIT_USER,                           //重新编辑用户
		REQNON        //当前无请求		
};



typedef struct _hdr
{
	int	type;	//类型		
	u_short	len;	//数据包长度(头+包体)
	u_long 	fpos;	//文件位置
	u_long 	flen;	//文件长度
	char    fname[MAX_PATH];   //文件的名字
	char    NO_Order[20];    //订单编号
	//u_long  nIndex;  //查询数据库得到的匹配数的第几个
}hdr, *phdr;

typedef struct _Pos_Authority
{
	BOOL bLoadSuccess;
	BOOL autority[NUM_AUTHORITY];      //权限列表
}Pos_Authority,*p_Pos_Authority;

typedef	struct _FileDownloadParam  
{
	unsigned short	ID;			//标识
	hdr				header;		//包头
	SOCKET			s;			//套接字
	void*		    ptr;		//主线程
	char			strFilePath[MAX_PATH];//下载文件路径
}FileDownloadParam, *pFileDownloadParam;


typedef struct _DeductInfo     ////提成信息表
{
	char decorate_company[50];
	char discuss_deal_person[50];
	char emp[50];
	char exact_test_person[50];
	char has_inner_modify[50];
	char measure[50];
	char no_inner_modify[50];
}DeductInfo;


typedef struct _FileInfo   //上传和下载的文件信息
{
	char order_NO[20];   //订单编号
	char filename[MAX_PATH];
	char filepath[MAX_PATH];
	u_long filesize;
}FileInfo;
typedef struct _OrderInfo   //订单信息表
{
	char  NO_Order[20];  //订单编号
	char  date_accept_order[20];  //接单日期
	char  date_finished_order[20];  //完成时间
	char  name_person_charge_order[20];   //订单负责人
	char  base_ps_order[200];      //订单信息的基本备注
	char  ctm_from[20];    //客户来源
	char  name_intr[20];   //介绍人姓名
	char  phone_intr[20];   //介绍人电话
	char  name_serv_persion[20];    //接待人员姓名
	char  name_emp[20];      //业务员名称
	char  call_ctm[20];      //客户名称
	char  name_ctm[20];      //顾客姓名
	char  phone_ctm[20];     //顾客电话
	char  addr_ctm[50];      //顾客地址
	char  ps_ctm[100];       //顾客备注
	char  NO_contract[20];   //合同单号
	char  prog_business[20];   //业务进度
	char  price_total_contract[20];  //订单总价
	char  money_already_take[20];   //已收金额
	char  date_first_test_book[20];  //初测预约时间
	char  persion_first_test[20];   //初测人员
	char  persion_first_design[20];  //初次设计人员
	char  type_early_modify[20];    //前期修改类型
	char  persion_early_modify[20];    //前期修改人员
	char  persion_budget[20];       //预算人员
	char  type_discuss_order[20];    //谈单类型
	char  persion1_discuss_order[20];   //谈单人员1
	char  persion2_discuss_order[20];   //谈单人员2
	char  date_final_scheme[20];        //方案定稿日期
	char  date_book[20];				//放样预约时间
	char  person_exact_measure[20];		//放样精量人员
	char  date_out_order_book[20];      //发单预约时间
	char  person_out_order[20];       //发单人员
	char  factory_choose[50];       //单位选择
	char  pay_deal_choice[20];      //付款选择
	char  date_install_book[20];    //安装预约时间
	char  persion_install[20];      //安装人员
	char  problems_install[100];      //安装问题
	char  filepath_accessory[MAX_PATH];   //文件路径
	char  filesize[50];               //文件大小
}OrderInfo,*pOrderInfo;


typedef struct _NotePublic_MainView      //主页用来发布公告的结构体
	{
		char content[600];   //公告内容
		char title[100];      //公告题目
		char persion_public[50];   //公告发布的对象
		char time_public[50];   //公告发布的时间
	}NotePublic_MainView,*p_NotePublic_MainView;

typedef struct _ServInfo    //服务信息
{
	char serv_NO[20];        //服务单号
	char client_type[20];   //客户类型
	char ctm_name[20];		//客户名称
	char client_name[20];  //顾客姓名
	char ctm_sex[10];		//顾客性别
	char client_phone[20];  //顾客电话
	char addr[50];			//地址
	char ps[200];			//备注
	char serv_content[60];  //服务内容
	char req_serv_date[20];  //要求服务日期
	char serv_name[20];		//服务人员姓名
	char serv_phone[20];	//服务人员电话
	char prog[20];			//进度
	char delay_date[20];   //延期日期
	char charge[20];	   //收费金额
	char serv_type[30];   //服务类型
}ServInfo,pServInfo;

typedef struct _StoreInfo{    //用于记录库存的结构体
	//char store_NO[20];    //库存编号
	char Store_Name[50];    //名称
	char Store_Num[20];      //库存数量
	char Store_Alert_NUM[20];   //预警数量
}StoreInfo,*pStoreInfo;

typedef struct _StoreHistory    //库存历史记录
{
	char Store_Name[50];    //名称
	char num_change[20];    //变动数量
	char status[20];        //状态
	char time[30];           //变动时间
}StoreHistory,*pStoreHistory;

typedef struct _LoginInfo{    //用于用户登录的结构体
	char username[50];    //用户名
	char password[50];    //密码
	char DBName[50];      //数据库的名称
	char loginAddr[50];   //用户登录地点
	char loginPos[30];    //用户登录角色

}loginInfo,*pLogInfo;

typedef struct _CtmInfo
{
	char NO[50];
	char name[50];    //顾客姓名
	char call_ctm[50];    //客户名称
	char sex[10];   
	char phone[20];
	char pay_times[20];   //消费次数
	char pay_total[20];   //消费金额
    char first_pay_time[30];  //首次消费时间
	char late_pay_time[30];   //最近消费时间
	char ps[300];        //备注

}CtmInfo,*p_CtmInfo;

typedef struct _fileuploadparam
{
	u_long	ulFileSize;		//文件长度
	u_long	ulFilePos;		//文件位置
	void* ptr;			   //指针
	char filepath[MAX_PATH];       //文件路径
	CRITICAL_SECTION cs;
}FileUploadParam, *pFileUploadParam;


typedef struct _FctInfo
{
	char	name_fct[100];   //厂家名字
	char	addr_fct[200];   //厂家地址
	char	contacts_fct[50];  //厂家联系人
	char    phone_fct[20];		//厂家电话
	char    main_product_fct[200];  //厂家主要产品
	char    ps_fct[400];      //厂家备注
}FctInfo,p_FctInfo;



typedef struct _EmpInfo
{
	char empNum[50];
	char empName[50];
	char empAddr[50];
	char empPos[50];
	char empPhone[20];
	char empPS[900];
}EmpInfo,*pEmpInfo;

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxdlgs.h>



////////*******以下为自己添加的内存泄露检测功能************////////////
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include "vld.h"
////////*******以下为自己添加的内存泄露检测功能************////////////





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


