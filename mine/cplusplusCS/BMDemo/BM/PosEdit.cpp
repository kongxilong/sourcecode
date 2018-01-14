// PosEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "PosEdit.h"
#include "afxdialogex.h"


// CPosEdit 对话框

IMPLEMENT_DYNAMIC(CPosEdit, CDialogEx)

CPosEdit::CPosEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPosEdit::IDD, pParent)
	, m_emp_num(_T(""))
	, m_password_confirm(_T(""))
	, m_emp_ps(_T(""))
	, m_emp_name(_T(""))
	, m_emp_password(_T(""))
{

}

CPosEdit::~CPosEdit()
{
}

void CPosEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Text(pDX, IDC_ET_POSNO, m_emp_num);
	DDX_Text(pDX, IDC_ET_PASSWORD_CONFIRM, m_password_confirm);
	DDX_Text(pDX, IDC_ET_EMPPS, m_emp_ps);
	DDX_Text(pDX, IDC_ET_EMPNM, m_emp_name);
	DDX_Text(pDX, IDC_ET_EMP_PASSWORD, m_emp_password);
	DDX_Control(pDX, IDC_COMBO_POS_NAME, m_pos_name);
}




BEGIN_MESSAGE_MAP(CPosEdit, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SAVE_POS, &CPosEdit::OnBnClickedBtnSavePos)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_PosEdit, &CPosEdit::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_PosEdit, &CPosEdit::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CPosEdit::OnBnClickedBtnCancel)
	ON_MESSAGE(WM_INITUSER_AUTHORITY_PosEdit, &CPosEdit::OnInitUserAuthority)
END_MESSAGE_MAP()


// CPosEdit 消息处理程序


HBRUSH CPosEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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



BOOL CPosEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitTipCtrl();
	InitComb();
	LoadUser();
	return TRUE;	
}

void CPosEdit::InitComb()
{
	m_pos_name.AddString(_T("系统管理员"));
	m_pos_name.AddString(_T("总经理"));
	m_pos_name.AddString(_T("客户经理"));
	m_pos_name.AddString(_T("设计师"));
	m_pos_name.AddString(_T("普通员工"));

	int index = m_pos_name.FindStringExact(0,m_str_pos_name);
	if(index != CB_ERR)
	{
		m_pos_name.SetCurSel(index);
	}else{
		m_pos_name.SetCurSel(4);
	}
}


void CPosEdit::LoadUser()
{
	CSock::StartReqLoadUserAuthority(this);
}

void CPosEdit::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}


void CPosEdit::AddItemToString(CString &_source,CString item_new,int flag)
{
	CString _start = _T("{#[#");
	CString _mid = _T("#][#");
	CString _end = _T("#]#}");
	switch (flag)
	{
	case -1:
		{
			_source += _start;
			_source += item_new;
			_source += _mid;
			break;
		}
	case 0:
		{
			_source += item_new;
			_source += _mid;
			break;
		}
	case 1:
		{
			_source += item_new;
			_source += _end;
			break;
		}
	default:
		break;
	}
}

BOOL CPosEdit::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max)    //true  有效 否则无效  
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
			if(!CSock::IsNum(_source,FALSE))
			{
				CString err = _item_name+_T("必须为数字");
				AfxMessageBox(err);
				return FALSE;
			}
		}
		if(CSock::ContainsCharsRemain(_source))
		{
			return FALSE;
		}
		
		int len = _source.GetLength();
		if(len>str_len_max)  //检查数据长度
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
					if(!CSock::IsNum(left,FALSE))
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
							if(CSock::IsNum(left,FALSE)&&CSock::IsNum(str,FALSE))
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


