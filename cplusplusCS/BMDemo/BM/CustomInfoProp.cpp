// CustomInfoProp.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "CustomInfoProp.h"
#include "afxdialogex.h"
#include "DataImportPropSheet.h"

// CCustomInfoProp 对话框

IMPLEMENT_DYNAMIC(CCustomInfoProp, CPropertyPage)

CCustomInfoProp::CCustomInfoProp()
	: CPropertyPage(CCustomInfoProp::IDD)
{

}

CCustomInfoProp::~CCustomInfoProp()
{
}

void CCustomInfoProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SPLIT_OPERATION, m_split);
	DDX_Control(pDX, IDC_SPLIT_OPERATION, m_spOperation);
	DDX_Control(pDX, IDC_LIST_CTMINFO, m_ctmInfoList);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CCustomInfoProp, CPropertyPage)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CTMINFO, &CCustomInfoProp::OnLvnItemchangedListCtminfo)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_CHOOSE_ALL, &CCustomInfoProp::OnChooseAll)
	ON_COMMAND(ID_CHOOSE_NONE, &CCustomInfoProp::OnChooseNone)
	ON_COMMAND(ID_REVERSE_CHOOSE, &CCustomInfoProp::OnReverseChoose)
	ON_COMMAND(ID_DEL_FROM_LIST, &CCustomInfoProp::OnDelFromList)
	ON_COMMAND(ID_CLEAR_LIST, &CCustomInfoProp::OnClearList)
	ON_BN_CLICKED(IDC_BTN_DATAIMPORT, &CCustomInfoProp::OnBnClickedBtnDataImport)
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_CtmIm, &CCustomInfoProp::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_CtmIM, &CCustomInfoProp::OnEndTask)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, &CCustomInfoProp::OnBnClickedBtnSubmit)
END_MESSAGE_MAP()



// CCustomInfoProp 消息处理程序

HBRUSH CCustomInfoProp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

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

BOOL CCustomInfoProp::OnInitDialog()
{
	    
    //m_ctmInfoList.GetClientRect(&rect); 
	CPropertyPage::OnInitDialog();
	InitList();
	SetSplitCtrl();
	InitTipCtrl();
	return TRUE;
}

void CCustomInfoProp::InitList(){
	//DWORD listStyle = m_ctmInfoList.GetExtendedStyle();
	// 为列表视图控件添加全行选中和栅格风格 
	m_ctmInfoList.SetBkColor(CONCTRLBKCOLOR);
	m_ctmInfoList.SetExtendedStyle( m_ctmInfoList.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctmInfoList.SetTextColor(RGB(139, 90, 43));
	m_ctmInfoList.SetTextBkColor(RGB(255, 236, 139));
	
	m_ctmInfoList.InsertColumn(0, _T("客户编号"), LVCFMT_LEFT, 75);    
    m_ctmInfoList.InsertColumn(1, _T("客户名称"), LVCFMT_LEFT, 75);
	m_ctmInfoList.InsertColumn(2, _T("顾客姓名"), LVCFMT_LEFT, 75);
    m_ctmInfoList.InsertColumn(3, _T("性别"), LVCFMT_LEFT, 50);
	m_ctmInfoList.InsertColumn(4, _T("电话"), LVCFMT_LEFT, 100);
	m_ctmInfoList.InsertColumn(5, _T("次数"), LVCFMT_LEFT, 50);    
    m_ctmInfoList.InsertColumn(6, _T("金额"), LVCFMT_LEFT, 100);
	m_ctmInfoList.InsertColumn(7, _T("首次消费日期"), LVCFMT_LEFT, 100);
    m_ctmInfoList.InsertColumn(8, _T("最近消费日期"), LVCFMT_LEFT, 100);
	m_ctmInfoList.InsertColumn(9, _T("备注"), LVCFMT_LEFT, 300);
}

void CCustomInfoProp::SetSplitCtrl() {
	m_spOperation.SetDropDownMenu(IDM_CTM_IM_OPERATION,0);
}

void CCustomInfoProp::OnChooseAll()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_ctmInfoList.GetItemCount();
	for(int i=0;i<count;i++)
	{
		m_ctmInfoList.SetCheck(i,TRUE);
	}
}


