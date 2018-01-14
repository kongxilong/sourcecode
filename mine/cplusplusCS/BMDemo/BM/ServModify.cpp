// ServModify.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "ServModify.h"
#include "afxdialogex.h"


// CServModify 对话框

IMPLEMENT_DYNAMIC(CServModify, CDialogEx)

CServModify::CServModify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServModify::IDD, pParent)
	, m_addr(_T(""))
	, m_charge(0)
	, m_clientNM(_T(""))
	, m_ctmNM(_T(""))
	, m_phone(_T(""))
	, m_ps(_T(""))
	, m_servContent(_T(""))
	, m_servNM(_T(""))
	, m_servNO(_T(""))
	, m_servPhone(_T(""))
	, m_servType(_T(""))
{

}

CServModify::~CServModify()
{
}

void CServModify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CTM_SEX, m_ctmSex);
	DDX_Control(pDX, IDC_COMBO_CTM_TYPE, m_ctmType);
	DDX_Control(pDX, IDC_COMBO_PROG, m_prog);
	DDX_Control(pDX, IDC_DATETIME_DELAY, m_dateDelay);
	DDX_Control(pDX, IDC_DATETIME_REQ, m_dateReq);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_addr);
	DDX_Text(pDX, IDC_EDIT_CHARGE, m_charge);
	DDX_Text(pDX, IDC_EDIT_CLIENT_NM, m_clientNM);
	DDX_Text(pDX, IDC_EDIT_CTM_NM, m_ctmNM);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_phone);
	DDX_Text(pDX, IDC_EDIT_PS, m_ps);
	DDX_Text(pDX, IDC_EDIT_SERV_CONTENT, m_servContent);
	DDX_Text(pDX, IDC_EDIT_SERV_NM, m_servNM);
	DDX_Text(pDX, IDC_EDIT_SERV_NO, m_servNO);
	DDX_Text(pDX, IDC_EDIT_SERV_PHONE, m_servPhone);
	DDX_Text(pDX, IDC_EDIT_SERV_TYPE, m_servType);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CServModify, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_ServModify, &CServModify::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_ServModify, &CServModify::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CServModify::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CServModify::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CServModify 消息处理程序


HBRUSH CServModify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(BACKGROUNDCOLOR);	
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

BOOL CServModify::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitTipCtrl();
	InitComb();

	m_dateDelay.SetFormat(_T("yyyy-MM-dd"));
	m_dateReq.SetFormat(_T("yyyy-MM-dd"));
	LoadDate(m_strDateReq,m_dateReq);
	LoadDate(m_strDateDelay,m_dateDelay);
	return TRUE;
}

void CServModify::OnTimer(UINT_PTR nIDEvent)
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

void CServModify::InitComb()
{
	//性别下拉组合框
	m_ctmSex.AddString(_T("男"));
	m_ctmSex.AddString(_T("女"));
	if(_T("男") == m_strCtmSex)
	{
		m_ctmSex.SetCurSel(0);
	}else{
		m_ctmSex.SetCurSel(1);
	}
	CString         m_strCtmType;
	CString         m_strProg;
	//客户类型组合框
	m_ctmType.AddString(_T("老客户介绍"));
	m_ctmType.AddString(_T("老客户二次订单"));
	m_ctmType.AddString(_T("新客户"));
	m_ctmType.AddString(_T("装潢公司介绍"));
	m_ctmType.AddString(_T("业务员跑单"));
	if(!m_strCtmType.IsEmpty())
	{
		int index = m_ctmType.FindStringExact(0,m_strCtmType);
		if(index != CB_ERR)
		{
			m_ctmType.SetCurSel(index);
		}else{
			m_ctmType.SetCurSel(2);
		}
	}else{
		m_ctmType.SetCurSel(2);
	}
	//进度栏
	m_prog.AddString(_T("预约"));
	m_prog.AddString(_T("完成"));
	m_prog.AddString(_T("延期"));
	if(!m_strProg.IsEmpty())
	{
		int index = m_ctmType.FindStringExact(0,m_strProg);
		if(index != CB_ERR)
		{
			m_prog.SetCurSel(index);
		}else{
			m_prog.SetCurSel(0);
		}
	}else{
		m_prog.SetCurSel(0);
	}
}

void CServModify::LoadDate(CString _strTime,CDateTimeCtrl &date)
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

void CServModify::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}
void CServModify::ShowTip()
{
	m_time_take++;
	CString time = NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重试或者重新连接服务器"));
	}
}

void CServModify::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pServModify = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT CServModify::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CServModify::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


