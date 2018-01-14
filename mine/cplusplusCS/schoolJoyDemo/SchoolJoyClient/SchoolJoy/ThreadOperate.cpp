// ThreadOperate.cpp: implementation of the ThreadOperate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ThreadOperate.h"
#include "SchoolJoy.h"
#include "SchoolJoyDlg.h"
#include "Search.h"
#include "Register.h"
#include "ConnectSet.h"
#include "Download_Downloading.h"
#include "Upload.h"
#include "LogIn.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define		SIZEFILE			1024//传输文件大大小，可改变
#define		ELAPSE_DEFAULT		1000
#define		ELAPSE_SPEED		1000
#define		MAX_BUF_LEN			1024
#define		Kb					1024
#define		Mb					Kb*Kb

#define		P2PPort				12345
//////////////////////////////////////////////////////////////////////
CWinThread *pRequireFileThread;	
CString FileOwenerPath;
CString FileOwnerIP;
CString FILENAME;
CString FILETYPE;
float FILESIZE;
CString FILEPATH;
long OFFSET;
//////////////////////////////////////////////////////////////////////





ThreadOperate::ThreadOperate()
{

}

ThreadOperate::~ThreadOperate()
{

}




//////////////////////////////////////////////////////////////////
//////////////**********注册线程*****************////////////////
//////////////////////////////////////////////////////////////////
UINT RegisterThread(LPVOID pParam)
{
	CRegister *Mydlg =(CRegister *)pParam;

	AfxSocketInit(NULL);
	CSocket RegisterSocket;
	if(!RegisterSocket.Create())
	{
		//dialog->GetDlgItem(IDC_EDIT_Log)->SetWindowText(_T("客户端**请求文件信息**套接字 创建失败\r\n"));
		AfxMessageBox(_T("客户端**注册**套接字 创建失败\r\n"));
		return 0;
	}
	//CConnectSet connectset;
	if(!RegisterSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))
	{
		AfxMessageBox(_T("客户端**注册**套接字**连接失败\r\n"));
		return 0;
	}
	int stateCode=4;
	char buffer[sizeof(stateCode)]={0};
	sprintf_s(buffer,"%d",stateCode);

	int sendbytes= RegisterSocket.Send(buffer,sizeof(stateCode),0);

	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;
	}
	USERINFO userinfo;
	memset(&userinfo,0,sizeof(userinfo));
	strcpy_s(userinfo.password,CRegister::password);
	strcpy_s(userinfo.username,CRegister::username);
	//AfxMessageBox(userinfo.username);
	//AfxMessageBox(userinfo.password);


	int sendnum=RegisterSocket.Send(&userinfo,sizeof(userinfo),0);
	if(sendnum== SOCKET_ERROR)
	{
		AfxMessageBox("客户端发送的地址信息失败");
		return 0;
	}
	char flags[2]={0};
	int recvnum=RegisterSocket.Receive(flags,sizeof(flags),0);
	if(recvnum== SOCKET_ERROR)
	{
		AfxMessageBox("接收服务器发送注册状态失败");
		return 0;
	}
	if(flags[0]=='#'&&flags[1]=='1')
	{
		AfxMessageBox(_T("恭喜您注册成功，请牢记密码"));
		RegisterSocket.Close();
		return 0;
	}
	if(flags[0]=='#'&&flags[1]=='0')
	{
		AfxMessageBox(_T("用户名已存在，请重新注册"));
		RegisterSocket.Close();
		return 0;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////**********搜索线程*****************////////////////
//////////////////////////////////////////////////////////////////

UINT SearchThread(LPVOID pParam)
{
	CSearch *dialog = (CSearch *)pParam;
	AfxSocketInit(NULL);
	CSocket SearchSocket;
	//AfxMessageBox(dialog->m_SearchFile);
	if (!SearchSocket.Create())
	{
		AfxMessageBox(_T("客户端**搜索**套接字创建失败\r\n"));
		return 0;
	}
	//CConnectSet *_pconnectset=new CConnectSet;
	if(!SearchSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))//连接服务器IP和Port
	{
		AfxMessageBox(_T("客户端**搜索**套接字连接服务器失败\r\n"));
		return 0;
	}
	//delete [] _pconnectset;
	//发送状态码3
	int stateCode = 3;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= SearchSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;	
	}
	char SearchFileName[150]={0};
	strcpy_s(SearchFileName,dialog->m_SearchFile);//数据类型转化
	//AfxMessageBox(dialog->m_SearchFile);
	SearchSocket.Send(&SearchFileName,sizeof(SearchFileName),0);//发送搜索的文件名

	//char FileNum[10]={0};
	//SearchSocket.Receive(FileNum,sizeof(FileNum),0);//接收数据库中所含文件的个数c

	//int FileNum;
	//FileNum=atoi(SqlFileNumRecive);//数组到整型的转化
	int FileNum=0;  //获得搜索结果个数
	int recvnum=SearchSocket.Receive(&FileNum,sizeof(FileNum),0);
	if(recvnum==SOCKET_ERROR)
	{
		AfxMessageBox("接收搜索结果错误");
		SearchSocket.Close();
		return 0;
	}
	if(FileNum==0)
	{
		AfxMessageBox("很遗憾，没有可以匹配您的搜索选项！不妨换个关键词再次搜索试试。");
		SearchSocket.Close();
		return 0;
	}

	FILEINFO FileInfoSearchRecive;//结构体传输数据库文件信息
	int nCount=0;
	memset(&FileInfoSearchRecive,0,sizeof(FileInfoSearchRecive));
	//CString strFileSize;
	while(FileNum!=0)
	{
		SearchSocket.Receive(&FileInfoSearchRecive,sizeof(FileInfoSearchRecive),0);//接收数据库文件信息
		dialog->m_List_Search.InsertItem(nCount,NULL);
		dialog->m_List_Search.SetItemText(nCount,0,(LPCTSTR)FileInfoSearchRecive.fileName);//向ListSearch中写入文件名
		//向ListSearch中写入文件类型
		//AfxMessageBox(FileInfoSearchRecive.fileName);
		dialog->m_List_Search.SetItemText(nCount,1,(LPCTSTR)FileInfoSearchRecive.fileType);
		//AfxMessageBox(FileInfoSearchRecive.fileType);
		double SearchFileSize=(double)(FileInfoSearchRecive.fileSize);//int转化为double
		CString _str=dialog->FileSizeTransfer(SearchFileSize);
		//向ListSearch中写入文件大小
		dialog->m_List_Search.SetItemText(nCount,2,(LPCTSTR)_str);
		//AfxMessageBox(_str);
		dialog->m_List_Search.SetItemText(nCount,3,(LPCTSTR)FileInfoSearchRecive.filepath);

		nCount++;
		FileNum--;
	}
	SearchSocket.Close();
	return 0;
}


