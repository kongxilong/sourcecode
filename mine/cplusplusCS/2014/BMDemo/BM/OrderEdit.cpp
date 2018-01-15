// OrderEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "OrderEdit.h"
#include "afxdialogex.h"


// COrderEdit 对话框

IMPLEMENT_DYNAMIC(COrderEdit, CDialogEx)

COrderEdit::COrderEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrderEdit::IDD, pParent)
	, m_ps_base_info(_T(""))
	, m_person_charge(_T(""))
	, m_NO_contract(_T(""))
	, m_contract_total_price(0)
	, m_ctm_addr(_T(""))
	, m_ctm_call(_T(""))
	, m_ctm_name(_T(""))
	, m_ctm_phone(_T(""))
	, m_ctm_ps(_T(""))
	, m_person1_discuss_deal(_T(""))
	, m_person2_discuss_deal(_T(""))
	, m_person_early_stage_modify(_T(""))
	, m_person_exact_measure(_T(""))
	, m_fct_choose(_T(""))
	, m_install_person(_T(""))
	, m_problems_install(_T(""))
	, m_NO_Order(_T(""))
	, m_person_budget(_T(""))
	, m_person_first_design(_T(""))
	, m_person_first_test(_T(""))
	, m_person_out_order(_T(""))
	, m_money_already_take(0)
{

}

COrderEdit::~COrderEdit()
{
}

void COrderEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUSINESS_PROG, m_prog_business);
	DDX_Control(pDX, IDC_COMBO_CTM_FROM, m_ctm_from);
	DDX_Control(pDX, IDC_COMBO_EARLY_STAGE_TYPE, m_early_stage_type);
	DDX_Control(pDX, IDC_COMBO_ORDER_DISCUSS_TYPE, m_order_discuss_type);
	DDX_Control(pDX, IDC_COMBO_PAY_DEAL_CHOOSE, m_pay_deal_choose);
	DDX_Control(pDX, IDC_DATETIME_BOOK, m_date_book);
	DDX_Control(pDX, IDC_DATETIME_FINAL_SCHEME, m_date_final_scheme);
	DDX_Control(pDX, IDC_DATETIME_FIRST_TEST, m_date_first_test);
	DDX_Control(pDX, IDC_DATETIME_INSTALL_BOOK, m_date_install_book);
	DDX_Control(pDX, IDC_DATETIME_ORDER_ACCEPT, m_date_order_accept);
	DDX_Control(pDX, IDC_DATETIME_ORDER_FINISHED, m_date_order_finished);
	DDX_Text(pDX, IDC_EDIT_BASEINFO_PS, m_ps_base_info);
	DDX_Control(pDX, IDC_DATETIME_OUT_ORDER_BOOK, m_date_out_order_book);
	DDX_Text(pDX, IDC_EDIT_CHARGE_NM, m_person_charge);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_NO, m_NO_contract);
	//  DDX_Text(pDX, IDC_EDIT_CONTRACT_TOTAL_PRICE, m_contract_total_price);
	DDX_Text(pDX, IDC_EDIT_CONTRACT_TOTAL_PRICE, m_contract_total_price);
	DDX_Text(pDX, IDC_EDIT_CTM_ADDR, m_ctm_addr);
	DDX_Text(pDX, IDC_EDIT_CTM_CALL, m_ctm_call);
	DDX_Text(pDX, IDC_EDIT_CTM_NAME, m_ctm_name);
	DDX_Text(pDX, IDC_EDIT_CTM_PHONE, m_ctm_phone);
	DDX_Text(pDX, IDC_EDIT_CTM_PS, m_ctm_ps);
	DDX_Text(pDX, IDC_EDIT_DEAL_DISCUSS_PERSON1, m_person1_discuss_deal);
	DDX_Text(pDX, IDC_EDIT_DEAL_DISCUSS_PERSON2, m_person2_discuss_deal);
	DDX_Text(pDX, IDC_EDIT_EARLY_STAGE_MODIFY_PERSON, m_person_early_stage_modify);
	//  DDX_Text(pDX, IDC_EDIT_EMP_NAME, m_emp_name);
	DDX_Text(pDX, IDC_EDIT_EXACT_MEASURE_PERSON, m_person_exact_measure);
	DDX_Text(pDX, IDC_EDIT_FCTORY_CHOOSE, m_fct_choose);
	DDX_Text(pDX, IDC_EDIT_INSTALL_PERSON, m_install_person);
	DDX_Text(pDX, IDC_EDIT_INSTALL_PROBLEMS, m_problems_install);
	DDX_Control(pDX, IDC_EDIT_INTR_NM, m_intr_name);
	DDX_Control(pDX, IDC_EDIT_INTR_PHONE, m_intr_phone);
	DDX_Text(pDX, IDC_EDIT_ORDER_NO, m_NO_Order);
	DDX_Text(pDX, IDC_EDIT_PERSON_BUDGET, m_person_budget);
	DDX_Text(pDX, IDC_EDIT_PERSON_FIRST_DESIGN, m_person_first_design);
	DDX_Text(pDX, IDC_EDIT_PERSON_FIRST_TEST, m_person_first_test);
	DDX_Text(pDX, IDC_EDIT_PERSON_OUT_ORDER, m_person_out_order);
	DDX_Control(pDX, IDC_EDIT_SERV_PERSON, m_person_serv);
	DDX_Control(pDX, IDC_STATIC_EMP_DONE, m_emp_done_static);
	DDX_Control(pDX, IDC_STATIC_INTR_NAME, m_intr_name_static);
	DDX_Control(pDX, IDC_STATIC_INTR_PHONE, m_intr_phone_static);
	DDX_Control(pDX, IDC_STATIC_SERV_PERSON, m_serv_person_static);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Control(pDX, IDC_EDIT_EMP_NAME, m_emp_done);
	DDX_Text(pDX, IDC_EDIT_MONEY_ALREADY_TAKE, m_money_already_take);
}


