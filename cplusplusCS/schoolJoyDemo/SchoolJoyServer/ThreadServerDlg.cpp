// ThreadServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadServer.h"
#include "ThreadServerDlg.h"
#include "ThreadOperate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define  WM_TRAY WM_USER+1    //托盘;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#define SERVERPORT 12344



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadServerDlg dialog

CThreadServerDlg::CThreadServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreadServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThreadServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_serverIP=_T("");
	m_serverPORT=0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

}

void CThreadServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThreadServerDlg)
	//  DDX_Control(pDX, IDC_LIST_USERLIST, m_listView);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SERVERIP, m_serverIP);
	DDX_Text(pDX, IDC_EDIT_SERVERPORT, m_serverPORT);
	DDX_Control(pDX, IDC_LIST_USERLIST, m_Userlist);
	DDX_Control(pDX, IDC_LIST_RESOURCES, m_Resourceslist);
}

BEGIN_MESSAGE_MAP(CThreadServerDlg, CDialog)
	//{{AFX_MSG_MAP(CThreadServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_EN_CHANGE(IDC_EDIT_LOG, &CThreadServerDlg::OnEnChangeEditLog)
	ON_BN_CLICKED(IDC_BUTTON_STATRSERVICE, &CThreadServerDlg::OnBnClickedButtonStatrservice)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CThreadServerDlg::OnBnClickedButtonClear)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USERLIST, &CThreadServerDlg::OnLvnItemchangedListUserlist)
ON_BN_CLICKED(IDC_BUTTON_UPDATEUSERLIST, &CThreadServerDlg::OnBnClickedButtonUpdateuserlist)
ON_BN_CLICKED(IDC_BUTTON4, &CThreadServerDlg::OnBnClickedButton4)
ON_MESSAGE(WM_TRAY,OnTray)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreadServerDlg message handlers

BOOL CThreadServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//////////////////////////////////////////////////////////
	
	AfxSocketInit(NULL);
	CString strLocalName;
	GetLocalHostName(strLocalName);
	//AfxMessageBox(strLocalName);
	CString strLocalIP;
	GetIpAddress(strLocalName,strLocalIP);
	//AfxMessageBox(strLocalIP);
	m_serverIP=strLocalIP;	//设置默认IP为本机
	m_serverPORT=SERVERPORT;
	UpdateData(false);
	
	 ShowTray();   //托盘显示  
	//////////////////////////////////////////////////////////




	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT_LOG)->EnableWindow(TRUE);//系统日志显示框置灰
    m_Userlist.EnableWindow(TRUE);//
	//设置在线用户显示框基本参数
	m_Userlist.SetExtendedStyle(m_Userlist.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_Userlist.InsertColumn(0,_T("用户列表"),LVCFMT_CENTER,100);
	m_Userlist.InsertColumn(1,_T("IP"),LVCFMT_CENTER,100);
	showUserListView();
	////////////////////////////////////////////////////////
	m_Resourceslist.SetExtendedStyle(m_Userlist.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_Resourceslist.InsertColumn(0,_T("资源名"),LVCFMT_CENTER,150);
	m_Resourceslist.InsertColumn(1,_T("资源拥有者"),LVCFMT_CENTER,100);
	m_Resourceslist.InsertColumn(2,_T("资源类型"),LVCFMT_CENTER,100);
	m_Resourceslist.InsertColumn(3,_T("资源大小"),LVCFMT_CENTER,100);
	showResourcesListView();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThreadServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CThreadServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC   dc(this);   
		CRect   rect;   
		GetClientRect(&rect);   
		CDC   dcMem;   
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap   bmpBackground;   
		bmpBackground.LoadBitmap(IDB_BITMAP2); 
		
		BITMAP   bitmap;   
		bmpBackground.GetBitmap(&bitmap);   
		CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);  
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThreadServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




void CThreadServerDlg::OnBnClickedButtonStatrservice()
{
	// TODO: 在此添加控件通知处理程序代码
	ThreadOperate mainThread;//定义主线程
	void* dialog=(void*)this;
	mainThread.startMainThread(dialog);//转到开启主线程
}


void CThreadServerDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetDlgItem(IDC_EDIT_LOG)->SetWindowText(_T(""));
	return;
}
int CThreadServerDlg::GetLocalHostName(CString &sHostName)	//获得本地计算机名称
{
	
	char szHostName[256];
	int nRetCode;
	nRetCode=gethostname(szHostName,sizeof(szHostName));
	if(nRetCode!=0)
	{
		//产生错误
		sHostName=_T("没有取得");
		return -1;
	}
	sHostName=szHostName;
	return 0;
}
int CThreadServerDlg::GetIpAddress(const CString &sHostName, CString &sIpAddress)//获得本地IP
{
	struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
	if(lpHostEnt==NULL)
	{
		//产生错误
		sIpAddress=_T("");
		return -1;
	}
	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//转换为标准格式
		sIpAddress=inet_ntoa(inAddr);
		if(sIpAddress.IsEmpty())
			sIpAddress=_T("没有取得");
	}
	return 0;
}
int CThreadServerDlg::GetNamebyAddress(const CString &IpAddress,CString &sYouName)//获得对方计算机名称
{
	unsigned long addr;
	addr=inet_addr(IpAddress);
	struct hostent FAR * lpHostEnt=gethostbyaddr((char *)&addr,4,AF_INET);
	if(lpHostEnt==NULL)
	{
		//产生错误
		sYouName=_T("");

		AfxMessageBox("连接不上");//应该取得其错误
		return -1;
	}
	CString name=lpHostEnt->h_name;
	sYouName=name;
	return 0;
}
bool CThreadServerDlg::showUserListView(void)
{
	 ThreadOperate showUserThread;//
	 void* dialog=(void*)this;
	 showUserThread.startshowUserThread(dialog);
	 return 0;
}
bool CThreadServerDlg::showResourcesListView(void)
{
	 ThreadOperate showResourcesThread;//
	 void* dialog=(void*)this;
	 showResourcesThread.startshowResourcesThread(dialog);
	 return 0;
}
int CThreadServerDlg::Update(CString &_str)
{
	CString str;
	this->GetDlgItem(IDC_EDIT_LOG)->GetWindowText(str);
	str+="\r\n";
	str+=_str;
	str+="\r\n";
	this->GetDlgItem(IDC_EDIT_LOG)->SetWindowText(str);
	return 0;
}