////****客户端向P2P请求文件进程

UINT RequireFileThread(LPVOID pParam)
{
	CDownload_Downloading *MyDlg = (CDownload_Downloading *)pParam;
	AfxSocketInit(NULL);
	CSocket RequireFileSocket;
	if (!RequireFileSocket.Create())
	{
		AfxMessageBox("客户端请求文件资源套接字 创建失败\r\n");
		return 0;
	}
	CString P2PIP=FileOwnerIP;//连接资源拥有方IP、、、、、、、、可改、、、、、、、、
	if(!RequireFileSocket.Connect(P2PIP,P2PPort))//连接资源拥有方IP和Port
	{
		AfxMessageBox("客户端请求文件资源套接字 连接失败\r\n");
		return 0;
	}

	//发送状态码1
	int stateCode = 1;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= RequireFileSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;
	}
	char sendFilePath[150]={0};//////////////////传输用char，其他char*、CString不要用
	strcpy_s(sendFilePath,FileOwenerPath);
	RequireFileSocket.Send(sendFilePath,sizeof(sendFilePath),0);	//发送文件路径

	WIN32_FIND_DATA ReciveFileInfo;
	RequireFileSocket.Receive(&ReciveFileInfo, sizeof(ReciveFileInfo),0);
	int nFileSize = ReciveFileInfo.nFileSizeLow;
	CFileDialog dlg(false);	//另存文件
	dlg.m_ofn.lpstrFile=ReciveFileInfo.cFileName;	//文件名称
	dlg.m_ofn.lpstrFilter=_T("All Files(*.*)\0*.*\0\0");
	CString fpathname;
	if(dlg.DoModal()==IDOK)
	{   
		fpathname=dlg.GetPathName();	//得到文件名名称、路径
	}
	CFile file(fpathname,CFile::modeCreate | CFile::modeWrite| CFile::typeBinary);
	unsigned long RecvOnce = 0;
	unsigned long RecvPerTime = 0;
	//char *szBuff=NULL;
	wchar_t szBuff[SIZEFILE]={0};
	int i=MyDlg->m_NUMDownloading;


	double Speed=0;
	CString SpeedUpdate;
	int Count=0;

	// LARGE_INTEGER litmp; 
   //  LONGLONG QPart1,QPart2;
   //  double dfMinus, dfFreq, dfTim; 

	while(RecvOnce<(unsigned long)nFileSize)
	{ 
			//QueryPerformanceFrequency(&litmp);
			//dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率6.  
			//QueryPerformanceCounter(&litmp);
			//QPart1 = litmp.QuadPart;// 获得初始值
		
		DWORD starttime=::GetTickCount(); 
		RecvPerTime=RequireFileSocket.Receive(szBuff,SIZEFILE);
		file.Write(szBuff,RecvPerTime);
		RecvOnce+=RecvPerTime;
		double per=(double)RecvOnce/(double)nFileSize;
		MyDlg->m_Download_Downloading.SetItemData(i,per*100);

		DWORD endtime=::GetTickCount(); //获取毫秒级数目
		if(endtime-starttime!=0)							//计算下载速度
		{
			Speed=double(RecvPerTime)/double(endtime-starttime);
			Speed=10000*Speed;
		}
		
			//QueryPerformanceCounter(&litmp);
			//QPart2 = litmp.QuadPart;//获得中止值12.         
			//dfMinus = (double)(QPart2-QPart1);
			//if(dfMinus==0) Speed=0;
			//else
			//{
			//	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
			//	Speed=RecvPerTime/dfTim;
				
			//}
			if(Speed/1024/1024/1024>=1)
			{
				SpeedUpdate.Format("%.2f GB/S",Speed*1000/1024/1024);
			}
			if(Speed/1024/1024>=1)
			{
				SpeedUpdate.Format("%.2f MB/S",Speed*1000/1024/1024);
			}
			else
			{
				SpeedUpdate.Format("%.2f KB/S",Speed/1024);
			}
			if(Count%100==0)
			{
				MyDlg->m_Download_Downloading.SetItemText(i,5,SpeedUpdate);
			}
		Count++;
		//delete szBuff;	
		//delete szBuff;	
	}
	AfxMessageBox(_T("下载完毕"));
	MyDlg->m_Download_Downloading.SetItemText(i,5,_T(""));
	MyDlg->m_Download_Downloading.SetItemText(i,6,_T("下载完成"));
	file.Close();
	RequireFileSocket.Close();
	return 0;
}


