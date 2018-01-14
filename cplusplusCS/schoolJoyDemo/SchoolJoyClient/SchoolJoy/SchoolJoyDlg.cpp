
// SchoolJoyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SchoolJoy.h"
#include "SchoolJoyDlg.h"
#include "afxdialogex.h"
#include "ThreadOperate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  WM_TRAY WM_USER+1    //托盘;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSchoolJoyDlg 对话框


CSchoolJoyDlg::CSchoolJoyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSchoolJoyDlg::IDD, pParent)
//	, m_edit_main_show(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSchoolJoyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_Main, m_tab_main);
//	  DDX_Control(pDX, IDC_EDIT_Main_Show, m_Edit_Main_Show);
//	DDX_Text(pDX, IDC_EDIT_Main_Show, m_edit_main_show);
	DDX_Control(pDX, IDC_BUTTON_Download, m_button_download);
	DDX_Control(pDX, IDC_BUTTON_Upload, m_button_upload);
}

BEGIN_MESSAGE_MAP(CSchoolJoyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Search, &CSchoolJoyDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_Download, &CSchoolJoyDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_Upload, &CSchoolJoyDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_LogIn, &CSchoolJoyDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_Set, &CSchoolJoyDlg::OnBnClickedButtonSet)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_Main, &CSchoolJoyDlg::OnTcnSelchangeTabMain)
	ON_MESSAGE(WM_TRAY,OnTray)
END_MESSAGE_MAP()


// CSchoolJoyDlg 消息处理程序

BOOL CSchoolJoyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	UpdateData(TRUE);
	//m_edit_main_show = "未登录，请先登录";
	UpdateData(FALSE);

	 ShowTray();   //托盘显示    
	//创建Tab标签
	m_tab_main.InsertItem(0, _T("搜索"));         // 插入第一个标签“”    
	m_search.Create(IDD_DIALOG_Search, &m_tab_main);    // 创建第一个标签页 
	m_search.SetBackgroundColor(RGB(139,134,130));


	m_tab_main.InsertItem(1, _T("下载"));  // 插入第二个标签“”
	m_download.Create(IDD_DIALOG_Download, &m_tab_main);// 创建第二个标签页
	m_download.SetBackgroundColor(RGB(139,134,130));  

	m_tab_main.InsertItem(2, _T("上传"));
	m_upload.Create(IDD_DIALOG_Upload, &m_tab_main); 
	m_upload.SetBackgroundColor(RGB(139,134,130));

	

	
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页    
    CRect tabRect;    // 标签控件客户区的Rect 
	m_tab_main.GetClientRect(&tabRect);    
    tabRect.left += 1;    
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;

	m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);    
	m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

	ThreadOperate mainThread;//监听主线程
	void* dialog=(void*)this;////////////////
	mainThread.startMainThread(dialog);//////

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSchoolJoyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSchoolJoyDlg::OnPaint()
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
		
          CPaintDC   dc(this);   
          CRect   rect;   
          GetClientRect(&rect);   
          CDC   dcMem;   
          dcMem.CreateCompatibleDC(&dc);   
          CBitmap   bmpBackground;   
          bmpBackground.LoadBitmap(IDB_BITMAP_background3);   //IDB_BITMAP_background3 是你自己的图对应的ID  
                                                                             
                  
          BITMAP   bitmap;   
          bmpBackground.GetBitmap(&bitmap);   
          CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
          dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);   
	}
	   HBITMAP hBitmap;                          
       hBitmap = (HBITMAP)::LoadImage(NULL,".\\res\\download.bmp",
		       IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	   ((CButton *)GetDlgItem(IDC_BUTTON_Download))->SetBitmap(hBitmap); 
	   hBitmap = (HBITMAP)::LoadImage(NULL,".\\res\\set.bmp",
		       IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);      
	   ((CButton *)GetDlgItem(IDC_BUTTON_Set))->SetBitmap(hBitmap);
	    hBitmap = (HBITMAP)::LoadImage(NULL,".\\res\\upload.bmp",              //设置BUTTON的图标
		       IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	   ((CButton *)GetDlgItem(IDC_BUTTON_Upload))->SetBitmap(hBitmap);
	    hBitmap = (HBITMAP)::LoadImage(NULL,".\\res\\user.bmp",
		       IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	   ((CButton *)GetDlgItem(IDC_BUTTON_LogIn))->SetBitmap(hBitmap);
	    hBitmap = (HBITMAP)::LoadImage(NULL,".\\res\\search.bmp",
		       IMAGE_BITMAP,80,80,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	   ((CButton *)GetDlgItem(IDC_BUTTON_Search))->SetBitmap(hBitmap);
	  
	   
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSchoolJoyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSchoolJoyDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect tabRect;    // 标签控件客户区的Rect 
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
    m_tab_main.GetClientRect(&tabRect);    
    tabRect.left += 1;    
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;
	// 如果标签控件当前选择标签为“搜索”，隐藏“下载”“上传”    
    m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);    
    m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	int Label = m_tab_main.SetCurSel(0);

}


void CSchoolJoyDlg::OnBnClickedButtonDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect tabRect;    // 标签控件客户区的Rect    
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页    
    m_tab_main.GetClientRect(&tabRect);    
    tabRect.left += 1;    
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;
	
	// 如果标签控件当前选择标签为“下载”，隐藏“搜索”“上传”    
    m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
	m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	int Label = m_tab_main.SetCurSel(1);

}


void CSchoolJoyDlg::OnBnClickedButtonUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect tabRect;    // 标签控件客户区的Rect 
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页    
    m_tab_main.GetClientRect(&tabRect);    
    tabRect.left += 1;    
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;
	// 如果标签控件当前选择标签为“上传”，隐藏“下载”“搜索”    
	m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
    m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	int Label = m_tab_main.SetCurSel(2);
	
}


void CSchoolJoyDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CLogIn logIn;
	logIn.SetBackgroundColor(RGB(95,158,160));   //设置背景色
	INT_PTR nResponse = logIn.DoModal();
}


void CSchoolJoyDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CSet set;
	set.SetBackgroundColor(RGB(95,158,160));
	//set.ReadSet();
	INT_PTR nResponse = set.DoModal();
	
}
 
void CSchoolJoyDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CRect tabRect;    // 标签控件客户区的Rect 
    // 获取标签控件客户区Rect，并对其调整，以适合放置标签页    
    m_tab_main.GetClientRect(&tabRect);    
    tabRect.left += 1;    
    tabRect.right -= 1;    
    tabRect.top += 25;    
    tabRect.bottom -= 1;

	switch(m_tab_main.GetCurSel())
	{
		case 0:
			m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
			m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);    
			m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
			break;

		case 1:
			m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
			m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
			m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			break;

		case 2:
			m_upload.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			m_search.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);    
			m_download.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
			break;

		default:
			break;


	}
}


//----------------------------------------托盘显示
void CSchoolJoyDlg::ShowTray(){  
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
LRESULT CSchoolJoyDlg::OnTray(WPARAM wParam, LPARAM lParam){
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