// OrderContentProp.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "OrderContent.h"
#include "afxdialogex.h"


// COrderContent 对话框

IMPLEMENT_DYNAMIC(COrderContent, CDialogEx)

COrderContent::COrderContent(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrderContent::IDD, pParent)
	, m_accessory_filepath(_T(""))
	, m_baseinfo_ps(_T(""))
	, m_charge_name(_T(""))
	, m_contract_NO(_T(""))
	, m_contract_total_price(0)
	, m_ctm_addr(_T(""))
	, m_ctm_call(_T(""))
	, m_ctm_name(_T(""))
	, m_ctm_phone(_T(""))
	, m_ctm_ps(_T(""))
	, m_persion1_discuss(_T(""))
	, m_person2_discuss(_T(""))
	, m_person_earlyStageModify(_T(""))
	, m_person_exact_measure(_T(""))
	, m_factory_choose(_T(""))
	, m_person_install(_T(""))
	, m_problems_install(_T(""))
	, m_money_already_take(0)
	, m_order_NO(_T(""))
	, m_person_budget(_T(""))
	, m_person_out_order(_T(""))
	, m_persion_first_test(_T(""))
	, m_persion_first_design(_T(""))
{
	m_ulCurTotal = 0;
}

COrderContent::~COrderContent()
{
	
}

void COrderContent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CTM_FROM, m_ctmFrom);
	DDX_Control(pDX, IDC_STATIC_INTR_NAME, m_staticIntrName);
	DDX_Control(pDX, IDC_STATIC_INTR_PHONE, m_staticIntrPhone);
	DDX_Control(pDX, IDC_STATIC_SERV_PERSON, m_staticServPersonNM);
	DDX_Control(pDX, IDC_EDIT_INTR_NM, m_intrName);
	DDX_Control(pDX, IDC_EDIT_INTR_PHONE, m_intrPhone);
	DDX_Control(pDX, IDC_EDIT_SERV_PERSON, m_servPerson);
	DDX_Control(pDX, IDC_STATIC_EMP_DONE, m_static_emp_done);
	DDX_Control(pDX, IDC_EDIT_EMP_NAME, m_empName);
	DDX_Control(pDX, IDC_COMBO_BUSINESS_PROG, m_businessProg);
	DDX_Control(pDX, IDC_COMBO_PAY_DEAL_CHOOSE, m_payDealChoose);
	DDX_Control(pDX, IDC_COMBO_ORDER_DISCUSS_TYPE, m_orderDiscussType);
	DDX_Control(pDX, IDC_COMBO_EARLY_STAGE_TYPE, m_earlyStageType);
	DDX_Control(pDX, IDC_DATETIME_BOOK, m_date_book);
	DDX_Control(pDX, IDC_DATETIME_FINAL_SCHEME, m_date_final_scheme);
	DDX_Control(pDX, IDC_DATETIME_FIRST_TEST, m_date_firsttest);
	DDX_Control(pDX, IDC_DATETIME_INSTALL_BOOK, m_date_installBOOK);
	DDX_Control(pDX, IDC_DATETIME_ORDER_ACCEPT, m_date_orderaccept);
	DDX_Control(pDX, IDC_DATETIME_ORDER_FINISHED, m_order_orderfinished);
	DDX_Control(pDX, IDC_DATETIME_OUT_ORDER_BOOK, m_date_out_order_book);
	DDX_Text(pDX, IDC_EDIT_ACCESSORY_FILE_PATH, m_accessory_filepath);
	DDX_Text(pDX, IDC_EDIT_BASEINFO_PS, m_baseinfo_ps);
	DDX_Text(pDX, IDC_EDIT_CHARGE_NM, m_charge_name);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_NO, m_contract_NO);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_TOTAL_PRICE, m_contract_total_price);
	DDX_Text(pDX, IDC_EDIT_CTM_ADDR, m_ctm_addr);
	DDX_Text(pDX, IDC_EDIT_CTM_CALL, m_ctm_call);
	DDX_Text(pDX, IDC_EDIT_CTM_NAME, m_ctm_name);
	DDX_Text(pDX, IDC_EDIT_CTM_PHONE, m_ctm_phone);
	DDX_Text(pDX, IDC_EDIT_CTM_PS, m_ctm_ps);
	DDX_Text(pDX, IDC_EDIT_DEAL_DISCUSS_PERSON1, m_persion1_discuss);
	DDX_Text(pDX, IDC_EDIT_DEAL_DISCUSS_PERSON2, m_person2_discuss);
	DDX_Text(pDX, IDC_EDIT_EARLY_STAGE_MODIFY_PERSON, m_person_earlyStageModify);
	DDX_Text(pDX, IDC_EDIT_EXACT_MEASURE_PERSON, m_person_exact_measure);
	DDX_Text(pDX, IDC_EDIT_FCTORY_CHOOSE, m_factory_choose);
	DDX_Text(pDX, IDC_EDIT_INSTALL_PERSON, m_person_install);
	DDX_Text(pDX, IDC_EDIT_INSTALL_PROBLEMS, m_problems_install);
	DDX_Text(pDX, IDC_EDIT_MONEY_ALREADY_TAKE, m_money_already_take);
	DDX_Text(pDX, IDC_EDIT_ORDER_NO, m_order_NO);
	DDX_Text(pDX, IDC_EDIT_PERSON_BUDGET, m_person_budget);
	DDX_Text(pDX, IDC_EDIT_PERSON_OUT_ORDER, m_person_out_order);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_progressCtrl);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Text(pDX, IDC_EDIT_PERDON_FIRST_TEST, m_persion_first_test);
	DDX_Text(pDX, IDC_EDIT_PERSION_FIRST_DESIGN, m_persion_first_design);
	DDX_Control(pDX, IDC_STATIC_UPLOAD_STATUS, m_static_upload_status);
}