///**文件发送线程

//UINT MyThreadFunction(LPVOID pParam）
UINT SendFileThread(LPVOID sendfileSocket)
{
	AfxSocketInit(NULL) ;
	SOCKET sendfile = (SOCKET)sendfileSocket;
	CSocket SendFileSocket;
	SendFileSocket.Attach(sendfile);

	char ReciveFilePath[150]={0};//////////////////传输用char，其他char*、CString不要用
	//Sleep(400);
	SendFileSocket.Receive(ReciveFilePath,sizeof(ReciveFilePath),0);
	//CString s;
	//s.Format(_T("%s"), ReciveFilePath);
	//AfxMessageBox(s);


	WIN32_FIND_DATA SendFileInfo;
	//根据文件名查找文件  Long，如执行成功，返回一个搜索句柄。
	//如果出错，返回一个INVALID_HANDLE_VALUE常数，一旦不再需要，应该用FindClose函数关闭这个句柄
	FindClose(FindFirstFile(ReciveFilePath, &SendFileInfo));
	SendFileSocket.Send(&SendFileInfo,sizeof(SendFileInfo),0);//发送文件大小和文件名

	CFile file;
	if(!file.Open(ReciveFilePath,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(_T("文件不存在"));
		return 1;
	}
	////////////////////////////////////////////////////////////
	unsigned long SendOnce = 0;
	unsigned long nSendPerTime = 0;
	file.Seek(0,CFile::begin);
	wchar_t szBuff[SIZEFILE]={0};
	//发送
	unsigned long nFileSize = SendFileInfo.nFileSizeLow;
	while(SendOnce<nFileSize)
	{
		nSendPerTime = file.Read(szBuff,SIZEFILE);
		SendFileSocket.Send(szBuff,nSendPerTime);//发送数据
		SendOnce+=nSendPerTime;			 
	}
	file.Close();
	SendFileSocket.Close();
	return 0;
}


////**连接服务器线程
UINT ConnectServerThread(LPVOID pParam)
{
	CSearch *dialog = (CSearch *)pParam;
	AfxSocketInit(NULL);
	CSocket ConnectServerSocket;
	if (!ConnectServerSocket.Create())
	{
		AfxMessageBox(_T("客户端心跳绑定端口失败！！！\r\n"));
		return 0;
	}
	if(!ConnectServerSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))//连接服务器IP和Port
	{
		AfxMessageBox(_T("连接服务器失败\r\n"));
		return 0;
	}
	int stateCode = 1;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= ConnectServerSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;
	}
	ConnectServerSocket.Close();
	return 0;

}
//客户端登录线程
UINT LoginThread(LPVOID pParam)
{
	CLogIn *MyDlg = (CLogIn *)pParam;
	AfxSocketInit(NULL);
	CSocket LoginSocket;
	if(!LoginSocket.Create())
	{
		AfxMessageBox("客户端**登录**套接字 创建失败\r\n");
		return 0;
	}
	//CConnectSet connectset;
	if(!LoginSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))//连接服务器IP和Port
	{
		AfxMessageBox("客户端**登录**套接字**连接失败\r\n");
		return 0;
	}
	
	int stateCode = 1;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= LoginSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;
	}
	USERINFO userinfo;
	memset(&userinfo,0,sizeof(userinfo));
	char   name[128];   
	::gethostname(name,128);//获得主机名  
	hostent* pHost=gethostbyname(name);//获得主机结构  
	strcpy_s(userinfo.userIP,inet_ntoa(*((in_addr*)pHost->h_addr_list[0])));
	strcpy_s(userinfo.username,CLogIn::username);
	strcpy_s(userinfo.password,CLogIn::password);
	//AfxMessageBox(userinfo.password);
	int err=LoginSocket.Send(&userinfo,sizeof(userinfo),0);
	if (err==SOCKET_ERROR)
	{
		AfxMessageBox(_T("客户端发送登录信息失败"));
		return 0;
	}
	char flags[2]={0};
	LoginSocket.Receive(flags,sizeof(flags),0);
	if (flags[0]=='#'&&flags[1]=='0')
	{
		LoginSocket.Close();
		AfxMessageBox(_T("亲，没有登录成功哟，用户名不存在，请亲注册下。。。"));
		return 0;
	}
	if (flags[0]=='#'&&flags[1]=='1')
	{
		LoginSocket.Close();
		AfxMessageBox(_T("亲，欢迎您使用校园快享，现在您可以开始畅快体验了。。。"));
		return 0;
	}
	if (flags[0]=='#'&&flags[1]=='2')
	{
		LoginSocket.Close();
		AfxMessageBox(_T("亲，您已经登录过校园快享，不用再次登录了哟。。。"));
		return 0;
	}
	if (flags[0]=='#'&&flags[1]=='3')
	{
		LoginSocket.Close();
		AfxMessageBox(_T("亲，没有登录成功哟，用户密码不正确，请亲注册下。。。"));
		return 0;
	}

	LoginSocket.Close();
	return 0;
}