void CPosEdit::OnBnClickedBtnSavePos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_str_pos_edit.Empty();
	if(!IsItemValidity(m_emp_num,_T("角色编号"),TRUE,TRUE,FALSE,20))
	{
		return;
	}
	AddItemToString(m_str_pos_edit,m_emp_num,DATA_START);
	CString temp;
	m_pos_name.GetWindowText(temp);
	AddItemToString(m_str_pos_edit,temp,DATA_MID);
	if(!IsItemValidity(m_emp_name,_T("员工姓名"),TRUE,FALSE,FALSE,20))
	{
		return;
	}
	AddItemToString(m_str_pos_edit,m_emp_name,DATA_MID);
	if(!IsItemValidity(m_emp_password,_T("登录密码"),TRUE,FALSE,FALSE,13))
	{
		
		return;
	}
	if(m_emp_password != m_password_confirm)
	{
		MessageBox(_T("两次输入的密码不一致，请重新输入"));
		m_password_confirm = _T("");
		UpdateData(FALSE);
		return;
	}
	AddItemToString(m_str_pos_edit,m_emp_password,DATA_MID);
	
	if(!IsItemValidity(m_emp_ps,_T("备注"),FALSE,FALSE,FALSE,200))
	{
		return;
	}
	AddItemToString(m_str_pos_edit,m_emp_ps,DATA_MID);

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEAL_NEW))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EDIT_DEAL))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_DEAL))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_DEAL))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_UPLOAD_FILE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DOWNLOAD_FILE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_FILE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_DEAL_INCOMPLETE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EDIT_DEAL_INCOMPLETE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_STORE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_ALERT_SET))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_STORE_ADD))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_STORE_OUT))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_HISTORY_FIND))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_HISTORY))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_EMP))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EDIT_EMP))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EMP_NEW))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_EMP))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_EMP_PERFERENCE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_CTM))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_CTM_NEW))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EDIT_CTM))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_CTM))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_POS_ADD))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_POS))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_POS))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EDIT_POS))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_PROD_IM))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_PROD_EX))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_CTM_IM))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}

	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_CTM_EX))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EMP_IM))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EMP_EX))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_MT_FIND))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_NODE_NEW))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_NODE_EDIT))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_NODE))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FIND_FCT))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_FCT_NEW))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_EDIT_FCT))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_DEL_FCT))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_NOTE_PUBLIC))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_MID);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_MID);
	}
	if(1 == ((CButton*)GetDlgItem(IDC_CHECK_NOTE_DEL))->GetCheck())
	{
		AddItemToString(m_str_pos_edit,_T("1"),DATA_END);
	}else{
		AddItemToString(m_str_pos_edit,_T("0"),DATA_END);
	}
	CSock::StartReqPosEdit(this);

}


void CPosEdit::OnTimer(UINT_PTR nIDEvent)
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

void CPosEdit::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 20)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重试或者重新连接服务器"));
	}
}

void CPosEdit::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSock::m_pPosEdit = NULL;
	CDialogEx::OnClose();
}


afx_msg LRESULT CPosEdit::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CPosEdit::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


void CPosEdit::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::m_pPosEdit = NULL;
	CDialogEx::OnCancel();
}


afx_msg LRESULT CPosEdit::OnInitUserAuthority(WPARAM wParam, LPARAM lParam)
{
	InitUserAuthority();
	return 0;
}


