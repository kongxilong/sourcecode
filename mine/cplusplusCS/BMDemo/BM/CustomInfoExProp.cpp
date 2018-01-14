// CustomInfoExProp.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "CustomInfoExProp.h"
#include "afxdialogex.h"
#include "DataExportPropSheet.h"

// CCustomInfoExProp 对话框

IMPLEMENT_DYNAMIC(CCustomInfoExProp, CPropertyPage)

CCustomInfoExProp::CCustomInfoExProp()
	: CPropertyPage(CCustomInfoExProp::IDD)
{

}

CCustomInfoExProp::~CCustomInfoExProp()
{
}

void CCustomInfoExProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CTMINFO, m_list);
	DDX_Control(pDX, IDC_SPLIT_OPERATION, m_split);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}



BEGIN_MESSAGE_MAP(CCustomInfoExProp, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_CustomExProp, &CCustomInfoExProp::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_CustomExProp, &CCustomInfoExProp::OnEndTask)
	ON_COMMAND(ID_CHOOSE_ALLL, &CCustomInfoExProp::OnChooseAlll)
	ON_COMMAND(ID_CHOOSE_NONE, &CCustomInfoExProp::OnChooseNone)
	ON_COMMAND(ID_REVERSE_CHOOSE, &CCustomInfoExProp::OnReverseChoose)
	ON_COMMAND(ID_DEL_FROM_LIST, &CCustomInfoExProp::OnDelFromList)
	ON_COMMAND(ID_CLEAR_LIST, &CCustomInfoExProp::OnClearList)
	ON_BN_CLICKED(IDC_BTN_FIND_ALLCTM, &CCustomInfoExProp::OnBnClickedBtnFindAllctm)
	ON_MESSAGE(WM_HANDLE_BATCH_CTM_CtmEx, &CCustomInfoExProp::OnHandleBatchCtm)
	ON_BN_CLICKED(IDC_BTN_DATAEXPORT, &CCustomInfoExProp::OnBnClickedBtnDataExport)
END_MESSAGE_MAP()


// CCustomInfoExProp 消息处理程序


HBRUSH CCustomInfoExProp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CCustomInfoExProp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitList();
	InitSplit();
	InitTipCtrl();
	return TRUE;
}

void CCustomInfoExProp::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}