BEGIN_MESSAGE_MAP(COrderContent, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CTM_FROM, &COrderContent::OnCbnSelchangeComboCtmFrom)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CANCLE, &COrderContent::OnBnClickedBtnCancle)
	ON_BN_CLICKED(IDC_BTN_NEW, &COrderContent::OnBnClickedBtnNew)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_OrderContent, &COrderContent::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_OrderContent, &COrderContent::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_ACCESSORY_CHOOSE, &COrderContent::OnBnClickedBtnAccessoryChoose)
	ON_BN_CLICKED(IDC_BTN_ACCESSORY_CHECK, &COrderContent::OnBnClickedBtnAccessoryCheck)
	ON_MESSAGE(WM_STARTUPLOADFILE_OrderContent, &COrderContent::OnStartUploadFile)
	ON_MESSAGE(WM_CONFIRMUPLOADFILE_OrderContent, &COrderContent::OnConfirmUploadFile)
	ON_MESSAGE(WM_RESETUPLOAD_OrderContent, &COrderContent::OnResetUpload)
	ON_BN_CLICKED(IDC_BTN_FILE_UPLOAD, &COrderContent::OnBnClickedBtnFileUpload)
END_MESSAGE_MAP()


// COrderContent 消息处理程序

void COrderContent::InitCtmFromComb()
{
	m_ctmFrom.AddString(_T("老客户介绍"));
	m_ctmFrom.AddString(_T("老客户二次订单"));
	m_ctmFrom.AddString(_T("新客户"));
	m_ctmFrom.AddString(_T("装潢公司介绍"));
	m_ctmFrom.AddString(_T("业务员跑单"));

	m_ctmFrom.SetCurSel(0);
}

BOOL COrderContent::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitCtmFromComb();
	InitBusinessProgComb();
	InitEarlyStageTypeComb();
	InitTipCtrl();
	InitPayDealChooseComb();
	InitOrderDiscussTypeComb();
	m_date_book.SetFormat(_T("yyyy-MM-dd"));
	m_date_final_scheme.SetFormat(_T("yyyy-MM-dd"));
	m_date_firsttest.SetFormat(_T("yyyy-MM-dd"));
	m_date_installBOOK.SetFormat(_T("yyyy-MM-dd"));
	m_date_orderaccept.SetFormat(_T("yyyy-MM-dd"));
	m_order_orderfinished.SetFormat(_T("yyyy-MM-dd"));
	m_date_out_order_book.SetFormat(_T("yyyy-MM-dd"));
	return TRUE;
}

