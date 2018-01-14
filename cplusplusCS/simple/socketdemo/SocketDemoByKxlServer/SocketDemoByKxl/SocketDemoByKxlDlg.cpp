
// SocketDemoByKxlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDemoByKxl.h"
#include "SocketDemoByKxlDlg.h"
#include "socketclient/Client.h"
#include "socketclient/CommucationProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSocketDemoByKxlDlg 对话框
DWORD WINAPI RecordOperationInfo(void *pParam)   //既写到服务器的运行日志中 也更新到服务器的界面上
{
	USES_CONVERSION;
	CString backspace =  _T("\r\n");
	SysLogInfo * p_info = (p_SysLogInfo)pParam;
	CSocketDemoByKxlDlg* p_dlg =(CSocketDemoByKxlDlg*)p_info->p_voidTemp;
	CString log = A2T(p_info->showInfo);

	if(p_info != NULL){
		delete p_info;
		p_info = NULL;
	}
	p_dlg->WriteInfoToSysLog(log);	
	return 0;
}

DWORD WINAPI AcceptAndHandleClientThread(void *pParam)   //接受和处理客户端连接等请求  服务器核心
{
	CSocketDemoByKxlDlg* pServer = (CSocketDemoByKxlDlg*)pParam;
	SOCKET sListen = pServer->GetSocket();//获得服务器监听套接字

	//FD_SET	m_allSockfd;				
	FD_ZERO(&CSocketDemoByKxlDlg::m_allSockfd);			//清空集合
	FD_SET(sListen, &CSocketDemoByKxlDlg::m_allSockfd);	//将监听套接字加入该集合

	FD_SET readfd;					//定义满足可读套接字集合
	FD_SET	writefd;				//定义满足可写套接字集合

	while (pServer->IsRunning())//服务器运行状态
	{
		FD_ZERO(&readfd);			//清空可读集合
		FD_ZERO(&writefd);			//清空可写集合
		readfd = CSocketDemoByKxlDlg::m_allSockfd;		//赋值
		writefd = CSocketDemoByKxlDlg::m_allSockfd;		//赋值

		//更新界面信息
		pServer->ShowClientNumberInfor(CSocketDemoByKxlDlg::m_allSockfd.fd_count);

		//无限期等待套接字满足条件
		int nRet = select(0, &readfd, &writefd, NULL, NULL);	
		if (nRet > 0)
		{
			//遍历所有套接字集合
			for (u_int i = 0; i < CSocketDemoByKxlDlg::m_allSockfd.fd_count; i++)		
			{
				//存在可读的套接字
				if (FD_ISSET(CSocketDemoByKxlDlg::m_allSockfd.fd_array[i], &readfd) && pServer->IsRunning())
				{
					//接受客户端连接请求 
					if (CSocketDemoByKxlDlg::m_allSockfd.fd_array[i] == sListen)		
					{
						SOCKADDR_IN	addrClient;
						int			nAddrLen = sizeof(addrClient);
						SOCKET	sClient = accept(sListen, (sockaddr*)&addrClient, &nAddrLen);
						//新建一个 CClient类实例
						CClient *pClient = new CClient(sClient, pServer);	
						//加入客户端管理链表中
						pServer->AddClient(pClient);			
						//加入套接字集合
						FD_SET(sClient, &CSocketDemoByKxlDlg::m_allSockfd);		
						//更新界面信息
						pServer->ShowClientNumberInfor(CSocketDemoByKxlDlg::m_allSockfd.fd_count);						 
					}else //接收客户端数据
					{	
						//得到CClient类的实例
						CClient* pClient = pServer->GetClient(CSocketDemoByKxlDlg::m_allSockfd.fd_array[i]);
						if (pClient != NULL)
						{
							//接收数据
							BOOL bRet = pClient->RecvData();				
							//接收数据错误或者客户端关闭套接字
							if (FALSE == bRet)							
							{
								//取出套接字
								SOCKET sTemp = CSocketDemoByKxlDlg::m_allSockfd.fd_array[i];
								//从集合中删除
								FD_CLR(CSocketDemoByKxlDlg::m_allSockfd.fd_array[i], &CSocketDemoByKxlDlg::m_allSockfd);
								//从客户端管理链表中删除该客户端
								pServer->DeleteClient(sTemp);
								//更新界面信息
								pServer->ShowClientNumberInfor(CSocketDemoByKxlDlg::m_allSockfd.fd_count);
							}			 
						}						 
					}//else					 
				}//if
				//存在的可写套接字
				if (FD_ISSET(CSocketDemoByKxlDlg::m_allSockfd.fd_array[i], &writefd) && pServer->IsRunning())
				{
					//得到CClient类的实例
					CClient* pClient = pServer->GetClient(CSocketDemoByKxlDlg::m_allSockfd.fd_array[i]);
					if (pClient != NULL)
					{
						//发送数据
						BOOL bRet = pClient->SendData();
						if (FALSE == bRet)//发送数据失败
						{
							//被删除的套接字
							SOCKET sDelete = CSocketDemoByKxlDlg::m_allSockfd.fd_array[i];
							//从集合中删除该套接字
							FD_CLR(CSocketDemoByKxlDlg::m_allSockfd.fd_array[i], &CSocketDemoByKxlDlg::m_allSockfd);
							//从客户端管理链表中删除该客户端
							pServer->DeleteClient(sDelete);
							//更新界面信息
							pServer->ShowClientNumberInfor(CSocketDemoByKxlDlg::m_allSockfd.fd_count);
						}			
					}							 
				}
			}//for
		}//if
		Sleep(50);//线程睡眠
	}//while
	pServer->DeleteAllClient();//删除所有的客户端	
	//pServer->OnClose();
	return 0;
}

