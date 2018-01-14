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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "BM.h"

#include "MainFrm.h"
#include "MainFormView.h"
#include "EmpInfoView.h"
#include "DataImportPropSheet.h"
#include "DataExportPropSheet.h"
#include "SysPostion.h"
#include "MtAndFctSheet.h"
#include "CtmInfo.h"
#include "StoreInfo.h"
#include "StoreHistory.h"
#include "ServiceSearch.h"
#include "ServReg.h"
#include "DealListView.h"

#include "OrderIncompletedProgView.h"
#include "PerformanceMgView.h"
#include "Login.h"
#include "OrderContent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_BTN_DATAIMPORT, &CMainFrame::OnBtnDataImport)
	ON_COMMAND(ID_BUTTON_EMPLOYEEINFO, &CMainFrame::OnButtonEmployeeinfo)
	ON_COMMAND(ID_BUTTON_HOME, &CMainFrame::OnButtonHome)
	ON_COMMAND(ID_BUTTON_DATAEXPORT, &CMainFrame::OnButtonDataExport)
	ON_COMMAND(ID_BUTTON_SYSYTEMPOSTION, &CMainFrame::OnButtonSysytempostion)
	ON_COMMAND(ID_BUTTON_BACKOFF, &CMainFrame::OnBTNBackoff)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_GOFORWARD, &CMainFrame::OnUpdateButtonGoforward)
//	ON_UPDATE_COMMAND_UI(IDS_STATUS_PANE2,&CMainFrame::OnUpdateStatusCurTime)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BACKOFF, &CMainFrame::OnUpdateButtonBackoff)
	ON_COMMAND(ID_BUTTON_GOFORWARD, &CMainFrame::OnButtonGoforward)
	ON_COMMAND(ID_BUTTON_MATERIALANDFACTORY, &CMainFrame::OnButtonMaterialandfactory)
	ON_COMMAND(ID_BUTTON_CUSTOMINFO, &CMainFrame::OnBtnSwitchToCustominfo)
	ON_COMMAND(ID_BUTTON_STOCKINFO, &CMainFrame::OnButtonStockInfo)
	ON_COMMAND(ID_BUTTON_HISTORY, &CMainFrame::OnButtonHistory)
	ON_COMMAND(ID_BUTTON_SERVMG, &CMainFrame::OnBtnServMg)
	ON_COMMAND(ID_BUTTON_SERVREG, &CMainFrame::OnBtnServReg)
	ON_COMMAND(ID_BUTTON_DEALLIST, &CMainFrame::OnButtonDealList)
	ON_COMMAND(ID_BUTTON_DEALNEW, &CMainFrame::OnBtnDealNew)
	ON_COMMAND(ID_BUTTON_DEALTRACE, &CMainFrame::OnButtonDealtrace)
	ON_COMMAND(ID_BTN_PERFORMENCEMG, &CMainFrame::OnBtnPerformenceMg)
	ON_COMMAND(ID_BUTTON_RELOGIN, &CMainFrame::OnButtonRelogin)
	ON_WM_TIMER()
	ON_MESSAGE(WM_UPDATEOPERATIONSTATUS, &CMainFrame::OnUpdateOperationStatus)
	//ON_MESSAGE(WM_AFTERLOGIN_MainView, &CMainFrame::OnAfterLoginMainView)
	//ON_MESSAGE(WM_REQSTARTINITNOTE_MainView, &CMainFrame::OnReqStartInitNoteMainView)
END_MESSAGE_MAP()

// CMainFrame 构造/析构
/*
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	//IDS_STATUS_PANE_STATUS_OPERATION,
	IDS_STATUS_PANE_CUR_TIME,
	IDS_STATUS_PANE_DEAL_NEW_TIME,
	IDS_STATUS_PANE_USER,
};*/

CMainFrame::CMainFrame()
	: m_bForwardEnable(false)
	, m_bBackEnable(false)
{
	// TODO: 在此添加成员初始化代码
	m_pStrArr = new CStringArray();
	m_pStrArr->Add(_T("IDD_DLG_MAINVIEW"));
	m_index = 0;
	//m_pMainView = NULL; 
	//m_pMtAndFctSheet = NULL;
	//m_bMtAndFctCreated = FALSE;
}

