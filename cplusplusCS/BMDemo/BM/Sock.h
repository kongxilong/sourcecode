#ifndef SOCK_H_H_H_H
#define SOCK_H_H_H_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSock          //本程序的神经中枢 负责通信和多线程的调度和实现
{
public:
	CSock(void);
	~CSock(void);
public:
	static      BOOL      StartLookNoteDetail(void* ptr);          //查看通知栏
	static      BOOL	  StartReqLoadUserAuthority(void* ptr);
	static      BOOL      StartDelUsers(void* ptr);      //删除用户
	static      BOOL	  StartFindAllUser(void* ptr);      //查找所有用户
	static      BOOL	  StartReqPosEdit(void* ptr);       //请求编辑用户角色
	static      BOOL	  StartReqPosAdd(void* ptr);
	static      BOOL      StartFindEmpPerformance(void* ptr);    //查询员工数据
	static      BOOL      FindDealIncomplete(void* ptr);      //查询导出未完成订单
	static      BOOL      StartFindCtmForEx(void* ptr);      //批量导出客户资料
	static      BOOL      StartReqFindProd(void* ptr);        //批量导出产品数据
	static      BOOL      StartUpdateOrderIncomplete(void* ptr);    //批量更改未完成订单
	static      BOOL      StartSubmitProd(void* ptr);         //批量提交产品数据
	static      BOOL      StartSubmitCtmInfo(void* ptr);      //批量提交客户数据到数据库
	static      BOOL      StartImportEmpInfo(void* ptr);     //批量提交员工信息到数据库
	static      BOOL      StartReqFindEmpForEx(void* ptr);          //为了导出数据而查询员工数据
	static      void      UpdateOperationStatus(CString _status);  //更新状态栏的提示信息
	static      BOOL      StartReqPswEdit(void* ptr);       //修改密码
	static      BOOL      StartReqSetDeduct(void* ptr);     //设置提成
	static      BOOL      StartReqInitDeduct(void* ptr);         //初始化提成设置
	static      BOOL      StartReqOrderEdit(void* ptr);          //开始请求编辑订单
	static      BOOL      StartReqFindDeal(void* ptr);           //开始搜索订单
	static      BOOL      StartReqOrderNew(void* ptr);           //新建订单
	static      BOOL      StartDLFile(void* ptr);               //开始请求下载文件
	static      BOOL      StartUpLoadFile(void* ptr);           //开始上传文件
	static      BOOL      StartCheckFileExist(void* ptr);        //检查要上传的附件是否存在
	static      BOOL      StartFindServByKeyWord(void* ptr);      //请求查找关键字 利用关键字
	static      BOOL      StartDelDeal(void* ptr);             //请求删除订单
	static      BOOL      StartDelServ(void* ptr);            //请求删除服务
	static      BOOL      StartFindAllServ(void* ptr);        //请求查询所有服务信息
	static      BOOL      StartReqServModify(void* ptr);      //请求修改服务
	static      BOOL      StartReqServReg(void* ptr);        //请求服务注册
	static      BOOL      StartDelHistory(void* ptr);       //请求删除库存历史记录
	static      BOOL      StartFindAllStoreHistory(void* ptr);   //请求搜索所有库存记录
	static      BOOL      StartReqGoodsAlertNumSet(void* ptr);   //请求设置库存物品预警
	static      BOOL      StartReqStoreOut(void* ptr);       //请求出库
	static      BOOL      StartReqStoreAdd(void* ptr);       //请求添加库存物品
	static      BOOL      StartFindAllStore(void* ptr);      //请求搜索全部库存信息
	static      BOOL      StartDelCtm(void* ptr);            //请求删除客户资料
	static      BOOL      StartFindAllCtm(void* ptr);       //请求查找所有客户资料
	static      BOOL      StartReqCtmEdit(void* ptr);       //请求编辑客户资料
	static      BOOL      StartReqCtmNew(void* ptr);        //请求新建客户资料
	static      BOOL      StartReqFctNew(void* ptr);        //请求新建厂家资料
	static      BOOL	  StartReqEmpEdit(void* ptr);		//请求编辑员工资料
	static		BOOL      StartReqFctEdit(void* ptr);    //开启请求修改厂家资料
	static		BOOL	  StartReqEmpNew(void* ptr);
	static      BOOL	  StartDelNoteMainView(void* ptr);    //请求删除通知
	static		BOOL	  StartDelEmpInfo(void* ptr);        //请求删除员工资料
	static		BOOL      StartDelFct(void* ptr);            //请求删除厂家资料
	static      BOOL      StartReqLoadNote(void* ptr);        //请求加载主页的公告
	static		BOOL	  StartReqNotePublic(void* ptr);    //请求发布公告
	static      BOOL      StartReqDeleteNode(void* ptr);       //请求删除材料树的节点         
	static      BOOL      StartRefreshMtTree(void* ptr);      //更新材料树
	static      BOOL      StartExpandMtTree(void* ptr);
	static      BOOL      StartSendHeartPackage();   //定时发送心跳包
	static		BOOL      StartLogin(void* ptr);	//登录时候调用 传入CLogin指针 并开始连接服务器   负责与服务器的数据交换工作	
	static      BOOL      StartReqMtNodeNew(void* ptr);   //请求新建材料树节点
	static      BOOL      StartReqMtNodeEdit(void* ptr);     //请求编辑材料树的节点
	static		BOOL      StartFindALLFct(void* ptr);        //请求搜索所有厂家的资料
	static		BOOL	  StartSearchAllEmpInfo(void* ptr);		//搜索全部职员信息并显示
	static		BOOL	  StartSearchEmpByKeyWord(void* ptr);   //根据关键字搜索员工信息
	static		BOOL	  ConnectServer();			//完成连接服务器的任务 在登录的时候调用
	static		UINT	  SendAndRecvClientThread(LPVOID pParam);		//负责接收和发送数据的线程
	static		BOOL	  SendReq();			//向服务器发送请求  
	static		BOOL	  RecvData();			//接收从服务器返回的数据
	static      void      InitSocket();
	static		BOOL	  ReqLogin();			//向服务器发送用户名密码验证登录
	static      BOOL      FindServByKeyWord();    //根据关键字查找服务
	static      BOOL      SearchEmpByKeyWord();    //根据关键字搜索员工信息
	static		BOOL      ReqFindAllCtmInfo();     //向服务器发送查询所有客户资料的申请
	static      BOOL      ReqProdForEx();   //请求所有产品数据
	static      BOOL	  ReqCtmForEx();    //请求所有客户资料
	static      BOOL      ReqFindOrderIncomplete();    //查询未完成订单
	static      BOOL      ReqFindEmpPerformance();   //查询员工业绩表
	static		BOOL      ReqFindAllUsers();    //查找所有用户
	static      BOOL      ReqLoadUserAuthority();      //加载用户权限
	static      BOOL      ReqDelUsers();   //删除用户
	static      BOOL      ReqFindAllStoreInfo();     //请求搜索所有库存资料
	static      BOOL      ReqAddStore();        //请求添加物品库存
	static		BOOL      ReqRegServ();   //请求注册服务
	static      BOOL      ReqModifyServ();    //请求修改服务
	static      BOOL      ReqOutStore();        //请求出库
	static      BOOL      ReqStoreAlertNumSet();    //请求库存预警信息设置
	static      BOOL      ReqFindAllStoreHistory();   //搜索所有库存记录信息
	static      BOOL      ReqFindAllServ();			//请求查询所有服务
	static      BOOL      ReqDelStoreHistory();    //请求删除库存历史记录信息
	static      BOOL      ReqDelServ();        //请求删除服务信息
	static      BOOL      ReqDelDeal();         //请求删除订单
	static      BOOL      ReqCheckFileExist();   //请求查询文件是否存在
	static      BOOL      ReqUpLoadFile();    //向服务器上传文件
	static      BOOL	  ReqDLFile();     //请求下载文件
	static      BOOL      ReqOrderNew();    //请求新建订单
	static      BOOL      ReqFindDeal();     //请求查找订单
	static      BOOL      ReqOrderEdit();    //请求编辑订单
	static      BOOL      ReqInitDeduct();     //请求提成设置
	static      BOOL      ReqSetDeduct();     //请求设置提成
	static      BOOL      ReqPswEdit();       //请求修改密码
	static      BOOL      ReqPosAdd();    //请求添加用户
	static      BOOL      ReqPosEdit();    //请求编辑客户
	static      BOOL      ReqDetailNote();    //请求查找公告的详细信息
	static		DWORD WINAPI     UploadFileThread(void *pParam);	//上传文件线程函数
	static		BOOL	  ReqSearchAllFctInfo();   //向服务器请求发送搜索所有厂家信息
	static		BOOL	  ReqSearchAllEmpInfo();		//向服务器发送搜索所有员工信息的请求
	static      BOOL      ReqSearchAllEmpInfoForEx();   //向服务器请求所有员工数据 为了批量导出
	static      BOOL      ReqSubmitEmpInfo();    //批量提交员工数据到数据库
	static      BOOL      ReqSubmitProdInfo();   //批量插入产品数据
	static      BOOL	  ReqSubmitCtmInfo();   //批量提交客户数据到数据库
	static      BOOL      ReqUpdateOrderIncomplete();    //提交订单数据到数据库
	static      BOOL      ReqKeepHeart();
	static      BOOL      ReqInitMtTree();            //请求初始化树控件
	static      BOOL      ReqMtNodeNew();
	static      BOOL      ReqMtNodeRename();          //请求重命名材料节点
	static      BOOL      ReqMtNodeDelete();            //请求删除材料某个节点
	static      BOOL	  ReqMainViewNotePublic();      //请求发布通知
	static		BOOL	  ReqMainViewInitNote();		//请求加载通知
	static      BOOL      ReqMainViewDeleteSelNote();    //请求删除一些通知
	static		BOOL	  ReqDeleteSelEmpInfo_EmpInfoView();   //请求删除一些员工资料
	static      BOOL      ReqDeleteSelCtm();      //发起请求删除客户资料
	static		BOOL	  ReqDeleteSelFct_FctSet();     //发起请求删除厂家资料
	static		BOOL	  ReqEmpNew();				//请求新建员工
	static      BOOL      ReqFctNew();      //请求新建厂家资料
	static      BOOL      ReqCtmNew();   //请求新建用户资料
	static      BOOL      ReqFctEdit();   //请求编辑厂家资料
	static      BOOL      ReqEmpEdit();             //请求编辑新员工
	static      BOOL      ReqCtmEdit();    //请求编辑客户资料
	static      BOOL      ReqExpandMtTree();          //请求材料树子节点
	static		BOOL	  RecvReplyOfLoginIn(int len);			//接收回复信息 关于登录是否成功的
	static      BOOL      RecvReplyOfDeletNote(int len);
	static      BOOL      RecvReplyOfDetailNote(int len);    //回复用户查看通知
	static      BOOL      RecvReplyOfEmpEdit(int len);        //接收服务器关于修改员工资料的回复
	static      BOOL      RecvReplyOfFctEdit(int len);      //接收服务器关于修改厂家资料的回复
	static      BOOL      RecvReplyOfCtmEdit(int len);   //接收修改客户资料的结果
	static      BOOL      RecvReplyOfServModify(int len);   //修改服务
	static      BOOL      RecvReplyOfCtmDel(int len);      //接收服务器回应删除客户资料
	static      BOOL      RecvReplyOfStoreHistoryDel(int len);   //请求删除部分库存历史记录信息
	static      BOOL      RecvReplyOfServDel(int len);   //回复删除服务的请求
	static      BOOL      RecvReplyOfDealDel(int len);      //回复删除订单的请求
	static      BOOL      RecvReplyOfUsersDel(int len);    //回复删除用户
	static		BOOL      RecvReplyOfUsersInit(int len);      //接收服务器返还的用户权限
	static      BOOL      RecvReplyOfPosAdd(int len);    //创建新用户的回复
	static		BOOL      RecvReplyOfPosEdit(int len);     //接收修改用户的回复
	static      BOOL      RecvReplyOfPosAuthority(int len);     //接收用户权限
	static      BOOL      RecvReplyOfEmpInfoIm(int len);    //批量插入员工数据的回复
	static      BOOL      RecvReplyOfProdInfoIm(int len);   //批量插入产品数据的结果
	static      BOOL      RecvReplyOfCtmInfoIm(int len);    //批量提交客户数据到数据库
	static      BOOL      RecvReplyOfUpdateOrderBatch(int len);     //批量更新订单数据的回复
	static      BOOL      RecvReplyOfPswEdit(int len);    //修改密码的结果
	static      BOOL      RecvReplyOfCheckFile(int len);   //回复的文件是否存在的结果
	static      BOOL      RecvReplyOfFileDLInfo(int len);   //接收下载文件的信息
	static      BOOL      RecvReplyOfUploadFile(int len);    //上传文件的结果
	static      BOOL      RecvReplyOfDLFile(int len,char *strFilePath);        //接收下载文件的回复
	static      void      PrepareFileDownload(void);        //准备创建文件下载线程
	static      DWORD WINAPI   DownloadFileThread(void* pParam);	//文件下载线程
	static      DWORD WINAPI   CreateDownloadFileThread(void* pParam);	//创建文件下载线程
	static      BOOL	  RecvReplyOfEmpDel(int len);
	static      BOOL      RecvReplyOfFctDel(int len);      //接收删除厂家资料的回应
	static      BOOL	  RecvReplyOfFctNew(int len);         //接收关于新建厂家资料的回应
	static      BOOL      RecvReplyOfCtmNew(int len);   //接收返还新建客户资料的结果
	static      BOOL      RecvReplyOfAddStore(int len);    //请求添加物品库存信息
	static      BOOL      RecvReplyOfRegServ(int len);   //回复请求注册服务
	static      BOOL      RecvReplyOfOrderNew(int len);    //新建订单
	static      BOOL      RecvReplyOfOrderEdit(int len);    //编辑订单的回复
	static      BOOL      RecvReplyOfDeductSet(int len);     //关于设置提成的回复
	static      BOOL      RecvReplyOfOrderFind(int len);   //回复搜索订单
	static      BOOL      RecvReplyOfInitDeduct(int len);   //回复初始化提成
	static      BOOL      RecvReplyOfStoreAlertSet(int len);    //接收关于用户设置预警信息的回复
	static      BOOL      RecvReplyOfStoreFindAll(int len);    //回复用户搜索全部库存历史记录
	static      BOOL      RecvReplyOfServFindAll(int len);		//请求搜索服务信息
	static      BOOL      RecvReplyOfOutStore(int len);    //请求出库
	static		BOOL	  RecvReplyOfEmpNew(int len);        //请求新建员工
	static      BOOL      RecvReplyOfKeepHeart(int len);
	static      BOOL      RecvReplyOfInitMtTree(int len);
	static      BOOL	  RecvReplyOfMtNodeNew(int len);      //接收新建材料树的节点信息
	static      BOOL      RecvReplyOfMtNodeDelete(int len);     //接收删除材料树节点的回应
	static      BOOL      RecvReplyOfNotePublicMainView(int len);   //接收服务器关于发布新公告的回复
	static      BOOL	  RecvReplyOfInitNote(int len);
	static      BOOL	  RecvReplyOfMtNodeRename(int len);    //接收修改材料树节点的回应
	static      BOOL      RecvReplyOfSearchAllEmpInfo(int len);    //接收回复信息 关于搜索全部员工信息的
	static		BOOL      RecvReplyOfSearchAllFctInfo(int len);    //接收到回复信息  关于搜索全部厂家资料
	static      BOOL	  RecvReplyOfFindAllCtm(int len);      //关于搜索客户资料的回复
	static      BOOL      RecvReplyOfCtmEx(int len);     //关于客户资料批量导出
	static      BOOL	  RecvReplyOfProdEx(int len);     //接收批量导出产品数据
	static      BOOL      RecvReplyOfFindEmpPerformance(int len);   //寻找员工业绩的回复
	static      BOOL      RecvReplyOfFindAllUsers(int len);      //寻找所有用户
	static		BOOL      RecvReplyFindOrderIncomplete(int len);    //搜索未完成订单的回复
	static      BOOL      RecvReplyOfFindAllStore(int len);     //关于搜索库存资料
	static      BOOL      RecvReplyOfSearchAllEmpInfoForEx(int len);    //关于搜索员工资料以供导出
	//static      void      DeletePtr();				//删除临时指针
//////***********以下为一些全局的检验性函数*****************////////////////
public:
	static      BOOL     IsUserHasAuthority(int n_authority);
	static      CString	 GetItemFromString(CString source,int index);     //从字符串中取得子项数据
	static      void	 InitUserAuthority(char *p);   //将用户权限写到结构体里
	static      BOOL     RecvDataFromServer(SOCKET s,char* p_recv,int data_len,CString message);
	static      BOOL     SendDataToServer(SOCKET s,char* p_send,CString message,int data_len = HEADLEN);
	static      BOOL     IsNum(CString _source,BOOL bCheckDot);
	static      BOOL	 IsNum(CString str);    //传入一个字符串 ，检查是否为数字
	static      BOOL	 ContainsCharsRemain(CString str);     //检查字符串是否包含子字符串
	static      BOOL	 IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max);    //true  有效 否则无效 