FD_SET CSocketDemoByKxlDlg::m_allSockfd = {};

CSocketDemoByKxlDlg::CSocketDemoByKxlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketDemoByKxlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketDemoByKxlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_message);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_status);
	DDX_Control(pDX, IDC_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Control(pDX, IDC_EDIT_SENDSTATUS, m_statusSend);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_text);
}

BEGIN_MESSAGE_MAP(CSocketDemoByKxlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_LOG,&CSocketDemoByKxlDlg::OnRecordLog)
	ON_MESSAGE(WM_TEXT_MESSAGE, &CSocketDemoByKxlDlg::OnRecvTextMessage)
	ON_MESSAGE(WM_SEND_STATUS, &CSocketDemoByKxlDlg::OnRecvReplyFromClient)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CSocketDemoByKxlDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_CLEARMESSAGE, &CSocketDemoByKxlDlg::OnBnClickedBtnClearmessage)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSocketDemoByKxlDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSocketDemoByKxlDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_CLEAR_LOG, &CSocketDemoByKxlDlg::OnBnClickedBtnClearLog)
	ON_BN_CLICKED(IDC_BTN_SHOWLOG, &CSocketDemoByKxlDlg::OnBnClickedBtnShowlog)
END_MESSAGE_MAP()


LRESULT  CSocketDemoByKxlDlg::OnRecordLog(WPARAM wParam, LPARAM lParam)
{
	CString log = (LPCTSTR)lParam;
	RecordLog(log);
	return 0;
}


LRESULT  CSocketDemoByKxlDlg::OnRecvTextMessage(WPARAM wParam, LPARAM lParam)
{
	CString message = (LPCTSTR)lParam;
	CString text;
	m_message.GetWindowText(text);
    text += _T("\r\n")+message;
    m_message.SetWindowText(text);
	return 0;
}

LRESULT  CSocketDemoByKxlDlg::OnRecvReplyFromClient(WPARAM wParam, LPARAM lParam)
{
	CString message = (LPCTSTR)lParam;
	m_statusSend.SetWindowText(message);
	return 0;
}

// CSocketDemoByKxlDlg 消息处理程序

BOOL CSocketDemoByKxlDlg::OnInitDialog()
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
	// TODO: 在此添加额外的初始化代码
	InitSocket();  //初始化套接字 创建服务器socket
	GetLocalIp();   //获得本机IP地址 并显示在主界面上
	CString port;
	port.Format(_T("%d"), g_iServerPort);
	m_port.SetWindowText(port);

	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSocketDemoByKxlDlg::GetLocalIp()
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
void CSocketDemoByKxlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSocketDemoByKxlDlg::OnPaint()
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


void CSocketDemoByKxlDlg::InitSocket()
{
	WORD	wVersionRequested;	//请求的Windows Sockets 实现版本
	WSADATA	wsaData;			//返回协商结果
	int		nErrCode;			//调用API函数的返回值

	wVersionRequested = MAKEWORD(2, 2);	
	nErrCode = WSAStartup( wVersionRequested, &wsaData );
	if ( 0 != nErrCode ) 
	{
		MessageBox(_T("请求Windows Sockets失败"));
		return;
	}
	//创建套接字
	m_sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sockServer)
	{
		MessageBox(_T("创建套接字失败"));
		return;
	}
	//获取系统默认的发送数据缓冲区大小
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	nErrCode= getsockopt(m_sockServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		MessageBox(_T("获取系统默认的发送数据缓冲区大小失败"));
		return;
	}

	//设置系统发送数据缓冲区为默认值的g_iBufTimes倍
	uiRcvBuf *= g_iBufTimes;
	nErrCode = setsockopt(m_sockServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
	}


	//检查设置系统发送数据缓冲区是否成功
	unsigned int uiNewRcvBuf;
	getsockopt(m_sockServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
	}
	return ;
}