CMainFrame::~CMainFrame()
{
	if(NULL != m_pStrArr)
	{
		delete m_pStrArr;
		m_pStrArr = NULL;
	}
	/*
	if(NULL != m_pMtAndFctSheet)
	{
		delete m_pMtAndFctSheet;
		m_pMtAndFctSheet = NULL;
	}*/
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if(-1 == InitStatusBar())  //初始化状态栏
	{
		return -1;
	}
	SetTimer(1,1000,NULL);  //每一秒更新一次状态
	//SetTimer(2,2000,NULL);
	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 创建标题栏:
	if (!CreateCaptionBar())
	{
		TRACE0("未能创建标题栏\n");
		return -1;      // 未能创建
	}

	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	this->SetWindowText(_T("旭日经管助手客户端"));
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE;     //自己添加
	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("未能创建标题栏\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

//切换视图代码
void CMainFrame::SwitchToView(int nForm) {
	//CDocument* pDoc = GetActiveDocument(); 
	CView *pOldActiveView=GetActiveView(); //保存旧视图
	CView *pNewActiveView=(CView*)GetDlgItem(nForm); //取得新视图
		
	if(pNewActiveView==NULL){
		switch(nForm) //这些ID是对话框的标志符，但也可以用其他的标志
		{
			case IDD_DLG_MAINVIEW:
			//CRuntimeClass *pViewClass =
				pNewActiveView=(CView*)new CMainFormView;
				//m_pMainView = (CMainFormView*)pNewActiveView;  //用来登录成功后加载公告
				break;
			case IDD_DLG_EMPLOYEEINFOVIEW:
				pNewActiveView=(CView*)new CEmpInfoView;
				break;
			case IDD_FORMVIEW_SERVICE_SEARCH:
				pNewActiveView=(CView*)new CServiceSearch;
				break;
			case IDD_FORMVIEW_DEAL_LIST:
				pNewActiveView=(CView*)new CDealListView;
				break;
			case IDD_FORMVIEW_DEAL_INCOMPLETED:
				pNewActiveView=(CView*)new COrderIncompletedProgView;
				break;
			case IDD_FORMVIEW_PERFORMANCE_MG:
				pNewActiveView=(CView*)new CPerformanceMgView;
				break;
		}
		CCreateContext context;//将文挡和视图相连
		context.m_pCurrentDoc=pOldActiveView->GetDocument();
		pNewActiveView->Create(NULL,NULL, WS_BORDER|WS_CHILD, CFrameWnd::rectDefault,this,nForm,&context);
		pNewActiveView->OnInitialUpdate();
	}
	/*CRuntimeClass *pViewClass = NULL;
	if(pNewActiveView == NULL) {
		switch(nForm) //这些ID是对话框的标志符，但也可以用其他的标志
		{
			case IDD_DLG_MAINVIEW:
				pViewClass = RUNTIME_CLASS(CMainFormView);
				break;
			case IDD_DLG_EMPLOYEEINFOVIEW:
				pViewClass = RUNTIME_CLASS(CEmpInfoView);
				break;
		}
		CCreateContext context;
		CDocument *pDoc = GetActiveView()->GetDocument();

		context.m_pNewViewClass = pViewClass;
		context.m_pCurrentDoc = pDoc;
		context.m_pNewDocTemplate = NULL;
		context.m_pCurrentFrame = NULL;
		context.m_pLastView = NULL;
		//context.m_pLastView = NULL; 
		pNewActiveView = (CView *)this->CreateView(&context);
	}
	*/
	SetActiveView(pNewActiveView);//改变活动的视图
	pNewActiveView->ShowWindow(SW_SHOWMAXIMIZED); //显示新的视图
	pOldActiveView->ShowWindow(SW_HIDE); //隐藏旧的视图
	//pNewActiveView->GetDocument()-SetTitle(\"。。。\");
	if(pOldActiveView->GetRuntimeClass()==RUNTIME_CLASS(CMainFormView))
	{
		pOldActiveView->SetDlgCtrlID(IDD_DLG_MAINVIEW);
	}else if(pOldActiveView->GetRuntimeClass()==RUNTIME_CLASS(CEmpInfoView))
	{
		pOldActiveView->SetDlgCtrlID(IDD_DLG_EMPLOYEEINFOVIEW);
	}else if(pOldActiveView->GetRuntimeClass()==RUNTIME_CLASS(CServiceSearch))
	{
		pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_SERVICE_SEARCH);
	}else if(pOldActiveView->GetRuntimeClass()==RUNTIME_CLASS(CDealListView))
	{
		pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_DEAL_LIST);
	}else if(pOldActiveView->GetRuntimeClass()==RUNTIME_CLASS(COrderIncompletedProgView))
	{
		pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_DEAL_INCOMPLETED);
	}else if(pOldActiveView->GetRuntimeClass()==RUNTIME_CLASS(CPerformanceMgView))
	{
		pOldActiveView->SetDlgCtrlID(IDD_FORMVIEW_PERFORMANCE_MG);
	}

	pNewActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	//delete pOldActiveView; //删除旧视图
	RecalcLayout(); //调整框架窗口 
}



