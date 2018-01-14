// OrderIncompletedProgView.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "OrderIncompletedProgView.h"


// COrderIncompletedProgView

IMPLEMENT_DYNCREATE(COrderIncompletedProgView, CFormView)

COrderIncompletedProgView::COrderIncompletedProgView()
	: CFormView(COrderIncompletedProgView::IDD)
{

}

COrderIncompletedProgView::~COrderIncompletedProgView()
{
}

void COrderIncompletedProgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ORDER_INCOMPLETED_PROG, m_list);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Control(pDX, IDC_SPLIT_OPERATION, m_split);
}

BEGIN_MESSAGE_MAP(COrderIncompletedProgView, CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_OrderIncomplete, &COrderIncompletedProgView::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_OrderIncomplete, &COrderIncompletedProgView::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_FIND_DEAL_INCOMPLETE, &COrderIncompletedProgView::OnBnClickedBtnFindDealIncomplete)
	ON_MESSAGE(WM_FILLLIST_OrderIncomplete, &COrderIncompletedProgView::OnFillList)
	ON_BN_CLICKED(IDC_BTN_SAVE, &COrderIncompletedProgView::OnBnClickedBtnSave)
	ON_COMMAND(ID_CHOOSE_ALL, &COrderIncompletedProgView::OnChooseAll)
	ON_COMMAND(ID_REVERSE_CHOOSE, &COrderIncompletedProgView::OnReverseChoose)
	ON_COMMAND(ID_CHOOSE_NONE, &COrderIncompletedProgView::OnChooseNone)
	ON_COMMAND(ID_DEL_FROM_LIST, &COrderIncompletedProgView::OnDelFromList)
	ON_COMMAND(ID_CLEAR_LIST, &COrderIncompletedProgView::OnClearList)
END_MESSAGE_MAP()


// COrderIncompletedProgView 诊断

#ifdef _DEBUG
void COrderIncompletedProgView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COrderIncompletedProgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG




HBRUSH COrderIncompletedProgView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		pDC->SetBkColor(RGB(185,211,255)); 
		HBRUSH b=CreateSolidBrush(RGB(185,211,255)); 
		return b;
	}
	return hbr;
}

// COrderIncompletedProgView 消息处理程序
void COrderIncompletedProgView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	InitList();
	InitTipCtrl();
	InitSplit();
}

void COrderIncompletedProgView::InitSplit()
{
	m_split.SetDropDownMenu(IDM_ORDER_INCOMPLETE_OPERATION,0);
}

