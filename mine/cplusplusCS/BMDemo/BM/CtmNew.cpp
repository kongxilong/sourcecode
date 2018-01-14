// CtmNew.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "CtmNew.h"
#include "afxdialogex.h"


// CCtmNew 对话框

IMPLEMENT_DYNAMIC(CCtmNew, CDialogEx)

CCtmNew::CCtmNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCtmNew::IDD, pParent)
	, m_ctmcall(_T(""))
	, m_ctmNM(_T(""))	
	, m_ps(_T(""))
	, m_times(0)
	, m_price(0)
	, m_ctmNO(0)
	, m_phone(_T(""))
{

}

CCtmNew::CCtmNew(CCtmInfo *pdlg,CWnd* pParent /*=NULL*/)
	: CDialogEx(CCtmNew::IDD, pParent)
	, m_ctmcall(_T(""))
	, m_ctmNM(_T(""))	
	, m_price(0)
	, m_ps(_T(""))
	, m_times(0)
	, m_ctmNO(0)
{
	m_pParentDlg = pdlg;
}

CCtmNew::~CCtmNew()
{
	m_pParentDlg = NULL;
}

void CCtmNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CTMSEX, m_comboxSex);
	DDX_Control(pDX, IDC_DATE_FIRST, m_dateFist);
	DDX_Control(pDX, IDC_DATE_LAST, m_dateLast);
	DDX_Text(pDX, IDC_EDIT_CTMCALL, m_ctmcall);
	DDX_Text(pDX, IDC_EDIT_CTMNM, m_ctmNM);
	DDX_Text(pDX, IDC_EDIT_PS, m_ps);
	DDX_Text(pDX, IDC_EDIT_TIMES, m_times);
	DDV_MinMaxInt(pDX, m_times, 0, 10000);
	//  DDX_Text(pDX, IDC_EDIT_PRICE, m_price);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Text(pDX, IDC_EDIT_CTMNO, m_ctmNO);
	DDV_MinMaxInt(pDX, m_ctmNO, 0, 100000000);
	DDX_Text(pDX, IDC_EDIT_PHONENO, m_phone);
	DDX_Text(pDX, IDC_EDIT_PRICE, m_price);
}


BEGIN_MESSAGE_MAP(CCtmNew, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_CtmNew, &CCtmNew::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_CtmNew, &CCtmNew::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_CANCLE, &CCtmNew::OnBnClickedBtnCancle)
	ON_BN_CLICKED(IDC_BTN_NEW, &CCtmNew::OnBnClickedBtnNew)
END_MESSAGE_MAP()


// CCtmNew 消息处理程序


HBRUSH CCtmNew::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CCtmNew::OnTimer(UINT_PTR nIDEvent)
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


void CCtmNew::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pCtmNew = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT CCtmNew::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}

void CCtmNew::ShowTip()
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

afx_msg LRESULT CCtmNew::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}



void CCtmNew::OnBnClickedBtnCancle()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CCtmNew::OnBnClickedBtnNew()
{
	UpdateData(TRUE);
	if(!IsItemValidity(m_ctmNM,_T("顾客姓名"),TRUE,FALSE,FALSE,15,0))
	{
		return;
	}
	if(!IsItemValidity(m_ctmcall,_T("客户名称"),FALSE,FALSE,FALSE,24,0))
	{
		return;
	}
	if(!IsItemValidity(m_phone,_T("顾客电话"),FALSE,TRUE,FALSE,11,0))
	{
		return;
	}
	if(!IsItemValidity(m_ps,_T("客户备注"),FALSE,FALSE,FALSE,150,0))
	{
		return;
	}
	
	if(m_price<0)
	{
		MessageBox(_T("消费金额不能小于零"));
		return;
	}
	USES_CONVERSION;
	memset(&m_ctmNewStruct,0,sizeof(CtmInfo));
	char *p = NULL;
	CTime timeTime;
	DWORD dwResult = m_dateFist.GetTime(timeTime);
	CString str;
	if (dwResult == GDT_VALID)
	{
		if ((m_dateFist.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_ctmNewStruct.first_pay_time,p);
	}else
	{
		MessageBox(_T("请选择首次消费时间"));
		return;
	}


	dwResult = m_dateLast.GetTime(timeTime);
	if (dwResult == GDT_VALID)
	{
		if ((m_dateLast.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			str = timeTime.Format(_T("%Y-%m-%d"));
		}
		p = T2A(str);
		strcpy_s(m_ctmNewStruct.late_pay_time,p);
	}else{
		MessageBox(_T("请选择最近消费时间"));
		return;
	}

	CString temp ;
	temp.Format(_T("%d"),m_ctmNO);
	p = T2A(temp);
	strcpy_s(m_ctmNewStruct.NO,p);
	p = T2A(m_ctmcall);
	strcpy_s(m_ctmNewStruct.call_ctm,p);
	p = T2A(m_ctmNM);
	strcpy_s(m_ctmNewStruct.name,p);
	int sel = m_comboxSex.GetCurSel();
	m_comboxSex.GetLBText(sel,temp);
	p = T2A(temp);
	strcpy_s(m_ctmNewStruct.sex,p);
	p = T2A(m_phone);
	strcpy_s(m_ctmNewStruct.phone,p);
	temp.Format(_T("%d"),m_times);
	p = T2A(temp);
	strcpy_s(m_ctmNewStruct.pay_times,p);
	temp.Format(_T("%.2f"),m_price);
	p = T2A(temp);
	strcpy_s(m_ctmNewStruct.pay_total,p);
	p = T2A(m_ps);
	strcpy_s(m_ctmNewStruct.ps,p);

	CSock::StartReqCtmNew(this);
}

BOOL CCtmNew::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot)
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

BOOL CCtmNew::ContainsCharsRemain(CString str)     //检查字符串是否包含子字符串
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

BOOL CCtmNew::IsNum(CString str,BOOL bCheckDot)
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


BOOL  CCtmNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();

	InitCombSex();

	m_dateFist.SetFormat(_T("yyyy-MM-dd"));
	m_dateLast.SetFormat(_T("yyyy-MM-dd"));
	return TRUE;
}

void CCtmNew::InitCombSex()
{
	m_comboxSex.AddString(_T("男"));
	m_comboxSex.AddString(_T("女"));
	m_comboxSex.SetCurSel(0);
}