void CMainFrame::OnBtnDataImport()
{
	// TODO: 在此添加命令处理程序代码
	CDataImportPropSheet propSheet(_T("数据批量导入"));
	propSheet.DoModal();
}


void CMainFrame::OnButtonEmployeeinfo()
{
	// TODO: 在此添加命令处理程序代码
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CEmpInfoView))){
		return;
	}
	if(AddToTail(IDD_DLG_EMPLOYEEINFOVIEW))
	{
		m_pStrArr->Add(_T("IDD_DLG_EMPLOYEEINFOVIEW"));
	}
	m_index = m_pStrArr->GetSize();
	SetForwardAndBackState();

	this->SwitchToView(IDD_DLG_EMPLOYEEINFOVIEW);
}


void CMainFrame::OnButtonHome()
{
	// TODO: 在此添加命令处理程序代码
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CMainFormView))){
		return;
	}
	if(AddToTail(IDD_DLG_MAINVIEW))
	{
		m_pStrArr->Add(_T("IDD_DLG_MAINVIEW"));
	}
	m_index = m_pStrArr->GetSize();
	SetForwardAndBackState();
	
	this->SwitchToView(IDD_DLG_MAINVIEW);
}


void CMainFrame::OnButtonDataExport()
{
	// TODO: 在此添加命令处理程序代码
	CDataExportPropSheet propSheet(_T("数据导出"));
	propSheet.DoModal();
}


void CMainFrame::OnButtonSysytempostion()
{
	// TODO: 在此添加命令处理程序代码
	CSysPostion sysPos;
	sysPos.DoModal();
	
}
/*
int CMainFrame::InitStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, 
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
}*/


int CMainFrame::InitStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	
	BOOL bNameValid;
	CString curOperation;   //当前操作
	CString curTime;   //当前时间
	CString dealNewTime;     //开单时间
	CString onlineStatus;    //在线状态  
	bNameValid = curOperation.LoadString(IDS_STATUS_PANE_STATUS_OPERATION);   //当前操作
	ASSERT(bNameValid);
	bNameValid = curTime.LoadString(IDS_STATUS_PANE_CUR_TIME);   //当前时间
	ASSERT(bNameValid);
	bNameValid = dealNewTime.LoadString(IDS_STATUS_PANE_DEAL_NEW_TIME);   //开单时间
	ASSERT(bNameValid);
	bNameValid = onlineStatus.LoadString(IDS_STATUS_PANE_USER);   //用户状态
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(IDS_STATUS_PANE_USER, onlineStatus, TRUE), onlineStatus);
	m_wndStatusBar.AddSeparator();
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(IDS_STATUS_PANE_STATUS_OPERATION, curOperation, TRUE), curOperation);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(IDS_STATUS_PANE_CUR_TIME, curTime, TRUE), curTime);

	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(IDS_STATUS_PANE_DEAL_NEW_TIME, dealNewTime, TRUE), dealNewTime);	
	//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(
	//m_statusBarCurTime.Empty();
	//m_wndStatusBar.SetDlgItemText(IDS_STATUS_PANE_DEAL_NEW_TIME,_T("hehhe"));
	UpdateBaseStatus();
	return 0;
}

void CMainFrame::SetForwardAndBackState()
{
	if( m_index -1 > 0){
		m_bBackEnable = true;
	}else{
		m_bBackEnable = false;
	}

	
	if(m_index < m_pStrArr->GetSize()){
		m_bForwardEnable = true;
	}else{
		m_bForwardEnable = false;
	}
	//(CWnd*)GetDlgItem(ID_BUTTON_GOFORWARD)->EnableWindow(m_bForwardEnable);
	//(CWnd*)GetDlgItem(ID_BUTTON_BACKOFF)->EnableWindow(m_bBackEnable);
}