BEGIN_MESSAGE_MAP(COrderEdit, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_OrderEdit, &COrderEdit::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_OrderEdit, &COrderEdit::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_GIVEUP, &COrderEdit::OnBnClickedBtnGiveup)
	ON_CBN_SELCHANGE(IDC_COMBO_CTM_FROM, &COrderEdit::OnCbnSelchangeComboCtmFrom)
	ON_BN_CLICKED(IDC_BTN_SAVE, &COrderEdit::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// COrderEdit 消息处理程序

BOOL COrderEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitAllCtrls();
	return TRUE;
}


void  COrderEdit::InitAllCtrls()
{
	WT;
	InitTipCtrl();
	InitCtmFromComb();
	InitBusinessProgComb();
	InitPayDealChooseComb();
	InitOrderDiscussTypeComb();
	InitEarlyStageTypeComb();
	LoadDate(A2T(m_orderInfo_edit.date_book),m_date_book);
	LoadDate(A2T(m_orderInfo_edit.date_final_scheme),m_date_final_scheme);
	LoadDate(A2T(m_orderInfo_edit.date_first_test_book),m_date_first_test);
	LoadDate(A2T(m_orderInfo_edit.date_install_book),m_date_install_book);
	LoadDate(A2T(m_orderInfo_edit.date_accept_order),m_date_order_accept);
	LoadDate(A2T(m_orderInfo_edit.date_finished_order),m_date_order_accept);
	LoadDate(A2T(m_orderInfo_edit.date_out_order_book),m_date_out_order_book);
	
	InitEditCtrl();
}

void COrderEdit::InitEditCtrl()
{
	WT;
	m_ps_base_info = A2T(m_orderInfo_edit.base_ps_order);
	m_person_charge = A2T(m_orderInfo_edit.name_person_charge_order);
	m_NO_contract = A2T(m_orderInfo_edit.NO_contract);
//	CString m_contract_total_price;
	m_money_already_take = atof(m_orderInfo_edit.money_already_take);
	m_contract_total_price = atof(m_orderInfo_edit.price_total_contract);
	m_ctm_addr = A2T(m_orderInfo_edit.addr_ctm);
	m_ctm_call = A2T(m_orderInfo_edit.call_ctm);
	m_ctm_name = A2T(m_orderInfo_edit.name_ctm);
	m_ctm_phone = A2T(m_orderInfo_edit.phone_ctm);
	m_ctm_ps = A2T(m_orderInfo_edit.ps_ctm);
	m_person1_discuss_deal = A2T(m_orderInfo_edit.persion1_discuss_order);
	m_person2_discuss_deal = A2T(m_orderInfo_edit.persion2_discuss_order);
	m_person_early_stage_modify = A2T(m_orderInfo_edit.persion_early_modify);

	m_person_exact_measure = A2T(m_orderInfo_edit.person_exact_measure);
	m_fct_choose = A2T(m_orderInfo_edit.factory_choose);
	m_install_person = A2T(m_orderInfo_edit.persion_install);
	m_problems_install = A2T(m_orderInfo_edit.problems_install);
	m_intr_name.SetWindowText(A2T(m_orderInfo_edit.name_intr));
	m_intr_phone.SetWindowText(A2T(m_orderInfo_edit.phone_intr));
	m_NO_Order = A2T(m_orderInfo_edit.NO_Order);
	m_person_budget =  A2T(m_orderInfo_edit.persion_budget);
	m_person_first_design = A2T(m_orderInfo_edit.persion_first_design);
	m_person_first_test = A2T(m_orderInfo_edit.persion_first_test);
	m_person_out_order = A2T(m_orderInfo_edit.person_out_order);
	m_person_serv.SetWindowText(A2T(m_orderInfo_edit.name_serv_persion));
	m_emp_done.SetWindowText(A2T(m_orderInfo_edit.name_emp));
	UpdateData(FALSE);
}

