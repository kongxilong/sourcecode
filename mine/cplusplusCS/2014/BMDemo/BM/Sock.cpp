#include "stdafx.h"
#include "Sock.h"
#include "BM.h"
#include "Login.h"
#include "EmpInfoView.h"
#include "MtSetProp.h"
#include  "MainFrm.h"
#include  "NotePublic.h"
#include  "MainFormView.h"
#include  "EmpNew.h"
#include  "EmpEdit.h"
#include  "FactoryNew.h"
#include  "FctSet.h"
#include  "FactoryEdit.h"
#include  "CtmNew.h"
#include  "CtmEdit.h"
#include  "CtmInfo.h"
#include  "StoreInfo.h"
#include  "StoreAdd.h"
#include  "StoreOut.h"
#include  "StoreAlertSet.h"
#include  "StoreHistory.h"
#include  "ServReg.h"
#include  "ServModify.h"
#include  "ServiceSearch.h"
#include  "OrderContent.h"
#include  "DealListView.h"
#include  "OrderEdit.h"
#include  "DeductSet.h"
#include  "PwdEdit.h"
#include  "EmpInfoExProp.h"
#include  "EmpInfoProp.h"
#include  "CustomInfoExProp.h"
#include  "CustomInfoProp.h"
#include  "OrderIncompletedProgView.h"
#include  "ProdInfoProp.h"
#include  "ProdInfoExProp.h"
#include  "PerformanceMgView.h"
#include  "PosAdd.h"
#include  "SysPostion.h"
#include  "PosEdit.h"

///////////////初始化变量值//////////////////////
BOOL      CSock::m_bUpLoading = FALSE;
SOCKET    CSock::m_sockWhole = INVALID_SOCKET;
u_short   CSock::m_port = SERVPORT;
CString   CSock::m_IP = NULL;
BOOL	  CSock::m_bIsConnecting = FALSE;
//VOID*     CSock::m_pVoid = NULL;
REQTYPE   CSock::m_nCurReq = REQNON;        //是否可用根据m_bReqAvailable而定
BOOL      CSock::m_bOnline = FALSE;
CString   CSock::m_DB = NULL;
CString   CSock::m_nameOnline = NULL;
CString   CSock::m_user_pos = NULL;
HANDLE    CSock::m_hThreadFileDL = NULL;
p_Pos_Authority CSock::m_pPosAuthority = NULL;
//BOOL	  CSock::m_bReqAvailable = TRUE;      // 初始化请求时可以用的


///////////************/////////////////////
VOID*		CSock::m_pLogin = NULL;
VOID*		CSock::m_pNotePublic = NULL;
VOID*		CSock::m_pMtSetProp = NULL;
VOID*		CSock::m_pEmpInfoView = NULL;
VOID*       CSock::m_pEmpInfoEx = NULL;
VOID*       CSock::m_pEmpInfoIm = NULL;
VOID*       CSock::m_pCtmSubmit = NULL;
VOID*       CSock::m_pProdBatch = NULL;
VOID*	    CSock::m_pMainFormView = NULL;
VOID*		CSock::m_pEmpNew = NULL;
VOID*       CSock::m_pEmpEdit = NULL;
VOID*       CSock::m_pFctNew = NULL;
VOID*		CSock::m_pFctSet = NULL;
VOID*       CSock::m_pFactoryEdit = NULL;
VOID*       CSock::m_pCtmInfo = NULL;
VOID*       CSock::m_pCtmNew = NULL;
VOID*       CSock::m_pCtmEdit = NULL;
VOID*       CSock::m_pStoreInfo = NULL;
VOID*       CSock::m_pStoreAdd = NULL;
VOID*       CSock::m_pStoreOut = NULL;
VOID*       CSock::m_pStoreAlertSet = NULL;
VOID*       CSock::m_pStoreHistory = NULL;
VOID*       CSock::m_pServReg = NULL;
VOID*		CSock::m_pServModify = NULL;
VOID*		CSock::m_pServiceSearch = NULL;
VOID*       CSock::m_pOrderContent = NULL;
VOID*       CSock::m_pDealListView = NULL;
VOID*       CSock::m_pOrderEdit = NULL;
VOID*       CSock::m_pDeductSet = NULL;
VOID*       CSock::m_pPwdEdit = NULL;
VOID*       CSock::m_pCtmEx = NULL;
VOID*       CSock::m_pOrderIncompleteView = NULL;
VOID*       CSock::m_pProdEx = NULL;
VOID*		CSock::m_pEmpPerformance = NULL;
VOID*		CSock::m_pPosAdd = NULL;
VOID*		CSock::m_pSysPostion = NULL;
VOID*		CSock::m_pPosEdit = NULL;
///////////************/////////////////////
CSock::CSock(void)
{
}


CSock::~CSock(void)
{
}

BOOL CSock::StartSendHeartPackage()
{
	/*
	if(!CSock::m_bReqAvailable){
		return FALSE;
	}*/
	if(!m_bIsConnecting)      //如果已经断开连接了
	{
		//AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_nCurReq = REQKEEPHEART;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartRefreshMtTree(VOID* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		CMtSetProp* p_dlg = (CMtSetProp*)ptr;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)_T("与服务器断开连接，请重新登录！"));
		
		return FALSE;
	}
	/*
	if(!CSock::m_bReqAvailable){
		AfxMessageBox(_T("更新材料树失败,请耐心等待当前操作完成，并手动更新材料树结构！"));
		CMtSetProp* p_dlg = (CMtSetProp*)ptr;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}*/
	
	m_pMtSetProp = ptr;
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	p_dlg->SendMessage(WM_STRATREQMT_MtSetProp,0,0);   
	p_dlg->m_nodeName.Empty();
	m_nCurReq = REQ_INIT_MATERIAL_TREE;
	return TRUE;
}


BOOL CSock::StartExpandMtTree(void* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		CMtSetProp* p_dlg = (CMtSetProp*)ptr;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pMtSetProp = ptr;
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	p_dlg->SendMessage(WM_STRATREQMT_MtSetProp,0,0);
	m_nCurReq = REQ_Expant_MATERIAL_TREE;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqMtNodeEdit(VOID * ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_NODE_EDIT))
	{
		return FALSE;
	}
	
	m_pMtSetProp = ptr;
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	p_dlg->SendMessage(WM_STRATREQMT_MtSetProp,0,0);
	m_nCurReq = REQ_MT_NODE_RENAME;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqFctEdit(void* ptr)    //开启请求修改厂家资料
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_FCT_EDIT))
	{
		return FALSE;
	}
	m_pFactoryEdit = ptr;
	//CFactoryEdit* p_dlg = (CFactoryEdit*)m_pFactoryEdit;
	//p_dlg->SendMessage(WM_STARTTASK_FctEdit,0,0);
	m_nCurReq = REQ_FCT_EDIT_FctEdit;
	return TRUE;
}

BOOL  CSock::StartReqCtmEdit(void* ptr)       //请求编辑客户资料
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	m_pCtmEdit = ptr;
	//CCtmEdit* p_dlg = (CCtmEdit*)m_pCtmEdit;
	//p_dlg->SendMessage(WM_STARTTASK_CtmEdit,0,0);
	m_nCurReq = REQ_CTM_EDIT_CtmEdit;
	return TRUE;
}

BOOL CSock::StartReqEmpEdit(void* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_EDIT))
	{
		return FALSE;
	}
	m_pEmpEdit = ptr;
	//CEmpEdit* p_dlg = (CEmpEdit*)m_pEmpEdit;
	//p_dlg->SendMessage(WM_STARTTASK_EmpEdit,0,0);
	//p_dlg->DeleteSelNote();
	m_nCurReq = REQ_EMP_EDIT_EmpNew;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqCtmNew(void* ptr)   //请求新建厂家资料
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_CTM_NEW))
	{
		return FALSE;
	}
	m_pCtmNew = ptr;
	//CCtmNew* p_dlg = (CCtmNew*)m_pCtmNew;
	//p_dlg->SendMessage(WM_STARTTASK_CtmNew,0,0);
	m_nCurReq = REQ_CTM_NEW_CtmNew;
	return TRUE;
}

BOOL CSock::StartReqFctNew(void* ptr)   //请求新建厂家资料
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_FCT_NEW))
	{
		return FALSE;
	}
	m_pFctNew = ptr;
	//CFactoryNew* p_dlg = (CFactoryNew*)m_pFctNew;
	//p_dlg->SendMessage(WM_STARTTASK_FctNew,0,0);
	m_nCurReq = REQ_FCT_NEW_FctNew;
	return TRUE;
}

BOOL CSock::StartReqEmpNew(void* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_NEW))
	{
		return FALSE;
	}
	m_pEmpNew = ptr;
	//CEmpNew* p_dlg = (CEmpNew*)m_pEmpNew;
	//p_dlg->SendMessage(WM_REQEMPNEWSTART_EmpNew,0,0);
	//p_dlg->DeleteSelNote();
	m_nCurReq = REQ_EMP_NEW_EmpNew;
	return TRUE;
}

BOOL CSock::StartDelFct(void* ptr)    //请求删除厂家资料
{
	if( !(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_FCT_DEL))
	{
		return FALSE;
	}
	m_pFctSet = ptr;
	CFctSet* p_dlg = (CFctSet*)m_pFctSet;
	p_dlg->SendMessage(WM_STARTTASK_FctEdit,0,0);
	m_nCurReq = REQ_FCT_DEL_FctSet;
	return TRUE;
}

BOOL CSock::StartDelCtm(void* ptr)        //请求删除客户资料
{
	if( !(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_CTM_DEL))
	{
		return FALSE;
	}
	m_pCtmInfo = ptr;
	//CCtmInfo* p_dlg = (CCtmInfo*)m_pCtmInfo;
	//p_dlg->SendMessage(WM_STARTTASK_CtmInfo,0,0);
	m_nCurReq = REQ_CTM_DEL_CtmInfo;
	return TRUE;
}


BOOL CSock::StartDelEmpInfo(void* ptr)    //请求删除员工资料
{
	if( !(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_DEL))
	{
		return FALSE;
	}
	m_pEmpInfoView = ptr;
	//CEmpInfoView* p_dlg = (CEmpInfoView*)m_pEmpInfoView;
	//p_dlg->SendMessage(WM_STARTTASK_EmpInfoView,0,0);
	m_nCurReq = REQ_EMP_DEL_EmpInfoView;
	return TRUE;
}

BOOL CSock::StartDelNoteMainView(void* ptr)
{
	if( !(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_NOTE_DEL))
	{
		return FALSE;
	}
	m_pMainFormView = ptr;
	CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
	//p_dlg->SendMessage(WM_REQSTARTINITNOTE_MainView,0,0);
	//p_dlg->DeleteSelNote();
	m_nCurReq = REQ_DEL_NOTE_MAINVIEW;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqLoadNote(void* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	m_pMainFormView = ptr;
	CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
	//p_dlg->SendMessage(WM_REQSTARTINITNOTE_MainView,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_INIT_NOTE_MAINVIEW;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqNotePublic(VOID* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_NOTE_PUBLIC))
	{
		return FALSE;
	}
	
	m_pNotePublic = ptr;
	CNotePublic* p_dlg = (CNotePublic*)m_pNotePublic;
	//p_dlg->SendMessage(WM_REQNOTEPUBLIC_NotePublic,0,0);
	m_nCurReq = REQ_MAINVIEW_NOTE_PUBLIC;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqDeleteNode(VOID* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_NODE_DEL))
	{
		return FALSE;
	}
	
	m_pMtSetProp = ptr;
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	p_dlg->SendMessage(WM_STRATREQMT_MtSetProp,0,0);
	m_nCurReq = REQ_MT_NODE_DELETE;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartReqMtNodeNew(VOID * ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		CMtSetProp* p_dlg = (CMtSetProp*)ptr;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_NODE_NEW))
	{
		return FALSE;
	}
	
	m_pMtSetProp = ptr;
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	p_dlg->SendMessage(WM_STRATREQMT_MtSetProp,0,0);
	m_nCurReq = REQ_MT_NODE_NEW;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartLogin(VOID * ptr)
{
	m_pLogin = ptr;
	if(m_sockWhole == INVALID_SOCKET)
	{
		BOOL flag = ConnectServer();		//连接服务器
		if(FALSE == flag)
		{
			m_bOnline = FALSE;
			return FALSE;
		}
		AfxBeginThread(SendAndRecvClientThread,NULL);    //创建接收和发送数据线程
		
	}
	m_nCurReq = REQLOGIN;
	return TRUE;
}

BOOL CSock::StartFindServByKeyWord(void* ptr)   //精确查找
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pServiceSearch = ptr;
	CServiceSearch* p_dlg = (CServiceSearch*)m_pServiceSearch;
	//p_dlg->SendMessage(WM_STARTTASK_ServiceSearch,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_SERV_FIND_BY_KEYWORD; 
	return TRUE;
}

BOOL CSock::StartSearchEmpByKeyWord(void* ptr)   //精确查找
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_FIND))
	{
		return FALSE;
	}
	m_pEmpInfoView = ptr;
	CEmpInfoView* p_dlg = (CEmpInfoView*)m_pEmpInfoView;
	p_dlg->SendMessage(WM_STARTTASK_EmpInfoView,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_EMP_SEARCHBYKEYWORD_EmpInfoView; 
	return TRUE;
}

BOOL CSock::StartLookNoteDetail(void* ptr)    //查看通知栏
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pMainFormView = ptr;
	CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
	//p_dlg->SendMessage(WM_REQSTARTINITNOTE_MainView,0,0);
	//p_dlg->m_list.DeleteAllItems();
	p_dlg->m_status.SetWindowText(_T("请稍等，正在申请加载公告详细信息"));
	m_nCurReq = REQ_NOTE_DETAIL;
	return TRUE;
}

BOOL CSock::StartReqPosEdit(void* ptr)   //请求编辑用户
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_POS_EDIT))
	{
		return FALSE;
	}
	m_pPosEdit = ptr;
	CPosEdit* p_dlg = (CPosEdit*)m_pPosEdit;
	//p_dlg->SendMessage(WM_STARTTASK_PosEdit,0,0);
	p_dlg->m_tip.SetWindowText(_T("请稍等，正在申请添加用户"));
	m_nCurReq = REQ_EDIT_USER;
	return TRUE;
}

BOOL CSock::StartReqPosAdd(void* ptr)    //增加用户
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_POS_ADD))
	{
		return FALSE;
	}
	m_pPosAdd = ptr;
	CPosAdd* p_dlg = (CPosAdd*)m_pPosAdd;
	//p_dlg->SendMessage(WM_STARTTASK_PosAdd,0,0);
	p_dlg->m_tip.SetWindowText(_T("请稍等，正在申请添加用户"));
	m_nCurReq = REQ_POS_ADD;
	return TRUE;
}

BOOL CSock::StartReqPswEdit(void* ptr)       //修改密码
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pPwdEdit = ptr;
	CPwdEdit* p_dlg = (CPwdEdit*)m_pPwdEdit;
	p_dlg->SendMessage(WM_STARTTASK_PwdEdit,0,0);
	//p_dlg->m_list.DeleteAllItems();
	p_dlg->m_tip.SetWindowText(_T("请稍等，正在申请修改密码"));
	m_nCurReq = REQ_PSW_EDIT;
	return TRUE;
}

BOOL CSock::StartReqSetDeduct(void* ptr)     //设置提成
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pDeductSet = ptr;
	CDeductSet* p_dlg = (CDeductSet*)m_pDeductSet;
	p_dlg->SendMessage(WM_STARTTASK_DeductSet,0,0);
	//p_dlg->m_list.DeleteAllItems();
	p_dlg->m_tip.SetWindowText(_T("请稍等，正在保存设置"));
	m_nCurReq = REQ_SET_DEDUCT;
	return TRUE;
}

BOOL CSock::StartReqInitDeduct(void* ptr)    //初始化提成设置
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pDeductSet = ptr;
	CDeductSet* p_dlg = (CDeductSet*)m_pDeductSet;
	p_dlg->SendMessage(WM_STARTTASK_DeductSet,0,0);
	//p_dlg->m_list.DeleteAllItems();
	p_dlg->m_tip.SetWindowText(_T("请稍等，正在加载。。。"));
	m_nCurReq = REQ_INIT_DEDUCT;
	return TRUE;
}

BOOL CSock::StartReqOrderEdit(void* ptr)  //请求编辑订单
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!CSock::IsUserHasAuthority(AUTHORITY_DEAL_EDIT))
	{
		return FALSE;
	}
	m_pOrderEdit = ptr;
	COrderEdit* p_dlg = (COrderEdit*)m_pOrderEdit;
	//p_dlg->SendMessage(WM_STARTTASK_OrderEdit,0,0);
	m_nCurReq = REQ_ORDER_EDIT;
	return TRUE;
}

BOOL CSock::StartReqFindDeal(void* ptr)     //开始搜索订单
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}

	if(!CSock::IsUserHasAuthority(AUTHORITY_DEAL_FIND))
	{
		return FALSE;
	}
	m_pDealListView = ptr;
	CDealListView* p_dlg = (CDealListView*)m_pDealListView;
	//p_dlg->SendMessage(WM_STARTTASK_DealListView,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_FIND_DEAL;
	return TRUE;
}

BOOL CSock::StartReqOrderNew(void* ptr)    //新建订单
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}

	if(!CSock::IsUserHasAuthority(AUTHORITY_DEAL_NEW))
	{
		return FALSE;
	}
	m_pOrderContent = ptr;
	COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
	//p_dlg->SendMessage(WM_STARTTASK_OrderContent,0,0);
	m_nCurReq = REQ_ORDER_NEW;
	return TRUE;
}

BOOL CSock::StartDLFile(void* ptr)                //开始请求下载文件
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!CSock::IsUserHasAuthority(AUTHORITY_FILE_DOWNLOAD))
	{
		return FALSE;
	}
	m_pDealListView = ptr;
	CDealListView* p_dlg = (CDealListView*)m_pDealListView;
	p_dlg->SendMessage(WM_STRATDLOAD_DealListView,0,0);
	p_dlg->m_tip.SetWindowText(_T("请求下载"));
	m_nCurReq = REQ_FILE_SIZE;
	return TRUE;
}

BOOL CSock::StartUpLoadFile(void* ptr)           //开始上传文件
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!CSock::IsUserHasAuthority(AUTHORITY_FILE_UPLOAD))
	{
		return FALSE;
	}
	m_pOrderContent = ptr;
	COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
	p_dlg->SendMessage(WM_STARTTASK_OrderContent,0,0);
	p_dlg->m_static_upload_status.SetWindowText(_T("开始上传"));
	m_nCurReq = REQ_UPLOADFILE;
	return TRUE;
}

BOOL CSock::StartCheckFileExist(void* ptr)    //请求检验上传文件是否存在
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pOrderContent = ptr;
	COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
	p_dlg->SendMessage(WM_STARTTASK_OrderContent,0,0);
	p_dlg->m_static_upload_status.SetWindowText(_T("正在检查附件是否存在"));
	m_nCurReq = REQ_CHECK_FILEEXIST;
	return TRUE;
}

BOOL CSock::StartDelDeal(void* ptr)         //请求删除订单
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pDealListView = ptr;
	CDealListView* p_dlg = (CDealListView*)m_pDealListView;
	int index = p_dlg->m_strToDelDeal.Find(',');
	if(_T("删除附件") == p_dlg->m_strToDelDeal.Mid(0,index))
	{
		if(!CSock::IsUserHasAuthority(AUTHORITY_FILE_DEL))
		{
			return FALSE;
		}
	}else{
		if(!CSock::IsUserHasAuthority(AUTHORITY_DEAL_DEL))
		{
			return FALSE;
		}
	}
	//p_dlg->SendMessage(WM_STARTTASK_DealListView,0,0);
	m_nCurReq = REQ_DEL_DEAL;
	return TRUE;
}

BOOL CSock::StartDelServ(void* ptr)           //请求删除服务
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pServiceSearch = ptr;
	//CServiceSearch* p_dlg = (CServiceSearch*)m_pServiceSearch;
	//p_dlg->SendMessage(WM_STARTTASK_ServiceSearch,0,0);
	m_nCurReq = REQ_SERV_DEL;
	return TRUE;
}

BOOL CSock::StartDelHistory(void* ptr)    //请求删除历史记录
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_HISTORY_FIND))
	{
		return FALSE;
	}
	m_pStoreHistory = ptr;
	//CStoreHistory* p_dlg = (CStoreHistory*)m_pStoreHistory;
	//p_dlg->SendMessage(WM_STARTTASK_StoreHistory,0,0);
	m_nCurReq = REQ_DEL_STOREHISTORY;
	return TRUE;
}

BOOL CSock::StartFindAllServ(void* ptr)       //请求查询所有服务信息
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pServiceSearch = ptr;
	CServiceSearch* p_dlg = (CServiceSearch*)m_pServiceSearch;
	//p_dlg->SendMessage(WM_STARTTASK_ServiceSearch,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_SERV_FINDALL;
	return TRUE;
}

BOOL CSock::StartFindAllStoreHistory(void* ptr)   //请求搜索所有库存记录
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_HISTORY_FIND))
	{
		return FALSE;
	}
	m_pStoreHistory = ptr;
	CStoreHistory* p_dlg = (CStoreHistory*)m_pStoreHistory;
	//p_dlg->SendMessage(WM_STARTTASK_StoreHistory,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_FIND_ALL_STOREHISTORY_StoryHistory;
	return TRUE;
}

BOOL CSock::StartReqGoodsAlertNumSet(void* ptr)    //请求设置库存预警信息
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!CSock::IsUserHasAuthority(AUTHORITY_ALERT_SET))
	{
		return FALSE;
	}

	m_pStoreAlertSet = ptr;
	//CStoreAlertSet* p_dlg = (CStoreAlertSet*)m_pStoreAlertSet;
	//p_dlg->SendMessage(WM_STARTTASK_StoreAlertSet,0,0);
	m_nCurReq = REQ_GOODS_ALERT_NUM_StoreAlertSet;
	return TRUE;
}

BOOL CSock::StartReqStoreOut(void* ptr)    //请求出库
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_STORE_OUT))
	{
		return FALSE;
	}
	m_pStoreOut = ptr;
	//CStoreOut* p_dlg = (CStoreOut*)m_pStoreOut;
	//p_dlg->SendMessage(WM_STARTTASK_StoreOut,0,0);
	m_nCurReq = REQ_STORE_OUT_StoreOut;
	return TRUE;
}

BOOL CSock::StartReqServModify(void* ptr)   //请求修改服务
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pServModify = ptr;
	//CServModify* p_dlg = (CServModify*)m_pServModify;
	//p_dlg->SendMessage(WM_STARTTASK_ServModify,0,0);
	m_nCurReq = REQ_SERV_MODIFY_ServModify;
	return TRUE;
}

BOOL CSock::StartReqServReg(void* ptr)    //请求添加服务
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pServReg = ptr;
	//CServReg* p_dlg = (CServReg*)m_pServReg;
	//p_dlg->SendMessage(WM_STARTTASK_ServReg,0,0);
	m_nCurReq = REQ_SERV_REG_ServReg;
	return TRUE;
}

BOOL CSock::StartReqStoreAdd(void* ptr)    //请求添加库存物品
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_STORE_ADD))
	{
		return FALSE;
	}
	m_pStoreAdd = ptr;
	CStoreAdd* p_dlg = (CStoreAdd*)m_pStoreAdd;
	//p_dlg->SendMessage(WM_STARTTASK_StoreAdd,0,0);
	m_nCurReq = REQ_STORE_ADD_StoreAdd;
	return TRUE;
}

