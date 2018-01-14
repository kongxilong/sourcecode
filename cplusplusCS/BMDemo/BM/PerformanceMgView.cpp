// PerformanceMgView.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "PerformanceMgView.h"


// CPerformanceMgView

IMPLEMENT_DYNCREATE(CPerformanceMgView, CFormView)

CPerformanceMgView::CPerformanceMgView()
	: CFormView(CPerformanceMgView::IDD)
	, m_keyword(_T(""))
{
	m_str_emp_performance_batch.Empty();
}

CPerformanceMgView::~CPerformanceMgView()
{
}

void CPerformanceMgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PERFORMANCE, m_list);
	DDX_Control(pDX, IDC_SPLIT_DALIY_OPERATION, m_split);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Text(pDX, IDC_EDIT_KEYKORD, m_keyword);
}

BEGIN_MESSAGE_MAP(CPerformanceMgView, CFormView)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CPerformanceMgView::OnBnClickedBtnSearch)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_EmpPerformance, &CPerformanceMgView::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_EmpPerformance, &CPerformanceMgView::OnEndTask)
	ON_MESSAGE(WM_FILLDATATOLIST_EmpPerformance, &CPerformanceMgView::OnFillDatatoList)
	ON_COMMAND(ID_DEDUCT_SET, &CPerformanceMgView::OnDeductSet)
	ON_COMMAND(ID_ADD_ALL_DEDUCT, &CPerformanceMgView::OnAddAllDeduct)
	ON_COMMAND(ID_GET_DEDUCT, &CPerformanceMgView::OnGetDeduct)
	ON_COMMAND(ID_CHOOSE_ALL, &CPerformanceMgView::OnChooseAll)
	ON_COMMAND(ID_CHOOSE_NONE, &CPerformanceMgView::OnChooseNone)
	ON_COMMAND(ID_REVERSE_CHOOSE, &CPerformanceMgView::OnReverseChoose)
	ON_COMMAND(ID_CLEAR_CUR_LIST, &CPerformanceMgView::OnClearCurList)
	ON_COMMAND(ID_DEL_FROM_LIST, &CPerformanceMgView::OnDelFromList)
END_MESSAGE_MAP()


// CPerformanceMgView 诊断

#ifdef _DEBUG
void CPerformanceMgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPerformanceMgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPerformanceMgView 消息处理程序

void CPerformanceMgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	InitList();
	InitTipCtrl();
	InitSplit();
}

void CPerformanceMgView::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	CString nEditCol = _T("4#6#");
	m_list.SetItemEdit(TRUE,nEditCol);
	CRect rect;
	m_list.GetClientRect(rect);
    m_list.InsertColumn(0, _T("员工姓名"), LVCFMT_LEFT, rect.Width()/7);
	m_list.InsertColumn(1, _T("订单号"), LVCFMT_LEFT, rect.Width()/7);
    m_list.InsertColumn(2, _T("客户"), LVCFMT_LEFT, rect.Width()/7);
	m_list.InsertColumn(3, _T("提成内容"), LVCFMT_LEFT, rect.Width()/7);
	m_list.InsertColumn(4, _T("提成率"), LVCFMT_LEFT, rect.Width()/7);
	m_list.InsertColumn(5, _T("总价"), LVCFMT_LEFT, rect.Width()/7);
	m_list.InsertColumn(6, _T("提成金额"), LVCFMT_LEFT, rect.Width()/7); 
   
}

void CPerformanceMgView::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(!IsItemValidity(m_keyword,_T("员工姓名"),TRUE,FALSE,FALSE,25))
	{
		m_keyword = _T("");
		UpdateData(FALSE);
		return ;
	}
	CSock::StartFindEmpPerformance(this);
}


HBRUSH CPerformanceMgView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		pDC->SetBkMode(TRANSPARENT);  
		//hbr=(HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH b_static=CreateSolidBrush(BACKGROUNDCOLOR);	
		return b_static;	
	}
	if(nCtlColor == CTLCOLOR_DLG){
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(BACKGROUNDCOLOR); 
		HBRUSH b=CreateSolidBrush(BACKGROUNDCOLOR); 
		return b;
	}
	return hbr;
}