void COrderEdit::LoadDate(CString _strTime,CDateTimeCtrl &date)
{
	date.SetFormat(_T("yyyy-MM-dd"));
	CString year;
	CString month;
	CString day;
	CString temp = _strTime;
	USES_CONVERSION;
	int index = temp.Find(_T("-"));
	if(index != -1){
		year = temp.Mid(0,index);
		temp = temp.Mid(index+1);
		index = temp.Find(_T("-"));
		if(index != -1){
			month = temp.Mid(0,index);
			day = temp.Mid(index+1);
			CTime  time(atoi(T2A(year)),atoi(T2A(month)),atoi(T2A(day)),0,0,0);
			VERIFY(date.SetTime(&time));
		}else{
			return;
		}
	}else{
		return;
	}
}


void COrderEdit::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}
void COrderEdit::InitCtmFromComb()
{
	WT;
	m_ctm_from.AddString(_T("老客户介绍"));
	m_ctm_from.AddString(_T("老客户二次订单"));
	m_ctm_from.AddString(_T("新客户"));
	m_ctm_from.AddString(_T("装潢公司介绍"));
	m_ctm_from.AddString(_T("业务员跑单"));

	int index = m_ctm_from.FindStringExact(0,A2T(m_orderInfo_edit.ctm_from));
	if(index != CB_ERR)
	{
		m_ctm_from.SetCurSel(index);
	}else{
		m_ctm_from.SetCurSel(0);
	}
	

	CString  strTemp = A2T(m_orderInfo_edit.ctm_from);
	if("新客户" == strTemp){
		m_serv_person_static.ShowWindow(SW_SHOW);
		m_person_serv.ShowWindow(SW_SHOW);

		m_intr_name_static.ShowWindow(SW_HIDE);
		m_intr_name.ShowWindow(SW_HIDE);

		m_intr_phone_static.ShowWindow(SW_HIDE);
		m_intr_phone.ShowWindow(SW_HIDE);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("老客户二次订单" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_HIDE);
		m_intr_name.ShowWindow(SW_HIDE);

		m_intr_phone_static.ShowWindow(SW_HIDE);
		m_intr_phone.ShowWindow(SW_HIDE);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("老客户介绍" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_SHOW);
		m_intr_name.ShowWindow(SW_SHOW);

		m_intr_phone_static.ShowWindow(SW_SHOW);
		m_intr_phone.ShowWindow(SW_SHOW);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("装潢公司介绍" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_SHOW);
		m_intr_name.ShowWindow(SW_SHOW);

		m_intr_phone_static.ShowWindow(SW_SHOW);
		m_intr_phone.ShowWindow(SW_SHOW);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("业务员跑单" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_HIDE);
		m_intr_name.ShowWindow(SW_HIDE);

		m_intr_phone_static.ShowWindow(SW_HIDE);
		m_intr_phone.ShowWindow(SW_HIDE);

		m_emp_done_static.ShowWindow(SW_SHOW);
		m_emp_done.ShowWindow(SW_SHOW);
	}else{
		return;	
	}
}
void COrderEdit::InitBusinessProgComb()
{
	WT;
	m_prog_business.AddString(_T("意向客户"));
	m_prog_business.AddString(_T("已订单"));
	m_prog_business.AddString(_T("已初测"));
	m_prog_business.AddString(_T("已订单"));
	m_prog_business.AddString(_T("已签合同"));
	m_prog_business.AddString(_T("已确定最终方案"));
	m_prog_business.AddString(_T("已放样精量"));
	m_prog_business.AddString(_T("已发单"));
	m_prog_business.AddString(_T("已回单"));
	m_prog_business.AddString(_T("已安装"));
	m_prog_business.AddString(_T("已完成"));

	int index = m_prog_business.FindStringExact(0,A2T(m_orderInfo_edit.prog_business));
	if(index != CB_ERR)
	{
		m_prog_business.SetCurSel(index);
	}else{
		m_prog_business.SetCurSel(0);
	}
	
}
void COrderEdit::InitPayDealChooseComb()
{
	WT;
	m_pay_deal_choose.AddString(_T("已付"));
	m_pay_deal_choose.AddString(_T("未付"));
	m_pay_deal_choose.AddString(_T("累积"));

	int index = m_pay_deal_choose.FindStringExact(0,A2T(m_orderInfo_edit.pay_deal_choice));
	if(index != CB_ERR)
	{
		m_pay_deal_choose.SetCurSel(index);
	}else{
		m_pay_deal_choose.SetCurSel(0);
	}
	
}
void COrderEdit::InitOrderDiscussTypeComb()
{
	WT;
	m_order_discuss_type.AddString(_T("可选择独立谈单"));
	m_order_discuss_type.AddString(_T("两人合作谈单"));
	
	int index = m_order_discuss_type.FindStringExact(0,A2T(m_orderInfo_edit.type_discuss_order));
	if(index != CB_ERR)
	{
		m_order_discuss_type.SetCurSel(index);
	}else{
		m_order_discuss_type.SetCurSel(0);
	}
}
void COrderEdit::InitEarlyStageTypeComb()
{
	WT;
	m_early_stage_type.AddString(_T("内部修改"));
	m_early_stage_type.AddString(_T("客户沟通修改"));
	int index = m_early_stage_type.FindStringExact(0,A2T(m_orderInfo_edit.type_early_modify));
	if(index != CB_ERR)
	{
		m_early_stage_type.SetCurSel(index);
	}else{
		m_early_stage_type.SetCurSel(0);
	}
}