void CServModify::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CServModify::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	USES_CONVERSION;
	char *p = NULL;
	memset(&m_servInfo_struct,0,sizeof(m_servInfo_struct));
	if(m_servNO.IsEmpty())
	{
		MessageBox(_T("服务单号不能为空"));
		return ;
	}else if(m_servNO.GetLength() > 10)
	{
		MessageBox(_T("服务单号长度不能大于10个字符"));
		return ;
	}else {
		p = T2A(m_servNO);
		int len = m_servNO.GetLength();
		for(int i = 0;i < len;i++)
		{
			if(p[i] <'0'||p[i]>'9')
			{
				MessageBox(_T("服务单号必须为数字"));
				return;
			}
		}
	}
	if(m_ctmNM.IsEmpty())
	{
		MessageBox(_T("客户名称不能为空"));
		return ;
	}else if(m_ctmNM.GetLength()>15)
	{
		MessageBox(_T("客户名称不能超过15个字符"));
		return;
	}

	if(m_clientNM.IsEmpty())
	{
		MessageBox(_T("顾客姓名不能为空"));
		return ;
	}else if(m_clientNM.GetLength()>15)
	{
		MessageBox(_T("顾客姓名不能超过15个字符"));
		return;
	}

	if(m_phone.IsEmpty())
	{
		if(IDYES == MessageBox(_T("顾客电话为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_phone = _T("");
		}else{
			return ;
		}
	}else {
		p = T2A(m_phone);
		int len = m_phone.GetLength();
		if(len>11){
			MessageBox(_T("顾客电话不能超过11个字符"));
			return;
		}
		for(int i = 0; i<len;i++)
		{
			if(p[i] < '0'||p[i] >'9')
			{
				MessageBox(_T("顾客电话必须为数字"));
				return;
			}
		}
	}

	if(m_addr.IsEmpty())
	{
		if(IDYES == MessageBox(_T("地址为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_addr = _T("");
		}else{
			return ;
		}
	}else if(m_addr.GetLength()>25){
		MessageBox(_T("地址不能超过25个字符"));
		return;
	}

	if(m_servContent.IsEmpty())
	{
		if(IDYES == MessageBox(_T("服务内容为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_servContent = _T("");
		}else{
			return ;
		}
	}else if(m_servContent.GetLength()>30)
	{
		MessageBox(_T("服务内容不能超过30个字符"));
		return;
	}
	if(m_servNM.IsEmpty())
	{
		if(IDYES == MessageBox(_T("服务人员姓名为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_servNM = _T("");
		}else{
			return ;
		}
	}else if(m_servNM.GetLength()>10)
	{
		MessageBox(_T("服务人员姓名不能超过10个字符"));
		return;
	}
	if(m_ps.IsEmpty())
	{
		if(IDYES == MessageBox(_T("备注为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_ps = _T("");
		}else{
			return ;
		}
	}else if(m_ps.GetLength()>100)
	{
		MessageBox(_T("备注不能超过100个字符"));
		return;
	}
	if(m_servType.IsEmpty())
	{
		if(IDYES == MessageBox(_T("服务类型为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_servType = _T("");
		}else{
			return ;
		}
	}else if(m_servType.GetLength()>15)
	{
		MessageBox(_T("备注不能超过15个字符"));
		return;
	}
	
	if(m_servPhone.IsEmpty())
	{
		if(IDYES == MessageBox(_T("服务人员电话为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_servPhone = _T("");
		}else{
			return ;
		}
	}else{
		p = T2A(m_servPhone);
		int len = m_servPhone.GetLength();
		if(len>11){
			MessageBox(_T("服务人员电话不能超过11个字符"));
			return;
		}
		for(int i = 0; i<len;i++)
		{
			if(p[i] < '0'||p[i] >'9')
			{
				MessageBox(_T("服务人员电话必须为数字"));
				return;
			}
		}
	}

	if(m_addr.IsEmpty())
	{
		if(IDYES == MessageBox(_T("地址为空，确认继续吗"),_T("提示"),MB_YESNO))
		{
			m_addr = _T("");
		}else{
			return ;
		}
	}else if(m_addr.GetLength()>25)
	{
		MessageBox(_T("服务地址不能超过25个字符"));
		return;
	}
	p = T2A(m_servNO);
	strcpy_s(m_servInfo_struct.serv_NO,p);
	CString ctmType = NULL;
	m_ctmType.GetWindowText(ctmType);
	p = T2A(ctmType);
	strcpy_s(m_servInfo_struct.client_type,p);
	p = T2A(m_ctmNM);
	strcpy_s(m_servInfo_struct.ctm_name,p);
	p = T2A(m_clientNM);
	strcpy_s(m_servInfo_struct.client_name,p);
	CString sex = NULL;
	m_ctmSex.GetWindowText(sex);
	p = T2A(sex);
	strcpy_s(m_servInfo_struct.ctm_sex,p);
	p = T2A(m_phone);
	strcpy_s(m_servInfo_struct.client_phone,p);
	p = T2A(m_addr);
	strcpy_s(m_servInfo_struct.addr,p);
	p = T2A(m_ps);
	strcpy_s(m_servInfo_struct.ps,p);
	p = T2A(m_servContent);
	strcpy_s(m_servInfo_struct.serv_content,p);

	CTime timeTime;
	DWORD dwResult = m_dateReq.GetTime(timeTime);
	CString str;
	if (dwResult == GDT_VALID)
	{
		if ((m_dateReq.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_servInfo_struct.req_serv_date,p);
	}else
	{
		MessageBox(_T("请选择要求服务日期"));
		return;
	}

	p = T2A(m_servNM);
	strcpy_s(m_servInfo_struct.serv_name,p);
	p = T2A(m_servPhone);
	strcpy_s(m_servInfo_struct.serv_phone,p);
	CString prog = NULL;
	m_prog.GetWindowText(prog);
	p = T2A(prog);
	strcpy_s(m_servInfo_struct.prog,p);

	dwResult = m_dateDelay.GetTime(timeTime);
	if (dwResult == GDT_VALID)
	{
		if ((m_dateDelay.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_servInfo_struct.delay_date,p);
	}else
	{
		MessageBox(_T("请选择延期日期"));
		return;
	}
	CString charge = NULL;
	charge.Format(_T("%.2f"),m_charge);
	p = T2A(charge);
	strcpy_s(m_servInfo_struct.charge,p);
	p = T2A(m_servType);
	strcpy_s(m_servInfo_struct.serv_type,T2A(m_servType));

	CSock::StartReqServModify(this);
}