void CMainFrame::OnBTNBackoff()
{
	// TODO: 在此添加命令处理程序代码
	if((m_index -1 ) < 0 || (m_index -1 ) == 0){
		return;
	}else {
		m_index--;
		SetForwardAndBackState();
		int nItem;
		UndoToItem(nItem);
		SwitchToView(nItem);
	}
	
}

//取出链表存的ID资源
void CMainFrame::UndoToItem(int& nItem ) 
{
	int temp = m_index - 1;
	CString str =  m_pStrArr->GetAt(temp);
	if("IDD_DLG_MAINVIEW" == str){
		nItem = IDD_DLG_MAINVIEW;
	}else if("IDD_DLG_EMPLOYEEINFOVIEW" == str){
		nItem = IDD_DLG_EMPLOYEEINFOVIEW;
	}else if("IDD_FORMVIEW_SERVICE_SEARCH" == str){
		nItem = IDD_FORMVIEW_SERVICE_SEARCH;
	}else if("IDD_FORMVIEW_DEAL_LIST" == str){
		nItem = IDD_FORMVIEW_DEAL_LIST;
	}else if("IDD_FORMVIEW_DEAL_INCOMPLETED" == str){
		nItem = IDD_FORMVIEW_DEAL_INCOMPLETED;
	}else if("IDD_FORMVIEW_PERFORMANCE_MG" == str){
		nItem = IDD_FORMVIEW_PERFORMANCE_MG;
	}else{
		nItem = -1;
		return;
	}


	
}



void CMainFrame::OnUpdateButtonGoforward(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bForwardEnable);
}

void CMainFrame::OnUpdateButtonBackoff(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bBackEnable);
}


void CMainFrame::OnButtonGoforward()
{
	// TODO: 在此添加命令处理程序代码
	if(m_index == m_pStrArr->GetSize()){
		return;
	}
	m_index++;
	SetForwardAndBackState();
	int nItem = -1;
	UndoToItem(nItem);
	
	SwitchToView(nItem);
}

//判断是否要把新打开的页面加入到链表的尾端
//注意执行过这个函数以后，m_index会移动到链表的尾端
BOOL CMainFrame::AddToTail(int nItem)
{
	m_index = m_pStrArr->GetSize();
	int nTemp = 0;
	UndoToItem(nTemp);
	if( nTemp == nItem)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}
}


void CMainFrame::OnButtonMaterialandfactory()
{
	// TODO: 在此添加命令处理程序代码	
		CMtAndFctSheet dlg(_T("材料和厂家设置"));		
		dlg.DoModal();
}


void CMainFrame::OnBtnSwitchToCustominfo()
{
	// TODO: 在此添加命令处理程序代码
	CCtmInfo ctmInfo;
	ctmInfo.DoModal();
}


void CMainFrame::OnButtonStockInfo()
{
	// TODO: 在此添加命令处理程序代码
	CStoreInfo storeInfo;
	storeInfo.DoModal();
}


void CMainFrame::OnButtonHistory()
{
	// TODO: 在此添加命令处理程序代码
	CStoreHistory storeHistory;
	storeHistory.DoModal();
}


void CMainFrame::OnBtnServMg()
{
	// TODO: 在此添加命令处理程序代码
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CServiceSearch))){
		return;
	}
	if(AddToTail(IDD_FORMVIEW_SERVICE_SEARCH))
	{
		m_pStrArr->Add(_T("IDD_FORMVIEW_SERVICE_SEARCH"));
	}
	m_index = m_pStrArr->GetSize();
	SetForwardAndBackState();
	
	this->SwitchToView(IDD_FORMVIEW_SERVICE_SEARCH);
}


void CMainFrame::OnBtnServReg()
{
	// TODO: 在此添加命令处理程序代码
	CServReg servReg;
	servReg.DoModal();
}


void CMainFrame::OnButtonDealList()
{
	// TODO: 在此添加命令处理程序代码
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CDealListView))){
		return;
	}
	if(AddToTail(IDD_FORMVIEW_DEAL_LIST))
	{
		m_pStrArr->Add(_T("IDD_FORMVIEW_DEAL_LIST"));
	}
	m_index = m_pStrArr->GetSize();
	SetForwardAndBackState();
	
	this->SwitchToView(IDD_FORMVIEW_DEAL_LIST);
}


