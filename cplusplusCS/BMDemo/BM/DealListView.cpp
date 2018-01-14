// DealListView.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "DealListView.h"
#include "OrderEdit.h"

// CDealListView

IMPLEMENT_DYNCREATE(CDealListView, CFormView)

CDealListView::CDealListView()
	: CFormView(CDealListView::IDD)
	//, m_keyword(_T(""))
	, m_keywords(_T(""))
{
	m_ulFileSize = 0;
	m_time_take = 0;
	m_ulCurTotal = 0;
	m_time_take_dlFile = 0;
	m_bDLoading = FALSE;
}

CDealListView::~CDealListView()
{
}

void CDealListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIMECHOOSE, m_timeChoose);
	DDX_Control(pDX, IDC_DATETIME_DEADLINE, m_dateEnd);
	DDX_Control(pDX, IDC_DATETIME_START, m_dateStart);
	//DDX_Text(pDX, IDC_EDIT_KEYWORD, m_keyword);
	//  DDX_Control(pDX, IDC_LIST_DEAL, m_list);
	DDX_Control(pDX, IDC_LIST_DEAL, m_list);
	DDX_Control(pDX, IDC_SPLIT_OPERATION, m_split);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	DDX_Control(pDX, IDC_STATIC_DLSTATUS, m_dlStatus);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_progressCtrl);
	DDX_Text(pDX, IDC_EDIT_KEYWORD, m_keywords);
}

BEGIN_MESSAGE_MAP(CDealListView, CFormView)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CDealListView::OnBnClickedBtnEdit)
	ON_WM_CTLCOLOR()
	//ON_BN_CLICKED(IDC_BTN_SEARCH, &CDealListView::OnBnClickedBtnSearch)
	ON_COMMAND(ID_32814, &CDealListView::OnDownLoadFile)
	ON_WM_TIMER()
	ON_MESSAGE(WM_STARTTASK_DealListView, &CDealListView::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_DealListView, &CDealListView::OnEndTask)
	ON_MESSAGE(WM_RESETDLSTATUS_DealListView, &CDealListView::OnResetDLStatus)
	ON_MESSAGE(WM_VALID_FILEDL_DealListView, &CDealListView::OnValidFile)
	
	ON_BN_CLICKED(IDC_BTN_SEARCH_VIA_DATE, &CDealListView::OnBnClickedBtnSearchViaDate)
	
	ON_BN_CLICKED(IDC_BTN_SEARCH_ALL_DEAL, &CDealListView::OnBnClickedBtnSearchAllDeal)
	ON_BN_CLICKED(IDC_BTN_SEARCH_KEYWORDS, &CDealListView::OnBnClickedBtnSearchKeywords)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMECHOOSE, &CDealListView::OnCbnSelchangeComboTimechoose)
	ON_BN_CLICKED(IDC_RADIO_CHARGE_MAN, &CDealListView::OnBnClickedRadioChargeMan)
	ON_BN_CLICKED(IDC_RADIO_NAME_CTM, &CDealListView::OnBnClickedRadioNameCtm)
	ON_MESSAGE(WM_STRATDLOAD_DealListView, &CDealListView::OnStratDLoad)
	ON_MESSAGE(WM_ENDDLOAD_DealListView, &CDealListView::OnEndDLoad)
	ON_COMMAND(ID_32812, &CDealListView::OnDelFile)
	ON_COMMAND(ID_32801, &CDealListView::OnReverseChoose)
	ON_COMMAND(ID_32800, &CDealListView::OnChooseNone)
	ON_COMMAND(ID_32799, &CDealListView::OnChooseAll)
	ON_COMMAND(ID_32819, &CDealListView::OnDelDeal)
	ON_MESSAGE(WM_DEL_DEAL_DealListView, &CDealListView::OnDelDealOrFile)
END_MESSAGE_MAP()


// CDealListView 诊断

#ifdef _DEBUG
void CDealListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDealListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDealListView 消息处理程序

void CDealListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	InitList();
	InitSplit();
	InitComb();
	InitTipCtrl();
	InitSearchRadio();
	m_dateStart.SetFormat(_T("yyyy-MM-dd"));
	m_dateEnd.SetFormat(_T("yyyy-MM-dd"));
}