//////***********以上为一些全局的检验性函数*****************////////////////

//////***********以下为一些全局的下载文件函数*****************////////////////
public:
	static      u_long		AssembleLine(FILE* destFile, int startPos, FILE* resoFile);
	static		void		AssemblyFile(void* ptr);
	static      int			RecvFileData(SOCKET s, unsigned short ID,void* ptr);  //接收文件数据函数
	static      void		SaveFile(u_short nIndexThread, const char* pBuf, u_long ulLen, u_long ulPos,void* ptr);
//////***********以上为一些全局的下载文件函数*****************////////////////


public:
	static      BOOL      m_bUpLoading;      //上传文件标志
	static		SOCKET    m_sockWhole;         //全局socket 一直和服务器保持连接
	static	    u_short   m_port;             //服务器端口
	static      CString   m_IP;               //服务器地址
	static		BOOL      m_bIsConnecting;		//判断是否与服务器连接
	static		REQTYPE   m_nCurReq;            //请求类型  
	static      CString   m_DB;                  //数据库名称
	static      BOOL	  m_bOnline;             //判断用户是否在线
	static      CString   m_nameOnline;           //已经登录的用户
	static      CString   m_user_pos;            //用户角色
	static      p_Pos_Authority	m_pPosAuthority;     //用户权限结构体
	static      HANDLE	  m_hThreadFileDL;		//创建文件下载线程
	//static		BOOL	  m_bReqAvailable;        //判断请求是否可用