void CCustomInfoProp::OnChooseNone()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_ctmInfoList.GetItemCount();
	for(int i=0;i<count;i++)
	{
		m_ctmInfoList.SetCheck(i,FALSE);
	}
}


void CCustomInfoProp::OnReverseChoose()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_ctmInfoList.GetItemCount();
	for(int i=0;i<count;i++)
	{
		if(m_ctmInfoList.GetCheck(i))
		{
			m_ctmInfoList.SetCheck(i,FALSE);
		}else{
			m_ctmInfoList.SetCheck(i,TRUE);
		}
		
	}
}


void CCustomInfoProp::OnDelFromList()
{
	// TODO: 在此添加命令处理程序代码
	CStringArray arr;
	arr.RemoveAll();
	int count = m_ctmInfoList.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		if(m_ctmInfoList.GetCheck(i))
		{
			arr.Add(m_ctmInfoList.GetItemText(i,0));
		}
	}

	int size = arr.GetSize();
	for(int i = 0;i<size;i++)
	{
		int cur_count = m_ctmInfoList.GetItemCount();
		for(int j= 0;j<cur_count;j++)
		{
			if(m_ctmInfoList.GetItemText(j,0) == arr.GetAt(i))
			{
				m_ctmInfoList.DeleteItem(j);
				break;
			}
		}
	}
}


void CCustomInfoProp::OnClearList()
{
	// TODO: 在此添加命令处理程序代码
	m_ctmInfoList.DeleteAllItems();
}


void CCustomInfoProp::OnBnClickedBtnDataImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle = _T("批量导入客户数据");
	fileDlg.m_ofn.lpstrFilter = _T("Excel 2003(*.xls)\0*.xls\0Excel 2007(*.xlsx)\0*.xlsx\0\0");
	if(IDOK == fileDlg.DoModal())
	{
		if(IDYES == MessageBox(_T("请确认Excel表格当前没有被打开再继续操作！"),_T("温馨提示"),MB_YESNO))
		{
			CString pathName = fileDlg.GetPathName();
			CString fileExt = fileDlg.GetFileExt();
			CADOExcel ado;
			ado.SetFileType(fileExt);
			ado.SetExcelFilePath(pathName);
			if(!ado.OnInitConnect())
			{
				CString err = _T("连接Excel表失败，可能原因如下：\r\n");
				err += _T("1、请确认执行此操作之前，已经安装了数据库连接引擎；\r\n"); 
				err += _T("2、请确认要连接的Excel表格已经处在关闭状态；\r\n");
				MessageBox(err);
				return;
			}
			try{
				CString importSQL = _T("select * from [客户信息表$]"); 
				ado.m_pRecordset = ado.GetRecordSet(importSQL);
				if(NULL == ado.m_pRecordset)
				{
					MessageBox(_T("请确认打开的Excel中有客户信息表，再重试"));
					ado.ExitConnect();
					return;
				}
			}
			catch(_com_error e)
			{
				CString erro;
				erro.Format(_T("获取客户记录集失败，错误信息是：%s。错误描述是：%s"),e.ErrorMessage(),(LPCTSTR)e.Description());
				MessageBox(erro);
				ado.ExitConnect();
				return;
			}
			int nCount = 0;
			m_ctmInfoList.DeleteAllItems();
			while(!ado.m_pRecordset->adoEOF)
			{
				try{
					//_variant_t var
					m_ctmInfoList.InsertItem(nCount,_T(""));//插入新的一行
					m_ctmInfoList.SetItemText(nCount,0,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("客户编号")));
					m_ctmInfoList.SetItemText(nCount,1,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("客户名称")));
					m_ctmInfoList.SetItemText(nCount,2,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("顾客姓名")));
					m_ctmInfoList.SetItemText(nCount,3,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("客户性别")));
					m_ctmInfoList.SetItemText(nCount,4,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("移动电话")));
					m_ctmInfoList.SetItemText(nCount,5,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("消费次数")));
					m_ctmInfoList.SetItemText(nCount,6,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("消费金额")));
					m_ctmInfoList.SetItemText(nCount,7,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("首次消费时间")));
					m_ctmInfoList.SetItemText(nCount,8,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("最近消费时间")));
					m_ctmInfoList.SetItemText(nCount,9,SafeReadDataFromExcel(ado.m_pRecordset->GetCollect("备注")));
				}catch(_com_error e)
				{
					CString erro;
					erro.Format(_T("导入员工记录集失败，错误信息是：%s。错误描述是：%s"),e.ErrorMessage(),(LPCTSTR)e.Description());
					MessageBox(erro);
					ado.ExitConnect();
					return;
				}
				ado.m_pRecordset->MoveNext();
			}
			ado.ExitConnect();
			m_tip.SetWindowText(_T("导入完毕"));
		}else{
			return;
		}
	}else
	{
		return;
	}
}