//////////退出登录
UINT QuitThread(LPVOID pParam)
{
	CLogIn *MyDlg = (CLogIn *)pParam;
	AfxSocketInit(NULL);
	CSocket QuitSocket;
	if(!QuitSocket.Create())
	{
		AfxMessageBox("客户端**登录**套接字 创建失败\r\n");
		return 0;
	}
	//CConnectSet connectset;
	if(!QuitSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))//连接服务器IP和Port
	{
		AfxMessageBox("客户端**登录**套接字**连接失败\r\n");
		return 0;
	}
	int stateCode = 6;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= QuitSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;
	}
	USERINFO userinfo;
	memset(&userinfo,0,sizeof(userinfo));
	strcpy_s(userinfo.username,CLogIn::username);
	strcpy_s(userinfo.password,CLogIn::password);
	//AfxMessageBox(userinfo.password);
	int err=QuitSocket.Send(&userinfo,sizeof(userinfo),0);
	if (err==SOCKET_ERROR)
	{
		AfxMessageBox(_T("客户端发送登录信息失败"));
		return 0;
	}
	char flags[2]={0};
	QuitSocket.Receive(flags,sizeof(flags),0);
	if (flags[0]=='#'&&flags[1]=='1')
	{
		QuitSocket.Close();
		AfxMessageBox(_T("亲，您已经成功退出校园快享"));
		return 0;
	}
	if (flags[0]=='#'&&flags[1]=='0')
	{
		QuitSocket.Close();
		AfxMessageBox(_T("亲，您还没有登录，无法退出"));
		return 0;
	}


	return 0;
}
//////////////////////////////////////////////////////////////////
//////////////**********请求文件信息线程*************/////////////
//////////////////////////////////////////////////////////////////