void CDealListView::InitSearchRadio()
{
	CString chioce = NULL;
	CFileFind finder;
	BOOL isFind = finder.FindFile(_T(".//BMClient.ini"));
	if(isFind)
	{
		GetPrivateProfileString(_T("订单搜索"),_T("默认条件"),_T("-1"),chioce.GetBuffer(10),10,_T(".//BMClient.ini")); 
		if(_T("-1") == chioce)
		{
			CheckDlgButton(IDC_RADIO_CHARGE_MAN,1);
			WritePrivateProfileString(_T("订单搜索"),_T("默认条件"),_T("订单负责人"),_T(".//BMClient.ini")); 
		}else{
			if(_T("订单负责人") == chioce)
			{
				CheckDlgButton(IDC_RADIO_CHARGE_MAN,1);
			}else{
				CheckDlgButton(IDC_RADIO_NAME_CTM,1);
			}
		}
	}else{   //不存在该文件  加载默认配置
		CheckDlgButton(IDC_RADIO_CHARGE_MAN,1);
		WritePrivateProfileString(_T("订单搜索"),_T("默认条件"),_T("订单负责人"),_T(".//BMClient.ini")); 
	}
}

void CDealListView::InitTipCtrl()
{
	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
		FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	m_tip.SetFont(&font);
	m_dlStatus.SetFont(&font);
	font.Detach();
}
void CDealListView::InitComb()
{
	m_timeChoose.AddString(_T("接单日期"));
	m_timeChoose.AddString(_T("完成时间"));

	CString chioce = NULL;
	CFileFind finder;
	BOOL isFind = finder.FindFile(_T(".//BMClient.ini"));
	if(isFind)
	{
		GetPrivateProfileString(_T("订单搜索"),_T("日期选择"),_T("-1"),chioce.GetBuffer(10),10,_T(".//BMClient.ini")); 
		if(_T("-1") == chioce)
		{
			m_timeChoose.SetCurSel(0);
			WritePrivateProfileString(_T("订单搜索"),_T("日期选择"),_T("接单日期"),_T(".//BMClient.ini"));
		}else{
			if(_T("接单日期") == chioce)
			{
				m_timeChoose.SetCurSel(0);
			}else{
				m_timeChoose.SetCurSel(1);
			}
		}
	}else{
		m_timeChoose.SetCurSel(0);
		WritePrivateProfileString(_T("订单搜索"),_T("日期选择"),_T("接单日期"),_T(".//BMClient.ini"));
	}
}