void COrderContent::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	m_static_upload_status.SetFont(&font);
	font.Detach();
}

void COrderContent::InitOrderDiscussTypeComb()
{
	m_orderDiscussType.AddString(_T("可选择独立谈单"));
	m_orderDiscussType.AddString(_T("两人合作谈单"));
	m_orderDiscussType.SetCurSel(0);
}
void COrderContent::InitPayDealChooseComb()
{
	m_payDealChoose.AddString(_T("已付"));
	m_payDealChoose.AddString(_T("未付"));
	m_payDealChoose.AddString(_T("累积"));
	m_payDealChoose.SetCurSel(0);
}
void COrderContent::InitEarlyStageTypeComb()
{
	m_earlyStageType.AddString(_T("内部修改"));
	m_earlyStageType.AddString(_T("客户沟通修改"));
	m_earlyStageType.SetCurSel(0);

}

void COrderContent::InitBusinessProgComb()
{
	m_businessProg.AddString(_T("意向客户"));
	m_businessProg.AddString(_T("已订单"));
	m_businessProg.AddString(_T("已初测"));
	m_businessProg.AddString(_T("已签合同"));
	m_businessProg.AddString(_T("已确定最终方案"));
	m_businessProg.AddString(_T("已放样精量"));
	m_businessProg.AddString(_T("已发单"));
	m_businessProg.AddString(_T("已回单"));
	m_businessProg.AddString(_T("已安装"));
	m_businessProg.AddString(_T("已完成"));
	m_businessProg.SetCurSel(0);
}
void COrderContent::OnCbnSelchangeComboCtmFrom()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSel = m_ctmFrom.GetCurSel();
	CString strTemp = _T("");
	m_ctmFrom.GetLBText(curSel,strTemp);

	if("新客户" == strTemp){
		m_staticServPersonNM.ShowWindow(SW_SHOW);
		m_servPerson.ShowWindow(SW_SHOW);

		m_staticIntrName.ShowWindow(SW_HIDE);
		m_intrName.ShowWindow(SW_HIDE);

		m_staticIntrPhone.ShowWindow(SW_HIDE);
		m_intrPhone.ShowWindow(SW_HIDE);

		m_static_emp_done.ShowWindow(SW_HIDE);
		m_empName.ShowWindow(SW_HIDE);
	}else if("老客户二次订单" == strTemp){
		m_staticServPersonNM.ShowWindow(SW_HIDE);
		m_servPerson.ShowWindow(SW_HIDE);

		m_staticIntrName.ShowWindow(SW_HIDE);
		m_intrName.ShowWindow(SW_HIDE);

		m_staticIntrPhone.ShowWindow(SW_HIDE);
		m_intrPhone.ShowWindow(SW_HIDE);

		m_static_emp_done.ShowWindow(SW_HIDE);
		m_empName.ShowWindow(SW_HIDE);
	}else if("老客户介绍" == strTemp){
		m_staticServPersonNM.ShowWindow(SW_HIDE);
		m_servPerson.ShowWindow(SW_HIDE);

		m_staticIntrName.ShowWindow(SW_SHOW);
		m_intrName.ShowWindow(SW_SHOW);

		m_staticIntrPhone.ShowWindow(SW_SHOW);
		m_intrPhone.ShowWindow(SW_SHOW);

		m_static_emp_done.ShowWindow(SW_HIDE);
		m_empName.ShowWindow(SW_HIDE);
	}else if("装潢公司介绍" == strTemp){
		m_staticServPersonNM.ShowWindow(SW_HIDE);
		m_servPerson.ShowWindow(SW_HIDE);

		m_staticIntrName.ShowWindow(SW_SHOW);
		m_intrName.ShowWindow(SW_SHOW);

		m_staticIntrPhone.ShowWindow(SW_SHOW);
		m_intrPhone.ShowWindow(SW_SHOW);

		m_static_emp_done.ShowWindow(SW_HIDE);
		m_empName.ShowWindow(SW_HIDE);
	}else if("业务员跑单" == strTemp){
		m_staticServPersonNM.ShowWindow(SW_HIDE);
		m_servPerson.ShowWindow(SW_HIDE);

		m_staticIntrName.ShowWindow(SW_HIDE);
		m_intrName.ShowWindow(SW_HIDE);

		m_staticIntrPhone.ShowWindow(SW_HIDE);
		m_intrPhone.ShowWindow(SW_HIDE);

		m_static_emp_done.ShowWindow(SW_SHOW);
		m_empName.ShowWindow(SW_SHOW);
	}else{
		return;	
	}
}