void CMainFrame::OnBtnDealNew()
{
	COrderContent dlg;
	dlg.SetBackgroundColor(RGB(185,211,255)); 
	dlg.DoModal();
	
}


void CMainFrame::OnButtonDealtrace()
{
	// TODO: 在此添加命令处理程序代码
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(COrderIncompletedProgView))){
		return;
	}
	if(AddToTail(IDD_FORMVIEW_DEAL_INCOMPLETED))
	{
		m_pStrArr->Add(_T("IDD_FORMVIEW_DEAL_INCOMPLETED"));
	}
	m_index = m_pStrArr->GetSize();
	SetForwardAndBackState();
	
	this->SwitchToView(IDD_FORMVIEW_DEAL_INCOMPLETED);

}


void CMainFrame::OnBtnPerformenceMg()
{
	// TODO: 在此添加命令处理程序代码
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CPerformanceMgView))){
		return;
	}
	if(AddToTail(IDD_FORMVIEW_PERFORMANCE_MG))
	{
		m_pStrArr->Add(_T("IDD_FORMVIEW_PERFORMANCE_MG"));
	}
	m_index = m_pStrArr->GetSize();
	SetForwardAndBackState();
	
	this->SwitchToView(IDD_FORMVIEW_PERFORMANCE_MG);
}


void CMainFrame::OnButtonRelogin()
{
	// TODO: 在此添加命令处理程序代码
	CLogin loginDlg;
	loginDlg.DoModal();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			UpdateBaseStatus();
			break;
		}
	case 2:
		{
			//this->SendMessage(CN_UPDATE_COMMAND_UI,0,0);
			break;
		}
	default:
		break;
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::UpdateBaseStatus()   
	//更新的状态栏信息包括当前时间状态栏  
	//当前的开单时间 当前的用户是否在线状态
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	CString dealNewTime;
	CString curTime;
	dealNewTime.Format(_T("开单时间：%4d/%02d/%02d"),sys.wYear,sys.wMonth,sys.wDay);
	curTime.Format(_T("当前时间：%02d:%02d:%02d"),sys.wHour,sys.wMinute, sys.wSecond);
	
	CMFCRibbonBaseElement* p_curtime = m_wndStatusBar.FindByID(IDS_STATUS_PANE_CUR_TIME);
	p_curtime->SetText(curTime);
	CMFCRibbonBaseElement* p_dealnewtime = m_wndStatusBar.FindByID(IDS_STATUS_PANE_DEAL_NEW_TIME);
	p_dealnewtime->SetText(dealNewTime);
	CMFCRibbonBaseElement* p_onlinestatus = m_wndStatusBar.FindByID(IDS_STATUS_PANE_USER);
	if(!CSock::m_nameOnline.IsEmpty()&&CSock::m_bOnline == TRUE)
	{
		CString user = NULL;
		user.Format(_T("当前在线用户：'%s'"),CSock::m_nameOnline);
		p_onlinestatus->SetText(user);
	}else{
		p_onlinestatus->SetText(_T("未登录，请登录！"));
		
	}
	CMFCRibbonBaseElement* p_pane = m_wndStatusBar.FindByID(IDS_STATUS_PANE_STATUS_OPERATION);
	if(CSock::m_bOnline == FALSE)
	{	
		p_pane->SetText(_T("离线状态"));
	}
	m_wndStatusBar.RecalcLayout();
    m_wndStatusBar.RedrawWindow();
	return;
}




afx_msg LRESULT CMainFrame::OnUpdateOperationStatus(WPARAM wParam, LPARAM lParam)
{
	CString _strStatus = (LPCTSTR)lParam;
	UpdateOperationStatus(_strStatus);
	return 0;
}

void CMainFrame::UpdateOperationStatus(CString _operationStatus)
{
	if(_operationStatus.IsEmpty()||"0" == _operationStatus)
	{
		_operationStatus = _T("就绪");
	}
	CMFCRibbonBaseElement* p_pane = m_wndStatusBar.FindByID(IDS_STATUS_PANE_STATUS_OPERATION);
	p_pane->SetText(_operationStatus);
	m_wndStatusBar.RecalcLayout();
	m_wndStatusBar.RedrawWindow();
}