BOOL CSock::StartFindAllStore(void* ptr)   //请求搜索全部库存信息
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_STORE_FIND))
	{
		return FALSE;
	}
	m_pStoreInfo = ptr;
	CStoreInfo* p_dlg = (CStoreInfo*)m_pStoreInfo;
	//p_dlg->SendMessage(WM_STARTTASK_StoreInfo,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_STORE_FINDALL_StoreInfo;
	return TRUE;
}

BOOL CSock::StartFindAllCtm(void* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_CTM_FIND))
	{
		return FALSE;
	}
	m_pCtmInfo = ptr;
	CCtmInfo* p_dlg = (CCtmInfo*)m_pCtmInfo;
	//p_dlg->SendMessage(WM_STARTTASK_CtmInfo,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_CTM_FINDALL_CtmInfo;
	return TRUE;
}

BOOL CSock::StartFindALLFct(void* ptr)    //搜索全部厂家信息并显示
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_FCT_FIND))
	{
		return FALSE;
	}
	m_pFctSet = ptr;
	CFctSet* p_dlg = (CFctSet*)m_pFctSet;
	//p_dlg->SendMessage(WM_STARTTASK_FctSet,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_FCT_FINDALL_FctSet;
	return TRUE;
}

BOOL CSock::StartReqLoadUserAuthority(void* ptr)   //加载用户权限
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pPosEdit = ptr;
	CPosEdit* p_dlg = (CPosEdit*)m_pPosEdit;
	p_dlg->SendMessage(WM_STARTTASK_PosEdit,0,0);
	m_nCurReq = REQ_LOAD_USER_AUTHORITY;
	return TRUE;
}

BOOL CSock::StartDelUsers(void* ptr)     //将要被删除的用户
{
	
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_POS_DEL))
	{
		return FALSE;
	}
	m_pSysPostion = ptr;
	//CSysPostion* p_dlg = (CSysPostion*)m_pSysPostion;
	//p_dlg->SendMessage(WM_STARTTASK_SysPostion,0,0);
	m_nCurReq = REQ_DELE_USERS;
	return TRUE;
}

BOOL CSock::StartFindAllUser(void* ptr)
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_POS_FIND))
	{
		return FALSE;
	}
	m_pSysPostion = ptr;
	//CSysPostion* p_dlg = (CSysPostion*)m_pSysPostion;
	//p_dlg->SendMessage(WM_STARTTASK_SysPostion,0,0);
	m_nCurReq = REQ_FINDALL_USERS;
	return TRUE;
}

BOOL CSock::StartFindEmpPerformance(void* ptr)    //查询员工数据
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_PERFERENCE_FIND))
	{
		return FALSE;
	}
	m_pEmpPerformance = ptr;
	//CPerformanceMgView* p_view = (CPerformanceMgView*)m_pEmpPerformance;
	//p_view->SendMessage(WM_STARTTASK_EmpPerformance,0,0);
	m_nCurReq = REQ_EMP_PERFORMANCE;
	return TRUE;
}
BOOL CSock::FindDealIncomplete(void* ptr)      //查询导出未完成订单
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!CSock::IsUserHasAuthority(AUTHORITY_DEAL_INCOMPLETE_FIND))
	{
		return FALSE;
	}
	m_pOrderIncompleteView = ptr;
	//COrderIncompletedProgView* p_view = (COrderIncompletedProgView*)m_pOrderIncompleteView;
	//p_view->SendMessage(WM_STARTTASK_OrderIncomplete,0,0);
	m_nCurReq = REQ_FIND_ORDER_INCOMPLETE;
	return TRUE;
}

BOOL CSock::StartReqFindProd(void* ptr)    //批量导出产品资料
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_PROD_EX))
	{
		return FALSE;
	}
	m_pProdEx = ptr;
	CProdInfoExProp* p_dlg = (CProdInfoExProp*)m_pProdEx;
	p_dlg->SendMessage(WM_STARTTASK_ProdInfoExProp,0,0);
	p_dlg->m_prod_batch.Empty();
	//p_dlg->m_listEmp.DeleteAllItems();
	m_nCurReq = REQ_PROD_EX;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartFindCtmForEx(void* ptr)     //批量导出客户资料
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_CTM_EX))
	{
		return FALSE;
	}
	m_pCtmEx = ptr;
	CCustomInfoExProp* p_dlg = (CCustomInfoExProp*)m_pCtmEx;
	p_dlg->SendMessage(WM_STARTTASK_CustomExProp,0,0);
	p_dlg->m_ctm_batch_from_db.Empty();
	//p_dlg->m_listEmp.DeleteAllItems();
	m_nCurReq = REQ_CTM_EX;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartUpdateOrderIncomplete(void* ptr)   //批量更改未完成订单
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}

	if(!CSock::IsUserHasAuthority(AUTHORITY_DEAL_INCOMPLETE_EDIT))
	{
		return FALSE;
	}
	m_pOrderIncompleteView = ptr;
	//COrderIncompletedProgView* p_dlg = (COrderIncompletedProgView*)m_pOrderIncompleteView;
	//p_dlg->SendMessage(WM_STARTTASK_OrderIncomplete,0,0);
	m_nCurReq = REQ_UPDATE_ORDER_BATCH;
	return TRUE;
}

BOOL CSock::StartSubmitProd(void* ptr)    //批量提交产品数据
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_PROD_IM))
	{
		return FALSE;
	}
	m_pProdBatch = ptr;
	CProdInfoProp* p_dlg = (CProdInfoProp*)m_pProdBatch;
	p_dlg->SendMessage(WM_STARTTASK_ProdIm,0,0);
	m_nCurReq = REQ_UPDATE_PROD_BATCH;
	return TRUE;
}

BOOL CSock::StartSubmitCtmInfo(void* ptr)     //批量提交客户数据到数据库
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_CTM_IM))
	{
		return FALSE;
	}
	m_pCtmSubmit = ptr;
	CCustomInfoProp* p_dlg = (CCustomInfoProp*)m_pCtmSubmit;
	p_dlg->SendMessage(WM_STARTTASK_CtmIm,0,0);
	m_nCurReq = REQ_CTM_IM;
	return TRUE;
}

BOOL CSock::StartImportEmpInfo(void* ptr)     //批量提交员工信息到数据库
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_IM))
	{
		return FALSE;
	}
	m_pEmpInfoIm = ptr;
	CEmpInfoProp* p_dlg = (CEmpInfoProp*)m_pEmpInfoIm;
	p_dlg->SendMessage(WM_STARTTASK_EmpInfoProp,0,0);
	//p_dlg->m_listEmp.DeleteAllItems();
	m_nCurReq = REQ_EMP_IM;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL  CSock::StartReqFindEmpForEx(void* ptr)          //为了导出数据而查询员工数据
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	if(!IsUserHasAuthority(AUTHORITY_EMP_EX))
	{
		return FALSE;
	}
	m_pEmpInfoEx = ptr;
	CEmpInfoExProp* p_dlg = (CEmpInfoExProp*)m_pEmpInfoEx;
	p_dlg->SendMessage(WM_STARTTASK_EmpInfoExProp,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQ_EMP_EX;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

BOOL CSock::StartSearchAllEmpInfo(VOID* ptr)		//搜索全部职员信息并显示
{
	if(!(m_bIsConnecting && !m_nameOnline.IsEmpty()))      //如果已经断开连接了
	{
		AfxMessageBox(_T("与服务器断开连接，请重新登录！"));
		return FALSE;
	}
	m_pEmpInfoView = ptr;
	CEmpInfoView* p_dlg = (CEmpInfoView*)m_pEmpInfoView;
	//p_dlg->SendMessage(WM_STARTTASK_EmpInfoView,0,0);
	p_dlg->m_list.DeleteAllItems();
	m_nCurReq = REQSEARCHALLEMPINFO;
	//m_bReqAvailable = FALSE;   //使当前请求不再可用  只有当整个操作完成之后才让请求可用
	return TRUE;
}

void CSock::InitSocket()
{
	WORD	wVersionRequested;			//请求socket版本
	WSADATA	wsaData;					//wsaData结构
	int		nErrCode;					//返回值
	
	wVersionRequested = MAKEWORD( 2, 2 );//请求windows Sockets 2.2版本	
	nErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( 0 != nErrCode )
	{
		return;
	}
	
	//创建套接字
	m_sockWhole = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sockWhole)
	{
		AfxMessageBox(_T("创建套接字失败"));
		return;
	}
	
	//设置系统接收数据为默认的BUF_TIMES倍
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	nErrCode= getsockopt(m_sockWhole, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("获取系统默认发送数据缓冲区失败！"));
		return;
	}
	uiRcvBuf *= BUF_TIMES;
	nErrCode = setsockopt(m_sockWhole, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
		return;
	}

	//检查设置系统接收数据缓冲区是否成功
	unsigned int uiNewRcvBuf;
	getsockopt(m_sockWhole, SOL_SOCKET, SO_RCVBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));;
		return ;
	}

	return;
}

