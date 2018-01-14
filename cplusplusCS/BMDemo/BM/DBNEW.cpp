// DBNew.cpp : 实现文件
//

#include "stdafx.h"
#include "BM.h"
#include "DBNew.h"
#include "afxdialogex.h"


// CDBNew 对话框

IMPLEMENT_DYNAMIC(CDBNew, CDialogEx)

CDBNew::CDBNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDBNew::IDD, pParent)
	, m_db_name(_T(""))
	, m_name_confirm(_T(""))
{

}

CDBNew::~CDBNew()
{
}

void CDBNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DB_NAME, m_db_name);
	DDX_Text(pDX, IDC_EDIT_NAME_CONFIRM, m_name_confirm);
	DDX_Control(pDX, IDC_STATIC_TIP, m_tip);
	//  DDX_Text(pDX, IDC_EDIT_DB_SAVE_PATH, m_db_save_path);
}


BEGIN_MESSAGE_MAP(CDBNew, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDBNew::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDBNew::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTTASK_DBNew, &CDBNew::OnStartTask)
	ON_MESSAGE(WM_ENDTASK_DBNew, &CDBNew::OnEndTask)
END_MESSAGE_MAP()


// CDBNew 消息处理程序
BOOL CDBNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON_BMMAIN);
	
	this->SetIcon(hIcon,TRUE);
	this->SetIcon(hIcon,FALSE);

	CFont font;
	LOGFONT m_tempfont={23,0,0,0,FW_BOLD,     //黑体还是正常
			FALSE,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T( "宋体")};
	font.CreateFontIndirect(&m_tempfont);     //新建字体
	((CStatic*)GetDlgItem(IDC_STATIC_TIP))->SetFont(&font);
	//((CStatic*)GetDlgItem(IDC_STATIC_TIPS))->SetFont(&font);
	font.Detach();
	return TRUE;
}

HBRUSH CDBNew::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)  
	{
		
		if(pWnd->GetDlgCtrlID() == IDC_STATIC_TIP)
		{
			pDC->SetTextColor(RGB(255,48,48));			
		}
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


void CDBNew::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_db_name.IsEmpty())
	{
		MessageBox(_T("数据库名字不能为空"));
		return;
	}else if(m_db_name.GetLength()>20)
	{
		MessageBox(_T("数据库名字不能超过20个字符"));
		return;
	}
	
	if(m_name_confirm.IsEmpty())
	{
		MessageBox(_T("数据库名称确认不能为空"));
		return;
	}else if(m_name_confirm != m_db_name)
	{
		MessageBox(_T("新建数据库名称和名称确认不一致，请重新确认"));
		return;
	}
	//SendMessage(WM_STARTTASK_DBNew,0,0);
	OnStartTask(0,0);
	WT;
	//CDBNew* p_dlg = (CDBNew*)pParam;
	CString ip = m_ip;
	CString port = m_port;
	SOCKET  s = INVALID_SOCKET;
	BOOL flag = CreateSocket(s);
	CString db_name = m_db_name;
	if(flag == FALSE)
	{
		OnEndTask(0,0);
		return;
	}
	hdr header;
	memset(&header,0,sizeof(header));
	header.type= DATABASE_NEW;				//新建数据库包头
	char buff[MAX_PATH] = {0};
	strcpy_s(buff,T2A(db_name));
	int len = sizeof(buff);
	header.len = HEADLEN + len;
	int nErrCode;//返回值

	//发送包头
	nErrCode = send(s, (char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("发送新建数据库请求数据包失败！"));
		OnEndTask(0,0);
		return;
	}
	
	//发送包体
	nErrCode = send(s, buff, len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("发送新建数据库请求数据包失败！"));
		OnEndTask(0,0);
		return ;
	}

	FD_SET readfd;//可读集合
	//接收文件
	while (TRUE)
	{
		FD_ZERO(&readfd);								//清空集合
		FD_SET(s, &readfd);					//加入集合
		nErrCode = select(0, &readfd, NULL, NULL, NULL);//等待
		if (SOCKET_ERROR == nErrCode)					//错误
		{
			AfxMessageBox(_T("关于新建数据库的selcet失败"));
			closesocket(s);
			OnEndTask(0,0);
			return ;
		}

		if (nErrCode > 0)
		{
			if (FD_ISSET(s, &readfd))						//可以读文件
			{
				int nErr = RecvDBNewData(s);	//接收文件
				if (EXCEPT == nErr)
				{
					AfxMessageBox(_T("关于新建数据库请求的selcet失败"));
					OnEndTask(0,0);
					closesocket(s);
					return ;
				}else if (FINISH == nErr)								//下载结束
				{
					closesocket(s);
					return ;
				}
			}
		}
		Sleep(THREAD_SLEEP);
	}
	return ;
	//DWORD dwThread;	
	//FileDownloadParam* pParam = new FileDownloadParam;
	//pParam->ptr = this;
    //HANDLE hThreadDBNew = CreateThread(NULL, 0, DBNewThread, pParam, 0, &dwThread);
	//WaitForSingleObject(hThreadDBNew,INFINITE);
	
	//CloseHandle(hThreadDBNew);
	//OnEndTask(0,0);
	
}

