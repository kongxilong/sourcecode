// ProdInfoExProp.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "ProdInfoExProp.h"
#include "afxdialogex.h"
#include "DataExportPropSheet.h"

// CProdInfoExProp 对话框

IMPLEMENT_DYNAMIC(CProdInfoExProp, CPropertyPage)

CProdInfoExProp::CProdInfoExProp()
	: CPropertyPage(CProdInfoExProp::IDD)
{

}

CProdInfoExProp::~CProdInfoExProp()
{
}


void CProdInfoExProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROD, m_list);
	DDX_Control(pDX, IDC_SPLIT_OPER2, m_split);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
}


BEGIN_MESSAGE_MAP(CProdInfoExProp, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_ProdInfoExProp, &CProdInfoExProp::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_ProdInfoExProp, &CProdInfoExProp::OnEndTask)
	ON_COMMAND(ID__CHOOSE_ALL, &CProdInfoExProp::OnChooseAll)
	ON_COMMAND(ID_CHOOSE_NONE, &CProdInfoExProp::OnChooseNone)
	ON_COMMAND(ID_REVERSE_CHOOSE, &CProdInfoExProp::OnReverseChoose)
	ON_COMMAND(ID_DEL_FROM_LIST, &CProdInfoExProp::OnDelFromList)
	ON_COMMAND(ID_CLEAR_LIST, &CProdInfoExProp::OnClearList)
	ON_BN_CLICKED(IDC_BTN_DATAEXPORT, &CProdInfoExProp::OnBnClickedBtnDataExport)
	ON_BN_CLICKED(IDC_BTN_PRODSEARCH, &CProdInfoExProp::OnBnClickedBtnProdsearch)
	ON_MESSAGE(WM_HANDLE_BATCH_ProdEx, &CProdInfoExProp::OnHandleBatch)
END_MESSAGE_MAP()


// CProdInfoExProp 消息处理程序


HBRUSH CProdInfoExProp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CProdInfoExProp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitSplit();
	InitList();
	InitTipCtrl();
	return TRUE;
}


void CProdInfoExProp::InitSplit()
{
	m_split.SetDropDownMenu(IDM_PROD_EX_OPERATION,0);
}

void CProdInfoExProp::InitList()
{
	m_list.SetBkColor(CONCTRLBKCOLOR);
	m_list.SetExtendedStyle( m_list.GetExtendedStyle()| LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	
	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("产品名称"), LVCFMT_LEFT, rect.Width()/4);    
    m_list.InsertColumn(1, _T("产品类型"), LVCFMT_LEFT, rect.Width()/4);
	m_list.InsertColumn(2, _T("供应商"), LVCFMT_LEFT, rect.Width()/4);
    m_list.InsertColumn(3, _T("备注"), LVCFMT_LEFT, rect.Width()/4);
	
}


void CProdInfoExProp::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	font.Detach();
}

CString CProdInfoExProp::GetItemFromArray(CString source,int index)     //从字符串中取得子项数据
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

void CProdInfoExProp::EnableParentDlgShutDownBtn(BOOL Enable)
{
	CDataExportPropSheet * p_parentDlg = (CDataExportPropSheet *)GetParent();
	//TRUE表示可用 FALSE表示不可用   
	p_parentDlg->SetCloseBtnEable(Enable);
}

void CProdInfoExProp::TransferStrToArr(CString source,CStringArray &des)    //将字符串解开  变成array形式的数据
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

void CProdInfoExProp::OnTimer(UINT_PTR nIDEvent)
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

void CProdInfoExProp::ShowTip()
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

afx_msg LRESULT CProdInfoExProp::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(FALSE);
	//SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CProdInfoExProp::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	EnableParentDlgShutDownBtn(TRUE);
	//KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


void CProdInfoExProp::OnChooseAll()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int j = 0; j<count;j++)
	{
		m_list.SetCheck(j,TRUE);
	}
}


void CProdInfoExProp::OnChooseNone()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int j = 0; j<count;j++)
	{
		
		m_list.SetCheck(j,FALSE);
		
	}
}


void CProdInfoExProp::OnReverseChoose()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int j = 0; j<count;j++)
	{
		if(m_list.GetCheck(j))
		{
			m_list.SetCheck(j,FALSE);
		}else{
			m_list.SetCheck(j,TRUE);
		}
	}
}


void CProdInfoExProp::OnDelFromList()
{
	// TODO: 在此添加命令处理程序代码
	int count = m_list.GetItemCount();
	for (int j = 0; j<count;j++)
	{
		if(m_list.GetCheck(j))
		{
			m_list.DeleteItem(j);
		}
	}
}


void CProdInfoExProp::OnClearList()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
}


void CProdInfoExProp::OnBnClickedBtnDataExport()
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
		MessageBox(_T("当前没有要导出的产品数据，请选择有效的产品数据"));
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
				CString sql = _T("select * from [产品信息表$]");
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
				CString _sqlcreatTable;
				_sqlcreatTable = _T("CREATE TABLE 产品信息表(产品名称 char(25), 产品类型 char(50),供应商 char(50),备注 char(200));");
				
				if(!ado.ExecuteSQL(_sqlcreatTable))
				{
					MessageBox(_T("创建产品信息表格失败"));
					ado.ExitConnect();
					return;
				}
			}
			CString sql = _T("select * from [产品信息表$]");
			ado.m_pRecordset = ado.GetRecordSet(sql);
			if(ado.m_pRecordset == NULL)
			{
				MessageBox(_T("读取产品信息表格失败"));
				ado.ExitConnect();
				return;
			}
			CString _sql = NULL;
			for(int i = 0; i< size;i++)
			{
				int index = atoi(T2A(empArr.GetAt(i)));
				CString prod_name = m_list.GetItemText(index,0);
				CString prod_type = m_list.GetItemText(index,1);
				CString prod_factory = m_list.GetItemText(index,2);
				CString prod_ps = m_list.GetItemText(index,3);
				try{
					ado.m_pRecordset->AddNew();
					ado.m_pRecordset->PutCollect("产品名称",(const _variant_t)prod_name);
					ado.m_pRecordset->PutCollect("产品类型",(const _variant_t)prod_type);
					ado.m_pRecordset->PutCollect("供应商",(const _variant_t)prod_factory);
					ado.m_pRecordset->PutCollect("备注",(const _variant_t)prod_ps);		
					ado.m_pRecordset->Update();
				}catch(_com_error e)
				{
					CString erro;
					erro.Format(_T("导出产品记录集失败，错误信息是：%s。错误描述是：%s"),e.ErrorMessage(),(LPCTSTR)e.Description());
					MessageBox(erro);
					ado.ExitConnect();
					return;
				}
				
			}
			ado.ExitConnect();
			m_tip.SetWindowText(_T("导出产品数据成功"));
		}else
		{
			return ;
		}
	}else
	{
		return;
	}
}


void CProdInfoExProp::OnBnClickedBtnProdsearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CSock::StartReqFindProd(this);
}


afx_msg LRESULT CProdInfoExProp::OnHandleBatch(WPARAM wParam, LPARAM lParam)
{
	FillDataToList();
	return 0;
}


void CProdInfoExProp::FillDataToList()
{
	m_tip.SetWindowText(_T("正在处理从数据库中读到的数据，请耐心等待。。"));
	CString _str = m_prod_batch;
	m_prod_batch.Empty();
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
				for(int j = 0;j<4;j++)
				{
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