HBRUSH COrderEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void COrderEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			ShowTip();
			break;
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void COrderEdit::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowTextW(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowTextW(_T("请求超时，请重试或者重新连接服务器"));
	}
}

void COrderEdit::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pOrderEdit = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT COrderEdit::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT COrderEdit::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


void COrderEdit::OnBnClickedBtnGiveup()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::m_pOrderEdit = NULL;
	CDialogEx::OnCancel();
}


void COrderEdit::OnCbnSelchangeComboCtmFrom()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSel = m_ctm_from.GetCurSel();
	CString strTemp = _T("");
	m_ctm_from.GetLBText(curSel,strTemp);

	if("新客户" == strTemp){
		m_serv_person_static.ShowWindow(SW_SHOW);
		m_person_serv.ShowWindow(SW_SHOW);

		m_intr_name_static.ShowWindow(SW_HIDE);
		m_intr_name.ShowWindow(SW_HIDE);

		m_intr_phone_static.ShowWindow(SW_HIDE);
		m_intr_phone.ShowWindow(SW_HIDE);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("老客户二次订单" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_HIDE);
		m_intr_name.ShowWindow(SW_HIDE);

		m_intr_phone_static.ShowWindow(SW_HIDE);
		m_intr_phone.ShowWindow(SW_HIDE);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("老客户介绍" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_SHOW);
		m_intr_name.ShowWindow(SW_SHOW);

		m_intr_phone_static.ShowWindow(SW_SHOW);
		m_intr_phone.ShowWindow(SW_SHOW);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("装潢公司介绍" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_SHOW);
		m_intr_name.ShowWindow(SW_SHOW);

		m_intr_phone_static.ShowWindow(SW_SHOW);
		m_intr_phone.ShowWindow(SW_SHOW);

		m_emp_done_static.ShowWindow(SW_HIDE);
		m_emp_done.ShowWindow(SW_HIDE);
	}else if("业务员跑单" == strTemp){
		m_serv_person_static.ShowWindow(SW_HIDE);
		m_person_serv.ShowWindow(SW_HIDE);

		m_intr_name_static.ShowWindow(SW_HIDE);
		m_intr_name.ShowWindow(SW_HIDE);

		m_intr_phone_static.ShowWindow(SW_HIDE);
		m_intr_phone.ShowWindow(SW_HIDE);

		m_emp_done_static.ShowWindow(SW_SHOW);
		m_emp_done.ShowWindow(SW_SHOW);
	}else{
		return;	
	}
}