////////////***以下为对话框指针区*****//////////////
public:
	static      void*		m_pLogin;            //指向登录对话框的指针
	static      void*		m_pNotePublic;       //指向发布通知对话框的指针	
	static		void*		m_pMtSetProp;
	static      void*		m_pEmpInfoView; 
	static      void*       m_pProdBatch;          //批量产品数据
	static      void*       m_pCtmSubmit;          //批量提交客户资料到数据库
	static      void*       m_pEmpInfoIm;         //员工数据批量提交到数据库
	static      void*       m_pEmpInfoEx;        //员工数据批量导出属性页
	static      void*       m_pMainFormView;         //指向主页的表单窗口
	static      void*		m_pEmpNew;               //指向新建员工对话框
	static      void*       m_pEmpEdit;              //指向编辑员工的对话框
	static      void*       m_pFctNew;				//指向新建工厂类
	static      void*       m_pFactoryEdit;			//指向厂家编辑类
	static		void*		m_pFctSet;				//指向属性页 厂家
	static      void*		m_pCtmInfo;             //指向客户资料类
	static      void*       m_pCtmNew;				//指向新建客户窗口类
	static      void*       m_pCtmEdit;            //指向编辑客户资料窗口类
	static      void*       m_pStoreAdd;			//指向添加商品类
	static      void*       m_pStoreInfo;			//指向商品的库存类
	static      void*       m_pStoreOut;            //指向商品的出库类
	static      void*       m_pStoreHistory;        //指向库存历史记录类
	static      void*       m_pStoreAlertSet;		//指向商品的预警设置类
	static      void*       m_pServReg;             //指向注册服务窗口类
	static      void*       m_pServModify;          //指向编辑服务窗口类
	static      void*       m_pServiceSearch;		//指向服务查询类
	static      void*       m_pOrderContent;        //指向新建订单窗口
	static      void*       m_pDealListView;         //指向订单视图类
	static      void*       m_pOrderEdit;           //编辑订单窗口
	static      void*       m_pDeductSet;           //指向提成设置窗口
	static      void*       m_pPwdEdit;             //指向密码编辑窗口
	static      void*       m_pCtmEx;               //指向批量导出客户资料窗口
	static      void*       m_pProdEx;               //产品资料导出
	static      void*       m_pOrderIncompleteView;      //未完成订单视图窗口
	static      void*       m_pEmpPerformance;           //员工业绩视图
	static      void*		m_pPosAdd;                  //创建新用户
	static      void*		m_pSysPostion;				//用户管理对话框
	static      void*       m_pPosEdit;                  //编辑用户对话框
	///////////****以上为对话框指针区******/////////////////
};

#endif