void CDealListView::InitSplit()
{
	m_split.SetDropDownMenu(IDR_MENU_ORDER_OPERATION,0);
}
void CDealListView::InitList()
{
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle = dwStyle | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ;
	m_list.SetExtendedStyle(dwStyle);
	m_list.SetTextColor(RGB(139, 90, 43));
	m_list.SetTextBkColor(RGB(255, 236, 139));
	m_list.SetBkColor(CONCTRLBKCOLOR);

	CRect rect;
	m_list.GetClientRect(rect);
	m_list.InsertColumn(0, _T("订单单号"), LVCFMT_LEFT, 80);    
    m_list.InsertColumn(1, _T("接单日期"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(2, _T("完成时间"), LVCFMT_LEFT, 80);
    m_list.InsertColumn(3, _T("订单负责人"), LVCFMT_LEFT,120);
	m_list.InsertColumn(4, _T("订单备注"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(5, _T("客户来源"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(6, _T("介绍人"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(7, _T("介绍人电话"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(8, _T("接待人员"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(9, _T("业务员"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(10, _T("客户名称"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(11, _T("顾客姓名"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(12, _T("顾客电话"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(13, _T("顾客地址"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(14, _T("顾客备注"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(15, _T("合同单号"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(16, _T("业务进度"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(17, _T("订单总价(元)"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(18, _T("已付金额(元)"), LVCFMT_LEFT,120);
	m_list.InsertColumn(19, _T("初测预约时间"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(20, _T("初测人员"), LVCFMT_LEFT,80);
	m_list.InsertColumn(21, _T("初次设计人员"), LVCFMT_LEFT,120);
	m_list.InsertColumn(22, _T("前期修改类型"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(23, _T("前期修改人员"), LVCFMT_LEFT,120);    
    m_list.InsertColumn(24, _T("预算人员"), LVCFMT_LEFT,80);
	m_list.InsertColumn(25, _T("谈单人员1"), LVCFMT_LEFT,80);
    m_list.InsertColumn(26, _T("谈单人员2"), LVCFMT_LEFT,80);
	m_list.InsertColumn(27, _T("方案定稿日期"), LVCFMT_LEFT,120);
	m_list.InsertColumn(28, _T("放样预约时间"), LVCFMT_LEFT,120);
	m_list.InsertColumn(29, _T("放样测量人员"), LVCFMT_LEFT,120);
	m_list.InsertColumn(30, _T("发单预约时间"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(31, _T("发单人员"), LVCFMT_LEFT, 80);
	m_list.InsertColumn(32, _T("单位选择"), LVCFMT_LEFT,80);
	m_list.InsertColumn(33, _T("付款选择"), LVCFMT_LEFT,80);
	m_list.InsertColumn(34, _T("安装预约时间"), LVCFMT_LEFT,120);
	m_list.InsertColumn(35, _T("安装人员"), LVCFMT_LEFT,80);
	m_list.InsertColumn(36, _T("安装问题"), LVCFMT_LEFT,200);
	m_list.InsertColumn(37, _T("附件名"), LVCFMT_LEFT, 200);
}

void CDealListView::OnBnClickedBtnEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	WT;
	OrderInfo info;
	memset(&info,0,sizeof(info));
	CString temp;
	CString NO; //订单编号
	int count = 0;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				CString str;
				str.Format(_T("当前有多个选中项，默认将编辑第一个选中项，订单编号为：%s。确认继续吗"),NO);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			NO = m_list.GetItemText(i,0);
			strcpy_s(info.NO_Order,T2A(NO));
			strcpy_s(info.date_accept_order,T2A(m_list.GetItemText(i,1)));
			strcpy_s(info.date_finished_order,T2A(m_list.GetItemText(i,2)));
			strcpy_s(info.name_person_charge_order,T2A(m_list.GetItemText(i,3)));
			strcpy_s(info.base_ps_order,T2A(m_list.GetItemText(i,4)));
			strcpy_s(info.ctm_from,T2A(m_list.GetItemText(i,5)));
			strcpy_s(info.name_intr,T2A(m_list.GetItemText(i,6)));
			strcpy_s(info.phone_intr,T2A(m_list.GetItemText(i,7)));
			strcpy_s(info.name_serv_persion,T2A(m_list.GetItemText(i,8)));
			strcpy_s(info.name_emp,T2A(m_list.GetItemText(i,9)));
			strcpy_s(info.call_ctm,T2A(m_list.GetItemText(i,10)));
			strcpy_s(info.name_ctm,T2A(m_list.GetItemText(i,11)));
			strcpy_s(info.phone_ctm,T2A(m_list.GetItemText(i,12)));
			strcpy_s(info.addr_ctm,T2A(m_list.GetItemText(i,13)));
			strcpy_s(info.ps_ctm,T2A(m_list.GetItemText(i,14)));
			strcpy_s(info.NO_contract,T2A(m_list.GetItemText(i,15)));
			strcpy_s(info.prog_business,T2A(m_list.GetItemText(i,16)));
			strcpy_s(info.price_total_contract,T2A(m_list.GetItemText(i,17)));
			strcpy_s(info.money_already_take,T2A(m_list.GetItemText(i,18)));
			strcpy_s(info.date_first_test_book,T2A(m_list.GetItemText(i,19)));
			strcpy_s(info.persion_first_test,T2A(m_list.GetItemText(i,20)));
			strcpy_s(info.persion_first_design,T2A(m_list.GetItemText(i,21)));
			strcpy_s(info.type_early_modify,T2A(m_list.GetItemText(i,22)));
			strcpy_s(info.persion_early_modify,T2A(m_list.GetItemText(i,23)));
			strcpy_s(info.persion_budget,T2A(m_list.GetItemText(i,24)));
			strcpy_s(info.persion1_discuss_order,T2A(m_list.GetItemText(i,25)));
			strcpy_s(info.persion2_discuss_order,T2A(m_list.GetItemText(i,26)));
			strcpy_s(info.date_final_scheme,T2A(m_list.GetItemText(i,27)));
			strcpy_s(info.date_book,T2A(m_list.GetItemText(i,28)));
			strcpy_s(info.person_exact_measure,T2A(m_list.GetItemText(i,29)));
			strcpy_s(info.date_out_order_book,T2A(m_list.GetItemText(i,30)));
			strcpy_s(info.person_out_order,T2A(m_list.GetItemText(i,31)));
			strcpy_s(info.factory_choose,T2A(m_list.GetItemText(i,32)));
			strcpy_s(info.pay_deal_choice,T2A(m_list.GetItemText(i,33)));
			strcpy_s(info.date_install_book,T2A(m_list.GetItemText(i,34)));
			strcpy_s(info.persion_install,T2A(m_list.GetItemText(i,35)));
			strcpy_s(info.problems_install,T2A(m_list.GetItemText(i,36)));
		}
	}
	if(NO.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个订单进行编辑"));
		return;
	}
	COrderEdit	dlg;
	dlg.m_orderInfo_edit = info;
	dlg.DoModal();
}


HBRUSH CDealListView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDealListView::ResetDLStatus()
{
	m_bDLoading = FALSE;
	m_ulCurTotal = 0;
	m_progressCtrl.SetPos(0);
	m_dlStatus.SetWindowText(_T(""));
	m_progressCtrl.ShowWindow(SW_HIDE);
}
void CDealListView::ShowDLStatus()
{
	if (m_ulCurTotal > 0)//当前下载文件的字节数
	{
		int nPercent = (float)m_ulCurTotal / m_ulFileSize * 100;//百分比
		m_progressCtrl.SetPos(nPercent);							//设置进度条位置
		
		m_progressCtrl.ShowWindow(SW_SHOW);						//显示进度条
		
		//显示下载进度信息
		m_time_take_dlFile++;
		CString strProgInfor;
		strProgInfor.Format(_T("已下载%d%s,已用时%d秒"), nPercent,_T("%"),m_time_take_dlFile);
		m_dlStatus.SetWindowText(strProgInfor);
	}
}

void CDealListView::OnDownLoadFile()
{
	// TODO: 在此添加命令处理程序代码
	if(m_bDLoading)   //表示当前有下载任务
	{
		AfxMessageBox(_T("当前有下载任务正在进行中，请耐心等待。。"));
		return;
	}
	int count = 0;
	CString NO = NULL;
	CString filename = NULL;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++ ;
			if(count > 1)  //表示有多个选中项
			{
				if(IDYES != MessageBox(_T("每次只能单独下载一个附件，否则可能造成文件下载错误，继续吗"),_T("提示"),MB_YESNO)){
					return;
				}

				CString str;
				str.Format(_T("当前有多个选中项，默认将下载第一个选中项，订单编号为：%s。确认继续吗"),NO);
				if(IDYES == MessageBox(str,_T("提示"),MB_YESNO)){
					break;
				}else
				{
					return;
				}
			}
			NO = m_list.GetItemText(i,0);
			filename = m_list.GetItemText(i,37);
		}
	}
	if(NO.IsEmpty())
	{
		MessageBox(_T("当前没有选中项，请至少选中一个附件进行下载"));
		return;
	}
	USES_CONVERSION;
	//m_strToDLFile = NO + _T(",")+ filename;
	if(filename.IsEmpty())
	{
		MessageBox(_T("当前选中的订单没有附件，无法下载"));
		return;
	}
	m_strToDLFile = NO + _T(",") + filename;
	//弹出另存为对话框
	TCHAR tcSaveFilePath[MAX_PATH];	//临时字符数组
	BROWSEINFO browInfo = {0};		//清零
	browInfo.hwndOwner = GetSafeHwnd();
	browInfo.ulFlags = BIF_RETURNFSANCESTORS   ;
	browInfo.lpszTitle = _T("选择保存文件路径");//标题

	LPITEMIDLIST browseFolder = SHBrowseForFolder(&browInfo);//打开浏览文件夹对话框
	if( browseFolder != NULL )
	{
		SHGetPathFromIDList(browseFolder, tcSaveFilePath);	//得到文件夹路径    
		m_strFilePath = tcSaveFilePath;
		m_strFilePath += _T("\\");
		CFileFind find;
		BOOL flag = find.FindFile(m_strFilePath+filename);
		if(flag)    //该路径已经有一个文件名为要下载的文件了 提示用户是否覆盖
		{
			CString err;
			err.Format(_T("此路径下已经有一个名为'%s'的文件了，请重新选择另一个路径"),filename);
			MessageBox(err);
		}else{
			NameTheDLFile(m_DLFileTempName);
			
			CSock::StartDLFile(this);  
		}
	}else{
		return;
	}
}

void CDealListView::NameTheDLFile(CString &temp_name)
{
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	//strDate.Format(_T("%d"),5);
	strDate.Format(_T("%4d%02d%02d"),(int)st.wYear,(int)st.wMonth,(int)st.wDay);
	strTime.Format(_T("%02d%02d%02d"),st.wHour,st.wMinute,st.wSecond);
	temp_name = strDate + strTime;
}


/*
 * 合并临时文件
 */
/*
u_long  CDealListView::AssembleLine(FILE* destFile, int startPos, FILE* resoFile)
{
	//获取读文件的长度
	fseek(resoFile, 0, SEEK_END);
	unsigned long resoFileLen = ftell(resoFile);
	
	unsigned long  nOffsetLen = 0;	//已经读取字节数 
	unsigned long  nReadLen = 0;	//fread读取字节数

	char buf[BUF_SIZE];				//临时数组
	memset(buf, 0, BUF_SIZE);		//清空
	while (resoFileLen != nOffsetLen && !feof(resoFile))
	{
		fseek(resoFile,0 + nOffsetLen,SEEK_SET);		//移动读文件指针
		nReadLen = fread(buf,1,BUF_SIZE, resoFile);		//读入数据
		fseek(destFile,startPos + nOffsetLen,SEEK_SET);	//移动写文件指针
		fwrite(buf,1,nReadLen,destFile);				//写入数据
		nOffsetLen += nReadLen;							//增加已读数据
		memset(buf, 0, BUF_SIZE);						//清空
	}

	return nOffsetLen;									//返回实际文件长度
}
*/
/*
void CDealListView::AssemblyFile(void)
{
	WT;
	//文件指针	
	//得到文件名
	int nIndex = m_strToDLFile.Find(',');//找到末尾的'\'	
	CString strFileName = m_strToDLFile.Mid(nIndex + 1);   //文件名	
	CString strFile = m_strFilePath;//文件路径
	strFile += strFileName;

	//检查该文件是否存在
	CFileFind find;
	BOOL flag = find.FindFile(strFile);
	if(flag)   //如果有这个文件存在那就把下载的文件重命名了
	{
		nIndex = strFile.ReverseFind('.');
		CString _str_temp = strFile.Mid(0,nIndex)+_T("1");
		_str_temp += strFile.Mid(nIndex+1);
		strFile = _str_temp;     //重命名为文件名+1的文件
	}
	
	FILE *streamIn = NULL;//写
	//streamIn =  fopen(strFile.c_str(), "wb+");//打开文件
	errno_t err = fopen_s(&streamIn,T2A(strFile), "wb+");
	ASSERT(streamIn);
	int threadNum = MAX_RECVFILE_THREAD;   //最大接收线程数
	u_long file_len = 0;   //文件长度
	for(int i =0;i<threadNum;i++)
	{
		FILE* stream = NULL;
		CString strTemp = m_strFilePath;    //第i个临时文件
		strTemp += m_DLFileTempName;
		CString _str_temp = NULL;
		_str_temp.Format(_T("%d"),i);
		_str_temp += _T(".dl");
		strTemp += _str_temp;

		errno_t err1 = fopen_s(&stream,T2A(strTemp), "rb");   //打开文件
		ASSERT(stream);
		u_long	ulFileLen = AssembleLine(streamIn, file_len, stream);  //写入文件
		file_len += ulFileLen;
		fclose(stream);
		//删除临时文件	
		DeleteFile(strTemp);
	
	}
	fclose(streamIn);
}
*/


BOOL CDealListView::CreateSocket(SOCKET &sock)
{
	WT;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
	{
		return FALSE;
	}

	//设置系统接收数据为默认的BUF_TIMES倍
	int nErrCode;
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	nErrCode= getsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return FALSE;
	}
	uiRcvBuf *= BUF_TIMES;
	nErrCode = setsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
	}
	//检查设置系统发送数据缓冲区是否成功
	int NewUiRecvBuf;
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&NewUiRecvBuf, &uiRcvBufLen);	
	if (SOCKET_ERROR == nErrCode || NewUiRecvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
		return FALSE;
	}
	
	//创建套接字
//	UpdateData(TRUE);
	

    //服务器套结字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	char *pIPTemp = T2A(CSock::m_IP);		//将CString类型的IP地址转换为char指针所指的数组	
	long IP = ntohl((u_long)inet_addr(pIPTemp));
	servAddr.sin_addr.S_un.S_addr = htonl(IP);
	servAddr.sin_port = htons(CSock::m_port);
	//连接服务器

	nErrCode = connect(sock,(sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("连接服务器失败！"),MB_OK, 0);	
		return FALSE;
	}
	
	//显示连接服务器成功信息
	m_tip.SetWindowText(_T("开始下载文件!"));

	return TRUE;
}

/*
 * 接收文件的数据0:正常1:异常2:发送数据结束
 */
/*
int CDealListView::RecvFileData(SOCKET s, unsigned short ID,void* ptr)
{
	int nErrCode;	//返回错误
	hdr header;		//包头
	
	//接收包头
	nErrCode = recv(s, (char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{	
		int err = WSAGetLastError();
		CString _err_1;
		_err_1.Format(_T("接收下载文件数据包头失败,SOCKET_ERROR错误：%d"),err);
		AfxMessageBox(_err_1);
		return EXCEPT;
	}	
	if (REPLY_REQ_FILE_DOWNLOAD != header.type)
	{
		AfxMessageBox(_T("服务器发送文件错误！"));
		return EXCEPT;
	}
	
	//发送文件结束
	if (0 == header.fpos && 0 == header.flen )
	{
		return FINISH;
	}


	unsigned long ulFileSeg = header.flen;	//文件长度
	unsigned long ulFilePos = header.fpos;	//文件位置(相对位置)
	
	m_ulCurTotal += ulFileSeg;				//下载文件总长度

	unsigned long nlTotal = 0;				//每次接收下载文件总长度
	unsigned long nRead = 0;				//接收长度
	char cBuf[RECV_BUFF] = {0};
	while (CSock::m_bIsConnecting && nlTotal != ulFileSeg)
	{
		memset(cBuf, 0, RECV_BUFF);
		nRead = recv(s, cBuf, ulFileSeg, 0);//准备接收1024*20个字节长度
		if (SOCKET_ERROR == nRead)
		{
			int err = WSAGetLastError();
			CString _err_1;
			_err_1.Format(_T("接收文件包体失败,SOCKET_ERROR错误：%d"),err);
			AfxMessageBox(_err_1);
			return EXCEPT;
		}else if (0 == nRead)
		{
			int err = WSAGetLastError();
			CString _err_1;
			_err_1.Format(_T("接收文件包体失败,SOCKET_ERROR错误：%d"),err);
			AfxMessageBox(_err_1);
			return EXCEPT;
		}

		SaveFile(ID, cBuf, nRead, ulFilePos + nlTotal,ptr);//保存临时文件
		nlTotal += nRead;//加入接收字节数
	}
	return NORMAL;
}
*/
/*
 * 保存临时文件
 */
/*
void CDealListView::SaveFile(u_short nIndexThread, const char* pBuf, u_long ulLen, u_long ulPos,void* ptr)
{
	CDealListView* p_dlg = (CDealListView*)ptr;
	WT;
	FILE *stream = NULL;
	CString strTemp = p_dlg->m_strFilePath;	//保存文件路径
	for(int i= 0;i<MAX_RECVFILE_THREAD;i++)
	{
		if(i == nIndexThread)
		{
			strTemp += p_dlg->m_DLFileTempName;
			CString temp = NULL;
			temp.Format(_T("%d.dl"),i);
			strTemp += temp;    //第i个临时文件名
			break;
		}
	}
	//stream = fopen(strTemp.c_str(), "ab+");		//打开文件
	errno_t err = fopen_s(&stream,T2A(strTemp),"ab+");
	ASSERT(err == 0);
	//ASSERT(stream);
	
	int nErrCode = fseek(stream,ulPos,SEEK_SET);//移动文件指针
	ASSERT(nErrCode == 0);
	
	nErrCode = fwrite(pBuf, 1, ulLen, stream);	//写文件
	ASSERT(nErrCode == ulLen);

	fflush(stream);								//清除文件缓冲区
	fclose(stream);								//关闭文件
}
*/
void CDealListView::ValidFileDL()    //校验下载文件有效性
{
	OnEndDLoad(0,0);
	ResetDLStatus();
	if(m_ulFileSize == GetFileSize())
	{
		m_tip.SetWindowText(_T("恭喜，下载文件完成。"));
	}else{
		m_tip.SetWindowText(_T("抱歉，下载文件损坏，请重试"));
	}
}

u_long CDealListView::GetFileSize()
{
	WT;
	int index = m_strToDLFile.Find(',');
	CString filename = m_strToDLFile.Mid(index+1); 
	CString filepath = m_strFilePath + filename;
	char* p = T2A(filepath);
	long retFileLen;//文件长度
	FILE *stream;	//文件指针
	int err = fopen_s(&stream,p,"rb");
	if(err != 0)//打开文件失败
	{
		return 0;
	}
	fseek(stream, 0, SEEK_END);	//指针移动到文件尾
	retFileLen = ftell( stream );	//获取文件长度
	fclose(stream );				//关闭文件
	return retFileLen;
}


void CDealListView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			ShowTip();
			break;
		}
	case 2:
		{
			//ShowTip();
			ShowDLStatus();
			break;
		}
	default:
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CDealListView::ShowTip()
{
	m_time_take++;
	CString tip;
	tip.Format(_T("用时：%d秒"),m_time_take);
	m_tip.SetWindowText(tip);
}

afx_msg LRESULT CDealListView::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CDealListView::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}



afx_msg LRESULT CDealListView::OnResetDLStatus(WPARAM wParam, LPARAM lParam)
{
	ResetDLStatus();
	return 0;
}


afx_msg LRESULT CDealListView::OnValidFile(WPARAM wParam, LPARAM lParam)
{
	ValidFileDL();
	return 0;
}





void CDealListView::OnBnClickedBtnSearchViaDate()   //安装日期查询
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	WT;
	CString temp;
	m_timeChoose.GetWindowText(temp);
	m_search_Keyword = _T("模糊搜索$");
	m_search_Keyword += temp;
	m_search_Keyword += _T("$");
	DWORD dwResult;
	CTime timeTime;

	dwResult = m_dateStart.GetTime(timeTime);   //开始时间
	if (dwResult == GDT_VALID)
	{
		if ((m_dateStart.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			temp = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			temp = timeTime.Format(_T("%Y-%m-%d"));
		}
		m_search_Keyword += temp;
		m_search_Keyword += _T("$");
	}else
	{
		MessageBox(_T("请选择开始时间"));
		return;
	}
	dwResult = m_dateEnd.GetTime(timeTime);   //结束时间
	if (dwResult == GDT_VALID)
	{
		if ((m_dateEnd.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
		{
			temp = timeTime.Format(_T("%Y-%m-%d"));
		}else{
			temp = timeTime.Format(_T("%Y-%m-%d"));
		}
		m_search_Keyword += temp;
		m_search_Keyword += _T("$");
	}else
	{
		MessageBox(_T("请选择结束时间"));
		return;
	}
	CSock::StartReqFindDeal(this);
}





void CDealListView::OnBnClickedBtnSearchAllDeal()
{
	// TODO: 在此添加控件通知处理程序代码
	m_search_Keyword = _T("搜索全部$");
	CSock::StartReqFindDeal(this);
}


void CDealListView::OnBnClickedBtnSearchKeywords()
{
	// TODO: 在此添加控件通知处理程序代码
	m_search_Keyword.Empty();
	m_search_Keyword = _T("精确查找$");
	UpdateData(TRUE);
	if(m_keywords.IsEmpty())   //如果搜索为空将搜索全部员工资料
	{
		MessageBox(_T("搜索关键字为空"));
		return;
	}else{
		CString _strFind = _T("$");
		int flag = m_keywords.Find(_strFind);
		if(-1 != flag)   //包含逗号
		{
			MessageBox(_T("关键字不能包含$！"));
			m_keywords.Empty();
			UpdateData(FALSE);
			return;
		}
		if(1 == ((CButton*)GetDlgItem(IDC_RADIO_NAME_CTM))->GetCheck())
		{
			m_search_Keyword += _T("顾客姓名$");
			m_search_Keyword += m_keywords;
		}else if(1 == ((CButton*)GetDlgItem(IDC_RADIO_CHARGE_MAN))->GetCheck())
		{
			m_search_Keyword += _T("订单负责人$");
			m_search_Keyword += m_keywords;
		}
	}
	//WriteDefaultChoiceToIni();
	CSock::StartReqFindDeal(this);
}


void CDealListView::OnCbnSelchangeComboTimechoose()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	m_timeChoose.GetWindowText(temp);
	if(_T("接单日期") == temp){
		WritePrivateProfileString(_T("订单搜索"),_T("日期选择"),_T("接单日期"),_T(".//BMClient.ini"));
	}else{
		WritePrivateProfileString(_T("订单搜索"),_T("日期选择"),_T("完成时间"),_T(".//BMClient.ini"));
	}
}


void CDealListView::OnBnClickedRadioChargeMan()
{
	// TODO: 在此添加控件通知处理程序代码
	WritePrivateProfileString(_T("订单搜索"),_T("默认条件"),_T("订单负责人"),_T(".//BMClient.ini")); 
}


void CDealListView::OnBnClickedRadioNameCtm()
{
	// TODO: 在此添加控件通知处理程序代码
	WritePrivateProfileString(_T("订单搜索"),_T("默认条件"),_T("顾客姓名"),_T(".//BMClient.ini")); 
}


afx_msg LRESULT CDealListView::OnStratDLoad(WPARAM wParam, LPARAM lParam)
{
	m_bDLoading = TRUE;
	SetTimer(2,1000,NULL);
	m_time_take_dlFile = 0;
	m_dlStatus.SetWindowText(_T(""));
	return 0;
}


afx_msg LRESULT CDealListView::OnEndDLoad(WPARAM wParam, LPARAM lParam)
{
	m_bDLoading = FALSE;
	KillTimer(2);
	m_time_take_dlFile = 0;
	m_dlStatus.SetWindowText(_T(""));
	return 0;
}





void CDealListView::OnDelFile()
{
	// TODO: 在此添加命令处理程序代码
	WT;
	m_strToDelDeal.Empty();
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除附件的订单单号是：");
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		if(m_list.GetCheck(i))
		{	
			count++;
			if(count >1)
			{
				CString str;
				str.Format(_T("当前有多个选中项，将删除订单编号为%s的附件，继续吗？"),num);
				if(MessageBox(str,_T("提示"),MB_YESNO) == IDYES){
					m_strToDelDeal = _T("删除附件,") + m_strToDelDeal;
					CSock::StartDelDeal(this);
					return;
				}else{
					return;
				}
			}
			CString temp = m_list.GetItemText(i,37);
			num = m_list.GetItemText(i,0);
			if(temp.IsEmpty())
			{
				CString str;
				str.Format(_T("订单号为%s的附件为空，无法删除"),num);
				MessageBox(str);
				return;
			}
			m_strToDelDeal += m_list.GetItemText(i,0);
			m_strToDelDeal += ",";   //两个时间段的分隔符
		}
	}
	
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个订单，然后再删除"));
		return;
	}else{   //有选中项  count=1
		lineToDelete +=  m_strToDelDeal + _T("确认删除吗？");
		if(IDYES == MessageBox(lineToDelete,_T("警告"),MB_YESNO))
		{
			m_strToDelDeal = _T("删除附件,") + m_strToDelDeal;
			CSock::StartDelDeal(this);
		}else{
			return;
		}
	}
}


void CDealListView::OnReverseChoose()
{
	// TODO: 在此添加命令处理程序代码
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{
			m_list.SetCheck(i,0);
		}else{
			m_list.SetCheck(i);
		}
	}
}


void CDealListView::OnChooseNone()
{
	// TODO: 在此添加命令处理程序代码
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		m_list.SetCheck(i,0);
		//if( m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ||m_list.GetCheck(i))
		//{
		//}
	}
}


void CDealListView::OnChooseAll()
{
	// TODO: 在此添加命令处理程序代码
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		m_list.SetCheck(i);
	}
}


void CDealListView::OnDelDeal()
{
	// TODO: 在此添加命令处理程序代码
	WT;
	m_strToDelDeal.Empty();
	int count = 0;
	CString num;
	CString  lineToDelete = _T("将要被删除订单的单号是：");
	int total = m_list.GetItemCount();
	for(int i=0; i< total; i++)
	{
		if(m_list.GetCheck(i))
		{	
			count++;
			if(count >1)
			{
				CString str;
				str.Format(_T("当前有多个选中项，将删除订单编号为%s的订单，继续吗？"),num);
				if(MessageBox(str,_T("提示"),MB_YESNO) == IDYES){
					m_strToDelDeal = _T("删除订单,") + m_strToDelDeal;
					CSock::StartDelDeal(this);
					return;
				}else{
					return;
				}
			}
			num = m_list.GetItemText(i,0);
			m_strToDelDeal += num;
			m_strToDelDeal += ",";   //两个时间段的分隔符
		}
	}
	if(count == 0)
	{
		MessageBox(_T("请先至少勾选一个订单，然后再删除"));
		return;
	}else{   //有选中项  count=1
		lineToDelete +=  m_strToDelDeal + _T("确认删除吗？");
		if(IDYES == MessageBox(lineToDelete,_T("警告"),MB_YESNO))
		{
			m_strToDelDeal = _T("删除订单,") + m_strToDelDeal;
			CSock::StartDelDeal(this);
		}else{
			return;
		}
	}
}

afx_msg LRESULT CDealListView::OnDelDealOrFile(WPARAM wParam, LPARAM lParam)
{
	CString _strToDelete = m_strToDelDeal;
	CString temp; 
	CString _strToFind = _T(",");    //找到这样的标志位 进行字符段的截取
	int _strEnd = _strToDelete.Find(_strToFind);
	if(-1 == _strEnd)   //没有找到
	{
		return 0;
	}else{
		temp = _strToDelete.Mid(0,_strEnd);
		_strToDelete = _strToDelete.Mid(_strEnd+1);
		if(_T("删除订单") == temp)
		{
			while(TRUE)
			{
				_strEnd = _strToDelete.Find(_strToFind);
				if(-1 == _strEnd)   //没有找到
				{
					break;
				}else{
					temp = _strToDelete.Mid(0,_strEnd);
					int num =m_list.GetItemCount();
					for(int i = 0;i< num;i++){
						if(temp == m_list.GetItemText(i,0))
						{
							m_list.DeleteItem(i);
						}
					}
					_strToDelete = _strToDelete.Mid(_strEnd+1);
				}
			}
		}else{
			while(TRUE){   //清空附件那一栏
				_strEnd = _strToDelete.Find(_strToFind);
				if(-1 == _strEnd)   //没有找到
				{
					break;
				}else{
					temp = _strToDelete.Mid(0,_strEnd);
					int num =m_list.GetItemCount();
					for(int i = 0;i< num;i++){
						if(temp == m_list.GetItemText(i,0))
						{
							m_list.SetItemText(i,37,_T(""));
						}
					}
					_strToDelete = _strToDelete.Mid(_strEnd+1);
				}
			}
		}
	}
	return 0;
}
