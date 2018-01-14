// OrderContentProp.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "OrderContentProp.h"
#include "afxdialogex.h"


// COrderContentProp 对话框

IMPLEMENT_DYNAMIC(COrderContentProp, CPropertyPage)

COrderContentProp::COrderContentProp()
	: CPropertyPage(COrderContentProp::IDD)
	//, m_intrNM(_T(""))
	//, m_intrPhone(0)
	//, m_servPersonName(_T(""))
{

}

COrderContentProp::~COrderContentProp()
{
}

void COrderContentProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CTM_FROM, m_ctmFrom);
	//  DDX_Text(pDX, IDC_EDIT_INTR_NM, m_intrNM);
	//  DDX_Text(pDX, IDC_EDIT_INTR_PHONE, m_intrPhone);
	DDX_Control(pDX, IDC_STATIC_INTR_NAME, m_staticIntrName);
	DDX_Control(pDX, IDC_STATIC_INTR_PHONE, m_staticIntrPhone);
	DDX_Control(pDX, IDC_STATIC_SERV_PERSON, m_staticServPersonNM);
	//  DDX_Text(pDX, IDC_EDIT_SERV_PERSON, m_servPersonName);
	DDX_Control(pDX, IDC_EDIT_INTR_NM, m_intrName);
	DDX_Control(pDX, IDC_EDIT_INTR_PHONE, m_intrPhone);
	DDX_Control(pDX, IDC_EDIT_SERV_PERSON, m_servPerson);
	DDX_Control(pDX, IDC_STATIC_EMP_DONE, m_static_emp_done);
	DDX_Control(pDX, IDC_EDIT_EMP_NAME, m_empName);
	DDX_Control(pDX, IDC_COMBO_BUSINESS_PROG, m_businessProg);
	DDX_Control(pDX, IDC_COMBO_PAY_DEAL_CHOOSE, m_payDealChoose);
	DDX_Control(pDX, IDC_COMBO_ORDER_DISCUSS_TYPE, m_orderDiscussType);
	DDX_Control(pDX, IDC_COMBO_EARLY_STAGE_TYPE, m_earlyStageType);
}


BEGIN_MESSAGE_MAP(COrderContentProp, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_CTM_FROM, &COrderContentProp::OnCbnSelchangeComboCtmFrom)
	//ON_EN_CHANGE(IDC_EDIT_ACCESSORY_FILE_PATH, &COrderContentProp::OnEnChangeEditAccessoryFilePath)
END_MESSAGE_MAP()


// COrderContentProp 消息处理程序

void COrderContentProp::InitCtmFromComb()
{
	m_ctmFrom.AddString(_T("老客户介绍"));
	m_ctmFrom.AddString(_T("老客户二次订单"));
	m_ctmFrom.AddString(_T("新客户"));
	m_ctmFrom.AddString(_T("装潢公司介绍"));
	m_ctmFrom.AddString(_T("业务员跑单"));

	m_ctmFrom.SetCurSel(0);

	

}

BOOL COrderContentProp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitCtmFromComb();
	InitBusinessProgComb();
	InitEarlyStageTypeComb();
	
	InitPayDealChooseComb();
	InitOrderDiscussTypeComb();

	return TRUE;
}

void COrderContentProp::InitOrderDiscussTypeComb()
{
	m_orderDiscussType.AddString(_T("可选择独立谈单"));
	m_orderDiscussType.AddString(_T("两人合作谈单"));
	m_orderDiscussType.SetCurSel(0);
}
void COrderContentProp::InitPayDealChooseComb()
{
	m_payDealChoose.AddString(_T("已付"));
	m_payDealChoose.AddString(_T("未付"));
	m_payDealChoose.AddString(_T("累积"));
	m_payDealChoose.SetCurSel(0);

}
void COrderContentProp::InitEarlyStageTypeComb()
{
	m_earlyStageType.AddString(_T("内部修改"));
	m_earlyStageType.AddString(_T("客户沟通修改"));
	m_earlyStageType.SetCurSel(0);

}

void COrderContentProp::InitBusinessProgComb()
{
	m_businessProg.AddString(_T("意向客户"));
	m_businessProg.AddString(_T("已订单"));
	m_businessProg.AddString(_T("已初测"));
	m_businessProg.AddString(_T("已订单"));
	m_businessProg.AddString(_T("已签合同"));
	m_businessProg.AddString(_T("已确定最终方案"));
	m_businessProg.AddString(_T("已放样精量"));
	m_businessProg.AddString(_T("已发单"));
	m_businessProg.AddString(_T("已回单"));
	m_businessProg.AddString(_T("已安装"));
	m_businessProg.AddString(_T("已完成"));
	m_businessProg.SetCurSel(0);
}
void COrderContentProp::OnCbnSelchangeComboCtmFrom()
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