UINT RequireInfoThread(LPVOID pParam)
{
	CDownload_Downloading *MyDlg = (CDownload_Downloading *)pParam;
	AfxSocketInit(NULL);
	CSocket RequireSocket;
	if(!RequireSocket.Create())
	{
		AfxMessageBox("客户端**请求文件信息**套接字 创建失败\r\n");
		return 0;
	}
	//CConnectSet connectset;
	if(!RequireSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))//连接服务器IP和Port
	{
		AfxMessageBox("客户端**请求文件信息**套接字**连接失败\r\n");
		return 0;
	}

	//发送状态码2
	int stateCode = 2;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= RequireSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("接收客户端发送的地址信息失败");
		return 0;
	}
	if(MyDlg==NULL)
	{
		AfxMessageBox("指针为空");
		return 0;
	}
	FileOwenerPath=MyDlg->m_filepath;//获取文件的路径到变量中
	FILENAME=MyDlg->m_filename;
	//////////////////////////////////////////请求文件信息
	FILEINFO FileInfo1;
	memset(&FileInfo1,0,sizeof(FileInfo1));
	strcpy_s(FileInfo1.fileName,FILENAME);//得到文件名称
	RequireSocket.Send(&FileInfo1,sizeof(FileInfo1));	//发送请求的文件信息
	FILEINFO FileInfo2;
	memset(&FileInfo2,0,sizeof(FileInfo2));
	RequireSocket.Receive(&FileInfo2,sizeof(FileInfo2));
	//FileOwenerPath=FileInfo2.filepath;//获取文件的路径到变量中
	FileOwnerIP=FileInfo2.fileownerIP;//获取文件拥有者的IP到变量FileOwnerIP中
	RequireSocket.Close();
	pRequireFileThread=AfxBeginThread(RequireFileThread,MyDlg);  //开启请求文件线程
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////**********客户端请求上传文件信息线程*************///
//////////////////////////////////////////////////////////////////