void COrderIncompletedProgView::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}
void COrderIncompletedProgView::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	CString editCol = _T("1#2#3#4#5#6#7#8#9#10#11#12#");   //这些列都可以编辑
	m_list.SetItemEdit(TRUE,editCol);

	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("订单编号"), LVCFMT_LEFT, rect.Width()/13);    
    m_list.InsertColumn(1, _T("订单进度"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(2, _T("下一步工作"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(3, _T("下一步工作预完成时间"), LVCFMT_LEFT, rect.Width()/13);
    m_list.InsertColumn(4, _T("订单总价"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(5, _T("已收金额"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(6, _T("客户名称"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(7, _T("顾客姓名"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(8, _T("顾客电话"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(9, _T("顾客地址"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(10, _T("负责人姓名"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(11, _T("负责人电话"), LVCFMT_LEFT, rect.Width()/13);
	m_list.InsertColumn(12, _T("备注"), LVCFMT_LEFT, rect.Width()/13); 
}

void COrderIncompletedProgView::OnTimer(UINT_PTR nIDEvent)
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

void COrderIncompletedProgView::ShowTip()
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


afx_msg LRESULT COrderIncompletedProgView::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT COrderIncompletedProgView::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


void COrderIncompletedProgView::FillDataToList()
{
	m_tip.SetWindowText(_T("正在处理从数据库中读到的数据，请耐心等待。。"));
	CString _str = m_list_IncompleteOrder;
	m_list_IncompleteOrder.Empty();
	if(_str.IsEmpty())
	{
		m_tip.SetWindowText(_T("没有数据"));
		CString temp = _T("没有读到有效的数据，原因可能有：\r\n");
		temp += _T("1:数据库中没有未完成的订单数据；\r\n");
		temp += _T("2:数据库中还没有订单数据；\r\n");
		temp += _T("3:如果确认有未完成的订单，那么从数据库中读取订单数据失败，请重试或联系管理员；\r\n");
		MessageBox(temp);
	}else
	{
		CStringArray _str_arr;
		_str_arr.RemoveAll();
		TransferStrToArr(_str,_str_arr);
		if(_str_arr.IsEmpty())
		{
			m_tip.SetWindowText(_T("没有数据"));
			CString temp = _T("没有读到有效的数据，原因可能有：\r\n");
			temp += _T("1:数据库中没有未完成的订单数据；\r\n");
			temp += _T("2:数据库中还没有订单数据；\r\n");
			temp += _T("3:如果确认有未完成的订单，那么从数据库中读取订单数据失败，请重试或联系管理员；\r\n");
			MessageBox(temp);
		}else{
			int size = _str_arr.GetSize();
			m_list.DeleteAllItems();
			for(int i = 0;i<size;i++)
			{
				m_list.InsertItem(i,_T(""));//插入新的一行
				CString _str = _str_arr.GetAt(i);
				for(int j = 0;j<13;j++)
				{
					m_list.SetItemText(i,j,GetItemFromArray(_str,j+1));
				}
			}
			int _count = m_list.GetItemCount();
			CString _str_message;
			_str_message.Format(_T("共返回%d条未完成订单记录"),_count);
			m_tip.SetWindowText(_str_message);
		}
	}
}


void COrderIncompletedProgView::TransferStrToArr(CString source,CStringArray &des)    //将字符串解开  变成array形式的数据
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

CString COrderIncompletedProgView::GetItemFromArray(CString source,int index)     //从字符串中取得子项数据
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

void COrderIncompletedProgView::OnBnClickedBtnFindDealIncomplete()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::FindDealIncomplete(this);
}


afx_msg LRESULT COrderIncompletedProgView::OnFillList(WPARAM wParam, LPARAM lParam)
{
	FillDataToList();
	return 0;
}


void COrderIncompletedProgView::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	m_OrderToUpdateArr.RemoveAll();

	int count = m_list.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		CString temp;
		temp.Format(_T("提交失败，第%d行的数据不符合规范，请重新检查再次提交！"),i+1);
		if(m_list.GetCheck(i))
		{
			if(!CSock::IsItemValidity(m_list.GetItemText(i,0),_T("订单编号"),TRUE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,1),_T("订单进度"),TRUE,FALSE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}else{
				if(!IsOrderProgVality(m_list.GetItemText(i,1)))
				{
					MessageBox(temp);
					return;
				}
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,2),_T("下一步工作"),FALSE,FALSE,FALSE,150))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,3),_T("下一步工作预完成时间"),FALSE,FALSE,TRUE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,4),_T("订单总价"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,5),_T("已收金额"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,6),_T("客户名称"),FALSE,FALSE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,7),_T("顾客姓名"),FALSE,FALSE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,8),_T("顾客电话"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,9),_T("顾客地址"),FALSE,FALSE,FALSE,50))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,10),_T("负责人姓名"),FALSE,FALSE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,11),_T("负责人电话"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!CSock::IsItemValidity(m_list.GetItemText(i,12),_T("备注"),FALSE,FALSE,FALSE,200))
			{
				MessageBox(temp);
				return;

			}
			
			CString strTemp;
			strTemp.Format(_T("%d"),i);
			m_OrderToUpdateArr.Add(strTemp);
		}
	}
	if(m_OrderToUpdateArr.GetSize() == 0)
	{
		MessageBox(_T("提交失败，没有选中有效的数据"));
	}else{
		CSock::StartUpdateOrderIncomplete(this);
	}
}


BOOL COrderIncompletedProgView::IsOrderProgVality(CString source)    //校验填入的订单进度是否合法
{
	if(source == _T("意向客户")
		|| source == _T("已订单")
		|| source == _T("已初测")
		|| source == _T("已签合同")
		|| source == _T("已确定最终方案")
		|| source == _T("已放样精量")
		|| source == _T("已发单")
		|| source == _T("已回单")
		|| source == _T("已安装")
		|| source == _T("已完成"))
	{
		return TRUE;
	}else{
		CString str = _T("订单进度必须为以下之一：\r\n");
		str += _T("1:意向客户\r\n");
		str += _T("2:已订单\r\n");
		str += _T("3:已初测\r\n");
		str += _T("4:已签合同\r\n");
		str += _T("5:已确定最终方案\r\n");
		str += _T("6:已放样精量\r\n");
		str += _T("7:已发单\r\n");
		str += _T("8:已回单\r\n");
		str += _T("9:已安装\r\n");
		str += _T("10:已完成\r\n");
		MessageBox(str);
		return FALSE;
	}
}

void COrderIncompletedProgView::OnChooseAll()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		m_list.SetCheck(i,TRUE);
	}
}


void COrderIncompletedProgView::OnReverseChoose()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		if(m_list.GetCheck(i))
		{
			m_list.SetCheck(i,FALSE);
		}else{
			m_list.SetCheck(i,TRUE);
		}
		
	}
}


void COrderIncompletedProgView::OnChooseNone()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		m_list.SetCheck(i,FALSE);
	}
}


void COrderIncompletedProgView::OnDelFromList()
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


void COrderIncompletedProgView::OnClearList()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
}