DWORD CDBNew::DBNewThread(void* pParam)
{

	WT;
	CDBNew* p_dlg = (CDBNew*)pParam;
	CString ip = p_dlg->m_ip;
	CString port = p_dlg->m_port;
	SOCKET  s = INVALID_SOCKET;
	BOOL flag = p_dlg->CreateSocket(s);
	//p_dlg->m_tip.SetWindowText(_T("hhe"));
	//return 1;
	CString m_db_name = p_dlg->m_db_name;
	if(flag == FALSE)
	{
		p_dlg->OnEndTask(0,0);
		return FALSE;
	}
	hdr header;
	memset(&header,0,sizeof(header));
	header.type= DATABASE_NEW;				//新建数据库包头
	char buff[MAX_PATH] = {0};
	strcpy_s(buff,T2A(m_db_name));
	int len = sizeof(buff);
	header.len = HEADLEN + len;
	int nErrCode;//返回值

	//发送包头
	nErrCode = send(s, (char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("发送新建数据库请求数据包失败！"));
		p_dlg->OnEndTask(0,0);
		return 0;
	}
	
	//发送包体
	nErrCode = send(s, buff, len, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("发送新建数据库请求数据包失败！"));
		p_dlg->OnEndTask(0,0);
		return 0;
	}

	FD_SET readfd;//可读集合
	//接收文件
	while (TRUE)
	{
		FD_ZERO(&readfd);								//清空集合
		FD_SET(s, &readfd);					//加入集合
		nErrCode = select(0, &readfd, NULL, NULL, NULL);//等待
		if (SOCKET_ERROR == nErrCode)					//错误
		{
			AfxMessageBox(_T("关于新建数据库的selcet失败"));
			p_dlg->OnEndTask(0,0);
			return 0;
		}

		if (nErrCode > 0)
		{
			if (FD_ISSET(s, &readfd))						//可以读数据
			{
				int nErr = p_dlg->RecvDBNewData(s);	//接收新建数据库结果
				if (EXCEPT == nErr)
				{
					AfxMessageBox(_T("关于新建数据库请求的selcet失败"));
					p_dlg->OnEndTask(0,0);
					return 0;
				}else if (FINISH == nErr)								//下载结束
				{
					closesocket(s);
					return 0;
				}
			}
		}
		Sleep(THREAD_SLEEP);
	}
	return 0;
}

int CDBNew::RecvDBNewData(SOCKET s)
{
	int nErrCode;	//返回错误
	hdr header;		//包头
	CString tip;
	//接收包头
	nErrCode = recv(s, (char*)&header, HEADLEN, 0);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("服务器关于新建数据库的回复失败！"));
		return EXCEPT;
	}	
	if (REPLY_DATABASE_NEW != header.type)
	{
		AfxMessageBox(_T("服务器关于新建数据库的回复失败！"));
		return EXCEPT;
	}
	int flag = -100;
	nErrCode = recv(s, (char *)&flag, sizeof(int), 0);//准备接收1024*20个字节长度
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("服务器关于新建数据库的回复失败！"));
		return EXCEPT;
	}else if (0 == nErrCode)
	{
		int err = WSAGetLastError();
		return EXCEPT;
	}else{
		switch (flag)
		{
		case -1:
			{
				tip = _T("数据库连接失败，请检查服务器端设置");
				break;
			}
		case 0:
			{
				tip = _T("创建失败，该数据库已经存在，请换一个名字");
				break;
			}
		case -2:
			{
				tip = _T("未知原因造成的错误，请重试或者联系管理员");
				break;
			}
		case 1:
			{
				tip = _T("恭喜，创建成功。现在可以用管理员密码登录了");
				break;
			}
		default:
			break;
		}
		OnEndTask(0,0);
		m_tip.SetWindowText(tip);
		return FINISH;
	}
	return NORMAL;
}


void CDBNew::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDBNew::OnTimer(UINT_PTR nIDEvent)
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


void CDBNew::ShowTip()
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

void CDBNew::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


afx_msg LRESULT CDBNew::OnStartTask(WPARAM wParam, LPARAM lParam)
{
	SetTimer(1,1000,NULL);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}

BOOL CDBNew::CreateSocket(SOCKET &sock)
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
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);	

	
	//创建套接字
//	UpdateData(TRUE);
	

    //服务器套结字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	char *pIPTemp = T2A(m_ip);		//将CString类型的IP地址转换为char指针所指的数组	
	long IP = ntohl((u_long)inet_addr(pIPTemp));
	servAddr.sin_addr.S_un.S_addr = htonl(IP);
	char *p = T2A(m_port);
	servAddr.sin_port = htons(atoi(p));
	//连接服务器

	nErrCode = connect(sock,(sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("连接服务器失败,请检查ip地址和端口是否正确！"),MB_OK, 0);	
		return FALSE;
	}
	
	//显示连接服务器成功信息
	m_tip.SetWindowText(_T("连接服务器成功!"));

	return TRUE;
}


afx_msg LRESULT CDBNew::OnEndTask(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	m_time_take = 0;
	m_tip.SetWindowText(_T(""));
	return 0;
}