UINT UploadThread(LPVOID pParam)
{
	CUpload *MyDlg = (CUpload *)pParam;
	AfxSocketInit(NULL);
	CSocket UploadSocket;
	if(!UploadSocket.Create())
	{
		AfxMessageBox("客户端**请求文件信息**套接字 创建失败\r\n");
		return 0;
	}
	if(!UploadSocket.Connect((LPCTSTR)CConnectSet::ServerIP,(UINT)CConnectSet::ServerPort))//连接服务器IP和Port
	{
		AfxMessageBox("客户端**请求文件信息**套接字**连接失败\r\n");
		return 0;
	}

	//发送状态码5
	int stateCode = 5;
	char buffer[sizeof(stateCode)]="0";
	sprintf_s(buffer,"%d",stateCode);
	int sendbytes= UploadSocket.Send(buffer,sizeof(stateCode),0);
	if(sendbytes == SOCKET_ERROR)
	{
		AfxMessageBox("上传客户端发送的状态信息失败");
		return 0;
	}
	if(MyDlg==NULL)
	{
		AfxMessageBox("指针为空");
		return 0;
	}

//////////////////////////////////////////请求文件信息
	FILEINFO FileUpload1;
	memset(&FileUpload1,0,sizeof(FileUpload1));
	char   name[128];  
	hostent*   pHost;  
	gethostname(name,128);//获得主机名  
	pHost=gethostbyname(name);//获得主机结构  
	strcpy_s(FileUpload1.fileownerIP,inet_ntoa(*((in_addr*)pHost->h_addr_list[0])));
	//AfxMessageBox(FileUpload1.fileownerIP);
	strcpy_s(FileUpload1.fileName,MyDlg->strFileName);//得到文件名称
	strcpy_s(FileUpload1.filepath,MyDlg->strFilePath);//得到文件路径
	FileUpload1.fileSize = MyDlg->fFileTotalSize;//得到文件大小
	strcpy_s(FileUpload1.fileType,MyDlg->strFileType);//得到文件类型
	strcpy_s(FileUpload1.username,MyDlg->strFileUserList);

	UploadSocket.Send(&FileUpload1,sizeof(FileUpload1),0);	//发送请求的文件信息

	char flags[2]={0};
	int recvnum=UploadSocket.Receive(flags,sizeof(flags),0);
	if(recvnum== SOCKET_ERROR)
	{
		AfxMessageBox("接收服务器发送上传状态失败");
		UploadSocket.Close();
		return 0;
	}
	if(flags[0]=='#'&&flags[1]=='1')
	{
		AfxMessageBox("恭喜您，文件上传成功！");
		CString strNum = "";
		strNum.Format("%d", MyDlg->m_uploadNum); //将int变成CString类型
		CString filesize;
		//filesize.Format("%f",MyDlg->strFileTotalSize);
		MyDlg->m_List_Upload.InsertItem(MyDlg->m_uploadNum, strNum);    
		MyDlg->m_List_Upload.SetItemText(MyDlg->m_uploadNum, 1, MyDlg->strFileName);    
		MyDlg->m_List_Upload.SetItemText(MyDlg->m_uploadNum, 2, MyDlg->strFileUserList); 
		MyDlg->m_List_Upload.SetItemText(MyDlg->m_uploadNum, 3, MyDlg->strFileTotalSize); 
		MyDlg->m_uploadNum ++;

	}
	else if(flags[0]=='#'&&flags[1]=='0')
	{
		AfxMessageBox("该文件已经上传过了");
	}
	UploadSocket.Close();
	return 0;
}
//////////////////////////////////////////////////////////////////
//////////////**********开始主线程*****************////////////////
//////////////////////////////////////////////////////////////////

UINT StartClientThread(LPVOID pParam)
{
	CDialog *dialog = (CDialog *)pParam;
	AfxSocketInit(NULL);
    CSocket ClientListenSocket;
	if (!ClientListenSocket.Socket())
	{
		AfxMessageBox(_T("初始化socket失败"));
		return 0;
	}
	UINT optValue = 1 ;
	if (!ClientListenSocket.SetSockOpt(SO_REUSEADDR , &optValue,sizeof(UINT)))
	{
		AfxMessageBox(_T("设置socket重用属性失败")) ;
		return 0 ;
	}
	if (!ClientListenSocket.Bind(P2PPort))
	{
		AfxMessageBox(_T("客户端绑定端口失败\r\n"));
		return 0 ;
	}
	if(!ClientListenSocket.Listen(10))
	{
		//dialog->GetDlgItem(IDC_EDIT_Log)->SetWindowText(_T("启动监听过程失败,\r\n"));
		AfxMessageBox(_T("启动监听过程失败,\r\n"));
		return 0;
	}
	//AfxMessageBox(_T("监听程序已启动，正在等待连接\r\n"));
	CSocket P2PClientSocketIn;
	int stateCode=0;
	char buffer[sizeof(stateCode)]="0";
	while(true)
	{
		ClientListenSocket.Accept(P2PClientSocketIn);
		int bytesRec =P2PClientSocketIn.Receive(buffer,sizeof(stateCode),0);
		stateCode = atoi(buffer);//转变为整型
		switch(stateCode)
		{
		case 1:
			{
				SOCKET SendFilehandleSocket;
				SendFilehandleSocket = P2PClientSocketIn.Detach();
				//::AfxBeginThread(SendFileHandleThread,(LPVOID)SendFilehandleSocket);
				AfxBeginThread(SendFileThread,(LPVOID)SendFilehandleSocket);
				//AfxBeginThread(
				break;
			}
		case 2:
			{
				SOCKET handleSocket2;
				handleSocket2 =P2PClientSocketIn.Detach();
				::AfxBeginThread(RequireFileThread,(LPVOID)handleSocket2);
				break;
			}
		//case 3:
			//{
				//SOCKET handleSocket3;
				//handleSocket3 = P2PClientSocketIn.Detach();
				//AfxBeginThread(RequireFileThread,(LPVOID)handleSocket3);
				//break;
			//}
		}
		P2PClientSocketIn.Close();
	}
	return 0;
}



