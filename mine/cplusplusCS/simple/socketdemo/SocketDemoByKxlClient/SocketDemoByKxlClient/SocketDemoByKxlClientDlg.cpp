
// SocketDemoByKxlClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDemoByKxlClient.h"
#include "SocketDemoByKxlClientDlg.h"
#include "socket/SocketDefines.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT SendAndRecvClientThread(LPVOID pParam)		//接收和发送数据线程
{
    CSocketDemoByKxlClientDlg* pMainDlg = (CSocketDemoByKxlClientDlg*)pParam;
	SOCKET	sockClient = pMainDlg->m_sockWhole;//客户端套接字	
	if(INVALID_SOCKET == sockClient)
	{
		return 0;
	}
	FD_SET writefd;	//可写集合
	FD_SET readfd;	//可读集合

	while(pMainDlg->m_bIsRunning)
	{
		FD_ZERO(&writefd);		//清零
		FD_ZERO(&readfd);		//清零
		FD_SET(sockClient, &writefd);//添加到可写集合
		FD_SET(sockClient, &readfd);	//添加到可读集合

		int reVal = 0;
		reVal = select(0, &readfd, &writefd, NULL, NULL);//等待套接字满足条件
		if (SOCKET_ERROR == reVal)
		{
			AfxMessageBox(_T("select错误"));
			return 0;
		}else if ( reVal > 0)
		{
			if (FD_ISSET(sockClient, &writefd))			//满足可写的条件
			{
				if (FALSE == pMainDlg->SendReq())	//发送数据
				{
					pMainDlg->m_sockWhole = INVALID_SOCKET;
					AfxMessageBox(_T("发送数据失败,请重新登录！"));
					pMainDlg->m_bIsRunning = FALSE;     //连接断开 让用户重新登录
					pMainDlg->m_sockWhole = INVALID_SOCKET;   //这样可以重新初始化socket
					return 0;
				}			
			}			

			if (FD_ISSET(sockClient, &readfd))			//满足可读的条件
			{
				if(FALSE == pMainDlg->RecvData())//接收数据
				{
					//AfxMessageBox(_T("接收目录信息失败！"));
					pMainDlg->m_sockWhole = INVALID_SOCKET;
					AfxMessageBox(_T("接收数据失败,请重新登录！"));
					pMainDlg->m_bIsRunning = FALSE;   //连接断开 让用户重新登录
					//m_bReqAvailable = TRUE;   //使得请求可用
					pMainDlg->m_sockWhole = INVALID_SOCKET;  //这样可以重新初始化socket
					return 0;
				}
			}			
		}
		Sleep(50);		
	}

	return 0;
}




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSocketDemoByKxlClientDlg 对话框




CSocketDemoByKxlClientDlg::CSocketDemoByKxlClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketDemoByKxlClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketDemoByKxlClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_chat);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_message);
	DDX_Control(pDX, IDC_EDIT_STATUSSEND, m_statusSend);
	DDX_Control(pDX, IDC_EDIT_LINKSTATUS, m_statusLink);
	DDX_Control(pDX, IDC_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_ip);
}

BEGIN_MESSAGE_MAP(CSocketDemoByKxlClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_LINK, &CSocketDemoByKxlClientDlg::OnBnClickedBtnLink)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSocketDemoByKxlClientDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CSocketDemoByKxlClientDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSocketDemoByKxlClientDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CSocketDemoByKxlClientDlg 消息处理程序

BOOL CSocketDemoByKxlClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_sockWhole = INVALID_SOCKET;
	m_bIsRunning = FALSE;
	m_nCurReq = REQ_NONE;
	CString port;

	port.Format(_T("%d"),g_iPort);
	m_port.SetWindowText(port);
    InitSocket();
	GetLocalIp();
    
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	return TRUE;      // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL  CSocketDemoByKxlClientDlg::PreTranslateMessage(MSG*   pMsg) 
{ 
	//   修改回车键的操作反应 
	if(pMsg-> message== WM_KEYDOWN  &&  pMsg-> wParam==VK_RETURN) 
	{ 
		return TRUE;
	} 
	return   CDialog::PreTranslateMessage(pMsg); 
}

void CSocketDemoByKxlClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSocketDemoByKxlClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocketDemoByKxlClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocketDemoByKxlClientDlg::OnBnClickedBtnLink()
{
	// TODO: 在此添加控件通知处理程序代码

    m_statusLink.SetWindowText(_T("开始连接服务器"));
	
	//获得服务器的IP地址
	USES_CONVERSION;
	CString ip;
    m_ip.GetWindowText(ip);
	char *pIPTemp = T2A(ip);		//将CString类型的IP地址转换为char指针所指的数组	
	long IP = ntohl((u_long)inet_addr(pIPTemp));
	//ntohl()是将一个无符号长整形数从网络字节顺序转换为主机字节顺序
	if(IP == INADDR_NONE)
	{
		AfxMessageBox(_T("IP地址不合法，请重新输入"));
        m_statusLink.SetWindowText(_T("IP地址不合法，请重新输入"));
		return ;
	}

	//服务器套结字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.S_un.S_addr = htonl(IP);
	CString port;
	m_port.GetWindowText(port);
    
	servAddr.sin_port = htons(atoi(T2A(port)));
	if(m_sockWhole == INVALID_SOCKET)
	{
		InitSocket();
		if(m_sockWhole == INVALID_SOCKET)
		{
			AfxMessageBox(_T("获得初始化的套接字失败！"));
            m_statusLink.SetWindowText(_T("获得初始化的套接字失败！"));
			m_sockWhole = INVALID_SOCKET;		
			return ;
		}
	}
	int nErrCode = connect(m_sockWhole,(sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == nErrCode)
	{
        m_statusLink.SetWindowText(_T("连接服务器失败，请检查IP地址和端口是否正确"));
		m_sockWhole = INVALID_SOCKET;
		AfxMessageBox(_T("连接服务器失败！"),MB_OK, 0);
		return ;
	}
	m_statusLink.SetWindowText(_T("连接服务器成功"));
	m_bIsRunning = TRUE;
	AfxBeginThread(SendAndRecvClientThread,this);    //创建接收和发送数据线程
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_LINK)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(FALSE);
	return ;
}

BOOL CSocketDemoByKxlClientDlg::InitSocket()
{
	WORD	wVersionRequested;			//请求socket版本
	WSADATA	wsaData;					//wsaData结构
	int		nErrCode;					//返回值

	wVersionRequested = MAKEWORD( 2, 2 );//请求windows Sockets 2.2版本	
	nErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( 0 != nErrCode )
	{
		return FALSE;
	}

	//创建套接字
	m_sockWhole = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sockWhole)
	{
		AfxMessageBox(_T("创建套接字失败"));
		return FALSE;
	}

	//设置系统接收数据为默认的BUF_TIMES倍
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	nErrCode= getsockopt(m_sockWhole, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("获取系统默认发送数据缓冲区失败！"));
		return FALSE;
	}
	uiRcvBuf *= g_iBufTimes;
	nErrCode = setsockopt(m_sockWhole, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
		return FALSE;
	}

	//检查设置系统接收数据缓冲区是否成功
	unsigned int uiNewRcvBuf;
	getsockopt(m_sockWhole, SOL_SOCKET, SO_RCVBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));;
		return FALSE;
	}
    
	return TRUE;
}

void CSocketDemoByKxlClientDlg::GetLocalIp()
{
	//获得计算机名
	char hostName[256];
	memset(hostName,0,256);
	int nRetCode = gethostname(hostName,sizeof(hostName));
	if(nRetCode!=0)
	{
		//产生错误
		AfxMessageBox(_T("没有取得本机名！"));
		return ;
	}
	//通过计算机名字获得本地IP地址
	hostent* lpHostEnt = gethostbyname(hostName);
	if(lpHostEnt == NULL)
	{
		//产生错误
		AfxMessageBox(_T("没有取得本机IP！"));
		return ;
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	CString sIpAddress = NULL;
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr, lpAddr,4);
		//转换为标准格式
		sIpAddress=inet_ntoa(inAddr);
		if(sIpAddress.IsEmpty()){
			AfxMessageBox(_T("没有取得本机IP"));
		}
	}
	USES_CONVERSION;
	long IP=ntohl((u_long)inet_addr(T2A(sIpAddress)));  //将字符数组类型的IP地址转换为long型的

	m_ip.SetAddress(IP);   //设置IP地址
	return;
}

BOOL CSocketDemoByKxlClientDlg::SendReq()		//向服务器发送请求
{
	BOOL reVal = TRUE;			//返回值
	switch(m_nCurReq)	//请求类型
	{
	case REQ_TEXT_MESSAGE:
		{
			reVal = SendTextMessage();
			m_nCurReq = REQ_NONE;
			break;
		}
	case REQ_REPLY_MESSAGE_FROM_SERVER:
		{
			reVal = SendReplyMessageToServer();
			m_nCurReq = REQ_NONE;
			break;
		}
	default:
		{
			break;
		}
	}
	return reVal;
}

BOOL CSocketDemoByKxlClientDlg::RecvData()		//接收从服务器返回的数据
{
	BOOL	reVal = TRUE;	//返回值	
	int		nErrCode;		//错误值
	//读取包头
	hdr header;
	nErrCode = recv(m_sockWhole,(char*)&header, HEADLEN,0);		
	if (SOCKET_ERROR == nErrCode || 0 == nErrCode)//服务器关闭了
	{
		AfxMessageBox(_T("服务器关闭！"));
		reVal = FALSE;	
		return reVal;
	}	

	//读取包体
	int nDataLen = header.usTotalLen - HEADLEN;//包体的长度	
	switch(header.iReqType)					//根据数据包的类型分类 再读取包体
	{
	case ENUM_PACKET_RECV_TEXT_MESSAGE:				//收到的回复信息 登录是否成功
		{
			reVal = RecvTextMessage(nDataLen);
			break;
		}
	case ENUM_PACKET_RECV_MESSAGE_FROM_SERVER:				//收到的回复信息 登录是否成功
		{
			reVal = RecvMessageFromServer(nDataLen);
			break;
		}
	default:
		break;						
	}	
	if(FALSE == reVal)
	{
		AfxMessageBox(_T("接收来自服务器的回复信息失败,与服务器断开连接，请重新链接！"));
	}
	return reVal;	
}