void COrderEdit::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	USES_CONVERSION;
	if(!IsItemValidity(m_NO_Order,_T("订单编号"),TRUE,TRUE,FALSE,12,0))
	{
		return ;
	}
	
	memset(&m_orderInfo_edit,0,sizeof(m_orderInfo_edit));
	strcpy_s(m_orderInfo_edit.NO_Order,T2A(m_NO_Order));
	char *p = NULL;
	CTime timeTime;
	DWORD dwResult = m_date_order_accept.GetTime(timeTime);   //接单日期
	CString str;
	if (dwResult == GDT_VALID)
	{
		if ((m_date_order_accept.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo_edit.date_accept_order,p);
	}else
	{
		MessageBox(_T("请选择接单日期"));
		return;
	}
	dwResult = m_date_order_finished.GetTime(timeTime);   //完成日期
	if (dwResult == GDT_VALID)
	{
		if ((m_date_order_finished.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo_edit.date_finished_order,p);
	}else
	{
		MessageBox(_T("请选择完成日期"));
		return;
	}
	if(!IsItemValidity(m_person_charge,_T("负责人姓名"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.name_person_charge_order,T2A(m_person_charge));
	if(!IsItemValidity(m_ps_base_info,_T("基本信息备注"),FALSE,FALSE,FALSE,180,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.base_ps_order,T2A(m_ps_base_info));

	if(!IsItemValidity(m_person_first_design,_T("初次设计人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion_first_design,T2A(m_person_first_design));
	CString earlyStageType;
	m_early_stage_type.GetWindowText(earlyStageType);
	strcpy_s(m_orderInfo_edit.type_early_modify,T2A(earlyStageType));


	if(!IsItemValidity(m_person_early_stage_modify,_T("前期修改人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion_early_modify,T2A(m_person_early_stage_modify));

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
		strcpy_s(m_orderInfo_edit.date_final_scheme,p);
	}else
	{
		MessageBox(_T("请选择方案定稿日期"));
		return;
	}

	CString intrname;   //介绍人姓名
	m_intr_name.GetWindowText(intrname);
	if(!IsItemValidity(intrname,_T("介绍人"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.name_intr,T2A(intrname));
	
	CString intrphone;
	m_intr_phone.GetWindowText(intrphone);
	if(!IsItemValidity(intrphone,_T("介绍人电话"),FALSE,TRUE,FALSE,11,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.phone_intr,T2A(intrphone));
	CString servPersion;
	m_person_serv.GetWindowText(servPersion);
	if(!IsItemValidity(servPersion,_T("接待人"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.name_serv_persion,T2A(servPersion));

	CString empname;
	m_emp_done.GetWindowText(empname);
	if(!IsItemValidity(empname,_T("业务员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.name_emp,T2A(empname));

	if(!IsItemValidity(m_ctm_call,_T("客户名称"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.call_ctm,T2A(m_ctm_call));

	if(!IsItemValidity(m_ctm_name,_T("顾客姓名"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.name_ctm,T2A(m_ctm_name));
	if(!IsItemValidity(m_ctm_phone,_T("顾客电话"),FALSE,TRUE,FALSE,11,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.phone_ctm,T2A(m_ctm_phone));

	if(!IsItemValidity(m_ctm_ps,_T("顾客备注"),FALSE,FALSE,FALSE,100,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.ps_ctm,T2A(m_ctm_ps));
	
	if(!IsItemValidity(m_ctm_addr,_T("顾客地址"),FALSE,FALSE,FALSE,50,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.addr_ctm,T2A(m_ctm_addr));

	if(!IsItemValidity(m_person_budget,_T("预算人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion_budget,T2A(m_person_budget));
	
	CString temp;
	m_order_discuss_type.GetWindowText(temp);
	strcpy_s(m_orderInfo_edit.type_discuss_order,T2A(temp));
	
	if(!IsItemValidity(m_person1_discuss_deal,_T("谈单人员1"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion1_discuss_order,T2A(m_person1_discuss_deal));
	
	if(!IsItemValidity(m_person2_discuss_deal,_T("谈单人员2"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion2_discuss_order,T2A(m_person2_discuss_deal));

	if(!IsItemValidity(m_NO_contract,_T("合同单号"),FALSE,TRUE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.NO_contract,T2A(m_NO_contract));

	m_prog_business.GetWindowText(temp);
	strcpy_s(m_orderInfo_edit.prog_business,T2A(temp));
	temp.Format(_T("%.2f"),m_contract_total_price);
	strcpy_s(m_orderInfo_edit.price_total_contract,T2A(temp));
	temp.Format(_T("%.2f"),m_money_already_take);
	strcpy_s(m_orderInfo_edit.money_already_take,T2A(temp));

	dwResult = m_date_first_test.GetTime(timeTime);   //初测预约时间
	if (dwResult == GDT_VALID)
	{
		if ((m_date_first_test.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo_edit.date_first_test_book,p);
	}else
	{
		MessageBox(_T("请选择初测预约时间"));
		return;
	}
	
	if(!IsItemValidity(m_person_first_test,_T("初测人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion_first_test,T2A(m_person_first_test));
	

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
		strcpy_s(m_orderInfo_edit.date_out_order_book,p);
	}else
	{
		MessageBox(_T("请选择发单预约时间"));
		return;
	}
	
	if(!IsItemValidity(m_person_out_order,_T("发单人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.person_out_order,T2A(m_person_out_order));

	if(!IsItemValidity(m_fct_choose,_T("单位"),FALSE,FALSE,FALSE,30,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.factory_choose,T2A(m_fct_choose));
	
	m_pay_deal_choose.GetWindowText(temp);
	strcpy_s(m_orderInfo_edit.pay_deal_choice,T2A(temp));

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
		strcpy_s(m_orderInfo_edit.date_book,p);
	}else
	{
		MessageBox(_T("请选择放样预约时间"));
		return;
	}

	if(!IsItemValidity(m_person_exact_measure,_T("放样精量人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.person_exact_measure,T2A(m_person_exact_measure));
	

	dwResult = m_date_install_book.GetTime(timeTime);   //放样预约时间
	if (dwResult == GDT_VALID)
	{
		if ((m_date_install_book.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_orderInfo_edit.date_install_book,p);
	}else
	{
		MessageBox(_T("请选择安装预约时间"));
		return;
	}
	
	if(!IsItemValidity(m_install_person,_T("安装人员"),FALSE,FALSE,FALSE,15,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.persion_install,T2A(m_install_person));
	if(!IsItemValidity(m_problems_install,_T("安装问题"),FALSE,FALSE,FALSE,100,0))
	{
		return ;
	}
	strcpy_s(m_orderInfo_edit.problems_install,T2A(m_problems_install));
	CString ctmFrom;
	m_ctm_from.GetWindowText(ctmFrom);
	strcpy_s(m_orderInfo_edit.ctm_from,T2A(ctmFrom));
	CheckCtmFrom();

	CSock::StartReqOrderEdit(this);
}

BOOL COrderEdit::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot)
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

BOOL COrderEdit::ContainsCharsRemain(CString str)     //检查字符串是否包含子字符串
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

BOOL COrderEdit::IsNum(CString str,BOOL bCheckDot)
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


void COrderEdit::CheckCtmFrom()
{
	UpdateData(TRUE);
	CString strTemp;
	CString temp;
	m_ctm_from.GetWindowText(strTemp);
	if("新客户" == strTemp){
		memset(m_orderInfo_edit.phone_intr,0,sizeof(m_orderInfo_edit.phone_intr));
		memset(m_orderInfo_edit.name_emp,0,sizeof(m_orderInfo_edit.name_emp));
		memset(m_orderInfo_edit.name_intr,0,sizeof(m_orderInfo_edit.name_intr));
	}else if("老客户二次订单" == strTemp){
		memset(m_orderInfo_edit.phone_intr,0,sizeof(m_orderInfo_edit.phone_intr));
		memset(m_orderInfo_edit.name_emp,0,sizeof(m_orderInfo_edit.name_emp));
		memset(m_orderInfo_edit.name_intr,0,sizeof(m_orderInfo_edit.name_intr));
		memset(m_orderInfo_edit.name_serv_persion,0,sizeof(m_orderInfo_edit.name_serv_persion));
	}else if("老客户介绍" == strTemp){
		memset(m_orderInfo_edit.name_emp,0,sizeof(m_orderInfo_edit.name_emp));
		memset(m_orderInfo_edit.name_serv_persion,0,sizeof(m_orderInfo_edit.name_serv_persion));
	}else if("装潢公司介绍" == strTemp){
		memset(m_orderInfo_edit.name_emp,0,sizeof(m_orderInfo_edit.name_emp));
		memset(m_orderInfo_edit.name_serv_persion,0,sizeof(m_orderInfo_edit.name_serv_persion));
	}else if("业务员跑单" == strTemp){
		memset(m_orderInfo_edit.name_emp,0,sizeof(m_orderInfo_edit.name_emp));
	}else{
		return;	
	}
}