void CPerformanceMgView::OnTimer(UINT_PTR nIDEvent)
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
	CFormView::OnTimer(nIDEvent);
}

void CPerformanceMgView::InitSplit()
{
	m_split.SetDropDownMenu(IDM_EMP_PERFORMANCE,0);
}
void CPerformanceMgView::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}

void CPerformanceMgView::ShowTip()
{
	m_time_take++;
	CString time =NULL;
	time.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(time);
	if(m_time_take == 120)
	{
		OnEndTask(0,0);
		m_tip.SetWindowText(_T("请求超时，请重试或者重新连接服务器"));
	}
}

afx_msg LRESULT CPerformanceMgView::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CPerformanceMgView::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CPerformanceMgView::OnFillDatatoList(WPARAM wParam, LPARAM lParam)
{
	FillDataToList();
	return 0;
}


void CPerformanceMgView::FillDataToList()
{
	m_tip.SetWindowText(_T("正在处理从数据库中读到的数据，请耐心等待。。"));
	CString _str = m_str_emp_performance_batch;
	m_str_emp_performance_batch.Empty();
	if(_str.IsEmpty())
	{
		m_tip.SetWindowText(_T("没有有效数据返回"));
		CString temp = _T("没有读到有效的数据，原因可能有：\r\n");
		temp += _T("1:数据库中没有关于该员工的订单数据；\r\n");
		temp += _T("2:数据库中还没有订单数据；\r\n");
		temp += _T("3:如果确认有有效数据并且失败，请重试或联系管理员；\r\n");
		MessageBox(temp);
	}else
	{
		CStringArray _str_arr;
		_str_arr.RemoveAll();
		TransferStrToArr(_str,_str_arr);
		if(_str_arr.IsEmpty())
		{
			m_tip.SetWindowText(_T("没有有效数据返回"));
			CString temp = _T("没有读到有效的数据，原因可能有：\r\n");
			temp += _T("1:数据库中没有关于该员工的订单数据；\r\n");
			temp += _T("2:数据库中还没有订单数据；\r\n");
			temp += _T("3:如果确认有有效数据并且失败，请重试或联系管理员；\r\n");
			MessageBox(temp);
		}else{
			int size = _str_arr.GetSize();
			m_list.DeleteAllItems();
			for(int i = 0;i<size;i++)
			{
				m_list.InsertItem(i,_T(""));//插入新的一行
				CString _str = _str_arr.GetAt(i);
				for(int j = 0;j < 5;j++)
				{
					if(j == 4)
					{
						m_list.SetItemText(i,j+1,GetItemFromArray(_str,j+1));
						break;
					}
					m_list.SetItemText(i,j,GetItemFromArray(_str,j+1));
				}
			}
			int _count = m_list.GetItemCount();
			CString _str_message;
			_str_message.Format(_T("共返回%d条记录"),_count);
			m_tip.SetWindowText(_str_message);
		}
	}
}

void CPerformanceMgView::TransferStrToArr(CString source,CStringArray &des)    //将字符串解开  变成array形式的数据
{
	CString strSource = source;
	des.RemoveAll();
	CString sub_str_start = _T("{#");
	CString sub_str_end = _T("#}");
	CString temp = NULL;
	while(TRUE)
	{
		int index = strSource.Find(sub_str_start);
		if(-1 == index)   //表示没找到 意味着 结束了
		{
			break;
		}else{
			strSource = strSource.Mid(index+2);
			index = strSource.Find(sub_str_end);
			temp = strSource.Mid(0,index);
			des.Add(temp);
			temp.Empty();
			strSource = strSource.Mid(index+2);
		}
	}
}

CString CPerformanceMgView::GetItemFromArray(CString source,int index)     //从字符串中取得子项数据
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


BOOL CPerformanceMgView::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max)    //true  有效 否则无效  
	//检查项是否为空 是否是数字 以及长度是否超过限制  如果不要检查是否是数字的话 那么就校验是否包含特殊字符  最后还有是否符合日期格式
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
			if(!CSock::IsNum(_source))
			{
				CString err = _item_name+_T("必须为数字");
				MessageBox(err);
				return FALSE;
			}
		}
		if(CSock::ContainsCharsRemain(_source))
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
					if(!CSock::IsNum(left))
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
							if(CSock::IsNum(left)&&CSock::IsNum(str))
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