HBRUSH COrderContent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(RGB(185,211,255));	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b;
	}
	return hbr;
}


void COrderContent::OnBnClickedBtnCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CSock::m_bUpLoading){
		if(IDYES == MessageBox(_T("当前正在上传文件任务，确认取消吗"),_T("警告"),MB_YESNO))
		{
			CSock::m_bUpLoading = FALSE;
			CSock::m_pOrderContent = NULL;
			CDialogEx::OnCancel();
		}else{
			return;
		}
	}else{
		CSock::m_pOrderContent = NULL;
		CDialogEx::OnCancel();
	}
}


void COrderContent::OnBnClickedBtnNew()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	USES_CONVERSION;
	if(!IsItemValidity(m_order_NO,_T("订单编号"),TRUE,TRUE,FALSE,12,0))
	{
		return ;
	}
	
	memset(&m_orderInfo,0,sizeof(m_orderInfo));
	strcpy_s(m_orderInfo.NO_Order,T2A(m_order_NO));
	char *p = NULL;
	CTime timeTime;
	DWORD dwResult = m_date_orderaccept.GetTime(timeTime);   //接单日期
	CString str;
	if (dwResult == GDT_VALID)
	{
		if ((m_date_orderaccept.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_accept_order,p);
	}else
	{
		MessageBox(_T("请选择接单日期"));
		return;
	}
	dwResult = m_order_orderfinished.GetTime(timeTime);   //完成日期
	if (dwResult == GDT_VALID)
	{
		if ((m_order_orderfinished.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_finished_order,p);
	}else
	{
		MessageBox(_T("请选择完成日期"));
		return;
	}
	if(!IsItemValidity(m_charge_name,_T("负责人姓名"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.name_person_charge_order,T2A(m_charge_name));
	if(!IsItemValidity(m_baseinfo_ps,_T("基本信息备注"),FALSE,FALSE,FALSE,180,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.base_ps_order,T2A(m_baseinfo_ps));

	if(!IsItemValidity(m_persion_first_design,_T("初次设计人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion_first_design,T2A(m_persion_first_design));
	CString earlyStageType;
	m_earlyStageType.GetWindowText(earlyStageType);
	strcpy_s(m_orderInfo.type_early_modify,T2A(earlyStageType));


	if(!IsItemValidity(m_person_earlyStageModify,_T("前期修改人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion_early_modify,T2A(m_person_earlyStageModify));

	dwResult = m_date_final_scheme.GetTime(timeTime);   //方案定稿日期
	if (dwResult == GDT_VALID)
	{
		if ((m_date_final_scheme.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_final_scheme,p);
	}else
	{
		MessageBox(_T("请选择方案定稿日期"));
		return;
	}

	CString intrname;   //介绍人姓名
	m_intrName.GetWindowText(intrname);
	if(!IsItemValidity(intrname,_T("介绍人"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.name_intr,T2A(intrname));
	
	CString intrphone;
	m_intrPhone.GetWindowText(intrphone);
	if(!IsItemValidity(intrphone,_T("介绍人电话"),FALSE,TRUE,FALSE,11,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.phone_intr,T2A(intrphone));
	CString servPersion;
	m_servPerson.GetWindowText(servPersion);
	if(!IsItemValidity(servPersion,_T("接待人"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.name_serv_persion,T2A(servPersion));

	CString empname;
	m_empName.GetWindowText(empname);
	if(!IsItemValidity(empname,_T("业务员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.name_emp,T2A(empname));

	if(!IsItemValidity(m_ctm_call,_T("客户名称"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.call_ctm,T2A(m_ctm_call));

	if(!IsItemValidity(m_ctm_name,_T("顾客姓名"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.name_ctm,T2A(m_ctm_name));
	if(!IsItemValidity(m_ctm_phone,_T("顾客电话"),FALSE,TRUE,FALSE,11,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.phone_ctm,T2A(m_ctm_phone));

	if(!IsItemValidity(m_ctm_ps,_T("顾客备注"),FALSE,FALSE,FALSE,100,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.ps_ctm,T2A(m_ctm_ps));
	
	if(!IsItemValidity(m_ctm_addr,_T("顾客地址"),FALSE,FALSE,FALSE,50,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.addr_ctm,T2A(m_ctm_addr));

	if(!IsItemValidity(m_person_budget,_T("预算人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion_budget,T2A(m_person_budget));
	
	CString temp;
	m_orderDiscussType.GetWindowText(temp);
	strcpy_s(m_orderInfo.type_discuss_order,T2A(temp));
	
	if(!IsItemValidity(m_persion1_discuss,_T("谈单人员1"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion1_discuss_order,T2A(m_persion1_discuss));
	
	if(!IsItemValidity(m_person2_discuss,_T("谈单人员2"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion2_discuss_order,T2A(m_person2_discuss));

	if(!IsItemValidity(m_contract_NO,_T("合同单号"),FALSE,TRUE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.NO_contract,T2A(m_contract_NO));

	m_businessProg.GetWindowText(temp);
	strcpy_s(m_orderInfo.prog_business,T2A(temp));
	temp.Format(_T("%.2f"),m_contract_total_price);
	strcpy_s(m_orderInfo.price_total_contract,T2A(temp));
	temp.Format(_T("%.2f"),m_money_already_take);
	strcpy_s(m_orderInfo.money_already_take,T2A(temp));

	dwResult = m_date_firsttest.GetTime(timeTime);   //初测预约时间
	if (dwResult == GDT_VALID)
	{
		if ((m_date_firsttest.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_first_test_book,p);
	}else
	{
		MessageBox(_T("请选择初测预约时间"));
		return;
	}
	
	if(!IsItemValidity(m_persion_first_test,_T("初测人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion_first_test,T2A(m_persion_first_test));
	

	dwResult = m_date_out_order_book.GetTime(timeTime);   //初测预约时间
	if (dwResult == GDT_VALID)
	{
		if ((m_date_out_order_book.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_out_order_book,p);
	}else
	{
		MessageBox(_T("请选择发单预约时间"));
		return;
	}
	
	if(!IsItemValidity(m_person_out_order,_T("发单人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.person_out_order,T2A(m_person_out_order));

	if(!IsItemValidity(m_factory_choose,_T("单位"),FALSE,FALSE,FALSE,30,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.factory_choose,T2A(m_factory_choose));
	
	m_payDealChoose.GetWindowText(temp);
	strcpy_s(m_orderInfo.pay_deal_choice,T2A(temp));

	dwResult = m_date_book.GetTime(timeTime);   //放样预约时间
	if (dwResult == GDT_VALID)
	{
		if ((m_date_book.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_book,p);
	}else
	{
		MessageBox(_T("请选择放样预约时间"));
		return;
	}

	if(!IsItemValidity(m_person_exact_measure,_T("放样精量人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.person_exact_measure,T2A(m_person_exact_measure));
	

	dwResult = m_date_installBOOK.GetTime(timeTime);   //放样预约时间
	if (dwResult == GDT_VALID)
	{
		if ((m_date_installBOOK.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo.date_install_book,p);
	}else
	{
		MessageBox(_T("请选择安装预约时间"));
		return;
	}
	
	if(!IsItemValidity(m_person_install,_T("安装人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.persion_install,T2A(m_person_install));
	if(!IsItemValidity(m_problems_install,_T("安装问题"),FALSE,FALSE,FALSE,100,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo.problems_install,T2A(m_problems_install));
	
	CString ctmFrom;
	m_ctmFrom.GetWindowText(ctmFrom);
	strcpy_s(m_orderInfo.ctm_from,T2A(ctmFrom));
	CheckCtmFrom();
	CSock::StartReqOrderNew(this);
}

BOOL COrderContent::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot)
{
	if(bCheckEmpty)    //TRUE  代表需要检查是否为空 不允许为空
	{
		if(_source.IsEmpty())   //
		{
			CString err = _item_name+_T("不能为空");
			MessageBox(err);
			return FALSE;
		}
	}
	if(!_source.IsEmpty())
	{
		if(bCheckNum)    //需要检查是否为数字
		{
			if(!IsNum(_source,bCheckDot))
			{
				CString err = _item_name+_T("必须为数字");
				MessageBox(err);
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
			MessageBox(err);
			return FALSE;
		}
		if(_source.GetLength() < data_min_len)  //检查数据长度
		{
			CString err;
			err.Format(_T("%s数据长度不能少于%d"),_item_name,data_min_len);
			MessageBox(err);
			return FALSE;
		}
		if(bCheckDateFormat)   //检查是否符合日期格式
		{
			CString _err2 = _item_name+_T("不符合日期格式，标准格式如：2014-10-05");
			CString str = _source;
			int  index = _source.Find('-');
			if(index == -1)   //没找到
			{
				MessageBox(_err2);
				return FALSE;
			}else{
				CString left = str.Mid(0,index);
				str = str.Mid(index+1);
				if(left.GetLength()!=4)
				{
					MessageBox(_err2);
					return FALSE;
				}else{
					if(!IsNum(left,FALSE))
					{
						MessageBox(_err2);
						return FALSE;
					}else{    //前面还算合法的
						index = str.Find('-');
						if(index == -1)   //没找到
						{
							MessageBox(_err2);
							return FALSE;
						}else{
							left = str.Mid(0,index);
							str = str.Mid(index+1);
							if(IsNum(left,FALSE)&&IsNum(str,FALSE))
							{
								return TRUE;
							}else{
								MessageBox(_err2);
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

BOOL COrderContent::ContainsCharsRemain(CString str)     //检查字符串是否包含子字符串
{
	CString temp1 = _T("[#");
	CString temp2 = _T("#]");
	CString temp3 = _T("{#");
	CString temp4 = _T("#}");
	int flag = 0;
	flag = str.Find(temp1);
	if(flag != -1)
	{
		MessageBox(_T("数据中不能包含‘[#’字符"));
		return TRUE;
	}
	flag = str.Find(temp2);
	if(flag != -1)
	{
		MessageBox(_T("数据中不能包含‘#]’字符"));
		return TRUE;
	}
	flag = str.Find(temp3);
	if(flag != -1)
	{
		MessageBox(_T("数据中不能包含‘#}’字符"));
		return TRUE;
	}
	flag = str.Find(temp4);
	if(flag != -1)
	{
		MessageBox(_T("数据中不能包含‘{#’字符"));
		return TRUE;
	}
	return FALSE;
}

BOOL COrderContent::IsNum(CString str,BOOL bCheckDot)
{
	WT;
	char *p = T2A(str);
	int i = 0;
	int count = str.GetLength();
	while(count){
		if(p[i]>'9'||p[i]<'0')
		{
			if(bCheckDot)
			{
				if(p[i] != '.')
				{
					return FALSE;
				}
			}else
			{
				return FALSE;
			}
		}
		count--;
		i++;
	}
	return TRUE;
}



void COrderContent::CheckCtmFrom()
{
	UpdateData(TRUE);
	CString strTemp;
	CString temp;
	m_ctmFrom.GetWindowText(strTemp);
	if("新客户" == strTemp){
		memset(m_orderInfo.phone_intr,0,sizeof(m_orderInfo.phone_intr));
		memset(m_orderInfo.name_emp,0,sizeof(m_orderInfo.name_emp));
		memset(m_orderInfo.name_intr,0,sizeof(m_orderInfo.name_intr));
	}else if("老客户二次订单" == strTemp){
		memset(m_orderInfo.phone_intr,0,sizeof(m_orderInfo.phone_intr));
		memset(m_orderInfo.name_emp,0,sizeof(m_orderInfo.name_emp));
		memset(m_orderInfo.name_intr,0,sizeof(m_orderInfo.name_intr));
		memset(m_orderInfo.name_serv_persion,0,sizeof(m_orderInfo.name_serv_persion));
	}else if("老客户介绍" == strTemp){
		memset(m_orderInfo.name_emp,0,sizeof(m_orderInfo.name_emp));
		memset(m_orderInfo.name_serv_persion,0,sizeof(m_orderInfo.name_serv_persion));
	}else if("装潢公司介绍" == strTemp){
		memset(m_orderInfo.name_emp,0,sizeof(m_orderInfo.name_emp));
		memset(m_orderInfo.name_serv_persion,0,sizeof(m_orderInfo.name_serv_persion));
	}else if("业务员跑单" == strTemp){
		memset(m_orderInfo.name_emp,0,sizeof(m_orderInfo.name_emp));
		
	}else{
		return;	
	}
}

void COrderContent::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			ShowTip();
			ShowUploadStatus();
			break;
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
void COrderContent::ResetCurFileUpload()
{
	m_ulCurTotal = 0;
	m_progressCtrl.SetPos(0);
	m_static_upload_status.SetWindowText(_T(""));
	m_progressCtrl.ShowWindow(SW_HIDE);
}
void COrderContent::ShowUploadStatus()
{
	if (m_ulCurTotal > 0)//当前下载文件的字节数
	{
		int nPercent = (float)m_ulCurTotal / m_ulFileSize * 100;//百分比
		m_progressCtrl.SetPos(nPercent);							//设置进度条位置
		
		m_progressCtrl.ShowWindow(SW_SHOW);						//显示进度条
		
		//显示下载进度信息
		CString strProgInfor;
		strProgInfor.Format(_T("正在上传 %d%s"), nPercent,_T("%"));
		m_static_upload_status.SetWindowText(strProgInfor);
	}
}

void COrderContent::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
}

void COrderContent::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(CSock::m_bUpLoading){
		if(IDYES == MessageBox(_T("当前正在上传文件任务，确认关闭吗"),_T("警告"),MB_YESNO))
		{
			CSock::m_bUpLoading = FALSE;
			CSock::m_pOrderContent = NULL;
			CDialogEx::OnClose();
		}else{
			return;
		}
	}else{
		CSock::m_pOrderContent = NULL;
		CDialogEx::OnClose();
	}
}


afx_msg LRESULT COrderContent::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT COrderContent::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	(CButton*)GetDlgItem(IDC_BTN_NEW)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_BTN_ACCESSORY_CHECK)->EnableWindow(TRUE);
	return 0;
}


void COrderContent::OnBnClickedBtnAccessoryChoose()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle = _T("选择附件进行上传");
	fileDlg.m_ofn.lpstrFilter = _T("All Files(*.*)\0*.*\0\0");
	if(IDOK == fileDlg.DoModal())
	{
		CString pathName = fileDlg.GetPathName();
		CString fileExt = fileDlg.GetFileExt();
		m_accessory_filepath = pathName;
		m_filename = fileDlg.GetFileName();
		UpdateData(FALSE);
	}else{
		return;
	}
}


void COrderContent::OnBnClickedBtnAccessoryCheck()    //开始上传附件
{
	UpdateData(TRUE);
	USES_CONVERSION;
	if(m_order_NO.IsEmpty())
	{
		MessageBox(_T("订单编号不能为空"));
		return;
	}else if(m_order_NO.GetLength()>12){
		MessageBox(_T("订单编号长度不能超过12个字符"));
		return;
	}else{
		int len = m_order_NO.GetLength();
		char *p = T2A(m_order_NO);
		for(int i=0;i<len;i++)
		{
			if(p[i] < '0'||p[i] >'9')
			{
				MessageBox(_T("订单编号必须为数字"));
				m_order_NO.Empty();
				UpdateData(FALSE);
				return;
			}
		}
	}
	if(m_accessory_filepath.IsEmpty())
	{
		MessageBox(_T("请先选择要上传的附件"));
		return ;
	}else if(m_accessory_filepath.GetLength()>MAX_PATH){
		MessageBox(_T("附件路径不能超过260个字符，请重新选择"));
		return ;
	}
	CFileFind finder;
	BOOL isFind = finder.FindFile(m_accessory_filepath);
	if(isFind){ 
		m_ulFileSize = GetFileSize();
	}else{
		MessageBox(_T("要上传附件不存在，或许已经被删除"));
		return;
	}
	CString filesize = NULL;
	filesize.Format(_T("%u"),m_ulFileSize);
	memset(&m_orderInfo,0,sizeof(m_orderInfo));
	strcpy_s(m_orderInfo.NO_Order,T2A(m_order_NO));
	strcpy_s(m_orderInfo.filepath_accessory,T2A(m_filename));
	strcpy_s(m_orderInfo.filesize,T2A(filesize));
	(CButton*)GetDlgItem(IDC_BTN_NEW)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_BTN_ACCESSORY_CHECK)->EnableWindow(FALSE);
	
	CSock::StartCheckFileExist(this);
}

u_long COrderContent::GetFileSize()
{
	USES_CONVERSION;
	CString filepath = m_accessory_filepath;
	char* p = T2A(filepath);
	long retFileLen;//文件长度
	FILE *stream;	//文件指针
	int err = fopen_s(&stream,p,"rb");
	if(err != 0)//打开文件失败
	{
		return 0;
	}
	fseek( stream, 0, SEEK_END);	//指针移动到文件尾
	retFileLen = ftell( stream );	//获取文件长度
	fclose( stream );				//关闭文件
	return retFileLen;
}

afx_msg LRESULT COrderContent::OnStartUploadFile(WPARAM wParam, LPARAM lParam)
{
	if(IDYES == MessageBox(_T("该订单暂时没有附件，可以上传，继续吗？"),_T("提示"),MB_YESNO))
	{
		CSock::StartUpLoadFile(this);
	}else{
		(CButton*)GetDlgItem(IDC_BTN_NEW)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_ACCESSORY_CHECK)->EnableWindow(TRUE);
		m_tip.SetWindowText(_T("就绪"));
		m_static_upload_status.SetWindowText(_T("该订单还没有附件"));
	}
	return 0;
}


afx_msg LRESULT COrderContent::OnConfirmUploadFile(WPARAM wParam, LPARAM lParam)
{
	if(IDYES == MessageBox(_T("该订单有一个附件，确认上传将覆盖原有附件，继续吗？"),_T("警告"),MB_YESNO))
	{
		CSock::StartUpLoadFile(this);
	}else{
		(CButton*)GetDlgItem(IDC_BTN_NEW)->EnableWindow(TRUE);
		(CButton*)GetDlgItem(IDC_BTN_ACCESSORY_CHECK)->EnableWindow(TRUE);
		m_tip.SetWindowText(_T("就绪"));
		m_static_upload_status.SetWindowText(_T("该订单已有附件"));
	}
	return 0;
}


afx_msg LRESULT COrderContent::OnResetUpload(WPARAM wParam, LPARAM lParam)
{
	ResetCurFileUpload();
	return 0;
}


void COrderContent::OnBnClickedBtnFileUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	USES_CONVERSION;
	if(m_order_NO.IsEmpty())
	{
		MessageBox(_T("订单编号不能为空"));
		return;
	}else if(m_order_NO.GetLength()>12){
		MessageBox(_T("订单编号长度不能超过12个字符"));
		return;
	}else{
		int len = m_order_NO.GetLength();
		char *p = T2A(m_order_NO);
		for(int i=0;i<len;i++)
		{
			if(p[i] < '0'||p[i] >'9')
			{
				MessageBox(_T("订单编号必须为数字"));
				m_order_NO.Empty();
				UpdateData(FALSE);
				return;
			}
		}
	}
	if(m_accessory_filepath.IsEmpty())
	{
		MessageBox(_T("请先选择要上传的附件"));
		return ;
	}else if(m_accessory_filepath.GetLength()>MAX_PATH){
		MessageBox(_T("附件路径不能超过260个字符，请重新选择"));
		return ;
	}
	CFileFind finder;
	BOOL isFind = finder.FindFile(m_accessory_filepath);
	if(isFind){ 
		m_ulFileSize = GetFileSize();
	}else{
		MessageBox(_T("要上传附件不存在，或许已经被删除"));
		return;
	}
	memset(&m_fileULInfo,0,sizeof(m_fileULInfo));
	strcpy_s(m_fileULInfo.order_NO,T2A(m_order_NO));
	strcpy_s(m_fileULInfo.filename,T2A(m_filename));
	m_fileULInfo.filesize = m_ulFileSize;
	(CButton*)GetDlgItem(IDC_BTN_NEW)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_BTN_ACCESSORY_CHECK)->EnableWindow(FALSE);

	CSock::StartUpLoadFile(this);
}