CString CCustomInfoProp::SafeReadDataFromExcel(_variant_t var)
{
	CString _str;
	_str.Empty();
	if(VT_NULL != var.vt)
	{
		_str = (LPCTSTR)(_bstr_t)var;
	}
	return  _str;
}

void CCustomInfoProp::OnTimer(UINT_PTR nIDEvent)
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
	CPropertyPage::OnTimer(nIDEvent);
}


afx_msg LRESULT CCustomInfoProp::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(FALSE);
	//SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}

void CCustomInfoProp::EnableParentDlgShutDownBtn(BOOL bEnble)
{	
	CDataImportPropSheet * p_parentDlg = (CDataImportPropSheet *)GetParent();
	//TRUE表示可用 FALSE表示不可用   
	p_parentDlg->SetCloseBtnEable(bEnble);
}

afx_msg LRESULT CCustomInfoProp::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(TRUE);
	//KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}

void CCustomInfoProp::ShowTip()
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
void CCustomInfoProp::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}

void CCustomInfoProp::OnBnClickedBtnSubmit()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	m_ctm_to_submit_arr.RemoveAll();

	int count = m_ctmInfoList.GetItemCount();
	for(int i = 0;i<count;i++)
	{
		CString temp;
		temp.Format(_T("提交失败，第%d行的数据不符合规范，请重新检查再次提交！"),i+1);
		if(m_ctmInfoList.GetCheck(i))
		{
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,0),_T("客户编号"),TRUE,TRUE,FALSE,25))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,1),_T("客户名称"),FALSE,FALSE,FALSE,25))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,2),_T("顾客姓名"),FALSE,FALSE,FALSE,25))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,3),_T("客户性别"),FALSE,FALSE,FALSE,10))
			{
				MessageBox(temp);
				return;
			}else{
				CString sex = m_ctmInfoList.GetItemText(i,3);
				if(sex != _T("男") && sex!= _T("女") && !sex.IsEmpty())
				{
					MessageBox(_T("客户性别非空的话 就必须为男或者女"));
					MessageBox(temp);
					return;
				}
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,4),_T("移动电话"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,5),_T("消费次数"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,6),_T("消费金额"),FALSE,TRUE,FALSE,20))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,7),_T("首次消费时间"),FALSE,FALSE,TRUE,30))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,8),_T("最近消费时间"),FALSE,FALSE,TRUE,30))
			{
				MessageBox(temp);
				return;
			}
			if(!IsItemValidity(m_ctmInfoList.GetItemText(i,9),_T("备注"),FALSE,FALSE,FALSE,300))
			{
				MessageBox(temp);
				return;
			}
			
			CString strTemp;
			strTemp.Format(_T("%d"),i);
			m_ctm_to_submit_arr.Add(strTemp);
		}
	}
	if(m_ctm_to_submit_arr.GetSize() == 0)
	{
		MessageBox(_T("提交失败，没有选中有效的数据"));
	}else{
		CSock::StartSubmitCtmInfo(this);
	}
}

BOOL CCustomInfoProp::IsItemValidity(CString _source,CString _item_name,BOOL bCheckEmpty,BOOL bCheckNum,BOOL bCheckDateFormat,int str_len_max)    //true  有效 否则无效  
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
			if(!IsNum(_source))
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
					if(!IsNum(left))
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
							if(IsNum(left)&&IsNum(str))
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
BOOL CCustomInfoProp::ContainsCharsRemain(CString str)     //检查字符串是否包含子字符串
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

BOOL CCustomInfoProp::IsNum(CString str)
{
	WT;
	char *p = T2A(str);
	int i = 0;
	int count = str.GetLength();
	while(count){
		if(p[i]>'9'||p[i]<'0')
		{
			if(p[i] != '.')
			{
				return FALSE;
			}
			
		}
		count--;
		i++;
	}
	return TRUE;
}