void CThreadServerDlg::OnBnClickedButtonUpdateuserlist()   //手动更新用户信息列表
{
	// TODO: 在此添加控件通知处理程序代码
	this->showUserListView();
}


void CThreadServerDlg::OnBnClickedButton4()  //手动更新资源列表
{
	// TODO: 在此添加控件通知处理程序代码
	this->showResourcesListView();
}
CString CThreadServerDlg::FileSizeTransfer(double &NUM)  //用于将Byte转换为MB  方便显示
{
	CString _str;
	if(NUM/1024/1024/1024>=1)
	{
		_str.Format("%.2f GB",NUM/1024/1024/1024);
		return _str;
	}
	if(NUM/1024/1024>=1)
	{
		_str.Format("%.2f MB",NUM/1024/1024);
		return _str;
	}
	if(NUM/1024>=1)
	{
		_str.Format("%.2f KB",NUM/1024);
		return _str;
	}
	_str.Format("%.2f B",NUM);
	return _str;
	
}
//----------------------------------------托盘显示
void CThreadServerDlg::ShowTray(){  
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAY;    //WM_TRAY为自定义消息WM_USER+103
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	//wcscpy_s(p,_T("校园快享"));
	::Shell_NotifyIcon(NIM_ADD, &nid);
	this->ShowWindow(SW_HIDE); //隐藏主窗口
}

//--------------------------------------托盘消息处理
LRESULT CThreadServerDlg::OnTray(WPARAM wParam, LPARAM lParam){
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch (lParam){
	case WM_RBUTTONDOWN:     //鼠标按下右键时的处理
		{
			LPPOINT Ipoint=new tagPOINT;
			::GetCursorPos(Ipoint);  //得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();  //创建一个弹出菜单 
			//menu.AppendMenuW(MF_POPUP,(UINT_PTR)menu.m_hMenu,_T("关闭"));//添加子菜单项
			menu.AppendMenu(MF_STRING,WM_DESTROY,_T("关闭"));//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。
			menu.TrackPopupMenu(TPM_LEFTALIGN,Ipoint->x,Ipoint->y,this);//加载弹出菜单
			HMENU hmenu=menu.Detach();
			menu.DestroyMenu();
			delete Ipoint;
		}
		break;
	case WM_LBUTTONDOWN:  //鼠标按下左键时的处理
		{
			this->ShowWindow(SW_SHOWNORMAL);  //显示主窗口
		}
		break;
	}
	return 0;
}