void CSocketDemoByKxlDlg::ShowClientNumberInfor(int nNum)  //显示客户端数量信息
{
	CString strTip;
	strTip.Format(_T("目前共有%d客户连接。"),nNum -1);	
	m_status.SetWindowText(strTip);
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocketDemoByKxlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSocketDemoByKxlDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);				//更新对话框
	
	int		reVal;					//返回值	
	DWORD	dwServIP;		
	m_ip.GetAddress(dwServIP);	//得到服务器IP
	CString port;
	m_port.GetWindowText(port);
	USES_CONVERSION;
	u_short servPort = atoi(T2A(port)); 
	//服务器套接字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(dwServIP);
	servAddr.sin_port = htons(servPort);
	
	//绑定服务器
	reVal = bind(m_sockServer,(sockaddr*)&servAddr,sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == reVal)
	{
		AfxMessageBox(_T("服务器绑定失败"), MB_OK, 0);
		closesocket(m_sockServer);
		WSACleanup();
		return;
	}
	else 
	{
		//m_status.SetWindowText(_T("服务器绑定成功！"));
		CString _str = _T("服务器绑定成功！");
		RecordLog(_str);
	}
	//监听
	reVal = listen(m_sockServer,SOMAXCONN);
	if (SOCKET_ERROR == reVal)
	{
		AfxMessageBox(_T("服务器监听失败！"), MB_OK, 0);
		closesocket(m_sockServer);
		WSACleanup();
		return;
	}	
	m_bRunning = TRUE;//修改服务器状态
	
	//创建接受和处理客户端请求线程
	DWORD dwThread;	
	m_hAcceptAndHandle = CreateThread(NULL, 0, AcceptAndHandleClientThread, this, 0, &dwThread);
	CloseHandle(m_hAcceptAndHandle);
	
	//更新界面
	(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);	//启动按钮无效
	(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);	   //退出按钮有效

	(CButton *)GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);  
    GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
	(CButton *)GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
	
	m_ip.EnableWindow(FALSE);	//服务器地址控件无效
	
	CString _str = _T("服务器启动成功，开始运行！");
	//UpdateLog(_str);				 //显示成功信息
	//WriteLogToTxt(_str);
	RecordLog(_str);
}

void CSocketDemoByKxlDlg::RecordLog(CString log)
{
	USES_CONVERSION;
	SysLogInfo* p_info = new SysLogInfo;
	p_info->p_voidTemp = this;
	strcpy_s(p_info->showInfo,T2A(log));
	DWORD dwThread;	
	HANDLE hRecordOperation = CreateThread(NULL, 0, RecordOperationInfo, p_info, 0, &dwThread);
	CloseHandle(hRecordOperation);
}

void CSocketDemoByKxlDlg::WriteInfoToSysLog(CString log)    //将日志信息写入系统日志
{
	USES_CONVERSION;
	CString curTime = _T("");
	GetLogTime(curTime);
	CString backspace = _T("\r\n");
	CStdioFile mFile;
	CFileException mExcept;
	CString writeToPath = _T(".//ServerOperationLog.txt");
	while(!mFile.Open(writeToPath, CFile::typeText|CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::shareDenyWrite, &mExcept))
	{
		Sleep(500);	
	}

	mFile.SeekToEnd();
	curTime = backspace + curTime;
	curTime = curTime + backspace;
	CString _strToWrite = curTime;

	_strToWrite = _strToWrite + log;
	_strToWrite = _strToWrite + backspace;
	char *p = T2A(_strToWrite);
	mFile.Write(p,strlen(p));
	mFile.Close();
}

void CSocketDemoByKxlDlg::GetLogTime(CString &time)    //获得记录日志的时间
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	CString dayOfWeek = _T("星期");
	time.Empty();
	switch (sys.wDayOfWeek)
	{
	case 1:
		{
			dayOfWeek+= _T("一");
			break;
		}
	case 2:
		{
			dayOfWeek+= _T("二");
			break;
		}
	case 3:
		{
			dayOfWeek+= _T("三");
			break;
		}
	case 4:
		{
			dayOfWeek+= _T("四");
			break;
		}
	case 5:
		{
			dayOfWeek+= _T("五");
			break;
		}
	case 6:
		{
			dayOfWeek+= _T("六");
			break;
		}
	case 0:
		{
			dayOfWeek+= _T("日");
			break;

		}
	default:
		break;
	}
	time.Format(_T("%4d/%02d/%02d %s %02d:%02d:%02d "),(int)sys.wYear,(int)sys.wMonth,(int)sys.wDay,dayOfWeek,(int)sys.wHour,(int)sys.wMinute,(int)sys.wSecond);
}