BOOL CSock::ConnectServer()			//完成连接服务器的任务 在登录的时候调用
{
	if(m_pLogin != NULL){
		((CLogin *)m_pLogin)->m_status.SetWindowText(_T("开始连接服务器"));
	}
	//获得服务器的IP地址
	USES_CONVERSION;
	char *pIPTemp = T2A(m_IP);		//将CString类型的IP地址转换为char指针所指的数组	
	long IP = ntohl((u_long)inet_addr(pIPTemp));
	//ntohl()是将一个无符号长整形数从网络字节顺序转换为主机字节顺序
	if(IP==INADDR_NONE)
	{
		AfxMessageBox(_T("IP地址不合法，请重新输入"));
		if(m_pLogin != NULL){
			((CLogin *)m_pLogin)->SendMessage(WM_CURSORTONORMAL,0,0);
		}
		return FALSE;
	}

	//服务器套结字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.S_un.S_addr = htonl(IP);
	servAddr.sin_port = htons(m_port);

	//连接服务器
	CBMApp* p_App = (CBMApp *)AfxGetApp();
	m_sockWhole = p_App->m_sHost;
	if(m_sockWhole == INVALID_SOCKET)
	{
		InitSocket();
		if(m_sockWhole == INVALID_SOCKET)
		{
			AfxMessageBox(_T("获得初始化的套接字失败！"));
			if(m_pLogin != NULL){
				((CLogin *)m_pLogin)->SendMessage(WM_LOGINFAIL,0,0);
				((CLogin *)m_pLogin)->SendMessage(WM_CURSORTONORMAL,0,0);
			}
			m_sockWhole = INVALID_SOCKET;		
			return FALSE;
		}
	}
	int nErrCode = connect(m_sockWhole,(sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == nErrCode)
	{
		if(NULL != m_pLogin){
			((CLogin *)m_pLogin)->m_status.SetWindowText(_T("连接服务器失败，请检查IP地址和端口是否正确"));
			((CLogin *)m_pLogin)->SendMessage(WM_CURSORTONORMAL,0,0);
			((CLogin *)m_pLogin)->SendMessage(WM_LOGINFAIL,0,0);
		}
		m_sockWhole = INVALID_SOCKET;
		AfxMessageBox(_T("连接服务器失败！"),MB_OK, 0);
		return FALSE;
	}
	if(NULL != m_pLogin){
		((CLogin *)m_pLogin)->m_status.SetWindowText(_T("连接服务器成功"));
	}
	m_bIsConnecting = TRUE;
	return TRUE;
}


UINT CSock::SendAndRecvClientThread(LPVOID pParam)		//接收和发送数据线程
{
	SOCKET	sockClient = m_sockWhole;//客户端套接字	
	if(INVALID_SOCKET == sockClient)
	{
		return 0;
	}
	FD_SET writefd;	//可写集合
	FD_SET readfd;	//可读集合

	while(m_bIsConnecting)
	{
		FD_ZERO(&writefd);		//清零
		FD_ZERO(&readfd);		//清零
		FD_SET(sockClient, &writefd);//添加到可写集合
		FD_SET(sockClient, &readfd);	//添加到可读集合
		
		int reVal = 0;
		reVal = select(0, &readfd, &writefd, NULL, NULL);//等待套接字满足条件
		if (SOCKET_ERROR == reVal)
		{
			AfxMessageBox(_T("select错误"));
			return 0;
		}else if ( reVal > 0)
		{
			if (FD_ISSET(sockClient, &writefd))			//满足可写的条件
			{
				if (FALSE == SendReq())	//发送数据
				{
					m_sockWhole = INVALID_SOCKET;
					AfxMessageBox(_T("发送数据失败,请重新登录！"));
					m_bIsConnecting = FALSE;     //连接断开 让用户重新登录
					m_bOnline = FALSE;        // 断开线后 连接不可用
					m_nameOnline.Empty();     //断开连接后 清空在线的用户名
					((CBMApp*)AfxGetApp())->m_sHost = INVALID_SOCKET;   //这样可以重新初始化socket
					return 0;
				}			
			}			
			
			if (FD_ISSET(sockClient, &readfd))			//满足可读的条件
			{
				if(FALSE == RecvData())//接收数据
				{
					//AfxMessageBox(_T("接收目录信息失败！"));
					m_sockWhole = INVALID_SOCKET;
					AfxMessageBox(_T("接收数据失败,请重新登录！"));
					m_bIsConnecting = FALSE;   //连接断开 让用户重新登录
					m_bOnline = FALSE;         // 断开线后 连接不可用
					m_nameOnline.Empty();     //断开连接后 清空在线的用户名
					//m_bReqAvailable = TRUE;   //使得请求可用
					((CBMApp*)AfxGetApp())->m_sHost = INVALID_SOCKET;  //这样可以重新初始化socket
					return 0;
				}
			}			
		}
		Sleep(THREAD_SLEEP);		
	}
	
	return 0;
}

BOOL CSock::SendReq()
{
	BOOL reVal = TRUE;			//返回值
	switch(m_nCurReq)	//请求类型
	{
	case REQLOGIN:
		{
			reVal = ReqLogin();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_LOAD_USER_AUTHORITY:
		{
			reVal = ReqLoadUserAuthority();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_DELE_USERS:
		{
			reVal = ReqDelUsers();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FINDALL_USERS:
		{
			reVal = ReqFindAllUsers();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_PERFORMANCE:
		{
			reVal = ReqFindEmpPerformance();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FIND_ORDER_INCOMPLETE:
		{
			reVal = ReqFindOrderIncomplete();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_PROD_EX:
		{
			reVal = ReqProdForEx();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CTM_EX:
		{
			reVal = ReqCtmForEx();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_UPDATE_PROD_BATCH:
		{
			reVal = ReqSubmitProdInfo();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CTM_IM:
		{
			reVal = ReqSubmitCtmInfo();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_UPDATE_ORDER_BATCH:
		{
			reVal = ReqUpdateOrderIncomplete();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_IM:
		{
			reVal = ReqSubmitEmpInfo();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_EX:
		{
			reVal = ReqSearchAllEmpInfoForEx();
			m_nCurReq = REQNON;
			break;
		}
	case REQSEARCHALLEMPINFO:
		{
			reVal = ReqSearchAllEmpInfo();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_NOTE_DETAIL:
		{
			reVal = ReqDetailNote();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EDIT_USER:
		{
			reVal = ReqPosEdit();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_POS_ADD:
		{
			reVal = ReqPosAdd();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_PSW_EDIT:
		{
			reVal = ReqPswEdit();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_SET_DEDUCT:
		{
			reVal = ReqSetDeduct();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_INIT_DEDUCT:
		{
			reVal = ReqInitDeduct();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_ORDER_EDIT:
		{
			reVal = ReqOrderEdit();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FIND_DEAL:
		{
			reVal = ReqFindDeal();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_ORDER_NEW:
		{
			reVal = ReqOrderNew();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FILE_SIZE:
		{
			reVal = ReqDLFile();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_UPLOADFILE:
		{
			reVal = ReqUpLoadFile();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CHECK_FILEEXIST:
		{
			reVal = ReqCheckFileExist();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_DEL_DEAL:
		{
			reVal = ReqDelDeal();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_SERV_DEL:
		{
			reVal = ReqDelServ();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_DEL_STOREHISTORY:
		{
			reVal = ReqDelStoreHistory();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_SERV_FINDALL:
		{
			reVal = ReqFindAllServ();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FIND_ALL_STOREHISTORY_StoryHistory:
		{
			reVal = ReqFindAllStoreHistory();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_GOODS_ALERT_NUM_StoreAlertSet:
		{
			reVal = ReqStoreAlertNumSet();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_STORE_OUT_StoreOut:
		{
			reVal = ReqOutStore();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_SERV_MODIFY_ServModify:
		{
			reVal = ReqModifyServ();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_SERV_REG_ServReg:
		{
			reVal = ReqRegServ();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_STORE_ADD_StoreAdd:
		{
			reVal = ReqAddStore();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_STORE_FINDALL_StoreInfo:
		{
			reVal = ReqFindAllStoreInfo();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CTM_FINDALL_CtmInfo:
		{
			reVal = ReqFindAllCtmInfo();
			m_nCurReq = REQNON;
			break;
		} 
	case REQ_FCT_FINDALL_FctSet:
		{
			reVal = ReqSearchAllFctInfo();
			m_nCurReq = REQNON;
			break;
		}
	case REQKEEPHEART:
		{
			reVal = ReqKeepHeart();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_INIT_MATERIAL_TREE:
		{
			reVal = ReqInitMtTree();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_Expant_MATERIAL_TREE:
		{
			reVal = ReqExpandMtTree();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_MT_NODE_NEW:
		{
			reVal = ReqMtNodeNew();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_MT_NODE_RENAME:
		{
			reVal = ReqMtNodeRename();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_MT_NODE_DELETE:
		{
			reVal = ReqMtNodeDelete();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_MAINVIEW_NOTE_PUBLIC:
		{
			reVal = ReqMainViewNotePublic();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_INIT_NOTE_MAINVIEW:
		{
			reVal = ReqMainViewInitNote();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_DEL_NOTE_MAINVIEW:
		{
			reVal = ReqMainViewDeleteSelNote();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FCT_DEL_FctSet:
		{
			reVal = ReqDeleteSelFct_FctSet();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CTM_DEL_CtmInfo:
		{
			reVal = ReqDeleteSelCtm();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_DEL_EmpInfoView:
		{
			reVal = ReqDeleteSelEmpInfo_EmpInfoView();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_NEW_EmpNew:
		{
			reVal = ReqEmpNew();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CTM_NEW_CtmNew:
		{
			reVal = ReqCtmNew();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FCT_NEW_FctNew:
		{
			reVal = ReqFctNew();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_CTM_EDIT_CtmEdit:
		{
			reVal = ReqCtmEdit();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_EDIT_EmpNew:
		{
			reVal = ReqEmpEdit();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_FCT_EDIT_FctEdit:
		{
			reVal = ReqFctEdit();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_SERV_FIND_BY_KEYWORD:
		{
			reVal = FindServByKeyWord();
			m_nCurReq = REQNON;
			break;
		}
	case REQ_EMP_SEARCHBYKEYWORD_EmpInfoView:
		{
			reVal = SearchEmpByKeyWord();
			m_nCurReq = REQNON;
			break;
		}
	default:
		break;
	}
	return reVal;
}


BOOL CSock::RecvData()
{
	BOOL	reVal = TRUE;	//返回值	
	int		nErrCode;		//错误值
	//读取包头
	hdr header;
	nErrCode = recv(m_sockWhole,(char*)&header, HEADLEN,0);		
	if (SOCKET_ERROR == nErrCode || 0 == nErrCode)//服务器关闭了
	{
		AfxMessageBox(_T("服务器关闭！"));
		reVal = FALSE;	
		return reVal;
	}	
	
	//读取包体
	int nDataLen = header.len - HEADLEN;//包体的长度	
	switch(header.type)					//根据数据包的类型分类 再读取包体
	{
	case REPLY_LOGIN:				//收到的回复信息 登录是否成功
		{
			reVal = RecvReplyOfLoginIn(nDataLen);
			break;
		}
	case REPLY_EMP_SEARCHBYKEYWORD_EmpInfoView:
	case REPLY_SEARCH_ALL_EMP_INFO:
		{
			reVal = RecvReplyOfSearchAllEmpInfo(nDataLen);
			break;
		}
	case REPLY_EMP_INFO_FOR_EX:
		{
			reVal = RecvReplyOfSearchAllEmpInfoForEx(nDataLen);
			break;
		}
	case REPLY_STORE_FINDALL_StoreInfo:
		{
			reVal = RecvReplyOfFindAllStore(nDataLen);
			break;
		}
	case REPLY_FIND_ORDER_INCOMPLETE:
		{
			reVal = RecvReplyFindOrderIncomplete(nDataLen);
			break;
		}
	case REPLY_POS_FIND_ALL:
		{
			reVal = RecvReplyOfFindAllUsers(nDataLen);
			break;
		}
	case REPLY_FIND_EMP_PERPFORMANCE:
		{
			reVal = RecvReplyOfFindEmpPerformance(nDataLen);
			break;
		}
	case REPLY_PROD_EX:
		{
			reVal = RecvReplyOfProdEx(nDataLen);
			break;
		}
	case REPLY_CTM_EX:
		{
			reVal = RecvReplyOfCtmEx(nDataLen);
			break;
		}
	case REPLY_CTM_FINDALL_CtmInfo:
		{
			reVal = RecvReplyOfFindAllCtm(nDataLen);
			break;
		}
	case REPLY_FCT_FIND_ALL:
		{
			reVal = RecvReplyOfSearchAllFctInfo(nDataLen);
			break;
		}
	case REPLY_HEART_PACKAGE:
		{
			reVal = RecvReplyOfKeepHeart(nDataLen);
			break;
		}
	case REPLY_INIT_MATERIAL_TREE:
		{
			reVal = RecvReplyOfInitMtTree(nDataLen);
			break;
		}
	case REPLY_MT_NODE_NEW:
		{
			reVal = RecvReplyOfMtNodeNew(nDataLen);
			break;
		}
	case REPLY_MT_NODE_RENAME:
		{
			reVal = RecvReplyOfMtNodeRename(nDataLen);
			break;
		}
	case REPLY_MT_NODE_DELETE:
		{
			reVal = RecvReplyOfMtNodeDelete(nDataLen);
			break;
		}
	case REPLY_NOTE_PUBLIC_MainView:
		{
			reVal = RecvReplyOfNotePublicMainView(nDataLen);
			break;
		}
	case REPLY_INIT_NOTE_MainView:
		{
			reVal = RecvReplyOfInitNote(nDataLen);
			break;
		}
	
	case REPLY_NOTE_DETAIL:
		{
			reVal = RecvReplyOfDetailNote(nDataLen);
			break;
		}
	case REPLY_DEL_NOTE_MainVIew:
		{
			reVal = RecvReplyOfDeletNote(nDataLen);
			break;
		}
	case REPLY_EMP_NEW_EmpNew:
		{
			reVal = RecvReplyOfEmpNew(nDataLen);
			break;
		}
	case REPLY_STORE_OUT_StoreOut:
		{
			reVal = RecvReplyOfOutStore(nDataLen);
			break;
		}
	case REPLY_SERV_FIND_BY_KEYWORD:
	case REPLY_SERV_FINDALL:
		{
			reVal = RecvReplyOfServFindAll(nDataLen);
			break;
		}
	case REPLY_STOREHISTORY_FINDALL:
		{
			reVal = RecvReplyOfStoreFindAll(nDataLen);
			break;
		}
	case REPLY_STORE_GOODS_ALERT_SET:
		{
			reVal = RecvReplyOfStoreAlertSet(nDataLen);
			break;
		}
	case REPLY_DEDUCT_INIT:
		{
			reVal = RecvReplyOfInitDeduct(nDataLen);
			break;
		}
	case REPLY_ORDER_FIND_DealListView:
		{
			reVal = RecvReplyOfOrderFind(nDataLen);
			break;
		}
	case REPLY_DEDUCT_SET:
		{
			reVal = RecvReplyOfDeductSet(nDataLen);
			break;
		}
	case REPLY_ORDER_EDIT_OrderEdit:
		{
			reVal = RecvReplyOfOrderEdit(nDataLen);
			break;
		}
	case REPLY_ORDER_NEW_OrderContent:
		{
			reVal = RecvReplyOfOrderNew(nDataLen);
			break;
		}
	case REPLY_SERV_REG_ServReg:
		{
			reVal = RecvReplyOfRegServ(nDataLen);
			break;
		}
	case REPLY_STORE_ADD_StoreAdd:
		{
			reVal = RecvReplyOfAddStore(nDataLen);
			break;
		}
	case REPLY_CTM_NEW_CtmNew:
		{
			reVal = RecvReplyOfCtmNew(nDataLen);
			break;
		}
	case REPLY_FCT_NEW_FctNew:
		{
			reVal = RecvReplyOfFctNew(nDataLen);
			break;
		}
	case REPLY_SERV_MODIFY_ServModify:
		{
			reVal = RecvReplyOfServModify(nDataLen);
			break;
		}
	case REPLY_CTM_EDIT_CtmEdit:
		{
			reVal = RecvReplyOfCtmEdit(nDataLen);
			break;
		}
	case REPLY_FCT_EDIT_FactoryEdit:
		{
			reVal = RecvReplyOfFctEdit(nDataLen);
			break;
		}

	case REPLY_EMP_EDIT_EmpEdit:
		{
			reVal = RecvReplyOfEmpEdit(nDataLen);
			break;
		}
	case REPLY_FILE_SIZE:
		{
			RecvReplyOfDLFile(header.flen,header.fname);
			break;
		}
	case REPLY_FILE_UPLOAD:
		{
			reVal = RecvReplyOfUploadFile(nDataLen);
			break;
		}
	case REPLY_FILE_DLINFO:
		{
			reVal = RecvReplyOfFileDLInfo(nDataLen);
			break;
		}
	case REPLY_CHECK_FILE_EXIST:
		{
			reVal = RecvReplyOfCheckFile(nDataLen);
			break;
		}
	case REPLY_EMP_INFO_IMPORT:
		{
			reVal = RecvReplyOfEmpInfoIm(nDataLen);
			break;
		}
	case REPLY_UPDATE_ORDER_BATCH:
		{
			reVal = RecvReplyOfUpdateOrderBatch(nDataLen);
			break;
		}
	case REPLY_CTM_IM:
		{
			reVal = RecvReplyOfCtmInfoIm(nDataLen);
			break;
		}
	case REPLY_PROD_IM:
		{
			reVal = RecvReplyOfProdInfoIm(nDataLen);
			break;
		}
	case REPLY_PWD_EDIT:
		{
			reVal = RecvReplyOfPswEdit(nDataLen);
			break;
		}
	case REPLY_POS_AUTHORITY:
		{
			reVal = RecvReplyOfPosAuthority(nDataLen);
			break;
		}
	case REPLY_POS_EDIT:
		{
			reVal = RecvReplyOfPosEdit(nDataLen);
			break;
		}
	case REPLY_POS_ADD:
		{
			reVal = RecvReplyOfPosAdd(nDataLen);
			break;
		}
	case REPLY_USER_INIT:
		{
			reVal = RecvReplyOfUsersInit(nDataLen);
			break;
		}
	case REPLY_POS_DEL:
		{
			reVal = RecvReplyOfUsersDel(nDataLen);
			break;
		}
	case REPLY_REQ_DEL_DEAL:
		{
			reVal = RecvReplyOfDealDel(nDataLen);
			break;
		}
	case REPLY_SERV_DEL:
		{
			reVal = RecvReplyOfServDel(nDataLen);
			break;
		}
	case REPLY_STORYHISTORY_DEL:
		{
			reVal = RecvReplyOfStoreHistoryDel(nDataLen);
			break;
		}
	case REPLY_CTM_DEL_CtmInfo:
		{
			reVal = RecvReplyOfCtmDel(nDataLen);
			break;
		}
	case REPLY_EMP_DEL_EmpInfoView:
		{
			reVal = RecvReplyOfEmpDel(nDataLen);
			break;
		}
	case REPLY_FCT_DEL_FctSet:
		{
			reVal = RecvReplyOfFctDel(nDataLen);
			break;
		}
	default:
		break;						
	}	
	if(FALSE == reVal)
	{
		AfxMessageBox(_T("接收来自服务器的回复信息失败,与服务器断开连接，请重新登录！"));
	}
	return reVal;	
}

BOOL CSock::ReqLogin()			//向服务器发送用户名和密码
{
	int nflag;
	hdr header;
	CLogin* plg;
	if(NULL != m_pLogin)
	{
		plg = (CLogin*) m_pLogin;
	}else{
		return TRUE;
	}
	loginInfo userInfo;
	memset(&header, 0, sizeof(header));
	memset(&userInfo, 0, sizeof(userInfo));
	userInfo = plg->m_loginInfo_Stru;
	header.type = LOGIN;
	header.len = HEADLEN + sizeof(loginInfo);
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送登录信息失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char *)&userInfo,sizeof(loginInfo),0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送登录信息失败，请重试或重新登录"));
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::FindServByKeyWord()    //根据关键字查找服务
{
	CServiceSearch* p_dlg = (CServiceSearch*)m_pServiceSearch;
	char buff[200] = {0};
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_searchByKeyWord);
	p_dlg->m_searchByKeyWord.Empty();
	strcpy_s(buff,p);
	int nflag;
	hdr header;
	header.type = SERV_FIND_BY_KEYWORD;
	header.len = HEADLEN + sizeof(buff);
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索服务的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索服务的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("发送搜索服务的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送搜索服务的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::SearchEmpByKeyWord()    //根据关键字搜索员工信息
{
	if(m_pEmpInfoView == NULL)
	{
		UpdateOperationStatus(_T("精确搜索员工资料已取消"));
		return TRUE;
	}
	CEmpInfoView* p_dlg = (CEmpInfoView*)m_pEmpInfoView;
	char buff[200] = {0};
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_searchByKeyWord);
	strcpy_s(buff,p);
	int nflag;
	hdr header;
	header.type = EMP_SEARCHBYKEYWORD_EmpInfoView;
	header.len = HEADLEN + sizeof(buff);
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送精确搜索员工的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送精确搜索员工的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送精确搜索员工的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送精确搜索员工的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送精确搜索员工的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqDetailNote()    //请求查找公告的详细信息
{
	WT;
	CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
	char buff[MAX_PATH] = {0};
	strcpy_s(buff,T2A(p_dlg->m_note_to_find));
	int len = sizeof(buff);
	int nflag;
	hdr header;
	header.type = NOTE_DETAIL;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送查看公告的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送查看公告的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送查看公告的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送查看公告的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送查看公告的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqPosEdit()    //请求编辑客户
{
	WT;
	CPosEdit* p_dlg = (CPosEdit*)m_pPosEdit;
	if(p_dlg == NULL)
	{
		return TRUE;
	}
	char buff[1024] = {0};
	strcpy_s(buff,T2A(p_dlg->m_str_pos_edit));
	p_dlg->m_str_pos_edit.Empty();
	int len = sizeof(buff);
	hdr header;
	header.type = POS_EDIT;
	header.len = HEADLEN + len;
	CString _err = _T("发送编辑用户的请求失败，请重试或重新登录");
	if(!SendDataToServer(m_sockWhole,(char*)&header,_err))
	{
		return FALSE;
	}
	if(!SendDataToServer(m_sockWhole,buff,_err,len))
	{
		return FALSE;
	}
	UpdateOperationStatus(_T("发送编辑用户的请求成功，等待服务器回应"));
	p_dlg->m_tip.SetWindowText(_T("发送编辑用户的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqPosAdd()    //请求添加用户
{
	WT;
	CPosAdd* p_dlg = (CPosAdd*)m_pPosAdd;
	if(p_dlg == NULL)
	{
		return TRUE;
	}
	char buff[1024] = {0};
	strcpy_s(buff,T2A(p_dlg->m_str_pos_new));
	p_dlg->m_str_pos_new.Empty();
	int len = sizeof(buff);
	int nflag;
	hdr header;
	header.type = POS_ADD;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送创建新用户的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送创建新用户的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送创建新用户的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送创建新用户的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送创建新用户的请求成功，等待服务器回应"));
	p_dlg->m_tip.SetWindowText(_T("发送创建新用户的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqPswEdit()       //请求修改密码
{
	WT;
	CPwdEdit* p_dlg = (CPwdEdit*)m_pPwdEdit;
	if(p_dlg == NULL)
	{
		return TRUE;
	}
	char buff[MAX_PATH] = {0};
	strcpy_s(buff,T2A(p_dlg->m_strPswEdit));
	int len = sizeof(buff);
	int nflag;
	hdr header;
	header.type = PWD_EDIT;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送修改密码的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送修改密码的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送修改密码的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送修改密码的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送修改密码的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqSetDeduct()     //请求设置提成
{
	CDeductSet* p_dlg = (CDeductSet*)m_pDeductSet;
	if(p_dlg == NULL)
	{
		return TRUE;
	}
	DeductInfo info =  p_dlg->m_deductInfo;
	int len = sizeof(info);
	int nflag;
	hdr header;
	header.type = DEDUCT_SET;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送设置提成的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送设置提成的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&info,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送设置提成的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送设置提成的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送设置提成的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqInitDeduct()     //请求初始化提成
{
	int nflag;
	hdr header;
	header.type = DEDUCT_INIT;
	header.len = HEADLEN;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送初始化设置的请求失败，请重试或重新登录"));
		UpdateOperationStatus(_T("发送初始化设置的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送初始化设置的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqOrderEdit()    //请求编辑订单
{
	OrderInfo info;
	int len = sizeof(info);
	memset(&info,0,len);
	COrderEdit* p_dlg = (COrderEdit*)m_pOrderEdit;
	int nflag;
	info = p_dlg->m_orderInfo_edit;
	hdr header;
	header.type = ORDER_EDIT_OrderEdit;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("向服务器请求编辑订单发送失败，请重试或重新登录"));
		UpdateOperationStatus(_T("向服务器请求编辑订单发送失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&info,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("向服务器请求编辑订单发送失败，请重试或重新登录"));
		UpdateOperationStatus(_T("向服务器请求编辑订单发送失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("请求编辑订单发送成功，等待服务器回应"));
	UpdateOperationStatus(_T("请求编辑订单发送成功，等待服务器回应"));
	return TRUE; 
}

BOOL CSock::ReqFindDeal()     //请求查找订单
{
	WT;
	char buff[MAX_PATH] = {0};
	CDealListView* p_dlg = (CDealListView*)m_pDealListView;
	int nflag;
	strcpy_s(buff,T2A(p_dlg->m_search_Keyword));
	int len = sizeof(buff);
	hdr header;
	header.type = ORDER_FIND_DealListView;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("向服务器请求搜索订单发送失败，请重试或重新登录"));
		UpdateOperationStatus(_T("向服务器请求搜索订单发送失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("向服务器请求搜索订单发送失败，请重试或重新登录"));
		UpdateOperationStatus(_T("向服务器请求搜索订单发送失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("请求搜索订单发送成功，等待服务器回应"));
	UpdateOperationStatus(_T("请求搜索订单发送成功，等待服务器回应"));
	return TRUE; 

}

BOOL CSock::ReqOrderNew()    //请求新建订单
{
	OrderInfo info;
	int len = sizeof(OrderInfo);
	memset(&info,0,len);
	COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
	if(p_dlg == NULL)
	{
		AfxMessageBox(_T("新建订单已经取消"));
		return TRUE;
	}else{
		int nflag;
		info = p_dlg->m_orderInfo;
		hdr header;
		header.type = ORDER_NEW_OrderContent;
		header.len = HEADLEN + len;
		nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
		if(nflag == SOCKET_ERROR)
		{
			AfxMessageBox(_T("向服务器请求新建订单发送失败，请重试或重新登录"));
			UpdateOperationStatus(_T("向服务器请求新建订单发送失败，请重试或重新登录"));
			return FALSE;
		}
		nflag = send(m_sockWhole,(const char*)&info,len,0);
		if(nflag == SOCKET_ERROR)
		{
			AfxMessageBox(_T("向服务器请求新建订单发送失败，请重试或重新登录"));
			UpdateOperationStatus(_T("向服务器请求新建订单发送失败，请重试或重新登录"));
			return FALSE;
		}
		p_dlg->m_tip.SetWindowText(_T("请求新建订单发送成功，等待服务器回应"));
		UpdateOperationStatus(_T("请求新建订单发送成功，等待服务器回应"));
		return TRUE; 
	}
}

BOOL CSock::ReqDLFile()     //请求下载文件
{
	WT;
	char buff[MAX_PATH] = {0};   //请求下载文件字符串  包含订单号和附件名
	CDealListView* p_dlg = (CDealListView*)m_pDealListView;
	char *p = T2A(p_dlg->m_strToDLFile);
	strcpy_s(buff,p);
	int len = sizeof(buff);
	int nflag;
	hdr header;
	header.type = FILE_SIZE;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("向服务器请求下载文件发送失败，请重试或重新登录"));
		UpdateOperationStatus(_T("向服务器请求下载文件发送失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("向服务器请求下载文件发送失败，请重试或重新登录"));
		UpdateOperationStatus(_T("向服务器请求下载文件发送失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("请求下载文件发送成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqUpLoadFile()    //向服务器上传文件
{
	USES_CONVERSION;
	CString ulFilePath;    //文件路径
	u_long ulFileSize;   //文件大小
	int nErrCode;
	hdr header;
	memset(&header,0,sizeof(header));
	header.type = SEND_FILEINFO;
	int len = sizeof(FileInfo);
	header.len = HEADLEN + len;
	FileInfo info;
	memset(&info,0,sizeof(info));
	if(m_pOrderContent != NULL)
	{
		COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
		ulFileSize = p_dlg->m_fileULInfo.filesize;
		ulFilePath = p_dlg->m_accessory_filepath;  //文件路径
		info = p_dlg->m_fileULInfo;
	}else{  //已经取消了 上传附件
		return TRUE;
	} 
	nErrCode = send(m_sockWhole,(char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{	
		if(m_pOrderContent != NULL)
		{
			COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
			p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
			p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
			p_dlg->m_tip.SetWindowText(_T("发送失败，请重试"));
			p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，请重试"));
		}else{
			AfxMessageBox(_T("发送文件失败！"));
		}
		return TRUE;
	}

	//发送包体
	nErrCode = send(m_sockWhole,(char*)&info,len,0);
	if (SOCKET_ERROR == nErrCode)
	{
		if(m_pOrderContent != NULL)
		{
			COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
			p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
			p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
			p_dlg->m_tip.SetWindowText(_T("发送失败，请重试"));
			p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，请重试"));
		}else{
			AfxMessageBox(_T("发送文件失败！"));
		}
		return TRUE;
	}
	CFile File;
	if(!File.Open(ulFilePath,CFile::modeRead|CFile::typeBinary))
	{
		if(m_pOrderContent != NULL)
		{
			COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
			p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
			p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
			p_dlg->m_tip.SetWindowText(_T("打开文件失败，请重试"));
			p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，原因打开文件失败，请重试"));
		}else{
			AfxMessageBox(_T("文件不存在"));
		}
		return TRUE;
	}
	//File.Seek(0,CFile::begin);
	File.SeekToBegin();
	int		nSendTimes = ulFileSize / SEND_BUFF +1;	//计算发送的次数
	char	cSend[SEND_BUFF]={0};						//发送文件数据的缓冲区
	
	int		i = 0;									//发送次数
	u_long	nlTotal = 0;							//读取文件长度
	u_long	nlRead = 0;								//每次读取文件长度
	int		nReadIndex = 0;							//读序号
	m_bUpLoading = TRUE;
	while (nlTotal != ulFileSize &&
					//!feof(stream) &&
					CSock::m_bIsConnecting &&
					m_bUpLoading)		//满足发送文件的长度，或者读到文件尾
	{
		nReadIndex++;//读取次数加1
		
		memset(cSend, 0, SEND_BUFF);
		File.Seek(nlTotal,CFile::begin);
		if (nReadIndex != nSendTimes)
		{
			nlRead = File.Read(cSend,SEND_BUFF);
		}else{
			u_long ulLeft = ulFileSize - nlTotal;			//最后一次
			nlRead = File.Read(cSend, ulLeft);		//读取数据 
		}
		nlTotal += nlRead;//增加已读文件长度
		if(NULL != m_pOrderContent)
		{
			COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
			p_dlg->m_ulCurTotal = nlTotal;
		}
		//定义包头
		header.type = FILE_UPLOAD;
		header.len = HEADLEN + nlRead;
		//发送包头
		nErrCode = send(m_sockWhole,(char*)&header, HEADLEN, 0);
		if (SOCKET_ERROR == nErrCode)
		{
						
			//fclose(stream);
			File.Close();
			m_bUpLoading = FALSE;
			if(m_pOrderContent != NULL)
			{
				COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
				p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
				p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
				p_dlg->m_tip.SetWindowText(_T("发送失败，请重试"));
				p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，请重试"));
			}else{
				AfxMessageBox(_T("发送文件失败！"));
			}
			return TRUE;
		}
		
		//发送包体
		nErrCode = send(m_sockWhole,cSend,nlRead, 0);
		if (SOCKET_ERROR == nErrCode)
		{
			File.Close();
			m_bUpLoading = FALSE;
			if(m_pOrderContent != NULL)
			{
				COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
				p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
				p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
				p_dlg->m_tip.SetWindowText(_T("发送失败，请重试"));
				p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，请重试"));
			}else{
				AfxMessageBox(_T("发送文件失败！"));
			}
			return TRUE;
		}
		
	}	
	
	//核实数据
	if (nlTotal != ulFileSize)
	{
		if(m_pOrderContent != NULL)
		{
			COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
			p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
			p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
			p_dlg->m_tip.SetWindowText(_T("发送失败，请重试"));
			p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，请重试"));
		}else{
			AfxMessageBox(_T("发送文件失败！"));
		}
		return TRUE;		
	}
	File.Close();	
	m_bUpLoading = FALSE;
	//发送结束包
	//包头
	header.type = FILE_UPLOAD;
	header.len = HEADLEN;   //发送文件结束标识
	//header.fpos = -1;   //发送文件结束标识
	
	//发送包头
	nErrCode = send(m_sockWhole,(char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		if(m_pOrderContent != NULL)
		{
			COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
			p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
			p_dlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
			p_dlg->m_tip.SetWindowText(_T("发送失败，请重试"));
			p_dlg->m_static_upload_status.SetWindowText(_T("上传失败，请重试"));
		}else{
			AfxMessageBox(_T("发送文件失败！"));
		}
		return TRUE;
	}
	if(m_pOrderContent != NULL)
	{
		COrderContent* p_dlg = (COrderContent*)m_pOrderContent;
		p_dlg->SendMessage(WM_RESETUPLOAD_OrderContent,0,0);
		p_dlg->m_static_upload_status.SetWindowText(_T("已上传，等待服务器处理"));
	}
	//通知服务器，客户端不再发送数据
	//shutdown(s, SD_SEND);
	return TRUE;
}

BOOL CSock::ReqCheckFileExist()    //请求查询文件是否存在
{
	int nflag;
	COrderContent *p_dlg = (COrderContent *)m_pOrderContent;
	OrderInfo order = p_dlg->m_orderInfo;
	int len = sizeof(order);
	hdr header;
	header.type = CHECK_FILE_EXIST;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于检查附件是否存在的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&order,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于检查附件是否存在的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送关于检查附件是否存在的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqDelDeal()         //请求删除订单或者附件
{
	int nflag;
	char buff[1024*30] = {0};
	int len = sizeof(buff);
	memset(buff,0,len);
	CDealListView *p_dlg = (CDealListView *)m_pDealListView;
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_strToDelDeal);
	strcpy_s(buff,p);
	hdr header;
	header.type = DEL_DEAL;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于删除订单或附件的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于删除订单或附件的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("发送关于删除订单或附件的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于删除订单或附件的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqDelServ()        //请求删除服务信息
{
	int nflag;
	char buff[1024*30] = {0};
	int len = sizeof(buff);
	memset(buff,0,len);
	CServiceSearch *p_dlg = (CServiceSearch *)m_pServiceSearch;
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_strDelServ);
	strcpy_s(buff,p);
	hdr header;
	header.type = SERV_DEL;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于删除服务的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于删除服务的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("发送关于删除服务的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于删除服务的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqDelStoreHistory()    //请求删除库存历史记录信息
{
	int nflag;
	char buff[1024*30];
	int len = sizeof(buff);
	memset(buff,0,len);
	if(m_pStoreHistory == NULL)
	{
		return TRUE;
	}
	CStoreHistory *p_dlg = (CStoreHistory *)m_pStoreHistory;
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_strToDelHistory);

	strcpy_s(buff,p);
	hdr header;
	header.type = STORYHISTORY_DEL;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于删除库存记录的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于删除库存记录的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("发送关于删除库存记录的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于删除库存记录的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindAllServ()      //请求查询所有服务
{
	int nflag;
	hdr header;
	header.type = SERV_FINDALL;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于搜索全部服务信息的请求失败，请重试或重新登录"));
		return FALSE;
	}
	CServiceSearch* p_dlg = (CServiceSearch*)m_pServiceSearch;
	p_dlg->m_tip.SetWindowText(_T("发送关于搜索全部库存信息的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于搜索全部库存信息的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindAllStoreHistory()   //搜索所有库存记录信息
{
	int nflag;
	hdr header;
	header.type = STOREHISTORY_FINDALL;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于搜索全部库存记录的请求失败，请重试或重新登录"));
		return FALSE;
	}
	CStoreHistory* p_dlg = (CStoreHistory*)m_pStoreHistory;
	p_dlg->m_tip.SetWindowText(_T("发送关于搜索全部库存记录的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于搜索全部库存记录的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqStoreAlertNumSet()    //请求库存预警信息设置
{
	if(m_pStoreAlertSet == NULL)
	{
		AfxMessageBox(_T("已经取消添加库存"));
		return FALSE;
	}
	CStoreAlertSet* p_dlg = (CStoreAlertSet*)m_pStoreAlertSet;
	StoreInfo store = p_dlg->m_goods_alert_num_set_strutct;
	memset(&p_dlg->m_goods_alert_num_set_strutct,0,sizeof(p_dlg->m_goods_alert_num_set_strutct));
	int nflag;
	hdr header;
	int len = sizeof(StoreInfo);
	header.type = STORE_GOODS_ALERT_SET;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于设置库存预警信息的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&store,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于设置库存预警信息的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("发送关于设置库存预警信息的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于设置库存预警信息的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqOutStore()        //请求出库
{
	if(m_pStoreOut == NULL)
	{
		AfxMessageBox(_T("已经取消添加库存"));
		return FALSE;
	}
	CStoreOut* p_dlg = (CStoreOut*)m_pStoreOut;
	StoreInfo store = p_dlg->m_goods_out_struct;
	memset(&p_dlg->m_goods_out_struct,0,sizeof(p_dlg->m_goods_out_struct));
	int nflag;
	hdr header;
	int len = sizeof(StoreInfo);
	header.type = STORE_OUT_StoreOut;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于入库的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&store,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送关于入库的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("发送关于入库的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送关于入库的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqModifyServ()    //请求修改服务
{
	if(m_pServModify == NULL)
	{
		AfxMessageBox(_T("已经取消修改服务"));
		return FALSE;
	}
	CServModify* p_dlg = (CServModify*)m_pServModify;
	ServInfo serv = p_dlg->m_servInfo_struct;
	memset(&p_dlg->m_servInfo_struct,0,sizeof(p_dlg->m_servInfo_struct));
	int nflag;
	hdr header;
	int len = sizeof(ServInfo);
	header.type = SERV_MODIFY_ServModify;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("修改服务的请求发送失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&serv,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("修改服务的请求发送失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("修改服务的请求发送成功，等待服务器回应"));
	UpdateOperationStatus(_T("修改服务的请求发送成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqRegServ()   //请求注册服务
{
	if(m_pServReg == NULL)
	{
		AfxMessageBox(_T("已经取消添加库存"));
		return FALSE;
	}
	CServReg* p_dlg = (CServReg*)m_pServReg;
	ServInfo serv = p_dlg->m_servInfo_struct;
	memset(&p_dlg->m_servInfo_struct,0,sizeof(p_dlg->m_servInfo_struct));
	int nflag;
	hdr header;
	int len = sizeof(ServInfo);
	header.type = SERV_REG_ServReg;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("注册服务的请求发送失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&serv,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("注册服务的请求发送失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("注册服务的请求发送成功，等待服务器回应"));
	UpdateOperationStatus(_T("注册服务的请求发送成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqAddStore()        //请求添加物品库存
{
	if(m_pStoreAdd == NULL)
	{
		AfxMessageBox(_T("已经取消添加库存"));
		return FALSE;
	}
	CStoreAdd* p_dlg = (CStoreAdd*)m_pStoreAdd;
	StoreInfo strore = p_dlg->m_storeAddStruct;
	memset(&p_dlg->m_storeAddStruct,0,sizeof(p_dlg->m_storeAddStruct));
	int nflag;
	hdr header;
	int len = sizeof(StoreInfo);
	header.type = STORE_ADD_StoreAdd;
	header.len = HEADLEN + len;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("入库的请求发送失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)&strore,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("入库的请求发送失败，请重试或重新登录"));
		return FALSE;
	}
	p_dlg->m_tip.SetWindowText(_T("入库的请求发送成功，等待服务器回应"));
	UpdateOperationStatus(_T("入库的请求发送成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindAllStoreInfo()     //请求搜索所有库存资料
{
	int nflag;
	hdr header;
	header.type = STORE_FINDALL_StoreInfo;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部库存资料的请求失败，请重试或重新登录"));
		return FALSE;
	}
	CStoreInfo* p_dlg = (CStoreInfo*)m_pStoreInfo;
	p_dlg->m_tip.SetWindowText(_T("发送搜索全部库存资料的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送搜索全部库存资料的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqLoadUserAuthority()      //加载用户权限
{
	WT;
	char buff[1024] = {0};
	int len = sizeof(buff);
	CPosEdit* p_dlg = (CPosEdit*)m_pPosEdit;
	strcpy_s(buff,T2A(p_dlg->m_emp_name));
	hdr header;
	header.type = USER_INIT;
	header.len = HEADLEN + len;
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送加载用户权限的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送加载用户权限的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送加载用户权限的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqDelUsers()   //删除用户
{
	WT;
	char buff[1024] = {0};
	int len = sizeof(buff);
	CSysPostion* p_dlg = (CSysPostion*)m_pSysPostion;
	strcpy_s(buff,T2A(p_dlg->m_users_to_delete));
	hdr header;
	header.type = POS_DEL;
	header.len = HEADLEN + len;
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送删除用户的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送删除用户的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送删除用户的请求成功，等待服务器回应"));
	p_dlg->m_tip.SetWindowText(_T("发送删除用户的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindAllUsers()    //查找所有用户
{
	WT;
	char buff[1024] = {0};
	int len = sizeof(buff);
	CSysPostion* p_dlg = (CSysPostion*)m_pSysPostion;
	strcpy_s(buff,T2A(p_dlg->m_content_to_find));
	hdr header;
	header.type = POS_FIND_ALL;
	header.len = HEADLEN + len;
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索所有用户的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索所有用户的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索所有用户的请求成功，等待服务器回应"));
	p_dlg->m_tip.SetWindowText(_T("发送搜索所有用户的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindEmpPerformance()   //查询员工业绩表
{
	WT;
	char buff[1024] = {0};
	int len = sizeof(buff);
	CPerformanceMgView* p_view = (CPerformanceMgView*)m_pEmpPerformance;
	strcpy_s(buff,T2A(p_view->m_keyword));
	p_view->m_keyword.Empty();
	hdr header;
	header.type = FIND_EMP_PERPFORMANCE;
	header.len = HEADLEN + len;
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索员工业绩的请求失败，请重试或重新登录"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索员工业绩的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索员工业绩的请求成功，等待服务器回应"));
	p_view->m_tip.SetWindowText(_T("发送搜索员工业绩的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindOrderIncomplete()    //查询未完成订单
{
	int nflag;
	hdr header;
	header.type = FIND_ORDER_INCOMPLETE;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索未完成订单的请求失败，请重试或重新登录"));
		return FALSE;
	}
	COrderIncompletedProgView* p_view = (COrderIncompletedProgView*)m_pOrderIncompleteView;
	p_view->m_tip.SetWindowText(_T("发送搜索未完成订单的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送搜索未完成订单的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqProdForEx()   //请求所有产品数据
{
	int nflag;
	hdr header;
	header.type = PROD_EX;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部产品资料的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索全部产品资料的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqCtmForEx()    //请求所有客户资料
{
	int nflag;
	hdr header;
	header.type = CTM_EX;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部客户资料的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索全部厂家资料的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFindAllCtmInfo()     //向服务器发送查询所有客户资料的申请
{
	int nflag;
	hdr header;
	header.type = CTM_FINDALL_CtmInfo;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部客户资料的请求失败，请重试或重新登录"));
		return FALSE;
	}
	CCtmInfo* p_dlg = (CCtmInfo*)m_pCtmInfo;
	p_dlg->m_tip.SetWindowText(_T("发送搜索全部厂家资料的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送搜索全部厂家资料的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqSearchAllFctInfo()   //向服务器请求发送搜索所有厂家信息
{ 
	int nflag;
	hdr header;
	header.type = FCT_FIND_ALL;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部厂家的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索全部厂家的请求成功，等待服务器回应"));
	CFctSet* p_dlg = (CFctSet*)m_pFctSet;
	p_dlg->m_tip.SetWindowText(_T("发送搜索全部厂家的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqUpdateOrderIncomplete()    //提交订单数据到数据库
{
	WT;
	int nflag;
	hdr header;
	char buff[1024*10] = {0};
	int len = sizeof(buff);
	
	header.type = UPDATE_ORDER_BATCH;
	header.len = HEADLEN + len;
	COrderIncompletedProgView* p_view = (COrderIncompletedProgView*)m_pOrderIncompleteView;
	int size = p_view->m_OrderToUpdateArr.GetSize();
	
	int i = 0;
	CString temp = NULL;
	CString strSend = NULL;
	while(i<size)
	{
		int index_list = atoi(T2A(p_view->m_OrderToUpdateArr.GetAt(i)));
		temp += _T("{#[#");
		temp += p_view->m_list.GetItemText(index_list,0);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,1);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,2);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,3);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,4);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,5);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,6);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,7);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,8);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,9);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,10);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,11);
		temp += _T("#][#");
		temp += p_view->m_list.GetItemText(index_list,12);
		temp += _T("#]#}");
		strSend += temp;
		temp.Empty();
		i++;
		if((i == size) ||(i%10 == 0))   //最后一行或者每10条数据 发送一次
		{
			strcpy_s(buff,T2A(strSend));
			nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量更新订单数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			nflag = send(m_sockWhole,(const char*)buff,len,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量更新订单数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			strSend.Empty();
			memset(buff,0,len);
		}
	}
	
	header.len = HEADLEN;  //标志着发送结束
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送批量更新订单数据的请求失败，请重试或重新登录"));
		return FALSE;
	}
	p_view->m_OrderToUpdateArr.RemoveAll();
	p_view->m_tip.SetWindowText(_T("发送批量更新订单数据的请求成功，等待服务器回应"));
	UpdateOperationStatus(_T("发送批量更新订单数据的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqSubmitProdInfo()   //批量插入产品数据
{
	WT;
	int nflag;
	hdr header;
	char buff[1024*10] = {0};
	int len = sizeof(buff);
	
	header.type = PROD_IM;
	header.len = HEADLEN + len;
	CProdInfoProp* p_dlg = (CProdInfoProp*)m_pProdBatch;
	int size = p_dlg->m_arr_prodInfoForIm.GetSize();
	
	int i = 0;
	CString temp = NULL;
	CString strSend = NULL;
	while(i<size)
	{
		int index = atoi(T2A(p_dlg->m_arr_prodInfoForIm.GetAt(i)));
		temp += _T("{#[#");
		temp += p_dlg->m_listProd.GetItemText(index,0);
		temp += _T("#][#");
		temp += p_dlg->m_listProd.GetItemText(index,1);
		temp += _T("#][#");
		temp += p_dlg->m_listProd.GetItemText(index,2);
		temp += _T("#][#");
		temp += p_dlg->m_listProd.GetItemText(index,3);
		temp += _T("#]#}");
		strSend += temp;
		temp.Empty();
		i++;
		if((i == size) ||(i%10 == 0))   //最后一行或者每10条数据 发送一次
		{
			strcpy_s(buff,T2A(strSend));
			nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量提交产品数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			nflag = send(m_sockWhole,(const char*)buff,len,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量提交产品数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			strSend.Empty();
			memset(buff,0,len);
		}
	}
	header.len = HEADLEN;  //标志着发送结束
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送批量提交产品数据的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送批量提交产品数据的请求成功，等待服务器回应"));
	return TRUE;
}


BOOL CSock::ReqSubmitCtmInfo()   //批量提交客户数据到数据库
{
	WT;
	int nflag;
	hdr header;
	char buff[1024*10] = {0};
	int len = sizeof(buff);
	
	header.type = CTM_IM;
	header.len = HEADLEN + len;
	CCustomInfoProp* p_dlg = (CCustomInfoProp*)m_pCtmSubmit;
	int size = p_dlg->m_ctm_to_submit_arr.GetSize();
	
	int i = 0;
	CString temp = NULL;
	CString strSend = NULL;
	while(i<size)
	{
		temp += _T("{#[#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),0);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),1);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),2);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),3);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),4);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),5);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),6);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),7);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),8);
		temp += _T("#][#");
		temp += p_dlg->m_ctmInfoList.GetItemText(atoi(T2A(p_dlg->m_ctm_to_submit_arr.GetAt(i))),9);
		temp += _T("#]#}");
		strSend += temp;
		temp.Empty();
		i++;
		if((i == size) ||(i%10 == 0))   //最后一行或者每10条数据 发送一次
		{
			strcpy_s(buff,T2A(strSend));
			nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量提交客户数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			nflag = send(m_sockWhole,(const char*)buff,len,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量提交客户数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			strSend.Empty();
			memset(buff,0,len);
		}
	}
	header.len = HEADLEN;  //标志着发送结束
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送批量提交客户数据的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送批量提交客户数据的请求成功，等待服务器回应"));
	return TRUE;
}




BOOL CSock::ReqSubmitEmpInfo()    //批量提交员工数据到数据库
{
	WT;
	int nflag;
	hdr header;
	char buff[1024*10] = {0};
	int len = sizeof(buff);
	
	header.type = EMP_INFO_IMPORT;
	header.len = HEADLEN + len;
	CEmpInfoProp* p_dlg = (CEmpInfoProp*)m_pEmpInfoIm;
	int size = p_dlg->m_num_to_submit_to_db.GetSize();
	
	int i = 0;
	CString temp = NULL;
	CString strSend = NULL;
	while(i<size)
	{
		temp += _T("{#[#");
		temp += p_dlg->m_listEmp.GetItemText(atoi(T2A(p_dlg->m_num_to_submit_to_db.GetAt(i))),0);
		temp += _T("#][#");
		temp += p_dlg->m_listEmp.GetItemText(atoi(T2A(p_dlg->m_num_to_submit_to_db.GetAt(i))),1);
		temp += _T("#][#");
		temp += p_dlg->m_listEmp.GetItemText(atoi(T2A(p_dlg->m_num_to_submit_to_db.GetAt(i))),2);
		temp += _T("#][#");
		temp += p_dlg->m_listEmp.GetItemText(atoi(T2A(p_dlg->m_num_to_submit_to_db.GetAt(i))),3);
		temp += _T("#][#");
		temp += p_dlg->m_listEmp.GetItemText(atoi(T2A(p_dlg->m_num_to_submit_to_db.GetAt(i))),4);
		temp += _T("#][#");
		temp += p_dlg->m_listEmp.GetItemText(atoi(T2A(p_dlg->m_num_to_submit_to_db.GetAt(i))),5);
		temp += _T("#]#}");
		strSend += temp;
		temp.Empty();
		i++;
		if((i == size) ||(i%10 == 0))   //最后一行或者每10条数据 发送一次
		{
			strcpy_s(buff,T2A(strSend));
			nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量提交员工数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			nflag = send(m_sockWhole,(const char*)buff,len,0);
			if(nflag == SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送批量提交员工数据的请求失败，请重试或重新登录"));
				return FALSE;
			}
			strSend.Empty();
			memset(buff,0,len);
		}
	}
	header.len = HEADLEN;  //标志着发送结束
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送批量提交员工数据的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送批量提交员工数据的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqSearchAllEmpInfoForEx()   //向服务器请求所有员工数据 为了批量导出
{
	int nflag;
	hdr header;
	header.type = EMP_INFO_FOR_EX;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部员工的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索全部员工的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqSearchAllEmpInfo()		//向服务器发送搜索所有员工信息的请求
{
	int nflag;
	hdr header;
	header.type = SEARCH_ALL_EMP_INFO;
	header.len = HEADLEN ;
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送搜索全部员工的请求失败，请重试或重新登录"));
		return FALSE;
	}
	UpdateOperationStatus(_T("发送搜索全部员工的请求成功，等待服务器回应"));
	CEmpInfoView* p_dlg = (CEmpInfoView*)m_pEmpInfoView;
	p_dlg->m_tip.SetWindowText(_T("发送搜索全部员工的请求成功，等待服务器回应"));
	return TRUE;
}

BOOL CSock::ReqFctEdit()   //请求编辑厂家资料
{
	if(m_pFactoryEdit == NULL)
	{
		UpdateOperationStatus(_T("已经取消编辑厂家资料"));
		return TRUE;
	}
	CFactoryEdit* p_dlg = (CFactoryEdit*)m_pFactoryEdit;
	FctInfo fct;
	memset(&fct,0,sizeof(fct));
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_addr_fctEdit);
	strcpy_s(fct.addr_fct,p);
	p = T2A(p_dlg->m_name_fctEdit);
	strcpy_s(fct.name_fct,p);
	p = T2A(p_dlg->m_phone_fctEdit);
	strcpy_s(fct.phone_fct,p);
	p = T2A(p_dlg->m_contact_person_fctEdit);
	strcpy_s(fct.contacts_fct,p);
	p = T2A(p_dlg->m_main_product_fctEdit);
	strcpy_s(fct.main_product_fct,p);
	p = T2A(p_dlg->m_ps_fctEdit);
	strcpy_s(fct.ps_fct,p);

	
	hdr header;
	header.type = FCT_EDIT_FactoryEdit;
	header.len = HEADLEN + sizeof(fct);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求编辑厂家资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&fct,sizeof(fct),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求编辑厂家资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求编辑厂家资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}

BOOL CSock::ReqCtmEdit()    //请求编辑客户资料
{
	if(m_pCtmEdit == NULL)
	{
		UpdateOperationStatus(_T("已经取消编辑客户资料"));
		return TRUE;
	}
	CCtmEdit* p_dlg = (CCtmEdit*)m_pCtmEdit;
	CtmInfo ctm = p_dlg->m_ctmEditStruct;
	memset(&p_dlg->m_ctmEditStruct,0,sizeof(p_dlg->m_ctmEditStruct));
	int len = sizeof(CtmInfo);
	
	hdr header;
	header.type = CTM_EDIT_CtmEdit;
	header.len = HEADLEN + len;
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求修改客户资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&ctm,len,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求修改客户资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求修改客户资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}

BOOL CSock::ReqEmpEdit()     //请求编辑员工
{
	if(m_pEmpEdit == NULL)
	{
		UpdateOperationStatus(_T("已经取消编辑员工"));
		return TRUE;
	}
	CEmpNew* p_dlg = (CEmpNew*)m_pEmpEdit;
	EmpInfo emp;
	memset(&emp,0,sizeof(emp));
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_addr_emp);
	strcpy_s(emp.empAddr,p);
	p = T2A(p_dlg->m_name_emp);
	strcpy_s(emp.empName,p);
	p = T2A(p_dlg->m_num_emp);
	strcpy_s(emp.empNum,p);
	p = T2A(p_dlg->m_phone_emp);
	strcpy_s(emp.empPhone,p);
	p = T2A(p_dlg->m_pos_emp);
	strcpy_s(emp.empPos,p);
	p = T2A(p_dlg->m_ps_emp);
	strcpy_s(emp.empPS,p);

	
	hdr header;
	header.type = EMP_EDIT_EmpEdit;
	header.len = HEADLEN + sizeof(emp);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求修改员工资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&emp,sizeof(emp),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求修改员工资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求修改员工资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}

BOOL  CSock::ReqCtmNew()   //请求新建用户资料
{
	if(m_pCtmNew == NULL)
	{
		UpdateOperationStatus(_T("已经取消新建客户资料"));
		return TRUE;
	}
	CCtmNew* p_dlg = (CCtmNew*)m_pCtmNew;
	int len = sizeof(CtmInfo);
	CtmInfo ctm = p_dlg->m_ctmNewStruct;
	memset(&p_dlg->m_ctmNewStruct,0,sizeof(p_dlg->m_ctmNewStruct));
	hdr header;
	header.type = CTM_NEW_CtmNew;
	header.len = HEADLEN + len;
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求新建客户资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&ctm,len,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求新建客户资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求新建客户资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}


BOOL CSock::ReqFctNew()      //请求新建厂家资料
{
	if(m_pFctNew == NULL)
	{
		UpdateOperationStatus(_T("已经取消新建员工"));
		return TRUE;
	}
	CFactoryNew* p_dlg = (CFactoryNew*)m_pFctNew;
	FctInfo fct;
	memset(&fct,0,sizeof(fct));
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_addr_fctNew);
	strcpy_s(fct.addr_fct,p);
	p = T2A(p_dlg->m_name_fctNew);
	strcpy_s(fct.name_fct,p);
	p = T2A(p_dlg->m_phone_fctNew);
	strcpy_s(fct.phone_fct,p);
	p = T2A(p_dlg->m_contacts_fctNew);
	strcpy_s(fct.contacts_fct,p);
	p = T2A(p_dlg->m_mainProduct_fctNew);
	strcpy_s(fct.main_product_fct,p);
	p = T2A(p_dlg->m_ps_fctNew);
	strcpy_s(fct.ps_fct,p);

	
	hdr header;
	header.type = FCT_NEW_FctNew;
	header.len = HEADLEN + sizeof(fct);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求新建厂家资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&fct,sizeof(fct),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求新建厂家资料发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求新建厂家资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}

BOOL CSock::ReqEmpNew()     //请求新建员工
{
	if(m_pEmpNew == NULL)
	{
		UpdateOperationStatus(_T("已经取消新建员工"));
		return TRUE;
	}
	CEmpNew* p_dlg = (CEmpNew*)m_pEmpNew;
	EmpInfo emp;
	memset(&emp,0,sizeof(emp));
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_addr_emp);
	strcpy_s(emp.empAddr,p);
	p = T2A(p_dlg->m_name_emp);
	strcpy_s(emp.empName,p);
	p = T2A(p_dlg->m_num_emp);
	strcpy_s(emp.empNum,p);
	p = T2A(p_dlg->m_phone_emp);
	strcpy_s(emp.empPhone,p);
	p = T2A(p_dlg->m_pos_emp);
	strcpy_s(emp.empPos,p);
	p = T2A(p_dlg->m_ps_emp);
	strcpy_s(emp.empPS,p);

	
	hdr header;
	header.type = EMP_NEW_EmpNew;
	header.len = HEADLEN + sizeof(emp);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求新建员工发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&emp,sizeof(emp),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求新建员工发送失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求新建员工已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}

BOOL CSock::ReqDeleteSelFct_FctSet()     //发起请求删除厂家资料
{
	if(m_pFctSet == NULL)
	{
		UpdateOperationStatus(_T("已经取消删除员工资料"));
		return TRUE;
	}
	CFctSet* p_dlg = (CFctSet*)m_pFctSet;
	char buff[30*1024] = {0};
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_strfctToDel);
	strcpy_s(buff,p);
	
	hdr header;
	header.type = FCT_DEL_FctSet;
	header.len = HEADLEN + sizeof(buff);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除厂家资料失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除厂家资料失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求删除厂家资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	return TRUE;
}

BOOL CSock::ReqDeleteSelCtm()      //发起请求删除客户资料
{
	if(m_pCtmInfo == NULL)
	{
		UpdateOperationStatus(_T("已经取消删除员工资料"));
		return TRUE;
	}
	CCtmInfo* p_dlg = (CCtmInfo*)m_pCtmInfo;
	char buff[30*1024] = {0};
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_strToDelCtm);
	strcpy_s(buff,p);
	
	hdr header;
	header.type = CTM_DEL_CtmInfo;
	header.len = HEADLEN + sizeof(buff);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除客户资料失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除客户资料失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求删除客户资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;

}

BOOL CSock::ReqDeleteSelEmpInfo_EmpInfoView()   //发起请求删除员工资料
{
	if(m_pEmpInfoView == NULL)
	{
		UpdateOperationStatus(_T("已经取消删除员工资料"));
		return TRUE;
	}
	CEmpInfoView* p_dlg = (CEmpInfoView*)m_pEmpInfoView;
	char buff[30*1024] = {0};
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_empToDelete);
	strcpy_s(buff,p);
	
	hdr header;
	header.type = EMP_DEL_EmpInfoView;
	header.len = HEADLEN + sizeof(buff);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除员工资料失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除员工资料失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求删除员工资料已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	p_dlg->m_tip.SetWindowText(_str);
	return TRUE;
}

BOOL  CSock::ReqMainViewDeleteSelNote()
{
	if(m_pMainFormView == NULL)
	{
		UpdateOperationStatus(_T("已经取消删除通知"));
		return TRUE;
	}
	CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
	char buff[30*1024] = {0};
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_noteToDelete);
	strcpy_s(buff,p);
	
	hdr header;
	header.type = DEL_NOTE_MainVIew;
	header.len = HEADLEN + sizeof(buff);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除通知失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求删除通知失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求删除通知已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	return TRUE;
}

BOOL  CSock::ReqMainViewInitNote()  //请求加载通知
{
	CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
	int total = p_dlg->m_nNoteToLook;   //加载9条最近通知
	hdr header;
	header.type = INIT_NOTE_MainView;
	header.len = HEADLEN + sizeof(int);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求初始化通知失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&total,sizeof(int),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求初始化通知失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	CString _str = _T("请求初始化通知已发送，等待服务器响应");
	UpdateOperationStatus(_str);
	return TRUE;
}

BOOL  CSock::ReqMainViewNotePublic()      //请求发布通知
{
	NotePublic_MainView note;
	memset(&note,0,sizeof(note));
	CNotePublic* p_dlg;
	if(NULL != m_pNotePublic)
	{
		p_dlg = (CNotePublic*)m_pNotePublic;
	}else   //如果突然选择了终止请求 或者有了别的请求
	{
		p_dlg = NULL;
		return TRUE;
	}
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_content);
	strcpy_s(note.content,p);
	p = T2A(p_dlg->m_title);
	strcpy_s(note.title,p);
	p = T2A(p_dlg->m_personPublicName);
	strcpy_s(note.persion_public,p);
	p = T2A(p_dlg->m_cur_time);
	strcpy_s(note.time_public,p);
	
	
	CString _str = _T("请求已发起，等待服务器回应！");
	p_dlg->m_status.SetWindowText(_str);
	UpdateOperationStatus(_str);

	hdr header;
	header.type = NOTE_PUBLIC_MainView;
	header.len = HEADLEN + sizeof(note);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求发布公告失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&note,sizeof(note),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("请求发布公告失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	return TRUE;
}
BOOL CSock::ReqMtNodeDelete()     //请求删除材料某个节点
{
	char buff[100] = {0};
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	if(m_pMtSetProp == NULL){
		UpdateOperationStatus(_T("取消删除材料节点"));
		return TRUE;
	}
	USES_CONVERSION;
	char *p = T2A(p_dlg->m_nodeName);
	strcpy_s(buff,p);
	
	CString _str = _T("正在向服务器发起删除材料子节点请求，等待服务器回应！");
	p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
	UpdateOperationStatus(_str);

	hdr header;
	header.type = MT_NODE_DELETE;
	header.len = HEADLEN + sizeof(buff);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("发送请求删除材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("发送删除修改材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::ReqMtNodeRename()
{
	CMtSetProp* p_dlg;
	if(m_pMtSetProp != NULL)
	{
		p_dlg = (CMtSetProp*)m_pMtSetProp;
	}else
	{
		UpdateOperationStatus(_T("节点重命名取消"));
		return TRUE;
	}
	typedef struct _MtNodeEdit
	{
		char nodeOrgName[30];
		char nodeNewName[30];
	}MtNodeEdit;
	MtNodeEdit node;
	memset(&node,0,sizeof(node));
	USES_CONVERSION;
	char *p_nodeNewName = T2A(p_dlg->m_nodeNameNew_Edit);
	char *p_nodeOrgName = T2A(p_dlg->m_nodeNameOrg_Edit);
	strcpy_s(node.nodeOrgName,p_nodeOrgName);
	strcpy_s(node.nodeNewName,p_nodeNewName);
	hdr header;
	header.type = MT_NODE_RENAME;
	header.len = HEADLEN + sizeof(node);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("发送请求修改材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&node,sizeof(node),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("发送请求修改材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		return FALSE;
	}
	CString _str = _T("正在向服务器发起重命名材料节点请求，等待服务器回应！");
	p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
	UpdateOperationStatus(_str);
	return TRUE;
}

BOOL CSock::ReqMtNodeNew()
{
	CMtSetProp* p_dlg;
	if(m_pMtSetProp != NULL)
	{
		p_dlg = (CMtSetProp*)m_pMtSetProp;
	}else
	{
		UpdateOperationStatus(_T("已经取消新建材料节点的请求"));
		return TRUE;
	}
	typedef struct _MtNodeNew
	{
		char parentNodeName[30];
		char nodeNew[30];
	}MtNodeNew;
	MtNodeNew node;
	memset(&node,0,sizeof(node));
	USES_CONVERSION;
	char *p_nodeNewName = T2A(p_dlg->m_nodeNameNew);
	char *p_parentNode = T2A(p_dlg->m_parentNodeName);
	strcpy_s(node.nodeNew,p_nodeNewName);
	strcpy_s(node.parentNodeName,p_parentNode);
	hdr header;
	header.type = MT_NODE_NEW;
	header.len = HEADLEN + sizeof(node);
	int errcode = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("发送请求新建材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		//CMtSetProp* p_dlg = (CMtSetProp*)m_pVoid;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		return FALSE;
	}
	errcode = send(m_sockWhole,(const char*)&node,sizeof(node),0);
	if(errcode == SOCKET_ERROR)
	{
		CString _str = _T("发送请求新建材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		//CMtSetProp* p_dlg = (CMtSetProp*)m_pVoid;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		return FALSE;
	}
	CString _str = _T("正在向服务器发起材料节点请求，等待服务器回应！");
	p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
	UpdateOperationStatus(_str);
	return TRUE;
}

BOOL CSock::ReqExpandMtTree()
{
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	if(m_pMtSetProp == NULL){
		UpdateOperationStatus(_T("取消展开材料树"));
		return TRUE;
	}
	CString req = p_dlg->m_nodeName;
	USES_CONVERSION;
	char *p = T2A(req);
	char buff[100] = {0};
	strcpy_s(buff,p);
	hdr header;
	header.type = EXPAND_MATERIAL_TREE;
	header.len = HEADLEN + sizeof(buff);
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		CString _str = _T("发送请求材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char*)buff,sizeof(buff),0);
	if(nflag == SOCKET_ERROR)
	{
		CString _str = _T("发送请求材料树子节点失败，请重试或重新登录");
		AfxMessageBox(_str);
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::ReqInitMtTree()
{
	hdr header;
	header.type = INIT_MATERIAL_TREE;
	header.len = HEADLEN ;
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		CString _str = _T("发送初始化材料树数据包失败，请重试或重新登录");
		AfxMessageBox(_str);
		if(NULL != m_pMtSetProp){
			CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
			p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		}else{
			AfxMessageBox(_T("发送初始化材料树数据包失败，请重试或重新登录"));
		}
		return FALSE;
	}
	if(NULL != m_pMtSetProp)
	{
		CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
		p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)_T("等待服务器回应!"));
		p_dlg->SendMessage(WM_CLEARMTTREE_MtSetProp,0,0);
	}else{
		UpdateOperationStatus(_T("请求初始化材料树,等待服务器回应!"));
	}
	return TRUE;
}

BOOL CSock::ReqKeepHeart()
{
	int heartPackage = 1;
	hdr header;
	header.type = HEART_PACKAGE;
	header.len = HEADLEN + sizeof(int);
	int nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		CString _str = _T("发送心跳包信息失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}

	nflag = send(m_sockWhole,(const char*)&heartPackage,sizeof(int),0);
	if(nflag == SOCKET_ERROR)
	{
		CString _str = _T("发送心跳包信息失败，请重试或重新登录");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	CString str = _T("请求与服务器心跳包已发送！");
	UpdateOperationStatus(str);
	return TRUE;
}


BOOL CSock::RecvReplyOfNotePublicMainView(int len)   //接收服务器关于发布新公告的回复
{
	int result;
	int errCode = recv(m_sockWhole,(char *)&result,len,0);
	CString tip;
	if(errCode == SOCKET_ERROR)
	{
		CString _str = _T("接收服务器回复失败！");
		AfxMessageBox(_str);
		return FALSE;
	}
	switch(result)
	{
	case -1:
		{
			tip = _T("请求发布公告失败，原因数据库连接失败,请重试");
			break;
		}
	case 1:
		{
			tip = _T("恭喜，发布成功！");
			break;
		}
	case 0:
		{
			tip = _T("请求发布公告失败，未知原因,请重试或联系管理员");
			break;
		}
	default:
		break;
	}
	if(NULL != m_pNotePublic)
	{
		((CNotePublic*)m_pNotePublic)->m_status.SetWindowText(tip);
		if(result == 1)
		{
			((CNotePublic*)m_pNotePublic)->SendMessage(WM_NOTEPUBLICSUCCESS_NotePublic,0,0);
		}
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfMtNodeDelete(int len)
{
	int result;
	int errCode = recv(m_sockWhole,(char *)&result,len,0);
	if(errCode == SOCKET_ERROR)
	{
		CString _str = _T("接收材料树信息失败！");
		AfxMessageBox(_str);
		m_pMtSetProp = NULL;
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	switch(result)
	{
	case 0:
		{
			CString _str = _T("删除名称失败，数据库连接失败,请重试");
			if(m_pMtSetProp!= NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
				p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			}
			UpdateOperationStatus(_str);
			break;
		}
	case 1:
		{
			CString _str = _T("恭喜，删除节点成功！");
			if(m_pMtSetProp!= NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
				p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			}
			UpdateOperationStatus(_str);
			break;
		}
	case -1:
		{
			CString _str = _T("删除名称失败，其他未知原因造成的失败，请重试");
			if(m_pMtSetProp!= NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
				p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			}
			UpdateOperationStatus(_str);
			break;
		}
	default:
		break;
	}
	m_pMtSetProp = NULL;
	//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
	return TRUE;
}

BOOL CSock::RecvReplyOfMtNodeRename(int len)
{
	int result;
	int errCode = recv(m_sockWhole,(char *)&result,len,0);
	if(errCode == SOCKET_ERROR)
	{
		CString _str = _T("接收材料树信息失败！");
		AfxMessageBox(_str);
		m_pMtSetProp = NULL;
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
	if(m_pMtSetProp == NULL)
	{
		return TRUE;
	}
	switch(result)
	{
	case 0:
		{
			CString _str = _T("修改名称失败，新节点名已存在");
			//CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
			p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
			p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			UpdateOperationStatus(_str);
			break;
		}
	case 1:
		{
			CString _str = _T("恭喜，修改节点成功！");
			//CMtSetProp* p_dlg = (CMtSetProp*)m_pVoid;
			p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
			p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			UpdateOperationStatus(_str);
			break;
		}
	default:
		break;
	}
	m_pMtSetProp = NULL;
	//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
	return TRUE;
}

BOOL CSock::RecvReplyOfMtNodeNew(int len)   //接收新建材料树的节点信息
{
	int result;
	int errCode = recv(m_sockWhole,(char *)&result,len,0);
	if(errCode == SOCKET_ERROR)
	{
		CString _str = _T("接收材料树信息失败！");
		AfxMessageBox(_str);
		if(m_pMtSetProp != NULL){
			CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
			p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
		}//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	switch (result)
	{
	case -1:
		{
			CString _str = _T("该上级节点不存在，创建新节点失败");
			if(m_pMtSetProp != NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
				p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			}
			UpdateOperationStatus(_str);
			break;
		}
	case 0:
		{
			CString _str = _T("新建节点已经存在，创建失败");
			if(m_pMtSetProp != NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
				p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			}
			UpdateOperationStatus(_str);
			break;
		}
	case 1:
		{
			CString _str = _T("恭喜，新建节点成功！");
			if(m_pMtSetProp != NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
				p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
			}
			UpdateOperationStatus(_str);
			break;
		}
	default:
		break;
	}
	//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
	return TRUE;
}

BOOL CSock::RecvReplyOfInitMtTree(int len)
{
	if(len)
	{
		char buff[1024*55];
		int errCode = recv(m_sockWhole,(char *)buff,len,0);
		if(errCode == SOCKET_ERROR)
		{
			CString _str = _T("接收材料树信息失败！");
			AfxMessageBox(_str);
			if(m_pMtSetProp != NULL){
				CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
				p_dlg->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
			}
			m_pMtSetProp  = NULL;
			return FALSE;
		}
		USES_CONVERSION;
		CString temp = NULL;
		temp = A2T(buff);
		if(m_pMtSetProp != NULL){
			CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
			p_dlg->m_strMtInfo = temp;
			p_dlg->SendMessage(WM_ENDREQMT_MtSetProp,0,0);
		}else{
			UpdateOperationStatus(_T("服务器已回应初始化材料树请求"));
		}
	}else{   //接收完毕
		CString  _str = _T("接收材料树信息完毕！");
		if(m_pMtSetProp != NULL)
		{
			((CMtSetProp*)m_pMtSetProp)->SendMessage(WM_UPDATESTATUS_MtSetProp,0,(LPARAM)(LPCTSTR)_str);
			CMtSetProp* p_dlg = (CMtSetProp*)m_pMtSetProp;
			p_dlg->SendMessage(WM_PAITMTTREE,0,0);
			m_pMtSetProp = NULL;
		}
	}
	
	return TRUE;
}

BOOL CSock::RecvReplyOfKeepHeart(int len)
{
	BOOL conning = FALSE;
	int errCode = recv(m_sockWhole,(char *)&conning,len,0);
	if(errCode == SOCKET_ERROR)
	{
		CString _str = _T("接收保持心跳包的回复信息失败");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	if(FALSE == conning )
	{
		CString _str = _T("与服务器断开连接，请重新登录！");
		AfxMessageBox(_str);
		UpdateOperationStatus(_str);
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	CString _str = _T("与服务器连接正常！");
	UpdateOperationStatus(_str);
	return TRUE;
}

BOOL CSock::RecvReplyOfServModify(int len)   //修改服务
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收修改服务的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{

	case -1:            //修改客户资料失败原因 数据库名称错误
		{
			tip = _T("修改客户资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("修改客户资料失败,未知原因！");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，修改客户资料成功！");
			break;		
		}
	default:
		break;
	}
	if(NULL != m_pServModify)
	{
		CServModify* pDlg = (CServModify*)m_pServModify;
		//pDlg->SendMessage(WM_ENDTASK_ServModify,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pServModify = NULL;
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfCtmEdit(int len)   //接收修改客户资料的结果
{
	CString tip = NULL;
	int flag;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收编辑客户资料的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //修改客户资料失败原因 数据库名称错误
		{
			tip = _T("修改客户资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("修改客户资料失败,未知原因！");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，修改客户资料成功！");
			break;		
		}
	default:
		break;
	}
	if(NULL != m_pCtmEdit)
	{
		CCtmEdit* pDlg = (CCtmEdit*)m_pCtmEdit;
		//pDlg->SendMessage(WM_ENDTASK_CtmEdit,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pCtmEdit = NULL;
	}else{
		AfxMessageBox(tip);
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfFctEdit(int len)      //接收服务器关于修改厂家资料的回复
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收编辑厂家资料的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //修改厂家资料失败原因 数据库名称错误
		{
			tip = _T("修改厂家资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("修改员工资料失败,原因未知，请重试！");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，修改该员工资料成功");
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pFactoryEdit)
	{
		CFactoryEdit* pDlg = (CFactoryEdit*)m_pFactoryEdit;
		//pDlg->SendMessage(WM_ENDTASK_FctEdit,0,0);
		pDlg->m_tip.SetWindowText(_T("恭喜，修改该员工资料成功"));
		m_pFactoryEdit = NULL;
	}else{
		AfxMessageBox(_T("恭喜，修改该员工资料成功"));
	}
	UpdateOperationStatus(_T("恭喜，修改该员工资料成功"));
	return TRUE;
}

/*
 *	下载文件线程
 */
DWORD CSock::DownloadFileThread(void* pParam)
{
	WT;
	pFileDownloadParam pFileDownload = (pFileDownloadParam)pParam;
	unsigned short usID = pFileDownload->ID;		//线程序号
	SOCKET sFileDownload = pFileDownload->s;		//套接字
	hdr header = pFileDownload->header;				//包头
	CString strFile = A2T(pFileDownload->strFilePath);	//下载文件
	char buff[MAX_PATH] = {0};
	strcpy_s(buff,pFileDownload->strFilePath);
	int len = sizeof(buff);
	CDealListView *p_dlg = (CDealListView *)pFileDownload->ptr;		//指针
	if(NULL != pFileDownload){
		delete pFileDownload;//释放内存
		pFileDownload = NULL;
	}
	int nErrCode;//返回值

	//发送包头
	nErrCode = send(sFileDownload, (char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("发送文件下载请求数据包失败！"));
		return 0;
	}
	
	//发送包体
	nErrCode = send(sFileDownload, buff, len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("发送文件下载请求数据包失败！"));
		return 0;
	}

	FD_SET readfd;//可读集合
	FD_SET writefd;   //可写集合
	//接收文件
	while (CSock::m_bIsConnecting && CSock::m_bOnline)
	{
		FD_ZERO(&readfd);
		FD_ZERO(&writefd);						//清空集合
		FD_SET(sFileDownload, &readfd);					//加入集合
		FD_SET(sFileDownload,&writefd);
		nErrCode = select(0, &readfd, &writefd, NULL, NULL);//等待
		extern int errno;
		if (SOCKET_ERROR == nErrCode)					//错误
		{
			int err = WSAGetLastError();
			CString _err_1;
			_err_1.Format(_T("接收文件时，select socket失败,SOCKET_ERROR错误：%d"),err);
			AfxMessageBox(_err_1);
			return 0;
		}
		
		if (nErrCode > 0)
		{
			if (FD_ISSET(sFileDownload, &readfd)&& CSock::m_bOnline)						//可以读文件
			{
				int nErr = RecvFileData(sFileDownload, usID,p_dlg);	//接收文件
				if (EXCEPT == nErr)
				{
					AfxMessageBox(_T("接收文件出现异常，已关闭连接"));
					return 0;
				}else if (FINISH == nErr)								//下载结束
				{
					return 0;
				}
			}
		}
		Sleep(THREAD_SLEEP);
	}
	return 0;
}

/*
 * 创建文件下载线程
 */
DWORD CSock::CreateDownloadFileThread(void *pParam)
{
	WT;
	CDealListView* p_dlg = (CDealListView*)pParam;
	HANDLE  hThreadFileDL[MAX_RECVFILE_THREAD];			//下载文件线程句柄数组
	SOCKET  sFileDL[MAX_RECVFILE_THREAD];				//下载文件套接字数组

	u_long ulFileSize = p_dlg->m_ulFileSize;		//文件长度
	CString strFile = p_dlg->m_ulFilePath;				//下载文件路径和文件名
	char buff[MAX_PATH] = {0};
	int len = sizeof(buff);
	u_long ulSegment = ulFileSize / MAX_RECVFILE_THREAD;//每个线程下载文件平均长度

	hdr header[MAX_RECVFILE_THREAD];					//定义包头
	for (int i = 0; i < MAX_RECVFILE_THREAD; ++i)
	{
		memset(&header[i], 0, sizeof(hdr));
		header[i].type = REQ_FILE_DOWNLOAD;				//请求类型
		header[i].len = HEADLEN + len;		//数据包长度
		header[i].fpos = i * ulSegment;					//下载文件开始位置
		if (i != MAX_RECVFILE_THREAD -1)				//下载文件长度
		{
			header[i].flen = ulSegment;
		}else
		{
			header[i].flen = ulFileSize - ulSegment * i;//最后一个线程下载剩下文件部分
		}
		
		SOCKET sFileData = INVALID_SOCKET;
		if (FALSE == p_dlg->CreateSocket(sFileData))	//创建套接字
		{
			AfxMessageBox(_T("创建下载套接字失败！"));
			return FALSE;
		}
		sFileDL[i] = sFileData;

		//定义线程函数传递参数
		pFileDownloadParam pThreadPara= new FileDownloadParam();
		pThreadPara->ID = i;
		pThreadPara->header = header[i];
		pThreadPara->s = sFileDL[i];
		pThreadPara->ptr = p_dlg;
		strcpy_s(pThreadPara->strFilePath,T2A(strFile));

		//创建下载文件线程
		DWORD dwThread;		
		hThreadFileDL[i] = CreateThread(NULL, 0, DownloadFileThread, pThreadPara, 0, &dwThread);
	}

	//等待文件下载线程的退出
	WaitForMultipleObjects(MAX_RECVFILE_THREAD, hThreadFileDL, TRUE, INFINITE);	

	AssemblyFile(p_dlg);							//合并文件
	p_dlg->SendMessage(WM_VALID_FILEDL_DealListView,0,0);
	//p_dlg->m_tip.SetWindowText(_T("下载文件结束!"));//显示信息

	//pClient->EnableSomeControl(TRUE);					//使控件有效
	
	//p_dlg->ResetCurFileDL();							//隐藏进度条
	
	for (int i = 0; i < MAX_RECVFILE_THREAD; ++i)			//清理资源
	{
		CloseHandle(hThreadFileDL[i]);
		closesocket(sFileDL[i]);
	}
	return 0;
}

void CSock::PrepareFileDownload(void)
{
	DWORD dwThreadID;
	m_hThreadFileDL = CreateThread(NULL, 0, CreateDownloadFileThread, m_pDealListView, 0, &dwThreadID);
	CloseHandle(m_hThreadFileDL);
}

BOOL CSock::RecvReplyOfDLFile(int len,char *strFilePath)        //接收下载文件的回复
{
	WT;
	CDealListView* p_view = (CDealListView*)m_pDealListView;
	p_view->m_ulFileSize = len;	//接收文件大小
	p_view->m_ulFilePath = A2T(strFilePath);
	if(len == -1)
	{
		p_view->SendMessage(WM_ENDDLOAD_DealListView,0,0);
		p_view->m_tip.SetWindowText(_T("抱歉，服务器端该附件可能已经被误删除了，无法完成下载"));
		return TRUE;
	}else{
		PrepareFileDownload();		//准备创建文件下载线程
		m_nCurReq = REQNON;			//设置发送标志
		return TRUE;
	}
}
	/*
	CDealListView* p_view = (CDealListView*)m_pDealListView;
	char nRecv[SEND_BUFF] = {0};
	int nReadLen = 0;
	if(len == 0)   //用户上传文件已经结束
	{
		p_view->SendMessage(WM_VALID_FILEDL_DealListView,0,0);
		return TRUE;
	}else{   //接收文件 并写入
		u_long nlTotal = 0;
		CString strTemp = p_view->m_strFilePath + p_view->m_fileDlInfo.filename;  //保存文件路径
		CFile file(strTemp, CFile::modeCreate|CFile::modeWrite| CFile::typeBinary|CFile::modeNoTruncate);
		while (CSock::m_bIsConnecting && nlTotal != len)
		{
			memset(nRecv, 0, SEND_BUFF);
			nReadLen = recv(m_sockWhole,(char *)&nRecv,len,0);//准备接收1024*10个字节长度
			if (SOCKET_ERROR == nReadLen)
			{
				p_view->SendMessage(WM_ENDTASK_DealListView,0,0);
				p_view->SendMessage(WM_RESETDLSTATUS_DealListView,0,0);
				p_view->m_tip.SetWindowText(_T("下载文件出错，请重新下载"));
				memset(&nRecv,0,sizeof(nRecv));
				return TRUE;
			}else if (0 == nReadLen)
			{
				int err = WSAGetLastError();
				p_view->SendMessage(WM_ENDTASK_DealListView,0,0);
				p_view->SendMessage(WM_RESETDLSTATUS_DealListView,0,0);
				p_view->m_tip.SetWindowText(_T("下载文件出错，请重新下载"));
				memset(&nRecv,0,sizeof(nRecv));
				return TRUE;
			}
			file.SeekToEnd();
			//file.Write(nRecv,nReadLen);
			file.Write(nRecv,nReadLen);
			nlTotal += nReadLen;//加入接收字节数
			p_view->m_ulCurTotal += nlTotal;
		}
		file.Close();
		return TRUE;
	}
	*/


BOOL CSock::RecvReplyOfUploadFile(int len)    //上传文件的结果
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收上传文件的结果失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:
		{
			tip = _T("上传失败，文件已经损坏，请重新上传！");
			break;
		}
	case -1:            //删除服务失败原因 数据库名称错误
		{
			tip = _T("上传成功,但未能加入到数据库，数据库连接失败，请重试！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("上传成功,但未能加入到数据库，数据库更新失败，请重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，上传成功");
			break;
		}
	default:
		break;
	}
	if(m_pOrderContent != NULL){
		COrderContent* pDlg = (COrderContent*)m_pOrderContent;
		pDlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
		pDlg->m_tip.SetWindowText(tip);
		if(flag == 1){
			pDlg->m_static_upload_status.SetWindowText(_T("上传成功"));
		}else{
			pDlg->m_static_upload_status.SetWindowText(_T("上传失败"));
		}
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfFileDLInfo(int len)   //接收下载文件的信息
{
	CDealListView* p_view = (CDealListView*)m_pDealListView;
	FileInfo  file;
	memset(&file,0,sizeof(FileInfo));
	int errCode = recv(m_sockWhole,(char *)&file,len,0);
	if(errCode == SOCKET_ERROR)
	{
		p_view->SendMessage(WM_ENDTASK_DealListView,0,0);
		p_view->SendMessage(WM_RESETDLSTATUS_DealListView,0,0);
		p_view->m_tip.SetWindowText(_T("接收下载文件的信息失败,，请重试"));
		return TRUE;
	}else if(errCode == 0){
		p_view->SendMessage(WM_ENDTASK_DealListView,0,0);
		p_view->SendMessage(WM_RESETDLSTATUS_DealListView,0,0);
		p_view->m_tip.SetWindowText(_T("接收下载文件的信息失败,请重试"));
		return TRUE;
	}else{
		memset(&p_view->m_fileDlInfo,0,sizeof(FileInfo));
		p_view->m_fileDlInfo = file;
		if( 0 == strlen(file.filename))   //若回复的文件名 为空 代表服务器端 没有找到要下载的文件
		{
			p_view->SendMessage(WM_ENDTASK_DealListView,0,0);
			p_view->SendMessage(WM_RESETDLSTATUS_DealListView,0,0);
			p_view->m_tip.SetWindowText(_T("抱歉，将要下载的附件已经被删除，无法下载"));
			return TRUE;
		}
		p_view->m_ulFileSize = p_view->m_fileDlInfo.filesize;   //文件的总大小
		return TRUE;
	}
}

BOOL CSock::RecvReplyOfCheckFile(int len)   //回复的文件是否存在的结果
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收检查文件是否存在的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:
		{
			tip = _T("该订单已有附件！");
			break;
		}
	case -1:            //删除服务失败原因 数据库名称错误
		{
			tip = _T("校验失败,请重试！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("校验失败，请重试");
			break;			
		}
	case 1:
		{
			tip = _T("该订单暂时还没有附件");
			break;
		}
	default:
		break;
	}
	if(m_pOrderContent != NULL){
		COrderContent* pDlg = (COrderContent*)m_pOrderContent;
		pDlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
		pDlg->m_tip.SetWindowText(tip);
		pDlg->m_static_upload_status.SetWindowText(_T("校验结束"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfUpdateOrderBatch(int len)     //批量更新订单数据的回复
{
	WT;
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收批量更新订单数据的回复信息失败"));
		return FALSE;
	}
	
	switch (flag)
	{
	case -1:
		{
			tip = _T("更新数据失败，原因连接数据库失败，请重试或联系管理员");
			break;
		}
	case 1:
		{
			tip = _T("更新数据成功");
			break;
		}
	case 0:
		{
			tip = _T("更新数据失败，未知原因，请重试或联系管理员");
			break;
		}
	default:
		break;
	}
	COrderIncompletedProgView* pDlg = (COrderIncompletedProgView*)m_pOrderIncompleteView;
	if(pDlg != NULL)
	{
		//pDlg->SendMessage(WM_ENDTASK_OrderIncomplete,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pOrderIncompleteView = NULL;
	}else{
		AfxMessageBox(tip);
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfCtmInfoIm(int len)    //批量提交客户数据到数据库
{
	WT;
	char buff[1024] = {0};
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)buff,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收批量提交客户数据的回复信息失败"));
		return FALSE;
	}
	tip = A2T(buff);
	CCustomInfoProp* pDlg = (CCustomInfoProp*)m_pCtmSubmit;
	if(pDlg != NULL)
	{
		pDlg->SendMessage(WM_ENDTASK_CtmIM,0,0);
		pDlg->m_tip.SetWindowText(_T("就绪"));
	}
	AfxMessageBox(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfEmpInfoIm(int len)    //批量插入员工数据的回复
{
	WT;
	char buff[1024] = {0};
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)buff,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收批量插入员工数据的回复信息失败"));
		return FALSE;
	}
	tip = A2T(buff);
	CEmpInfoProp* pDlg = (CEmpInfoProp*)m_pEmpInfoIm;
	if(pDlg != NULL)
	{
		pDlg->SendMessage(WM_ENDTASK_EmpInfoProp,0,0);
		pDlg->m_tip.SetWindowText(_T("就绪"));
	}
	AfxMessageBox(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfProdInfoIm(int len)   //批量插入产品数据的结果
{
	WT;
	char buff[1024] = {0};
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)buff,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收批量插入产品数据的回复信息失败"));
		return FALSE;
	}
	tip = A2T(buff);
	CProdInfoProp* pDlg = (CProdInfoProp*)m_pProdBatch;
	if(pDlg != NULL)
	{
		pDlg->SendMessage(WM_ENDTASK_ProdIm,0,0);
		pDlg->m_tip.SetWindowText(_T("就绪"));
	}
	AfxMessageBox(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfPswEdit(int len)    //修改密码的结果
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收修改密码的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:
		{
			tip = _T("修改密码失败,请输入正确的密码！");
			break;
		}
	case -1:            //修改密码失败原因 数据库名称错误
		{
			tip = _T("修改密码,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("修改密码失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，修改成功，请重新登录");
			m_bIsConnecting = FALSE;     //连接断开 让用户重新登录
			m_bOnline = FALSE;        // 断开线后 连接不可用
			m_nameOnline.Empty();     //断开连接后 清空在线的用户名
					//m_bReqAvailable = TRUE;   //使得请求可用
			((CBMApp*)AfxGetApp())->m_sHost = INVALID_SOCKET; 
			closesocket(m_sockWhole);
			m_sockWhole = INVALID_SOCKET;

			break;
		}
	default:
		break;
	}
	CPwdEdit* pDlg = (CPwdEdit*)m_pPwdEdit;
	if(pDlg != NULL)
	{
		pDlg->SendMessage(WM_ENDTASK_PwdEdit,0,0);
		pDlg->m_tip.SetWindowText(tip);
	}else{
		AfxMessageBox(tip);
	}
	
	return TRUE;
}

BOOL CSock::RecvReplyOfPosAuthority(int len)     //接收用户权限
{
	CString tip = NULL;
	char buff[1024] = {0};
	int errCode = recv(m_sockWhole,buff,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收用户权限的回复信息失败"));
		return FALSE;
	}
	InitUserAuthority(buff);
	return TRUE;
}

void CSock::InitUserAuthority(char *p)
{
	WT;
	if( NULL == m_pPosAuthority)
	{
		m_pPosAuthority = new Pos_Authority;
	}
	CString authority = A2T(p);
	for(int i=0;i<NUM_AUTHORITY;i++)
	{
		CString temp = GetItemFromString(authority,i+1);
		if(temp.IsEmpty())
		{
			m_pPosAuthority->autority[i] = FALSE;
		}else if(IsNum(temp,FALSE))
		{
			if(atoi(T2A(temp)) == 0)
			{
				m_pPosAuthority->autority[i] = FALSE;
			}else if(atoi(T2A(temp)) == 1)
			{
				m_pPosAuthority->autority[i] = TRUE;
			}else{
				if(NULL != m_pPosAuthority)
				{
					delete m_pPosAuthority;
					m_pPosAuthority = NULL;
				}
				break;
			}
		}else{
			if(NULL != m_pPosAuthority)
			{
				delete m_pPosAuthority;
				m_pPosAuthority = NULL;
			}
			break;
		}
	}
	m_pPosAuthority->bLoadSuccess = TRUE;
	UpdateOperationStatus(_T("用户权限加载完毕"));
}

CString CSock::GetItemFromString(CString source,int index)     //从字符串中取得子项数据
{
	CString _str_source = source;
	CString subItem = NULL;
	CString sub_str_start = _T("[#");
	CString sub_str_end = _T("#]");
	CString temp = NULL;
	int i = 0;
	while(TRUE)
	{
		int index_t = _str_source.Find(sub_str_start);
		if(-1 == index_t)   //表示没找到 意味着 结束了
		{
			break;
		}else{
			i++;
			if( i == index)   //找到了对应的子项
			{
				_str_source = _str_source.Mid(index_t+2);
				index_t = _str_source.Find(sub_str_end);
				subItem = _str_source.Mid(0,index_t);
				break;
			}else{   //没找到 继续
				_str_source = _str_source.Mid(index_t+2);
			}
		}
	}	
	return subItem;
}

BOOL CSock::RecvReplyOfPosEdit(int len)     //接收修改用户的回复
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收编辑用户的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //创建新用户失败原因 数据库名称错误
		{
			tip = _T("编辑用户失败,原因是连接数据库失败，请重试！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("编辑用户失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，编辑用户成功");		
			break;
		}
	default:
		break;
	}
	CPosEdit* pDlg = (CPosEdit*)m_pPosEdit;
	//pDlg->SendMessage(WM_ENDTASK_PosEdit,0,0);
	pDlg->m_tip.SetWindowText(tip);
	m_pPosEdit = NULL;
	return TRUE;
}

BOOL CSock::RecvReplyOfPosAdd(int len)    //创建新用户的回复
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收创建新用户的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:
		{
			tip = _T("创建新用户失败,原因是该用户名已存在，请重新命名！");
			break;
		}
	case -1:            //创建新用户失败原因 数据库名称错误
		{
			tip = _T("创建新用户失败,原因是连接数据库失败，请重试！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("添加新用户失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，添加新用户成功");		
			break;
		}
	default:
		break;
	}
	CPosAdd* pDlg = (CPosAdd*)m_pPosAdd;
	//pDlg->SendMessage(WM_ENDTASK_PosAdd,0,0);
	pDlg->m_tip.SetWindowText(tip);
	m_pPosAdd = NULL;
	return TRUE;
}

BOOL CSock::RecvReplyOfUsersInit(int len)      //接收服务器返还的用户权限
{
	WT;
	CString tip = NULL;
	char  buff[1024] = {0};
	int errCode = recv(m_sockWhole,(char *)buff,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收用户权限的回复信息失败"));
		return FALSE;
	}
	
	if(NULL != m_pPosEdit)
	{
		tip = _T("接收到服务器返还的用户权限，正在初始化用户信息");
		CPosEdit* pDlg = (CPosEdit*)m_pPosEdit;
		pDlg->SendMessage(WM_ENDTASK_PosEdit,0,0);
		pDlg->m_tip.SetWindowText(tip);
		pDlg->m_str_pos_authorities = A2T(buff);
		pDlg->SendMessage(WM_INITUSER_AUTHORITY_PosEdit,0,0);
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfUsersDel(int len)    //回复删除用户
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除用户的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除订单失败原因 数据库名称错误
		{
			tip = _T("删除失败,原因是数据库连接出错！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，删除成功");
			break;
		}
	default:
		break;
	}
	if(NULL != m_pSysPostion)
	{
		CSysPostion* pDlg = (CSysPostion*)m_pSysPostion;
		//pDlg->SendMessage(WM_ENDTASK_SysPostion,0,0);
		pDlg->m_tip.SetWindowText(tip);
		if(1 == flag)
		{
			pDlg->SendMessage(WM_DEL_USERS_SysPostion,0,0);
		}
		m_pSysPostion = NULL;
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfDealDel(int len)      //回复删除订单的请求
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除信息的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除订单失败原因 数据库名称错误
		{
			tip = _T("删除失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，删除成功");
			CDealListView* pDlg = (CDealListView*)m_pDealListView;
			pDlg->SendMessage(WM_DEL_DEAL_DealListView,0,0);
			break;
		}
	default:
		break;
	}
	CDealListView* pDlg = (CDealListView*)m_pDealListView;
	//pDlg->SendMessage(WM_ENDTASK_DealListView,0,0);
	pDlg->m_tip.SetWindowText(tip);
	m_pDealListView = NULL;
	return TRUE;
}

BOOL CSock::RecvReplyOfServDel(int len)   //回复删除服务的请求
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除服务信息的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除服务失败原因 数据库名称错误
		{
			tip = _T("删除服务失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除服务失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，删除服务成功");
			CServiceSearch* pDlg = (CServiceSearch*)m_pServiceSearch;
			pDlg->SendMessage(WM_DELSERV_ServiceSearch,0,0);
			break;
		}
	default:
		break;
	}
	CServiceSearch* pDlg = (CServiceSearch*)m_pServiceSearch;
	//pDlg->SendMessage(WM_ENDTASK_ServiceSearch,0,0);
	pDlg->m_tip.SetWindowText(tip);
	m_pServiceSearch = NULL;
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfStoreHistoryDel(int len)   //请求删除部分库存历史记录信息
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除库存历史记录的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除客户资料失败原因 数据库名称错误
		{
			tip = _T("删除库存历史记录失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除库存历史记录失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，删除库存历史记录成功");
			if(NULL != m_pStoreHistory)
			{
				CStoreHistory* pDlg = (CStoreHistory*)m_pStoreHistory;
				pDlg->SendMessage(WM_DELHISTORY_StoreHistory,0,0);
			}
			break;
		}
	default:
		break;
	}
	if(NULL != m_pStoreHistory)
	{
		CStoreHistory* pDlg = (CStoreHistory*)m_pStoreHistory;
		//pDlg->SendMessage(WM_ENDTASK_StoreHistory,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pStoreHistory = NULL;
	}else{
		AfxMessageBox(tip);
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfCtmDel(int len)      //接收服务器回应删除客户资料
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除客户资料的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除客户资料失败原因 数据库名称错误
		{
			tip = _T("删除客户资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除客户资料失败,未知原因，请检查服务器连接状态或重试");
			break;			
		}
	case 1:
		{
			tip = _T("恭喜，删除客户资料成功");
			if(NULL != m_pCtmInfo)
			{
				CCtmInfo* pDlg = (CCtmInfo*)m_pCtmInfo;
				pDlg->SendMessage(WM_DELCTM_CtmInfo,0,0);
			}
			break;
		}
	default:
		break;
	}
	if(NULL != m_pCtmInfo)
	{
		CCtmInfo* pDlg = (CCtmInfo*)m_pCtmInfo;
		///pDlg->SendMessage(WM_ENDTASK_CtmInfo,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pCtmInfo = NULL;
	}else{
		AfxMessageBox(tip);
	}
	UpdateOperationStatus(tip);
	return TRUE;
}
 
BOOL CSock::RecvReplyOfEmpDel(int len)
{
	int flag;
	CString tip;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除员工资料的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除员工资料失败原因 数据库名称错误
		{
			tip = _T("删除厂家资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除员工资料失败，未知原因错误，请重试！");
			break;		
		}
	case 1:
		{
			tip = _T("恭喜，成功删除员工资料");
			break;
		}
	default:
		break;
	}
	if(NULL != m_pEmpInfoView)
	{
		CEmpInfoView* pDlg = (CEmpInfoView*)m_pEmpInfoView;
		//pDlg->SendMessage(WM_ENDTASK_EmpInfoView,0,0);
		pDlg->m_tip.SetWindowText(tip);
		if(flag == 1)
		{
			pDlg->SendMessage(WM_DELSELEMPINFO,0,0);
		}
		m_pEmpInfoView = NULL;
	}else{
		AfxMessageBox(_T("恭喜，删除厂家资料成功！"));
	}
	UpdateOperationStatus(_T("恭喜，删除厂家资料成功！"));
	return TRUE;
}

BOOL CSock::RecvReplyOfFctDel(int len)      //接收删除厂家资料的回应
{
	int flag;
	CString tip;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收删除厂家资料的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //删除厂家资料失败原因 数据库名称错误
		{
			tip = _T("删除厂家资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("删除厂家资料失败，未知原因错误，请重试！");
			break;		
		}
	case 1:
		{
			tip = _T("恭喜，删除厂家资料成功！");
			break;
		}
	default:
		break;
	}
	if(NULL != m_pFctSet)
	{
		CFctSet* pDlg = (CFctSet*)m_pFctSet;
		pDlg->SendMessage(WM_ENDTASK_FctEdit,0,0);
		pDlg->m_tip.SetWindowText(tip);
		if(flag == 1)
		{
			pDlg->SendMessage(WM_DEL_FCT_FctSet,0,0);
		}
		
	}else{
		AfxMessageBox(_T("删除厂家资料失败，未知原因错误，请重试！"));
		UpdateOperationStatus(_T("删除厂家资料失败，未知原因错误，请重试！"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfEmpEdit(int len)     //接收服务器关于修改员工资料的回复
{
	CString tip = NULL;
	int flag;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收编辑员工资料的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //修改员工失败原因 数据库名称错误
		{
			tip = _T("修改员工资料失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("修改员工资料失败,原因未知，请重试！");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，修改该员工资料成功");
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pEmpEdit)
	{
		CEmpEdit* pDlg = (CEmpEdit*)m_pEmpEdit;
		//pDlg->SendMessage(WM_ENDTASK_EmpEdit,0,0);
		pDlg->m_tip.SetWindowText(_T("恭喜，修改该员工资料成功"));
		m_pEmpEdit = NULL;
	}else{
		AfxMessageBox(_T("恭喜，修改员工资料成功"));
	}
	UpdateOperationStatus(_T("恭喜，修改员工资料成功"));
	return TRUE;
}


BOOL CSock::RecvReplyOfOutStore(int len)    //请求出库
{
	CString tip = NULL;
	int flag;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收添加物品库存请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -3:
		{
			tip = _T("出库失败，原因库存中此物品没有足够的数量");
			break;
		}
	case -2:
		{
			tip = _T("出库失败，原因库存中没有此物品记录，请先入库");
			break;
		}
	case -1:            //数据库连接失败
		{
			tip = _T("出库失败，原因数据库连接失败，请重试");
			break;
		}
	case 0:             //添加物品库存资料失败，原因：修改物品库存信息失败，请重试
		{
			tip = _T("出库失败，原因更新数据库失败，请重试");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，操作成功");
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pStoreOut)
	{
		CStoreOut* pDlg = (CStoreOut*)m_pStoreOut;
		//pDlg->SendMessage(WM_ENDTASK_StoreOut,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pStoreOut = NULL;
	}else{
		AfxMessageBox(tip);
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfStoreAlertSet(int len)    //接收关于用户设置预警信息的回复
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收关于用户设置库存预警信息的请求的回复失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //数据库连接失败
		{
			tip =  _T("设置库存预警信息失败，原因数据库连接失败，请重试");
			break;	
		}
	case 0:             //设置库存预警信息失败 原因未知
		{
			tip =  _T("设置库存预警信息失败，原因未知，请重试"); 
			break;	
		}
	case 1:
		{
			tip =  _T("恭喜，成功设置库存预警信息"); 
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pStoreAlertSet)
	{
		CStoreAlertSet* pDlg = (CStoreAlertSet*)m_pStoreAlertSet;
		//pDlg->SendMessage(WM_ENDTASK_StoreAlertSet,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pStoreAlertSet = NULL;
	}else{
		AfxMessageBox(tip);
	}
	UpdateOperationStatus(tip);
	
	return TRUE;
}

BOOL CSock::RecvReplyOfDeductSet(int len)     //关于设置提成的回复
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收设置提成请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //数据库连接失败
		{
			tip = _T("设置提成失败，原因数据库连接失败，请重试");
			break;	
		}
	case 0:             //设置提成失败，未知原因，请重试
		{
			tip = _T("设置提成失败，未知原因，请重试");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，设置提成订单成功");
			
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pDeductSet)
	{
		CDeductSet* pDlg = (CDeductSet*)m_pDeductSet;
		pDlg->SendMessage(WM_ENDTASK_DeductSet,0,0);
		pDlg->m_tip.SetWindowText(tip);
	}else{
		UpdateOperationStatus(tip);
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfOrderEdit(int len)    //编辑订单的回复
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收编辑订单请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //数据库连接失败
		{
			tip = _T("编辑订单失败，原因数据库连接失败，请重试");
			break;	
		}
	case 0:             //编辑订单失败，未知原因，请重试
		{
			tip = _T("编辑订单失败，未知原因，请重试");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，编辑订单成功");
			
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pOrderEdit)
	{
		COrderEdit* pDlg = (COrderEdit*)m_pOrderEdit;
		//pDlg->SendMessage(WM_ENDTASK_OrderEdit,0,0);
		pDlg->m_tip.SetWindowText(tip);
	}else{
		UpdateOperationStatus(tip);
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfOrderNew(int len)    //回复新建订单
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收新建订单请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //订单号已经存在  
		{
			tip = _T("新建订单失败，原因该订单号已经存在");
			break;	
		}
	case -1:            //数据库连接失败
		{
			tip = _T("新建订单失败，原因数据库连接失败，请重试");
			break;	
		}
	case 0:             //新建订单失败，未知原因，请重试
		{
			tip = _T("新建订单失败，未知原因，请重试");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，新建订单成功");
			
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pOrderContent)
	{
		COrderContent* pDlg = (COrderContent*)m_pOrderContent;
		//pDlg->SendMessage(WM_ENDTASK_OrderContent,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pOrderContent = NULL;
	}else{
		UpdateOperationStatus(tip);
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfRegServ(int len)   //回复请求注册服务
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收注册服务请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //注册服务失败，原因：该服务单号已经存在
		{
			tip = _T("注册服务失败，该服务单号已经存在");
			break;	
		}
	case -1:            //数据库连接失败
		{
			tip = _T("注册服务失败，原因数据库连接失败，请重试");
			break;	
		}
	case 0:             //注册服务失败失败，原因：修改物品库存信息失败，请重试
		{
			tip = _T("注册服务失败，未知原因，请重试");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，注册服务成功");
			
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pServReg)
	{
		CServReg* pDlg = (CServReg*)m_pServReg;
		//pDlg->SendMessage(WM_ENDTASK_ServReg,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pServReg = NULL;
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfAddStore(int len)    //请求添加物品库存信息
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收添加物品库存请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //添加物品库存资料失败，原因：新建物品库存信息失败
		{
			tip = _T("添加物品库存资料失败，请重试");
			break;	
		}
	case -1:            //数据库连接失败
		{
			tip = _T("添加物品库存资料失败，原因数据库连接失败，请重试");
			break;	
		}
	case 0:             //添加物品库存资料失败，原因：修改物品库存信息失败，请重试
		{
			tip = _T("添加物品库存资料失败，原因：修改物品库存信息失败，请重试");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，添加物品库存资料成功");
			
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pStoreAdd)
	{
		CStoreAdd* pDlg = (CStoreAdd*)m_pStoreAdd;
		//pDlg->SendMessage(WM_ENDTASK_StoreAdd,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pStoreAdd= NULL;
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfCtmNew(int len)   //接收返还新建客户资料的结果
{
	CString tip = NULL;
	int flag;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收新建客户资料请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //新建客户资料失败原因 客户已经存在
		{
			tip = _T("新建客户失败,原因是此客户编号已经存在！");
			break;	
		}
	case -1:            //
		{
			tip = _T("新建客户失败,原因是数据库连接失败！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("新建客户失败,原因不明");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，新建客户成功");
			
			break;		
		}
	default:
		break;
	}
	if(NULL != m_pCtmNew)
	{
		CCtmNew* pDlg = (CCtmNew*)m_pCtmNew;
		//pDlg->SendMessage(WM_ENDTASK_CtmNew,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pCtmNew = NULL;
	}else{
		AfxMessageBox(tip);
	}
	UpdateOperationStatus(tip);
	return TRUE;
}

BOOL CSock::RecvReplyOfFctNew(int len)
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收新建厂家资料请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //新建员工失败原因 员工已经存在
		{
			tip = _T("新建厂家失败,原因是此厂家已经存在！");
			break;	
		}
	case -1:            //
		{
			tip = _T("新建厂家失败,原因是数据库连接！");
			break;
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("新建厂家失败,未知原因！");
			break;	
		}
	case 1:
		{
			tip = _T("恭喜，新建厂家成功！");
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pFctNew)
	{
		CFactoryNew* pDlg = (CFactoryNew*)m_pFctNew;
		//pDlg->SendMessage(WM_ENDTASK_FctNew,0,0);
		pDlg->m_tip.SetWindowText(_T("恭喜，新建厂家成功！"));
		m_pFctNew = NULL;
	}else{
		AfxMessageBox(_T("恭喜，新建厂家成功！"));
	}
	UpdateOperationStatus(_T("恭喜，新建厂家成功！"));
	return TRUE;
}

BOOL CSock::RecvReplyOfEmpNew(int len)     //请求新建员工
{
	int flag;
	CString tip = NULL;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收新建员工资料请求的回复信息失败"));
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //新建员工失败原因 员工已经存在
		{
			tip = _T("新建员工失败,原因是此员工工号已经存在！");
			break;	
		}
	case -1:            //新建员工失败原因 数据库名称错误
		{
			tip = _T("新建员工失败,原因是数据库名称错误！");
			break;	
		}
	case 0:             //未知原因 请重试
		{
			tip = _T("新建员工失败,未知原因！");
			break;	
			//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
			break;
		}
	case 1:
		{
			tip = _T("恭喜，新建员工成功！");
			break;	
		}
	default:
		break;
	}
	if(NULL != m_pEmpNew)
	{
		CEmpNew* pDlg = (CEmpNew*)m_pEmpNew;
		//pDlg->SendMessage(WM_REQEMPNEWEND_EmpNew,0,0);
		pDlg->m_tip.SetWindowText(tip);
		m_pEmpNew = NULL;
	}else{
		AfxMessageBox(_T("新建员工失败,未知原因！"));
	}
	UpdateOperationStatus(_T("新建员工失败,未知原因！"));
	return TRUE;
}


BOOL CSock::RecvReplyOfDetailNote(int len)    //回复用户查看通知
{
	if(len)
	{
		NotePublic_MainView note;
		memset(&note,0,sizeof(NotePublic_MainView));
		int errCode = recv(m_sockWhole,(char *)&note,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收公告消息的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CString num;
		if(m_pMainFormView == NULL)
		{
			UpdateOperationStatus(_T("已经取消查看公告"));
			return TRUE;
		}
		CMainFormView* pView = (CMainFormView*)m_pMainFormView;
		pView->m_time_note_public = A2T(note.time_public);
		pView->m_persion_public_note = A2T(note.persion_public);
		pView->m_title_note = A2T(note.title);
		pView->m_content_note = A2T(note.content);
		pView->SendMessage(WM_SHOWNOTEINEDIT_MainView,0,0);
		//pView->SendMessage(WM_REQINITNOTEEND_MainView,0,0);
		pView->GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T(""));
		UpdateOperationStatus(_T("加载公告完成"));
	}else{   //接收完毕
		if(m_pMainFormView != NULL){
			CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
			//p_dlg->SendMessage(WM_REQINITNOTEEND_MainView,0,0);
			p_dlg->GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("加载公告失败，请查看连接状态并重试"));	
		}
		UpdateOperationStatus(_T("加载公告失败"));
	}
	return TRUE;
}


BOOL CSock::RecvReplyOfDeletNote(int len)
{
	int flag;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收登录的回复信息失败"));
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	switch (flag)
	{
	case -1:            //登录失败原因 数据库名称错误
		{
			if(NULL != m_pMainFormView)
			{
				CMainFormView* pDlg = (CMainFormView*)m_pMainFormView;
				UpdateOperationStatus(_T("删除通知失败，数据库名称错误"));
				//pDlg->SendMessage(WM_REQINITNOTEEND_MainView,0,0);
			}else{
				AfxMessageBox(_T("删除失败，数据库名称错误,要找的数据库不存在！"));
			}
			break;	
		}
	case 0:             //未知原因 请重试
		{
			if(NULL != m_pMainFormView)
			{
				CMainFormView* pDlg = (CMainFormView*)m_pMainFormView;
				//pDlg->SendMessage(WM_REQINITNOTEEND_MainView,0,0);
				UpdateOperationStatus(_T("未知原因失败，请重试"));
			}else
			{
				AfxMessageBox(_T("未知原因失败，请重试"));
			}
			//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
			break;
		}
	case 1:
		{
			if(NULL != m_pMainFormView)
			{
				CMainFormView* pDlg = (CMainFormView*)m_pMainFormView;
				//pDlg->SendMessage(WM_REQINITNOTEEND_MainView,0,0);
				UpdateOperationStatus(_T("删除通知成功"));
				pDlg->SendMessage(WM_DELSELNOTE_MainView,0,0);
			}else
			{
				AfxMessageBox(_T("删除通知成功"));
			}
			//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
			break;
		}
	default:
		break;
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfLoginIn(int len)		////接收回复信息 关于登录是否成功的
{
	//CLogin* pDlg = (CLogin *)m_pVoid;
	int flag;
	int errCode = recv(m_sockWhole,(char *)&flag,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收登录的回复信息失败"));
		//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
		return FALSE;
	}
	switch (flag)
	{
	case -2:            //登录失败原因 数据库名称错误
		{
			if(NULL != m_pLogin)
			{
				CLogin* pDlg = (CLogin*)m_pLogin;
				pDlg->SendMessage(WM_CURSORTONORMAL,0,0);
				pDlg->m_status.SetWindowText(_T("登录失败，数据库名称错误,请后退到上一页修改数据库名称！"));
				pDlg->SendMessage(WM_LOGINFAIL,0,0);
			}else{
				AfxMessageBox(_T("登录失败，数据库名称错误,请后退到上一页修改数据库名称！"));
			}
			break;	
		}
	case -1:             //登录失败原因 密码错误
		{
			if(NULL != m_pLogin)
			{
				CLogin* pDlg = (CLogin*)m_pLogin;
				pDlg->SendMessage(WM_CURSORTONORMAL,0,0);
				pDlg->m_status.SetWindowText(_T("登录失败，密码错误！"));
			//AfxMessageBox(_T("登录失败,用户名或密码错误！"));
				pDlg->SendMessage(WM_LOGINFAIL,0,0);
			}else
			{
				AfxMessageBox(_T("登录失败，密码错误！"));
			}
			//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
			break;
		}
	case 0:
		{
			if(NULL != m_pLogin)
			{
				CLogin* pDlg = (CLogin*)m_pLogin;
				pDlg->SendMessage(WM_CURSORTONORMAL,0,0);
				pDlg->m_status.SetWindowText(_T("登录失败，用户名或者角色选择错误！"));
			//AfxMessageBox(_T("登录失败,用户名或密码错误！"));
				pDlg->SendMessage(WM_LOGINFAIL,0,0);
			}else
			{
				AfxMessageBox(_T("登录失败，用户名或者角色选择错误！"));
			}
			//m_bReqAvailable = TRUE;   //使得请求可用 只要有结果 就能发起新的请求
			break;
		}
	case 1:
		{
			if(NULL != m_pLogin)
			{
				CLogin* pDlg = (CLogin*)m_pLogin;
				pDlg->m_status.SetWindowText(_T("恭喜您，登录成功！"));
				//m_bReqAvailable = TRUE;    //使得请求可用 才能完成其他操作
				pDlg->SendMessage(WM_CURSORTONORMAL,0,0);
				pDlg->SendMessage(WM_LOGINSUCCESS,0,0);
				UpdateOperationStatus(_T("登录成功，已就绪"));
			}else
			{
				AfxMessageBox(_T("恭喜您，登录成功！"));
			}
			break;
		}
	default:
		break;
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfInitNote(int len)
{
	if(len)
	{
		NotePublic_MainView note;
		memset(&note,0,sizeof(NotePublic_MainView));
		int errCode = recv(m_sockWhole,(char *)&note,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收初始化通知栏的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CString num;
		if(m_pMainFormView == NULL)
		{
			UpdateOperationStatus(_T("已经取消初始化通知栏"));
			return TRUE;
		}
		CMainFormView* pView = (CMainFormView*)m_pMainFormView;
		int nCount = pView->m_list.GetItemCount();
		num.Format(_T("%d"),(nCount+1));
		pView->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pView->m_list.SetItemText(nCount,0,num);
		pView->m_list.SetItemText(nCount,1,A2T(note.title));
		pView->m_list.SetItemText(nCount,2,A2T(note.time_public));
		pView->m_list.SetItemText(nCount,3,A2T(note.persion_public));
		if(nCount == 0)   //如果没有新消息 显示最新的消息
		{
			pView->m_time_note_public = A2T(note.time_public);
			pView->m_persion_public_note = A2T(note.persion_public);
			pView->m_title_note = A2T(note.title);
			pView->m_content_note = A2T(note.content);
			pView->SendMessage(WM_SHOWNOTEINEDIT_MainView,0,0);
		}
		UpdateOperationStatus(_T("正在加载通知栏"));
	}else{   //接收完毕
		if(m_pMainFormView != NULL){
			CMainFormView* p_dlg = (CMainFormView*)m_pMainFormView;
			//p_dlg->SendMessage(WM_REQINITNOTEEND_MainView,0,0);
			int count = p_dlg->m_list.GetItemCount();
			if(count == 0){
				p_dlg->GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("无通知"));
			}else{
				CString num;
				num.Format(_T("最新的%d条通知"),count);
				p_dlg->GetDlgItem(IDC_STATIC_TIP)->SetWindowText(num);
			}
		}
		UpdateOperationStatus(_T("通知栏加载完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfFindAllStore(int len)     //关于搜索库存资料
{
	if(len)   //如果接收没有结束
	{
		StoreInfo store;
		memset(&store,0,sizeof(StoreInfo));
		int errCode = recv(m_sockWhole,(char *)&store,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索库存资料的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CStoreInfo* pdlg = (CStoreInfo*)m_pStoreInfo;
		if(m_pStoreInfo == NULL){
			UpdateOperationStatus(_T("搜索库存资料的请求已经取消"));
			return TRUE;
		}
		int nCount = pdlg->m_list.GetItemCount();
		pdlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pdlg->m_list.SetItemText(nCount,0,A2T(store.Store_Name));
		pdlg->m_list.SetItemText(nCount,1,A2T(store.Store_Num));
		pdlg->m_list.SetItemText(nCount,2,A2T(store.Store_Alert_NUM));
	}else{    //如果接收已经结束
		if(m_pStoreInfo != NULL){
			CStoreInfo* pDlg = (CStoreInfo*)m_pStoreInfo;
			//pDlg->SendMessage(WM_ENDTASK_StoreInfo,0,0);
			pDlg->m_tip.SetWindowTextW(_T("搜索库存资料信息完成"));
			m_pStoreInfo = NULL;
		}
		UpdateOperationStatus(_T("搜索库存资料信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfInitDeduct(int len)   //回复初始化提成
{
	if(len)   //如果接收没有结束
	{
		DeductInfo info;
		memset(&info,0,sizeof(info));
		int errCode = recv(m_sockWhole,(char *)&info,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收初始化提成设置的回复信息失败"));
			return FALSE;
		}
		CDeductSet* p_dlg = (CDeductSet*)m_pDeductSet;
		if(p_dlg != NULL)
		{
			p_dlg->m_deductInfo = info;
		}
	}else{    //如果接收已经结束
		CDeductSet* pDlg = (CDeductSet*)m_pDeductSet;
		if(pDlg != NULL){
			pDlg->SendMessage(WM_ENDTASK_DeductSet,0,0);
			pDlg->m_tip.SetWindowText(_T("加载完毕"));
			pDlg->SendMessage(WM_LOADSETOVER_DeductSet,0,0);
			UpdateOperationStatus(_T("完成"));
		}else{
			UpdateOperationStatus(_T("完成"));
		}
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfOrderFind(int len)   //回复搜索订单
{
	if(len)   //如果接收没有结束
	{
		OrderInfo order;
		memset(&order,0,sizeof(OrderInfo));
		int errCode = recv(m_sockWhole,(char *)&order,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索订单的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CDealListView* pdlg = (CDealListView*)m_pDealListView;
		int nCount = pdlg->m_list.GetItemCount();
		pdlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pdlg->m_list.SetItemText(nCount,0,A2T(order.NO_Order));
		pdlg->m_list.SetItemText(nCount,1,A2T(order.date_accept_order));
		pdlg->m_list.SetItemText(nCount,2,A2T(order.date_finished_order));
		pdlg->m_list.SetItemText(nCount,3,A2T(order.name_person_charge_order));
		pdlg->m_list.SetItemText(nCount,4,A2T(order.base_ps_order));
		pdlg->m_list.SetItemText(nCount,5,A2T(order.ctm_from));
		pdlg->m_list.SetItemText(nCount,6,A2T(order.name_intr));
		pdlg->m_list.SetItemText(nCount,7,A2T(order.phone_intr));
		pdlg->m_list.SetItemText(nCount,8,A2T(order.name_serv_persion));
		pdlg->m_list.SetItemText(nCount,9,A2T(order.name_emp));
		pdlg->m_list.SetItemText(nCount,10,A2T(order.call_ctm));
		pdlg->m_list.SetItemText(nCount,11,A2T(order.name_ctm));
		pdlg->m_list.SetItemText(nCount,12,A2T(order.phone_ctm));
		pdlg->m_list.SetItemText(nCount,13,A2T(order.addr_ctm));
		pdlg->m_list.SetItemText(nCount,14,A2T(order.ps_ctm));
		pdlg->m_list.SetItemText(nCount,15,A2T(order.NO_contract));
		pdlg->m_list.SetItemText(nCount,16,A2T(order.prog_business));
		pdlg->m_list.SetItemText(nCount,17,A2T(order.price_total_contract));
		pdlg->m_list.SetItemText(nCount,18,A2T(order.money_already_take));
		pdlg->m_list.SetItemText(nCount,19,A2T(order.date_first_test_book));
		pdlg->m_list.SetItemText(nCount,20,A2T(order.persion_first_test));
		pdlg->m_list.SetItemText(nCount,21,A2T(order.persion_first_design));
		pdlg->m_list.SetItemText(nCount,22,A2T(order.type_early_modify));
		pdlg->m_list.SetItemText(nCount,23,A2T(order.persion_early_modify));
		pdlg->m_list.SetItemText(nCount,24,A2T(order.persion_budget));
		pdlg->m_list.SetItemText(nCount,25,A2T(order.persion1_discuss_order));
		pdlg->m_list.SetItemText(nCount,26,A2T(order.persion2_discuss_order));
		pdlg->m_list.SetItemText(nCount,27,A2T(order.date_final_scheme));
		pdlg->m_list.SetItemText(nCount,28,A2T(order.date_book));
		pdlg->m_list.SetItemText(nCount,29,A2T(order.person_exact_measure));
		pdlg->m_list.SetItemText(nCount,30,A2T(order.date_out_order_book));
		pdlg->m_list.SetItemText(nCount,31,A2T(order.person_out_order));
		pdlg->m_list.SetItemText(nCount,32,A2T(order.factory_choose));
		pdlg->m_list.SetItemText(nCount,33,A2T(order.pay_deal_choice));
		pdlg->m_list.SetItemText(nCount,34,A2T(order.date_install_book));
		pdlg->m_list.SetItemText(nCount,35,A2T(order.persion_install));
		pdlg->m_list.SetItemText(nCount,36,A2T(order.problems_install));
		pdlg->m_list.SetItemText(nCount,37,A2T(order.filepath_accessory));
	}else{    //如果接收已经结束
		CDealListView* pDlg = (CDealListView*)m_pDealListView;
		//pDlg->SendMessage(WM_ENDTASK_DealListView,0,0);
		int count= pDlg->m_list.GetItemCount();
		if(0 == count)
		{
			pDlg->m_tip.SetWindowText(_T("没有符合条件的结果"));
		}else{
			CString tip = NULL;
			tip.Format(_T("共搜索到%d个结果"),count);
			pDlg->m_tip.SetWindowText(tip);
		}
		UpdateOperationStatus(_T("搜索订单信息完成"));
	}
	return TRUE;
}


BOOL CSock::RecvReplyOfServFindAll(int len)    //搜索全部服务信息
{ 
	if(len)   //如果接收没有结束
	{
		ServInfo serv;
		memset(&serv,0,sizeof(ServInfo));
		int errCode = recv(m_sockWhole,(char *)&serv,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索全部服务的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CServiceSearch* pdlg = (CServiceSearch*)m_pServiceSearch;
		int nCount = pdlg->m_list.GetItemCount();
		pdlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pdlg->m_list.SetItemText(nCount,0,A2T(serv.serv_NO));
		pdlg->m_list.SetItemText(nCount,1,A2T(serv.prog));
		pdlg->m_list.SetItemText(nCount,2,A2T(serv.serv_type));
		pdlg->m_list.SetItemText(nCount,3,A2T(serv.req_serv_date));
		pdlg->m_list.SetItemText(nCount,4,A2T(serv.delay_date));
		pdlg->m_list.SetItemText(nCount,5,A2T(serv.client_type));
		pdlg->m_list.SetItemText(nCount,6,A2T(serv.ctm_name));
		pdlg->m_list.SetItemText(nCount,7,A2T(serv.client_name));
		pdlg->m_list.SetItemText(nCount,8,A2T(serv.ctm_sex));
		pdlg->m_list.SetItemText(nCount,9,A2T(serv.client_phone));
		pdlg->m_list.SetItemText(nCount,10,A2T(serv.addr));
		pdlg->m_list.SetItemText(nCount,11,A2T(serv.charge));
		pdlg->m_list.SetItemText(nCount,12,A2T(serv.serv_name));
		pdlg->m_list.SetItemText(nCount,13,A2T(serv.serv_phone));
		pdlg->m_list.SetItemText(nCount,14,A2T(serv.serv_content));
		pdlg->m_list.SetItemText(nCount,15,A2T(serv.ps));
	}else{    //如果接收已经结束
		CServiceSearch* pDlg = (CServiceSearch*)m_pServiceSearch;
		//pDlg->SendMessage(WM_ENDTASK_ServiceSearch,0,0);
		int count= pDlg->m_list.GetItemCount();
		if(0 == count)
		{
			pDlg->m_tip.SetWindowText(_T("没有符合条件的结果"));
		}else{
			CString tip = NULL;
			tip.Format(_T("共搜索到%d个结果"),count);
			pDlg->m_tip.SetWindowText(tip);
		}
		UpdateOperationStatus(_T("搜索服务信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfStoreFindAll(int len)    //回复用户搜索全部库存历史记录
{
	if(len)   //如果接收没有结束
	{
		StoreHistory history;
		memset(&history,0,sizeof(StoreHistory));
		int errCode = recv(m_sockWhole,(char *)&history,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索库存历史记录资料的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CStoreHistory* pdlg = (CStoreHistory*)m_pStoreHistory;
		if(m_pStoreHistory == NULL){
			UpdateOperationStatus(_T("搜索库存历史记录资料的请求已经取消"));
			return TRUE;
		}
		int nCount = pdlg->m_list.GetItemCount();
		pdlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pdlg->m_list.SetItemText(nCount,0,A2T(history.Store_Name));
		pdlg->m_list.SetItemText(nCount,1,A2T(history.num_change));
		pdlg->m_list.SetItemText(nCount,2,A2T(history.status));
		pdlg->m_list.SetItemText(nCount,3,A2T(history.time));
	}else{    //如果接收已经结束
		if(m_pStoreHistory != NULL){
			CStoreHistory* pDlg = (CStoreHistory*)m_pStoreHistory;
			//pDlg->SendMessage(WM_ENDTASK_StoreHistory,0,0);
			pDlg->m_tip.SetWindowTextW(_T("搜索库存历史记录信息完成"));
			m_pStoreHistory = NULL;
		}
		UpdateOperationStatus(_T("搜索库存历史记录信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyFindOrderIncomplete(int len)    //搜索未完成订单的回复
{
	if(len)    //没有接收完
	{
		WT;
		char buff[1024*10] = {0};
		int errCode = recv(m_sockWhole,(char *)buff,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索未完成订单的回复信息失败"));
			return FALSE;
		}else if(errCode == 0)
		{
			AfxMessageBox(_T("接收搜索未完成订单的回复信息失败,服务器已关闭"));
			return FALSE;
		}else if(errCode >0){
			COrderIncompletedProgView* p_view = (COrderIncompletedProgView*)m_pOrderIncompleteView;
			p_view->m_list_IncompleteOrder += A2T(buff);
			p_view->m_tip.SetWindowText(_T("接收到服务器返回的数据"));
			return TRUE;
		}
		return TRUE;
	}else{
		COrderIncompletedProgView* p_view = (COrderIncompletedProgView*)m_pOrderIncompleteView;
		//p_view->SendMessage(WM_ENDTASK_OrderIncomplete,0,0);
		p_view->SendMessage(WM_FILLLIST_OrderIncomplete,0,0);
		m_pOrderIncompleteView = NULL;
		return TRUE;
	}
}

BOOL CSock::RecvReplyOfFindAllUsers(int len)      //寻找所有用户
{
	WT;
	if(len)   //如果接收没有结束
	{	
		char buff[1024*10] = {0};
		int errCode = recv(m_sockWhole,(char *)buff,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收用户信息的回复信息失败"));
			return FALSE;
		}else if(errCode == 0)
		{
			AfxMessageBox(_T("接收用户信息的回复信息失败,服务器已关闭"));
			return FALSE;
		}else if(errCode >0){
			if(NULL != m_pSysPostion)
			{
				CSysPostion* p_dlg = (CSysPostion*)m_pSysPostion;
				p_dlg->m_pos_batch += A2T(buff);
				p_dlg->m_tip.SetWindowText(_T("正在接收服务器返回的用户数据"));
				return TRUE;
			}
			return TRUE;
		}
		return TRUE;
	}else{    //如果接收已经结束
		if(NULL != m_pSysPostion)
		{
			CSysPostion* p_dlg = (CSysPostion*)m_pSysPostion;
			//p_dlg->SendMessage(WM_ENDTASK_SysPostion,0,0);
			p_dlg->SendMessage(WM_FILLLIST_SysPostion,0,0);
			m_pSysPostion = NULL;
		}
		UpdateOperationStatus(_T("搜索用户信息完成"));
		return TRUE;
	}
}

BOOL CSock::RecvReplyOfFindEmpPerformance(int len)   //寻找员工业绩的回复
{
	WT;
	if(len)   //如果接收没有结束
	{	
		char buff[1024*10] = {0};
		int errCode = recv(m_sockWhole,(char *)buff,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收员工业绩信息的回复信息失败"));
			return FALSE;
		}else if(errCode == 0)
		{
			AfxMessageBox(_T("接收员工业绩信息的回复信息失败,服务器已关闭"));
			return FALSE;
		}else if(errCode >0){
			if(NULL != m_pEmpPerformance)
			{
				CPerformanceMgView* p_dlg = (CPerformanceMgView*)m_pEmpPerformance;
				p_dlg->m_str_emp_performance_batch += A2T(buff);
				p_dlg->m_tip.SetWindowText(_T("正在接收服务器返回的员工业绩数据"));
				return TRUE;
			}
			return TRUE;
		}
		return TRUE;
	}else{    //如果接收已经结束
		if(NULL != m_pEmpPerformance)
		{
			CPerformanceMgView* p_dlg = (CPerformanceMgView*)m_pEmpPerformance;
			//p_dlg->SendMessage(WM_ENDTASK_EmpPerformance,0,0);
			p_dlg->SendMessage(WM_FILLDATATOLIST_EmpPerformance,0,0);
			m_pEmpPerformance = NULL;
		}
		UpdateOperationStatus(_T("搜索员工业绩信息完成"));
		return TRUE;
	}
			
}

BOOL CSock::RecvReplyOfProdEx(int len)     //接收批量导出产品数据
{
	WT;
	if(len)   //如果接收没有结束
	{	
		char buff[1024*10] = {0};
		int errCode = recv(m_sockWhole,(char *)buff,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收批量导出产品资料的回复信息失败"));
			return FALSE;
		}else if(errCode == 0)
		{
			AfxMessageBox(_T("接收批量导出产品资料的回复信息失败,服务器已关闭"));
			return FALSE;
		}else if(errCode >0){
			if(NULL != m_pProdEx)
			{
				CProdInfoExProp* p_dlg = (CProdInfoExProp*)m_pProdEx;
				p_dlg->m_prod_batch += A2T(buff);
				p_dlg->m_tip.SetWindowText(_T("正在接收服务器返回的数据"));
				return TRUE;
			}
			return TRUE;
		}
		return TRUE;
	}else{    //如果接收已经结束
		if(m_pProdEx != NULL){
			CProdInfoExProp* p_dlg = (CProdInfoExProp*)m_pProdEx;
			p_dlg->SendMessage(WM_ENDTASK_ProdInfoExProp,0,0);
			p_dlg->SendMessage(WM_HANDLE_BATCH_ProdEx,0,0);
		}
		UpdateOperationStatus(_T("搜索产品资料信息完成"));
		return TRUE;
	}
			
}

BOOL CSock::RecvReplyOfCtmEx(int len)     //关于客户资料批量导出
{
	if(len)   //如果接收没有结束
	{
		WT;
		char buff[1024*10] = {0};
		CString _err = _T("接收搜索客户资料的回复信息失败");
		if(!RecvDataFromServer(m_sockWhole,buff,len,_err))
		{
			return FALSE;
		}
		if(NULL != m_pCtmEx)
		{
			CCustomInfoExProp* p_dlg = (CCustomInfoExProp*)m_pCtmEx;
			p_dlg->m_ctm_batch_from_db += A2T(buff);
			p_dlg->m_tip.SetWindowText(_T("正在接收服务器返回的数据"));
		}	
	}else{    //如果接收已经结束
		if(m_pCtmEx != NULL){
			CCustomInfoExProp* pDlg = (CCustomInfoExProp*)m_pCtmEx;
			pDlg->SendMessage(WM_ENDTASK_CustomExProp,0,0);
			pDlg->SendMessage(WM_HANDLE_BATCH_CTM_CtmEx,0,0);
		}
		UpdateOperationStatus(_T("搜索客户资料信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfFindAllCtm(int len)      //关于搜索客户资料的回复
{
	if(len)   //如果接收没有结束
	{
		CtmInfo info;
		memset(&info,0,sizeof(CtmInfo));
		int errCode = recv(m_sockWhole,(char *)&info,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索客户资料的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CCtmInfo* pdlg = (CCtmInfo*)m_pCtmInfo;
		if(m_pCtmInfo == NULL){
			UpdateOperationStatus(_T("搜索客户资料的请求已经取消"));
			return TRUE;
		}
		int nCount = pdlg->m_list.GetItemCount();
		pdlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pdlg->m_list.SetItemText(nCount,0,A2T(info.NO));
		pdlg->m_list.SetItemText(nCount,1,A2T(info.call_ctm));
		pdlg->m_list.SetItemText(nCount,2,A2T(info.name));
		pdlg->m_list.SetItemText(nCount,3,A2T(info.sex));
		pdlg->m_list.SetItemText(nCount,4,A2T(info.phone));
		pdlg->m_list.SetItemText(nCount,5,A2T(info.pay_times));
		pdlg->m_list.SetItemText(nCount,6,A2T(info.pay_total));
		pdlg->m_list.SetItemText(nCount,7,A2T(info.first_pay_time));
		pdlg->m_list.SetItemText(nCount,8,A2T(info.late_pay_time));
		pdlg->m_list.SetItemText(nCount,9,A2T(info.ps)); 
	}else{    //如果接收已经结束
		if(m_pCtmInfo != NULL){
			CCtmInfo* pDlg = (CCtmInfo*)m_pCtmInfo;
			//pDlg->SendMessage(WM_ENDTASK_CtmInfo,0,0);
			pDlg->m_tip.SetWindowTextW(_T("搜索客户资料信息完成"));
			m_pCtmInfo = NULL;
		}
		UpdateOperationStatus(_T("搜索客户资料信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfSearchAllFctInfo(int len)
{
	if(len)   //如果接收没有结束
	{
		FctInfo info;
		memset(&info,0,sizeof(FctInfo));
		int errCode = recv(m_sockWhole,(char *)&info,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索员工的回复信息失败"));
			return FALSE;
		}
		USES_CONVERSION;
		CFctSet* pdlg = (CFctSet*)m_pFctSet;
		if(m_pFctSet == NULL){
			UpdateOperationStatus(_T("搜索员工已经取消"));
			return TRUE;
		}
		int nCount = pdlg->m_list.GetItemCount();
		pdlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pdlg->m_list.SetItemText(nCount,0,A2T(info.name_fct));
		pdlg->m_list.SetItemText(nCount,1,A2T(info.addr_fct));
		pdlg->m_list.SetItemText(nCount,2,A2T(info.contacts_fct));
		pdlg->m_list.SetItemText(nCount,3,A2T(info.main_product_fct));
		pdlg->m_list.SetItemText(nCount,4,A2T(info.phone_fct));
		pdlg->m_list.SetItemText(nCount,5,A2T(info.ps_fct));
	}else{    //如果接收已经结束
		if(m_pFctSet != NULL){
			CFctSet* pDlg = (CFctSet*)m_pFctSet;
			pDlg->SendMessage(WM_ENDTASK_FctSet,0,0);
			pDlg->m_tip.SetWindowTextW(_T("搜索厂家信息完成"));
		}
		UpdateOperationStatus(_T("搜索厂家信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfSearchAllEmpInfoForEx(int len)    //关于搜索员工资料以供导出
{
	if(len)   //如果接收没有结束
	{
		EmpInfo empinfo;
		memset(&empinfo,0,sizeof(EmpInfo));
		int errCode = recv(m_sockWhole,(char *)&empinfo,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索员工的回复信息失败"));
			return FALSE;
		}

		USES_CONVERSION;
		CEmpInfoExProp* pDlg = (CEmpInfoExProp*)m_pEmpInfoEx;
		if(m_pEmpInfoEx == NULL){
			UpdateOperationStatus(_T("搜索员工已经取消"));
			return TRUE;
		}
		int nCount = pDlg->m_list.GetItemCount();
		pDlg->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pDlg->m_list.SetItemText(nCount,0,A2T(empinfo.empNum));
		pDlg->m_list.SetItemText(nCount,1,A2T(empinfo.empName));
		pDlg->m_list.SetItemText(nCount,2,A2T(empinfo.empAddr));
		pDlg->m_list.SetItemText(nCount,3,A2T(empinfo.empPos));
		pDlg->m_list.SetItemText(nCount,4,A2T(empinfo.empPhone));
		pDlg->m_list.SetItemText(nCount,5,A2T(empinfo.empPS));
	}else{    //如果接收已经结束
		if(m_pEmpInfoEx != NULL){
			CEmpInfoExProp* pDlg = (CEmpInfoExProp*)m_pEmpInfoEx;
			pDlg->SendMessage(WM_ENDTASK_EmpInfoExProp,0,0);
		}
		UpdateOperationStatus(_T("搜索员工信息完成"));
	}
	return TRUE;
}

BOOL CSock::RecvReplyOfSearchAllEmpInfo(int len)
{
	if(len)   //如果接收没有结束
	{
		EmpInfo empinfo;
		memset(&empinfo,0,sizeof(EmpInfo));
		int errCode = recv(m_sockWhole,(char *)&empinfo,len,0);
		if(errCode == SOCKET_ERROR)
		{
			AfxMessageBox(_T("接收搜索员工的回复信息失败"));
			return FALSE;
		}

		USES_CONVERSION;
		CEmpInfoView* pView = (CEmpInfoView*)m_pEmpInfoView;
		if(m_pEmpInfoView == NULL){
			UpdateOperationStatus(_T("搜索员工已经取消"));
			return TRUE;
		}
		int nCount = pView->m_list.GetItemCount();
		pView->m_list.InsertItem(nCount,_T(""));//插入新的一行
		pView->m_list.SetItemText(nCount,0,A2T(empinfo.empNum));
		pView->m_list.SetItemText(nCount,1,A2T(empinfo.empName));
		pView->m_list.SetItemText(nCount,2,A2T(empinfo.empAddr));
		pView->m_list.SetItemText(nCount,3,A2T(empinfo.empPos));
		pView->m_list.SetItemText(nCount,4,A2T(empinfo.empPhone));
		pView->m_list.SetItemText(nCount,5,A2T(empinfo.empPS));
	}else{    //如果接收已经结束
		if(m_pEmpInfoView != NULL){
			CEmpInfoView* pDlg = (CEmpInfoView*)m_pEmpInfoView;
			pDlg->SendMessage(WM_CURSORBACKNORMAL,0,0);
			//pDlg->SendMessage(WM_ENDTASK_EmpInfoView,0,0);
			pDlg->m_tip.SetWindowText(_T("搜索员工信息完成"));
			m_pEmpInfoView = NULL;
		}
		UpdateOperationStatus(_T("搜索员工信息完成"));
		return TRUE;
	}
	return TRUE;
}

void CSock::UpdateOperationStatus(CString _status)  //用以将当前操作信息更新到状态栏
{
	CBMApp *p_App = (CBMApp *)AfxGetApp();
	p_App->m_pMainWnd->SendMessage(WM_UPDATEOPERATIONSTATUS,0,(LPARAM)(LPCTSTR)_status);
}


BOOL CSock::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max)    //true  有效 否则无效  
	//检查项是否为空 是否是数字 以及长度是否超过限制  如果不要检查是否是数字的话 那么就校验是否包含特殊字符  最后还有是否符合日期格式
{
	WT;
	if(bCheckEmpty)    //TRUE  代表需要检查是否为空 不允许为空
	{
		if(_source.IsEmpty())   //
		{
			CString err = _item_name+_T("不能为空");
			AfxMessageBox(err);
			return FALSE;
		}
	}
	if(!_source.IsEmpty())
	{
		if(bCheckNum)    //需要检查是否为数字
		{
			if(!IsNum(_source))
			{
				CString err = _item_name+_T("必须为数字");
				AfxMessageBox(err);
				return FALSE;
			}
		}
		if(ContainsCharsRemain(_source))
		{
			return FALSE;
		}

		if(_source.GetLength()>str_len_max)  //检查数据长度
		{
			CString err;
			err.Format(_T("%s数据长度不能超过%d"),_item_name,str_len_max);
			AfxMessageBox(err);
			return FALSE;
		}
		if(bCheckDateFormat)   //检查是否符合日期格式
		{
			CString _err2 = _item_name+_T("不符合日期格式，标准格式如：2014-10-05");
			CString str = _source;
			int  index = _source.Find('-');
			if(index == -1)   //没找到
			{
				AfxMessageBox(_err2);
				return FALSE;
			}else{
				CString left = str.Mid(0,index);
				str = str.Mid(index+1);
				if(left.GetLength()!=4)
				{
					AfxMessageBox(_err2);
					return FALSE;
				}else{
					if(!IsNum(left))
					{
						AfxMessageBox(_err2);
						return FALSE;
					}else if(atoi(T2A(left))<2000 ||atoi(T2A(left))>3000){
						CString err3 = _item_name+_T("年份必须为2000到3000之间");
						AfxMessageBox(err3);
						return FALSE;
					}else{    //前面还算合法的
						index = str.Find('-');
						if(index == -1)   //没找到
						{
							AfxMessageBox(_err2);
							return FALSE;
						}else{
							left = str.Mid(0,index);
							str = str.Mid(index+1);
							if(IsNum(left)&&IsNum(str))
							{
								if(atoi(T2A(left))<1||atoi(T2A(left))>12)
								{
									CString err3 = _item_name+_T("月份必须为1到12之间");
									AfxMessageBox(err3);
									return FALSE;
								}else if(atoi(T2A(left))<1||atoi(T2A(left))>31)
								{
									CString err3 = _item_name+_T("天必须为1到31之间");
									AfxMessageBox(err3);
									return FALSE;
								}
							}else{
								AfxMessageBox(_err2);
								return FALSE;
							}
						}
					}
				}
			}
		}
	}
	return TRUE;
}


BOOL CSock::ContainsCharsRemain(CString str)     //检查字符串是否包含子字符串
{
	CString temp1 = _T("[#");
	CString temp2 = _T("#]");
	CString temp3 = _T("{#");
	CString temp4 = _T("#}");
	int flag = 0;
	flag = str.Find(temp1);
	if(flag != -1)
	{
		AfxMessageBox(_T("数据中不能包含‘[#’字符"));
		return TRUE;
	}
	flag = str.Find(temp2);
	if(flag != -1)
	{
		AfxMessageBox(_T("数据中不能包含‘#]’字符"));
		return TRUE;
	}
	flag = str.Find(temp3);
	if(flag != -1)
	{
		AfxMessageBox(_T("数据中不能包含‘#}’字符"));
		return TRUE;
	}
	flag = str.Find(temp4);
	if(flag != -1)
	{
		AfxMessageBox(_T("数据中不能包含‘{#’字符"));
		return TRUE;
	}
	return FALSE;
}

BOOL CSock::IsNum(CString str)    //传入一个字符串 ，检查是否为数字
{
	WT;
	char *p = T2A(str);
	int i = 0;
	int count = str.GetLength();
	while(count){
		if(p[i]>'9'||p[i]<'0')
		{
			if(p[i] != '.')
			{
				return FALSE;
			}
			
		}
		count--;
		i++;
	}
	return TRUE;
}

BOOL CSock::RecvDataFromServer(SOCKET s,char* p_recv,int data_len,CString message)
{
	int nflag = recv(s,p_recv,data_len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(message);
		UpdateOperationStatus(message);
		return FALSE;
	}else if(nflag == 0)
	{
		AfxMessageBox(message);
		UpdateOperationStatus(message);
		return FALSE;
	}else if(nflag >0)
	{
		return TRUE;
	}
	return TRUE;
}

BOOL CSock::SendDataToServer(SOCKET s,char* p_send,CString message,int data_len)
{
	int nflag = send(s,p_send,data_len,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(message);
		UpdateOperationStatus(message);
		return FALSE;
	}
	return TRUE;
}

BOOL CSock::IsNum(CString _source,BOOL bCheckDot)
{
	WT;
	char *p = T2A(_source);
	int i = 0;
	int count = _source.GetLength();
	while(count){
		if(p[i]>'9'||p[i]<'0')
		{
			if(bCheckDot)    //只有在需要检查是否为点的时候才检查
			{
				if(p[i] != '.')
				{
					return FALSE;
				}
			}else{
				return FALSE;
			}
			
			
		}
		count--;
		i++;
	}
	return TRUE;
}

BOOL CSock::IsUserHasAuthority(int n_authority)
{
	CString tip;
	if(NULL == m_pPosAuthority)
	{
		tip = _T("用户权限加载失败，即将退出在线状态，请重新登录！");
		AfxMessageBox(tip);
		m_sockWhole = INVALID_SOCKET;
		m_bIsConnecting = FALSE;     //连接断开 让用户重新登录
		m_bOnline = FALSE;        // 断开线后 连接不可用
		m_nameOnline.Empty();     //断开连接后 清空在线的用户名
		((CBMApp*)AfxGetApp())->m_sHost = INVALID_SOCKET;   //这样可以重新初始化socket
		return FALSE;
	}
	if(!m_pPosAuthority->bLoadSuccess)
	{
		tip = _T("用户权限加载失败，即将退出在线状态，请重新登录！");
		AfxMessageBox(tip);
		m_sockWhole = INVALID_SOCKET;
		m_bIsConnecting = FALSE;     //连接断开 让用户重新登录
		m_bOnline = FALSE;        // 断开线后 连接不可用
		m_nameOnline.Empty();     //断开连接后 清空在线的用户名
		((CBMApp*)AfxGetApp())->m_sHost = INVALID_SOCKET;   //这样可以重新初始化socket
		return FALSE;
	}
	if(n_authority < 0 || n_authority > 43)
	{
		tip = _T("注意，权限索引不在合法范围！");
		AfxMessageBox(tip);
		return FALSE;
	}
	if(FALSE == m_pPosAuthority->autority[n_authority])
	{
		tip = _T("抱歉，您不具有此操作的权限，请向系统管理员申请相应权限！");
		AfxMessageBox(tip);
		return FALSE;
	}
	return TRUE;
}


/*
 * 接收文件的数据0:正常1:异常2:发送数据结束
 */
int CSock::RecvFileData(SOCKET s, unsigned short ID,void* ptr)
{
	int nErrCode;	//返回错误
	hdr header;		//包头
	
	//接收包头
	nErrCode = recv(s, (char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{	
		int err = WSAGetLastError();
		CString _err_1;
		_err_1.Format(_T("接收下载文件数据包头失败,SOCKET_ERROR错误：%d"),err);
		AfxMessageBox(_err_1);
		return EXCEPT;
	}	
	if (REPLY_REQ_FILE_DOWNLOAD != header.type)
	{
		AfxMessageBox(_T("服务器发送文件错误！"));
		return EXCEPT;
	}
	
	//发送文件结束
	if (0 == header.fpos && 0 == header.flen )
	{
		return FINISH;
	}


	unsigned long ulFileSeg = header.flen;	//文件长度
	unsigned long ulFilePos = header.fpos;	//文件位置(相对位置)
	CDealListView* p_dlg = (CDealListView*)ptr;
	p_dlg->m_ulCurTotal += ulFileSeg;				//下载文件总长度

	unsigned long nlTotal = 0;				//每次接收下载文件总长度
	unsigned long nRead = 0;				//接收长度
	char cBuf[RECV_BUFF] = {0};
	while (CSock::m_bIsConnecting && nlTotal != ulFileSeg)
	{
		memset(cBuf, 0, RECV_BUFF);
		nRead = recv(s, cBuf, ulFileSeg, 0);//准备接收1024*20个字节长度
		if (SOCKET_ERROR == nRead)
		{
			int err = WSAGetLastError();
			CString _err_1;
			_err_1.Format(_T("接收文件包体失败,SOCKET_ERROR错误：%d"),err);
			AfxMessageBox(_err_1);
			return EXCEPT;
		}else if (0 == nRead)
		{
			int err = WSAGetLastError();
			CString _err_1;
			_err_1.Format(_T("接收文件包体失败,SOCKET_ERROR错误：%d"),err);
			AfxMessageBox(_err_1);
			return EXCEPT;
		}

		SaveFile(ID, cBuf, nRead, ulFilePos + nlTotal,ptr);//保存临时文件
		nlTotal += nRead;//加入接收字节数
	}
	return NORMAL;
}

/*
 * 保存临时文件
 */
void CSock::SaveFile(u_short nIndexThread, const char* pBuf, u_long ulLen, u_long ulPos,void* ptr)
{
	CDealListView* p_dlg = (CDealListView*)ptr;
	WT;
	FILE *stream = NULL;
	CString strTemp = p_dlg->m_strFilePath;	//保存文件路径
	for(int i= 0;i<MAX_RECVFILE_THREAD;i++)
	{
		if(i == nIndexThread)
		{
			strTemp += p_dlg->m_DLFileTempName;
			CString temp = NULL;
			temp.Format(_T("%d.dl"),i);
			strTemp += temp;    //第i个临时文件名
			break;
		}
	}
	//stream = fopen(strTemp.c_str(), "ab+");		//打开文件
	errno_t err = fopen_s(&stream,T2A(strTemp),"ab+");
	ASSERT(err == 0);
	//ASSERT(stream);
	
	int nErrCode = fseek(stream,ulPos,SEEK_SET);//移动文件指针
	ASSERT(nErrCode == 0);
	
	nErrCode = fwrite(pBuf, 1, ulLen, stream);	//写文件
	ASSERT(nErrCode == ulLen);

	fflush(stream);								//清除文件缓冲区
	fclose(stream);								//关闭文件
}


void CSock::AssemblyFile(void* ptr)      //合并下载文件的线程函数
{
	WT;
	//文件指针	
	//得到文件名
	CDealListView* p_dlg = (CDealListView*)ptr;
	int nIndex = p_dlg->m_strToDLFile.Find(',');//找到末尾的'\'	
	CString strFileName = p_dlg->m_strToDLFile.Mid(nIndex + 1);   //文件名	
	CString strFile = p_dlg->m_strFilePath;//文件路径
	strFile += strFileName;

	//检查该文件是否存在
	CFileFind find;
	BOOL flag = find.FindFile(strFile);
	if(flag)   //如果有这个文件存在那就把下载的文件重命名了
	{
		nIndex = strFile.ReverseFind('.');
		CString _str_temp = strFile.Mid(0,nIndex)+_T("1");
		_str_temp += strFile.Mid(nIndex+1);
		strFile = _str_temp;     //重命名为文件名+1的文件
	}
	
	FILE *streamIn = NULL;//写
	//streamIn =  fopen(strFile.c_str(), "wb+");//打开文件
	errno_t err = fopen_s(&streamIn,T2A(strFile), "wb+");
	ASSERT(streamIn);
	int threadNum = MAX_RECVFILE_THREAD;   //最大接收线程数
	u_long file_len = 0;   //文件长度
	for(int i =0;i<threadNum;i++)
	{
		FILE* stream = NULL;
		CString strTemp = p_dlg->m_strFilePath;    //第i个临时文件
		strTemp += p_dlg->m_DLFileTempName;
		CString _str_temp = NULL;
		_str_temp.Format(_T("%d"),i);
		_str_temp += _T(".dl");
		strTemp += _str_temp;

		errno_t err1 = fopen_s(&stream,T2A(strTemp), "rb");   //打开文件
		ASSERT(stream);
		u_long	ulFileLen = AssembleLine(streamIn, file_len, stream);  //写入文件
		file_len += ulFileLen;
		fclose(stream);
		//删除临时文件	
		DeleteFile(strTemp);
	
	}
	fclose(streamIn);
}

u_long  CSock::AssembleLine(FILE* destFile, int startPos, FILE* resoFile)
{
	//获取读文件的长度
	fseek(resoFile, 0, SEEK_END);
	unsigned long resoFileLen = ftell(resoFile);
	
	unsigned long  nOffsetLen = 0;	//已经读取字节数 
	unsigned long  nReadLen = 0;	//fread读取字节数

	char buf[BUF_SIZE];				//临时数组
	memset(buf, 0, BUF_SIZE);		//清空
	while (resoFileLen != nOffsetLen && !feof(resoFile))
	{
		fseek(resoFile,0 + nOffsetLen,SEEK_SET);		//移动读文件指针
		nReadLen = fread(buf,1,BUF_SIZE, resoFile);		//读入数据
		fseek(destFile,startPos + nOffsetLen,SEEK_SET);	//移动写文件指针
		fwrite(buf,1,nReadLen,destFile);				//写入数据
		nOffsetLen += nReadLen;							//增加已读数据
		memset(buf, 0, BUF_SIZE);						//清空
	}

	return nOffsetLen;									//返回实际文件长度
}