void CPerformanceMgView::OnDeductSet()
{
	// TODO: 在此添加命令处理程序代码
	m_deductSetDlg.DoModal();
}


void CPerformanceMgView::OnAddAllDeduct()
{
	// TODO: 在此添加命令处理程序代码
	WT;
	int count = m_list.GetItemCount();
	int nChoose = 0;
	float total = 0;
	for(int i = 0;i<count;i++)
	{
		if(m_list.GetCheck(i))
		{
			nChoose++;
			if(m_list.GetItemText(i,6).IsEmpty())   //判断提成率
			{
				continue;
			}else{
				CString str = m_list.GetItemText(i,6);
				if(!CSock::IsNum(str))
				{
					CString temp ;
					temp.Format(_T("第%d行第6列提成金额不是数字"),i+1);
					AfxMessageBox(temp);
					return ;
				}else{
					float f = atof(T2A(str));
					total += f;
					
				}
			}
		}
		if(i == (count-1))
		{
			CString tip ;
			tip.Format(_T("当前共有%d个选中项,提成金额共计%0.2f。"),nChoose,total);
			AfxMessageBox(tip);
			return ;
		}
	}
	if(nChoose == 0)
	{
		CString temp =_T("当前没有选中行，请选中要计算提成金额的行");
		
		AfxMessageBox(temp);
		return ;
	}
	
}


void CPerformanceMgView::OnGetDeduct()
{
	// TODO: 在此添加命令处理程序代码
	WT;
	int count = m_list.GetItemCount();
	int nChoose = 0;
	for (int i = 0;i<count;i++)
	{
		if(m_list.GetCheck(i))
		{
			nChoose++;
			if(m_list.GetItemText(i,4).IsEmpty()||m_list.GetItemText(i,5).IsEmpty())   //判断提成率
			{
				continue;
			}else{
				CString str = m_list.GetItemText(i,4);   //提成率
				//CString find;
				//find.Format(_T("%s"),'%');
				int index = str.Find('%');
				if(index != -1)
				{
					CString temp = str.Mid(0,index);
					if(!CSock::IsNum(temp))
					{
						temp.Format(_T("第%d行第4列数据不合法，合法格式如“5%”"),i+1);
						AfxMessageBox(temp);
						return ;
					}else{
						float a = atof(T2A(temp))/100;
						float b = atof(T2A(m_list.GetItemText(i,5)));
						temp.Format(_T("%0.2f"),a*b);
						m_list.SetItemText(i,6,temp);
					}
				}else{
					CString temp;
					temp.Format(_T("第%d行第4列数据提成率不符合格式规范，合法格式如“5%s”"),i+1,_T("%"));
					AfxMessageBox(temp);
					return ;
				}
			}
		}
	}
	if(nChoose == 0)
	{
		CString temp =_T("当前没有选中行，请选中要计算提成金额的行");
		
		AfxMessageBox(temp);
		return ;
	}
}


void CPerformanceMgView::OnChooseAll()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int i = 0;i<count;i++)
	{
		m_list.SetCheck(i,TRUE);
	}
}


void CPerformanceMgView::OnChooseNone()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int i = 0;i<count;i++)
	{
		
		m_list.SetCheck(i,FALSE);
		
	}
}


void CPerformanceMgView::OnReverseChoose()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int i = 0;i<count;i++)
	{
		if(m_list.GetCheck(i))
		{
			m_list.SetCheck(i,FALSE);
		}else{
			m_list.SetCheck(i,TRUE);
		}
	}
}


void CPerformanceMgView::OnClearCurList()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
}


void CPerformanceMgView::OnDelFromList()
{
	// TODO: 在此添加命令处理程序代码
	CStringArray arr;
	arr.RemoveAll();
	int count = m_list.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		if(m_list.GetCheck(i))
		{
			arr.Add(m_list.GetItemText(i,0));
		}
	}

	int size = arr.GetSize();
	for(int i = 0;i<size;i++)
	{
		int cur_count = m_list.GetItemCount();
		for(int j= 0;j<cur_count;j++)
		{
			if(m_list.GetItemText(j,0) == arr.GetAt(i))
			{
				m_list.DeleteItem(j);
				break;
			}
		}
	}
}