void CSocketDemoByKxlDlg::UnInitSocke()   //关闭套接字
{
	closesocket(m_sockServer);
	m_sockServer = INVALID_SOCKET;
	WSACleanup();
}

void CSocketDemoByKxlDlg::AddClient(CClient *pClient)//添加客户端节点
{	
	m_listLocker.Lock();
	m_listClient.push_back(pClient);
	m_listLocker.UnLock();
}

void CSocketDemoByKxlDlg::DeleteClient(SOCKET s)		//删除客户端节点
{
	m_listLocker.Lock();
	//遍历该链表
	list<CClient*>::iterator it = m_listClient.begin();
	for (it; it != m_listClient.end();)
	{
		CClient *pClient = (CClient*)*it;
		if (pClient->GetSocket() == s)//相同的套接字
		{
			CString _str = NULL;
			//_str.Format(_T("用户\"%s\"已与服务器断开连接。"),pClient->m_user);
			//pClient->m_pServer->SendMessage(WM_LOG,0,(LPARAM)(LPCTSTR)_str);
			m_listClient.erase(it++);//从链表中删除
			delete pClient;			//释放内存
			pClient = NULL;
			break;
		}else
		{
			it++;
		}
	}
	m_listLocker.UnLock();
}

CClient* CSocketDemoByKxlDlg::GetClient(SOCKET s)		//根据套接字得到客户端
{
	CClient* pClient = NULL;
	m_listLocker.Lock();
	list<CClient*>::iterator it = m_listClient.begin();
	for (it; it != m_listClient.end(); ++it)
	{
		pClient = *it;
		if (s == pClient->GetSocket())//找到
		{
			break;
		}
	}
	m_listLocker.UnLock();
	return pClient;	
}

void CSocketDemoByKxlDlg::DeleteAllClient(void)		//删除所有客户端节点
{
	//EnterCriticalSection(&m_cs);
	//遍历该链表
	list<CClient*>::iterator it = m_listClient.begin();
	for (it; it != m_listClient.end();)
	{
		CClient *pClient = (CClient*)*it;
		m_listClient.erase(it++);

		delete pClient;
		pClient = NULL;		
	}
	m_listClient.clear();
	//LeaveCriticalSection(&m_cs); 
}

void CSocketDemoByKxlDlg::OnBnClickedBtnClearmessage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_message.SetWindowText(_T(""));
}

void CSocketDemoByKxlDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	m_text.GetWindowText(text);
	m_listLocker.Lock();
	std::list<CClient*>::iterator it = m_listClient.begin();
	CClient* pClient = *it;
	if(pClient)
	{
        pClient->SetSendMessageBuff(text);
		pClient->SetReqType(REQ_SEND_TEXT_MESSAGE_TO_CLINET);
	}
    m_listLocker.Lock();
    
	CString time;
    GetCurTime(time);
    CString temp;
	CString chat;
	m_message.GetWindowText(chat);
	temp.Format(_T("%s\r\n%s  我说：\r\n%s"),chat,time,text);
    m_message.SetWindowText(temp);

	return;
}

void  CSocketDemoByKxlDlg::GetCurTime(CString& time)
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys); 
	time.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"),sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond);
}
void CSocketDemoByKxlDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRunning = FALSE;								//设置服务器状态
	//m_status.SetWindowText(_T("服务器退出！"));		 //更新界面
	CString _str = _T("服务器正在退出中！");
	RecordLog(_str);

	//DeleteAllClient();
	UnInitSocke();
	(CButton*)GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);	//启动按钮无效
	(CButton*)GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);	   //退出按钮有效

	(CButton *)GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);  
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	m_status.SetWindowText(_T("服务器正在退出中"));
	Sleep(200);
	InitSocket();
}

BOOL  CSocketDemoByKxlDlg::PreTranslateMessage(MSG*   pMsg) 
{ 
	//   修改回车键的操作反应 
	if(pMsg-> message== WM_KEYDOWN  &&  pMsg-> wParam==VK_RETURN) 
	{ 
		return TRUE;
	} 
	return   CDialog::PreTranslateMessage(pMsg); 
}

void CSocketDemoByKxlDlg::OnBnClickedBtnClearLog()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileFind fileFind;
	if(fileFind.FindFile(_T(".//ServerOperationLog.txt")))
	{
		DeleteFile(_T(".//ServerOperationLog.txt"));
	}else
	{
		CString _str = _T("抱歉无法删除系统详细的运行日志，因为它已经被删除！");
	}
}

void CSocketDemoByKxlDlg::OnBnClickedBtnShowlog()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileFind fileFind;

	if(fileFind.FindFile(_T(".//ServerOperationLog.txt")))
	{
		HINSTANCE h = ShellExecute(NULL,_T("open"),_T("ServerOperationLog.txt"),NULL,NULL,SW_SHOWNORMAL);
		int i = (int)h;

	}
}