BOOL CSocketDemoByKxlClientDlg::SendTextMessage()   //向服务器发送信息
{
	int nflag;
	hdr header;
	Package package;
	memset(&header, 0, sizeof(header));
	memset(&package, 0, sizeof(package));
	USES_CONVERSION;
    strcpy_s(package.buff,T2A(m_sSendMessage));
	header.iReqType = ENUM_PACKET_SEND_TEXT_MESSAGE;
	header.usTotalLen = HEADLEN + sizeof(package);
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送包头失败，请重试或重新链接"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char *)&package,sizeof(package),0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送文本信息失败，请重试或重新链接"));
		return FALSE;
	}
	return TRUE;
}

BOOL CSocketDemoByKxlClientDlg::SendReplyMessageToServer()   //向服务器发送信息
{
	int nflag;
	hdr header;
	Package package;
	memset(&header, 0, sizeof(header));
	memset(&package, 0, sizeof(package));
	USES_CONVERSION;
    m_sSendMessage = _T("成功收到");
	strcpy_s(package.buff,T2A(m_sSendMessage));
	header.iReqType = ENUM_PACKER_SEND_RELPY_MESSAGE_TO_SERVER;
	header.usTotalLen = HEADLEN + sizeof(package);
	nflag = send(m_sockWhole,(const char*)&header,HEADLEN,0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送包头失败，请重试或重新链接"));
		return FALSE;
	}
	nflag = send(m_sockWhole,(const char *)&package,sizeof(package),0);
	if(nflag == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送文本信息失败，请重试或重新链接"));
		return FALSE;
	}
	return TRUE;
}


BOOL CSocketDemoByKxlClientDlg::RecvTextMessage(int len)		////接收回复信息 关于登录是否成功的
{
	Package package;
	memset(&package,0,sizeof(package));
	int errCode = recv(m_sockWhole,(char *)&package,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收服务器回复信息失败"));
		return FALSE;
	}
	CString time;
	GetCurTime(time);
	m_statusSend.SetWindowText(time+_T("  ")+CString(package.buff));
	return TRUE;
}


BOOL CSocketDemoByKxlClientDlg::RecvMessageFromServer(int len)		////接收回复信息 关于登录是否成功的
{
	Package package;
	memset(&package,0,sizeof(package));
	int errCode = recv(m_sockWhole,(char *)&package,len,0);
	if(errCode == SOCKET_ERROR)
	{
		AfxMessageBox(_T("接收服务器信息失败"));
		return FALSE;
	}
	m_sRecvMessage = CString(package.buff);
	UpdateChatEdit(m_sRecvMessage);    //更新窗口信息
	m_nCurReq = REQ_REPLY_MESSAGE_FROM_SERVER;
	return TRUE;
}


void CSocketDemoByKxlClientDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	m_message.GetWindowText(text);
	m_sSendMessage = text;
	m_nCurReq = REQ_TEXT_MESSAGE;

	CString time;
	GetCurTime(time);
	CString temp;
	CString chat;
	m_chat.GetWindowText(chat);
	temp.Format(_T("%s\r\n%s  我说：\r\n%s"), chat,time,text);
	m_chat.SetWindowText(temp);
	return ;
}


void CSocketDemoByKxlClientDlg::UpdateChatEdit(CString &newMessgae)    //更新窗口信息
{
	CString text;
	m_chat.GetWindowText(text);
	CString time;
	GetCurTime(time);
    text += _T("\r\n")+ time + _T("服务器说：") + _T("\r\n") + newMessgae;
	m_chat.SetWindowText(text);
	return ;
}

void  CSocketDemoByKxlClientDlg::GetCurTime(CString& time)
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	time.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"),sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond);
}
void CSocketDemoByKxlClientDlg::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_chat.SetWindowText(_T(""));
}

void CSocketDemoByKxlClientDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsRunning = FALSE;
    UnInitSocke();
	Sleep(200);
	m_statusLink.SetWindowText(_T("与服务器断开连接"));
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LINK)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPADDRESS_IP)->EnableWindow(TRUE);
	InitSocket();
}

void CSocketDemoByKxlClientDlg::UnInitSocke()   //关闭套接字
{
	closesocket(m_sockWhole);
	m_sockWhole = INVALID_SOCKET;
	WSACleanup();
}