void CPosEdit::InitUserAuthority()
{
	UpdateData(TRUE);
	WT;
	CString user_authiority = m_str_pos_authorities;
	CString _data_start = _T("{#");
	CString _data_end = _T("#}");
	int index = user_authiority.Find(_data_start);
	user_authiority = user_authiority.Mid(index+2);
	index = user_authiority.Find(_data_end);
	user_authiority = user_authiority.Mid(0,index);
	m_emp_ps = GetItemFromArray(user_authiority,1);
	if(CSock::IsNum(GetItemFromArray(user_authiority,2),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEAL_NEW,atoi(T2A(GetItemFromArray(user_authiority,2))));
	}else{
		CheckDlgButton(IDC_CHECK_DEAL_NEW,FALSE);
	}
	
	if(CSock::IsNum(GetItemFromArray(user_authiority,3),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EDIT_DEAL,atoi(T2A(GetItemFromArray(user_authiority,3))));
	}else{
		CheckDlgButton(IDC_CHECK_EDIT_DEAL,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,4),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_DEAL,atoi(T2A(GetItemFromArray(user_authiority,4))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_DEAL,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,5),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_DEAL,atoi(T2A(GetItemFromArray(user_authiority,5))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_DEAL,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,6),FALSE))
	{
		CheckDlgButton(IDC_CHECK_UPLOAD_FILE,atoi(T2A(GetItemFromArray(user_authiority,6))));
	}else{
		CheckDlgButton(IDC_CHECK_UPLOAD_FILE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,7),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DOWNLOAD_FILE,atoi(T2A(GetItemFromArray(user_authiority,7))));
	}else{
		CheckDlgButton(IDC_CHECK_DOWNLOAD_FILE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,8),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_FILE,atoi(T2A(GetItemFromArray(user_authiority,8))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_FILE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,9),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_DEAL_INCOMPLETE,atoi(T2A(GetItemFromArray(user_authiority,9))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_DEAL_INCOMPLETE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,10),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EDIT_DEAL_INCOMPLETE,atoi(T2A(GetItemFromArray(user_authiority,10))));
	}else{
		CheckDlgButton(IDC_CHECK_EDIT_DEAL_INCOMPLETE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,11),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_STORE,atoi(T2A(GetItemFromArray(user_authiority,11))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_STORE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,12),FALSE))
	{
		CheckDlgButton(IDC_CHECK_ALERT_SET,atoi(T2A(GetItemFromArray(user_authiority,12))));
	}else{
		CheckDlgButton(IDC_CHECK_ALERT_SET,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,13),FALSE))
	{
		CheckDlgButton(IDC_CHECK_STORE_ADD,atoi(T2A(GetItemFromArray(user_authiority,13))));
	}else{
		CheckDlgButton(IDC_CHECK_STORE_ADD,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,14),FALSE))
	{
		CheckDlgButton(IDC_CHECK_STORE_OUT,atoi(T2A(GetItemFromArray(user_authiority,14))));
	}else{
		CheckDlgButton(IDC_CHECK_STORE_OUT,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,15),FALSE))
	{
		CheckDlgButton(IDC_CHECK_HISTORY_FIND,atoi(T2A(GetItemFromArray(user_authiority,15))));
	}else{
		CheckDlgButton(IDC_CHECK_HISTORY_FIND,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,16),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_HISTORY,atoi(T2A(GetItemFromArray(user_authiority,16))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_HISTORY,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,17),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_EMP,atoi(T2A(GetItemFromArray(user_authiority,17))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_EMP,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,18),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EDIT_EMP,atoi(T2A(GetItemFromArray(user_authiority,18))));
	}else{
		CheckDlgButton(IDC_CHECK_EDIT_EMP,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,19),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EMP_NEW,atoi(T2A(GetItemFromArray(user_authiority,19))));
	}else{
		CheckDlgButton(IDC_CHECK_EMP_NEW,FALSE);
	}

	if(CSock::IsNum(GetItemFromArray(user_authiority,20),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_EMP,atoi(T2A(GetItemFromArray(user_authiority,20))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_EMP,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,21),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_EMP_PERFERENCE,atoi(T2A(GetItemFromArray(user_authiority,21))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_EMP_PERFERENCE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,22),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_CTM,atoi(T2A(GetItemFromArray(user_authiority,22))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_CTM,FALSE);
	}

	if(CSock::IsNum(GetItemFromArray(user_authiority,23),FALSE))
	{
		CheckDlgButton(IDC_CHECK_CTM_NEW,atoi(T2A(GetItemFromArray(user_authiority,23))));
	}else{
		CheckDlgButton(IDC_CHECK_CTM_NEW,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,24),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EDIT_CTM,atoi(T2A(GetItemFromArray(user_authiority,24))));
	}else{
		CheckDlgButton(IDC_CHECK_EDIT_CTM,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,25),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_CTM,atoi(T2A(GetItemFromArray(user_authiority,25))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_CTM,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,26),FALSE))
	{
		CheckDlgButton(IDC_CHECK_POS_ADD,atoi(T2A(GetItemFromArray(user_authiority,26))));
	}else{
		CheckDlgButton(IDC_CHECK_POS_ADD,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,27),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_POS,atoi(T2A(GetItemFromArray(user_authiority,27))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_POS,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,28),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_POS,atoi(T2A(GetItemFromArray(user_authiority,28))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_POS,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,29),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EDIT_POS,atoi(T2A(GetItemFromArray(user_authiority,29))));
	}else{
		CheckDlgButton(IDC_CHECK_EDIT_POS,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,30),FALSE))
	{
		CheckDlgButton(IDC_CHECK_PROD_IM,atoi(T2A(GetItemFromArray(user_authiority,30))));
	}else{
		CheckDlgButton(IDC_CHECK_PROD_IM,FALSE);
	}

	if(CSock::IsNum(GetItemFromArray(user_authiority,31),FALSE))
	{
		CheckDlgButton(IDC_CHECK_PROD_EX,atoi(T2A(GetItemFromArray(user_authiority,31))));
	}else{
		CheckDlgButton(IDC_CHECK_PROD_EX,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,32),FALSE))
	{
		CheckDlgButton(IDC_CHECK_CTM_IM,atoi(T2A(GetItemFromArray(user_authiority,32))));
	}else{
		CheckDlgButton(IDC_CHECK_CTM_IM,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,33),FALSE))
	{
		CheckDlgButton(IDC_CHECK_CTM_EX,atoi(T2A(GetItemFromArray(user_authiority,33))));
	}else{
		CheckDlgButton(IDC_CHECK_CTM_EX,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,34),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EMP_IM,atoi(T2A(GetItemFromArray(user_authiority,34))));
	}else{
		CheckDlgButton(IDC_CHECK_EMP_IM,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,35),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EMP_EX,atoi(T2A(GetItemFromArray(user_authiority,35))));
	}else{
		CheckDlgButton(IDC_CHECK_EMP_EX,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,36),FALSE))
	{
		CheckDlgButton(IDC_CHECK_MT_FIND,atoi(T2A(GetItemFromArray(user_authiority,36))));
	}else{
		CheckDlgButton(IDC_CHECK_MT_FIND,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,37),FALSE))
	{
		CheckDlgButton(IDC_CHECK_NODE_NEW,atoi(T2A(GetItemFromArray(user_authiority,37))));
	}else{
		CheckDlgButton(IDC_CHECK_NODE_NEW,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,38),FALSE))
	{
		CheckDlgButton(IDC_CHECK_NODE_EDIT,atoi(T2A(GetItemFromArray(user_authiority,38))));
	}else{
		CheckDlgButton(IDC_CHECK_NODE_EDIT,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,39),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_NODE,atoi(T2A(GetItemFromArray(user_authiority,39))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_NODE,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,40),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FIND_FCT,atoi(T2A(GetItemFromArray(user_authiority,40))));
	}else{
		CheckDlgButton(IDC_CHECK_FIND_FCT,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,41),FALSE))
	{
		CheckDlgButton(IDC_CHECK_FCT_NEW,atoi(T2A(GetItemFromArray(user_authiority,41))));
	}else{
		CheckDlgButton(IDC_CHECK_FCT_NEW,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,42),FALSE))
	{
		CheckDlgButton(IDC_CHECK_EDIT_FCT,atoi(T2A(GetItemFromArray(user_authiority,42))));
	}else{
		CheckDlgButton(IDC_CHECK_EDIT_FCT,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,43),FALSE))
	{
		CheckDlgButton(IDC_CHECK_DEL_FCT,atoi(T2A(GetItemFromArray(user_authiority,43))));
	}else{
		CheckDlgButton(IDC_CHECK_DEL_FCT,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,44),FALSE))
	{
		CheckDlgButton(IDC_CHECK_NOTE_PUBLIC,atoi(T2A(GetItemFromArray(user_authiority,43))));
	}else{
		CheckDlgButton(IDC_CHECK_NOTE_PUBLIC,FALSE);
	}
	if(CSock::IsNum(GetItemFromArray(user_authiority,45),FALSE))
	{
		CheckDlgButton(IDC_CHECK_NOTE_DEL,atoi(T2A(GetItemFromArray(user_authiority,43))));
	}else{
		CheckDlgButton(IDC_CHECK_NOTE_DEL,FALSE);
	}

	m_emp_password = GetItemFromArray(user_authiority,46);
	m_password_confirm = GetItemFromArray(user_authiority,46);
	m_tip.SetWindowText(_T("就绪"));
	UpdateData(FALSE);
	
}

CString CPosEdit::GetItemFromArray(CString source,int index)     //从字符串中取得子项数据
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

