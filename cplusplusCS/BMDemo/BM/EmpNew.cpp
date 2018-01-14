// EmpNew.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "EmpNew.h"
#include "afxdialogex.h"
#include "EmpInfoView.h"

// CEmpNew 对话框

IMPLEMENT_DYNAMIC(CEmpNew, CDialogEx)

CEmpNew::CEmpNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmpNew::IDD, pParent)
	, m_addr_emp(_T(""))
	, m_name_emp(_T(""))
	, m_num_emp(_T(""))
	, m_phone_emp(_T(""))
	, m_ps_emp(_T(""))
	, m_pos_emp(_T(""))
{

}
CEmpNew::CEmpNew(CEmpInfoView* p_emp,CWnd* pParent /*=NULL*/)   // 自定义构造函数
	: CDialogEx(CEmpNew::IDD, pParent)
{
	m_pEmpInfoView = p_emp;
}

CEmpNew::~CEmpNew()
{
	m_pEmpInfoView = NULL;
}

void CEmpNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EMP_ADDR, m_addr_emp);
	DDX_Text(pDX, IDC_EDIT_EMP_NAME, m_name_emp);
	DDX_Text(pDX, IDC_EDIT_EMP_NO, m_num_emp);
	DDX_Text(pDX, IDC_EDIT_EMP_PHONE, m_phone_emp);
	DDX_Text(pDX, IDC_EDIT_EMP_PS, m_ps_emp);
	DDX_Text(pDX, IDC_EDIT_EMP_POS, m_pos_emp);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CEmpNew, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_NEW, &CEmpNew::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CEmpNew::OnBnClickedBtnCancel)
	ON_MESSAGE(WM_REQEMPNEWSTART_EmpNew, &CEmpNew::OnReqEmpNewStart)
	ON_MESSAGE(WM_REQEMPNEWEND_EmpNew, &CEmpNew::OnReqEmpNewEnd)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CEmpNew 消息处理程序
BOOL CEmpNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont font;
	LOGFONT m_tempfont={22,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
	return TRUE;
}

HBRUSH CEmpNew::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CEmpNew::OnBnClickedBtnNew()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(!IsItemValidity(m_num_emp,_T("员工工号"),TRUE,TRUE,FALSE,12,1,FALSE))
	{
		return ;
	}
	if(!IsItemValidity(m_name_emp,_T("员工姓名"),TRUE,FALSE,FALSE,15,1,FALSE))
	{
		return;
	}
	if(!IsItemValidity(m_addr_emp,_T("员工地址"),FALSE,FALSE,FALSE,45,0,FALSE))
	{
		return;
	}
	
	if(!IsItemValidity(m_ps_emp,_T("员工备注"),FALSE,FALSE,FALSE,200,0,FALSE))
	{
		return;
	}
	if(!IsItemValidity(m_pos_emp,_T("员工职务"),FALSE,FALSE,FALSE,45,0,FALSE))
	{
		return;
	}
	
	if(!IsItemValidity(m_phone_emp,_T("员工电话"),FALSE,TRUE,FALSE,11,0,FALSE))
	{
		return;
	}
	CSock::StartReqEmpNew(this);
}

BOOL CEmpNew::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max,int data_min_len,BOOL bCheckDot)
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

BOOL CEmpNew::ContainsCharsRemain(CString str)     //检查字符串是否包含子字符串
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

BOOL CEmpNew::IsNum(CString str,BOOL bCheckDot)
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

void CEmpNew::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::m_pEmpNew = NULL;
	CDialogEx::OnCancel();
}


afx_msg LRESULT CEmpNew::OnReqEmpNewStart(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CEmpNew::OnReqEmpNewEnd(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T("就绪"));
	return 0;
}


void CEmpNew::OnTimer(UINT_PTR nIDEvent)
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

void CEmpNew::ShowTip()
{
	m_time_take++;
	CString time;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 60){  //服务超时
		KillTimer(1);
		m_time_take = 0;
		m_tip.SetWindowText(_T("请求超时，请检查与服务器连接状态或重试"));
		CSock::m_pEmpNew = NULL;
	}
}

void CEmpNew::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pEmpNew = NULL;
	CDialogEx::OnClose();
}