void ThreadOperate::startMainThread(void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(StartClientThread,dialog);
}

void ThreadOperate::startConnectServerThread(void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(ConnectServerThread,dialog);
}

void ThreadOperate::startRequireInfoThread(void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(RequireInfoThread,dialog);
}

void ThreadOperate::startSearchThread(void* cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(SearchThread,dialog);
}

void ThreadOperate::startRegisterThread(void* cw)
{
	//this->dialog =(CDialog*)cw;
	CRegister *Mydlg=(CRegister *)cw;
	AfxBeginThread(RegisterThread,Mydlg);
}
void ThreadOperate::StartLoginThread(void* cw)
{
	CLogIn *MyDlg=(CLogIn*)cw;
	AfxBeginThread(LoginThread,MyDlg);
}
void ThreadOperate::StartQuitThread(void* cw)
{
	CLogIn *MyDlg=(CLogIn*)cw;
	AfxBeginThread(QuitThread,MyDlg);
}
void ThreadOperate::startUploadThread(void * cw)
{
	this->dialog =(CDialog*)cw;
	AfxBeginThread(UploadThread,dialog);
}
void ThreadOperate::UpdateSpeed(void *Mydlg)
{
	CDownload_Downloading *dlg=(CDownload_Downloading *)Mydlg;
	this->SetOffset(OFFSET);
	_endOffset = this->GetOffset();
	CString _strSpeed = GetSpeed();
	if (_strSpeed != _T(""))
	{
		//dlg->GetDlgItem(_speedID)->SetWindowText(_strSpeed);		//更新
		dlg->m_Download_Downloading.SetItemText(dlg->m_NUMDownloading,5,_strSpeed);
	}
	_startOffset = _endOffset;		//设置起始标志
}
long ThreadOperate::GetOffset(void)
{
	return this->Offset;
}
CString ThreadOperate::GetSpeed()
{
	long _speed = _endOffset - _startOffset;
	long _ratio = ELAPSE_DEFAULT / ELAPSE_SPEED;

	char szSpeed[MAX_BUF_LEN];
	CString _strSpeed;
	CString _strEx;

	if (_speed <= 0)
	{
		return _T("");
	}
	else if (_speed < Kb)		//字节
	{
		_speed = _speed * _ratio;
		_ltoa_s(_speed, szSpeed, 10);
		_strEx = _T(" B / s");
	}
	else if ( (_speed >= Kb) && (_speed < Mb) )		//KB
	{
		_speed = _speed / Kb;
		_speed = _speed * _ratio;
		_ltoa_s(_speed, szSpeed, 10);
		_strEx = _T(" KB / s");
	}
	else
	{
		_speed = _speed / Mb;
		_speed = _speed * _ratio;
		_ltoa_s(_speed, szSpeed, 10);
		_strEx = _T(" MB / s");
	}
	_strSpeed = szSpeed;
	_strSpeed = _strSpeed + _strEx;
	return _strSpeed;
}
long ThreadOperate::SetOffset(long offset)
{
	return this->Offset=offset;
}