void CCustomInfoExProp::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	m_list.InsertColumn(0, _T("客户编号"), LVCFMT_LEFT, 100);    
    m_list.InsertColumn(1, _T("客户名称"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("顾客姓名"), LVCFMT_LEFT, 100);
    m_list.InsertColumn(3, _T("性别"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(4, _T("电话"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, _T("次数"), LVCFMT_LEFT, 100);    
    m_list.InsertColumn(6, _T("金额"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(7, _T("首次消费日期"), LVCFMT_LEFT, 100);
    m_list.InsertColumn(8, _T("最近消费日期"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(9, _T("备注"), LVCFMT_LEFT, 100);

}

void CCustomInfoExProp::InitSplit()
{
	m_split.SetDropDownMenu(IDM_CTM_EX_OPERATION,0);

}

void CCustomInfoExProp::ShowTip()
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

void CCustomInfoExProp::OnTimer(UINT_PTR nIDEvent)
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

void CCustomInfoExProp::EnableParentDlgShutDownBtn(BOOL Enable)
{
	CDataExportPropSheet * p_parentDlg = (CDataExportPropSheet *)GetParent();
	//TRUE表示可用 FALSE表示不可用   
	p_parentDlg->SetCloseBtnEable(Enable);
}

afx_msg LRESULT CCustomInfoExProp::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(FALSE);
	//SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CCustomInfoExProp::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(TRUE);
	//KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}

void CCustomInfoExProp::TransferStrToArr(CString source,CStringArray &des)    //将字符串解开  变成array形式的数据
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

CString CCustomInfoExProp::GetItemFromArray(CString source,int index)     //从字符串中取得子项数据
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


void CCustomInfoExProp::OnChooseAlll()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int i = 0;i<count;i++)
	{
		m_list.SetCheck(i,TRUE);
		
	}
}


void CCustomInfoExProp::OnChooseNone()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int i = 0;i<count;i++)
	{
		m_list.SetCheck(i,FALSE);
		
	}
}


void CCustomInfoExProp::OnReverseChoose()
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


void CCustomInfoExProp::OnDelFromList()
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


void CCustomInfoExProp::OnClearList()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
}


void CCustomInfoExProp::OnBnClickedBtnFindAllctm()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::StartFindCtmForEx(this);
}


afx_msg LRESULT CCustomInfoExProp::OnHandleBatchCtm(WPARAM wParam, LPARAM lParam)
{
	FillDataToList();

	return 0;
}

void CCustomInfoExProp::FillDataToList()
{
	m_tip.SetWindowText(_T("正在处理从数据库中读到的数据，请耐心等待。。"));
	CString _str = m_ctm_batch_from_db;
	m_ctm_batch_from_db.Empty();
	if(_str.IsEmpty())
	{
		m_tip.SetWindowText(_T("没有数据"));
		CString temp = _T("没有读到有效的数据，原因可能有：");
		temp += _T("\r\n1:数据库中没有客户数据；");
		temp += _T("\r\n2:从数据库中读取客户数据失败，请重试或联系管理员；\r\n");
		MessageBox(temp);
	}else
	{
		CStringArray _str_arr;
		_str_arr.RemoveAll();
		TransferStrToArr(_str,_str_arr);
		if(_str_arr.IsEmpty())
		{
			m_tip.SetWindowText(_T("没有数据"));
			CString temp = _T("没有读到有效的数据，原因可能有：");
			temp += _T("\r\n1:数据库中没有客户数据；");
			temp += _T("\r\n2:从数据库中读取客户数据失败，请重试或联系管理员；\r\n");
			MessageBox(temp);
		}else{
			int size = _str_arr.GetSize();
			m_list.DeleteAllItems();
			for(int i = 0;i<size;i++)
			{
				m_list.InsertItem(i,_T(""));//插入新的一行
				CString  _str = _str_arr.GetAt(i);
				for(int j = 0;j<10;j++)
				{
					m_list.SetItemText(i,j,GetItemFromArray(_str,j+1));
				}
			}
			int _count = m_list.GetItemCount();
			CString _str_message;
			_str_message.Format(_T("共查询到%d条客户记录"),_count);
			m_tip.SetWindowText(_str_message);
		}
	}
}

void CCustomInfoExProp::OnBnClickedBtnDataExport()
{
	// TODO: 在此添加控件通知处理程序代码
	WT;
	CStringArray empArr;
	empArr.RemoveAll();
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{	
			CString num = NULL;
			num	= m_list.GetItemText(i,0);
			if(!num.IsEmpty())
			{
				CString _strTemp;
				_strTemp.Format(_T("%d"),i);
				empArr.Add(_strTemp);
			}
		}
	}
	int size = empArr.GetSize();
	if(size == 0)
	{
		MessageBox(_T("当前没有要导出的客户数据，请选择有效的客户数据"));
		return;
	}
	if(IDYES == MessageBox(_T("请先新建一个空表，不要做任何操作，新建表格后关闭表格或者事先已经有一个空表格！确认后再继续"),_T("温馨提示"),MB_YESNO))
	{
		//CFileDialog fileDlg(FALSE, _T("xls"), _T("员工信息表"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL,NULL );
		CFileDialog fileDlg(TRUE);
		fileDlg.m_ofn.lpstrTitle = _T("请选择一个空表格");
		fileDlg.m_ofn.lpstrFilter = _T("Excel 2003(*.xls)\0*.xls\0Excel 2007(*.xlsx)\0*.xlsx\0\0");
		fileDlg.m_ofn.lpstrDefExt = _T("xls");
		//fileDlg.m_ofn.lpstrFile = _T("员工信息表");
		if(IDOK == fileDlg.DoModal())
		{
			//CFile file(fileDlg.GetPathName(),CFile::modeCreate | CFile::modeReadWrite);
			//file.Close();
			CString pathName = fileDlg.GetPathName();
			CString fileExt = fileDlg.GetFileExt();
			CADOExcel ado;
			ado.SetFileType(fileExt);
			ado.SetExcelFilePath(pathName);
			CFileFind find;
			BOOL  flag = find.FindFile(pathName);
			if(!flag)
			{
				AfxMessageBox(_T("该文件不存在"));
				return ;
			}
			if(!ado.OnInitConnect())
			{
				CString err;
				err = _T("错误可能原因:\r\n");
				err += _T("1、请确认已经安装了数据库引擎\r\n");
				err += _T("2、文件未关闭，请尝试先关闭文件，再导出资料；\r\n");
				err += _T("3、如果还不成功，请联系管理员；\r\n");
				MessageBox(err);
				return;
			}

			try
			{
				CString sql = _T("select * from [客户信息表$]");
				_bstr_t bstrSQL = (_bstr_t)sql;

				if(ado.m_pConnection==NULL)
				{
					ado.OnInitConnect();
				}
				ado.m_pRecordset.CreateInstance(_uuidof(Recordset));
				ado.m_pRecordset->Open(bstrSQL,ado.m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
			}
			catch(_com_error e)
			{
				CString creatTable;
				creatTable = _T("CREATE TABLE 客户信息表(客户编号 char(50), 顾客姓名 char(50),客户名称 char(50),客户性别 char(10),移动电话 char(20),消费次数 char(20),消费金额 char(20),首次消费时间 char(30),最近消费时间 char(30),备注 char(250));");
				
				if(!ado.ExecuteSQL(creatTable))
				{
					MessageBox(_T("创建客户信息表格失败"));
					ado.ExitConnect();
					return;
				}
			}
			CString sql = _T("select * from [客户信息表$]");
			ado.m_pRecordset = ado.GetRecordSet(sql);
			if(ado.m_pRecordset == NULL)
			{
				MessageBox(_T("读取客户信息表格失败"));
				ado.ExitConnect();
				return;
			}
			CString _sql = NULL;
			for(int i = 0; i< size;i++)
			{
				int index = atoi(T2A(empArr.GetAt(i)));
				CString ctm_num = m_list.GetItemText(index,0);
				CString ctm_name = m_list.GetItemText(index,1);
				CString ctm_call = m_list.GetItemText(index,2);
				CString ctm_sex = m_list.GetItemText(index,3);
				CString ctm_phone = m_list.GetItemText(index,4);
				CString ctm_pay_time = m_list.GetItemText(index,5);
				CString ctm_pay_total = m_list.GetItemText(index,6);
				CString ctm_time_first_pay = m_list.GetItemText(index,7);
				CString ctm_time_late_pay = m_list.GetItemText(index,8);
				CString ctm_ps = m_list.GetItemText(index,9);
				/*
				CString temp;
				temp.Format(_T("INSERT INTO [客户信息表$](客户编号, 顾客姓名,客户名称,客户性别,移动电话,消费次数,消费金额,首次消费时间,最近消费时间,备注)VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s');"),
					ctm_num,
					ctm_name,
					ctm_call,
					ctm_sex,
					ctm_phone,
					ctm_pay_time,
					ctm_pay_total,
					ctm_time_first_pay,
					ctm_time_late_pay,
					ctm_ps);
				_sql += temp;
				*/
				try{
					ado.m_pRecordset->AddNew();
					ado.m_pRecordset->PutCollect("客户编号",(const _variant_t)ctm_num);
					ado.m_pRecordset->PutCollect("顾客姓名",(const _variant_t)ctm_name);
					ado.m_pRecordset->PutCollect("客户名称",(const _variant_t)ctm_call);
					ado.m_pRecordset->PutCollect("客户性别",(const _variant_t)ctm_sex);
					ado.m_pRecordset->PutCollect("移动电话",(const _variant_t)ctm_phone);
					ado.m_pRecordset->PutCollect("消费次数",(const _variant_t)ctm_pay_time);	
					ado.m_pRecordset->PutCollect("消费金额",(const _variant_t)ctm_pay_total);
					ado.m_pRecordset->PutCollect("首次消费时间",(const _variant_t)ctm_time_first_pay);
					ado.m_pRecordset->PutCollect("最近消费时间",(const _variant_t)ctm_time_late_pay);
					ado.m_pRecordset->PutCollect("备注",(const _variant_t)ctm_ps);		
					ado.m_pRecordset->Update();
				}catch(_com_error e)
				{
					CString erro;
					erro.Format(_T("导出员工记录集失败，错误信息是：%s。错误描述是：%s"),e.ErrorMessage(),(LPCTSTR)e.Description());
					MessageBox(erro);
					ado.ExitConnect();
					return;
				}
				
			}
			/*
			if(!ado.ExecuteSQL(_sql))
			{
				MessageBox(_T("导出数据失败，请对照步骤重试"));
				return;
			}*/
			ado.ExitConnect();
			m_tip.SetWindowText(_T("导出员工数据成功"));
		}else
		{
			return ;
		}
	}else
	{
		return;
	}
}
